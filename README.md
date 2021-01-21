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

# System inclusion (debian related)

To run the built executable as system daemon the files in the *scripts* folder have to be installed as root:  
* `bpi_temp7` goes to `/etc/init.d`  
* `bpi_temp7.service` goes to `/etc/systemd/system`  

To publish the new service in the system do a:  
`systemctl daemon-reload`  

And to start the service on system startup do a:  
`systemctl enable bpi_temp7.service`  

With scripts installed you also can start-stop the executable:  
* `service bpi_temp7 start`
* `service bpi_temp7 stop`

# References

* [Banana Pi DHT11](https://github.com/FlauschBert/bpi_dht11)
* [Banana Pi MAX7219](https://github.com/FlauschBert/bpi_max7219)

