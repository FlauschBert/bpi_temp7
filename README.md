# Temperature and humidity on 7-segment-leds on the Banana Pi

Show sensor data of DHT11 sensor on 8 7-segment-leds with MAX7219 driver and the Banana PI (which also should work on a Raspberry Pi).

# Compile and install dependencies

Clone the [WiringBP github repository](https://github.com/LeMaker/WiringBP) and run `./build`.  
See [References](#References) for the other needed libraries which also have to be built and installed for usage in this project.  

# Compile and install

Go to the *source* directory and use the mighty force of shining CMake :D to compile the pico project.  
The *wiringPi*, *dht11* and *max7219* sub folders contain internal interface libraries for the installed libraries in the */usr/local* folders.  
Do `make install` to install the binary in */usr/local/bin*.

(I used Armbian stretch but also more recent versions should work).

# Wiring

TODO

# Run

Simply start the built executable *bpi_temp7* to show the sensor data on the display. The executable has to be started as root.

# References

* [Banana Pi DHT11](https://github.com/FlauschBert/bpi_dht11)
* [Banana Pi MAX7219](https://github.com/FlauschBert/bpi_max7219)

