
#include <iostream>
#include <iomanip>
#include <deque>
#include <cstring>
#include <string>
#include <sstream>
#include <algorithm>
#include <atomic>
#include <sys/signal.h>

// wiringPi interface library
#include <wiringPi.h>

// dht11 interface library
#include <dht11.h>

// max7219 interface library
#include <max7219.h>
#include <digit.h>
#include <binary.h>
using namespace max7219;

namespace {

std::atomic<bool> sStop (false);

void tschuessCallback (int)
{
	sStop = true;
}

int activateTschuessCallback ()
{
	struct sigaction action;
	memset (&action, 0, sizeof (action));

	action.sa_handler = tschuessCallback;
	if (0 > sigaction (SIGTERM, &action, nullptr))
		return -1;
	if (0 > sigaction (SIGINT, &action, nullptr))
		return -1;

	return 0;
}

bool isEmpty (dht11::Data const& data)
{
	return data.humidity == -1.f && data.temperature == -1.f;
}

std::string getStringFrom (float const value)
{
	char tmp [5];
	sprintf (tmp, "%2.1f", std::max (0.0f, value));
	return tmp;
}

void showData (MAX7219& led, dht11::Data const& current, bool dots)
{
	auto const temperature = getStringFrom (current.temperature);
	if (4 == temperature.size ())
	{
		led.setDigit (0, 7, toDigit (temperature [0]));
		led.setDigit (0, 6, toDigit (temperature [1], dots));
		led.setDigit (0, 5, toDigit (temperature [3]));
		led.setDigit (0, 4, toDigit ('g'));
	}
	else
	{
		led.setDigit (0, 7, B01001111 /*E*/);
		led.setDigit (0, 6, toDigit ('r'));
		led.setDigit (0, 5, toDigit ('r'));
		led.setDigit (0, 4, 0x0);
	}

	auto const humidity = getStringFrom (current.humidity);
	if (4 == humidity.size ())
	{
		led.setDigit (0, 3, toDigit (humidity [0]));
		led.setDigit (0, 2, toDigit (humidity [1], dots));
		led.setDigit (0, 1, toDigit (humidity [3]));
		led.setDigit (0, 0, toDigit ('r'));
	}
	else
	{
		led.setDigit (0, 3, B01001111 /*E*/);
		led.setDigit (0, 2, toDigit ('r'));
		led.setDigit (0, 1, toDigit ('r'));
		led.setDigit (0, 0, 0x0);
	}

	//led.setDigit(0,7,toDigit('<'));
	//led.setDigit(0,7,toDigit('>'));
}

dht11::Data getDHT11Data ()
{
	// Use GPIO:0, Physical pin 11
	int constexpr pin = 0;

	// Trigger sending data on serial line
	dht11::sendStartSignal (pin);
	// Wait for data of client
	if (!dht11::waitForResponseSignal (pin))
		return {};
	// Read and evaluate data from client
	return dht11::getDataFromBits (pin);
}

void setupMax7219 (MAX7219& led, uint8_t intensity)
{
  /*
   The MAX7219 is in power-saving mode on startup,
   we have to do a wakeup call
   */
  led.shutdown(0,false);
  /* Set the brightness to a medium values */
  led.setIntensity(0,intensity);
  /* and clear the display */
  led.clearDisplay(0);
}

}

int main ()
{
	// Register callback for save shutdown
	if (0 > activateTschuessCallback ())
	{
		std::cerr << "Could not register shutdown callback.\n";
		return -1;
	}

	// Does also wiringPiSetup () for us
	if (-1 == dht11::setup ())
	{
		std::cerr << "DHT11 setup failed.\n";
		return -1;
	}

	MAX7219 led  (
		12 /*dataPin MOSI*/,
		14 /*clkPin SCLK*/,
		10 /*csPin CE0/CS0*/,
		1  /*numDevices*/
	);
	setupMax7219 (led, 1/*brightness 1..15*/);

	while (!sStop)
	{
		// Read next data until valid
		dht11::Data data;
		while (isEmpty (data))
		{
			// Wait 2 seconds before first and consecutive readouts
			delay (2000 /*ms*/);
			data = getDHT11Data ();

			if (sStop)
				break;
		}

		// show
		showData (led, data, false /*no dots*/);
		delay (250 /*ms*/);
		showData (led, data, true /*dots*/);
	}

  led.clearDisplay(0);
	led.shutdown(0,true /*powersave*/);
	return 0;
}
