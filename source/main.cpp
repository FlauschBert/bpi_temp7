
#include <iostream>
#include <iomanip>
#include <deque>
#include <string>
#include <sstream>
#include <algorithm>

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

int readoutDelay = 2000 /*ms*/;

bool sStop = false;

size_t constexpr maxDataSize = 15;
using Datas = std::deque<dht11::Data>;
Datas datas;
dht11::Data lastValid;

bool isEmpty (dht11::Data const& data)
{
	return data.humidity == -1 && data.temperature == -1;
}

std::string getStringFrom (int value)
{
	std::stringstream sstream;
	sstream << std::setw (2) << std::setfill ('0');
	sstream << std::max (0, value);
	return sstream.str ();
}

void showData (MAX7219& led, dht11::Data const& current, Datas const& data, bool dots = false)
{
	auto const temperature = getStringFrom (current.temperature);
	if (2 == temperature.size ())
	{
		led.setDigit (0, 7, toDigit (temperature [0]));
		led.setDigit (0, 6, toDigit (temperature [1], dots));
		led.setDigit (0, 5, toDigit ('g'));
		led.setDigit (0, 4, 0x0);
	}
	else
	{
		led.setDigit (0, 7, B01001111 /*E*/);
		led.setDigit (0, 6, toDigit ('r'));
		led.setDigit (0, 5, toDigit ('r'));
		led.setDigit (0, 4, 0x0);
	}

	auto const humidity = getStringFrom (current.humidity);
	if (2 == humidity.size ())
	{
		led.setDigit (0, 3, toDigit (humidity [0]));
		led.setDigit (0, 2, toDigit (humidity [1], dots));
		led.setDigit (0, 1, toDigit ('r'));
		led.setDigit (0, 0, 0x0);
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
	dht11::waitForResponseSignal (pin);
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

	// Wait 2 seconds before first readout
	delay (readoutDelay /*ms*/);

	while (!sStop)
	{
		auto const data = getDHT11Data ();

		// Consider parity bit check
		if (isEmpty (data))
		{
			// Show last valid state with dots
			if (!isEmpty (lastValid))
				showData (led, lastValid, datas, true /*dots*/);

			std::cerr << "Parity invalid. Read again.\n";
			delay (readoutDelay /*ms*/);
			continue;
		}

		// collect data for gradient
		datas.push_back (data);
		if (datas.size () > maxDataSize)
			datas.pop_front ();
		lastValid = data;

		// show
		showData (led, data, datas);

		delay (readoutDelay /*ms*/);
	}

  led.clearDisplay(0);
	led.shutdown(0,true /*powersave*/);
	return 0;
}
