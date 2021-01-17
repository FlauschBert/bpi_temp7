cmake_minimum_required(VERSION 3.7)

project(dht11)

find_path(INCLUDE_DIR_DHT11 NAMES dht11.h PATHS /usr/local/include/dht11 REQUIRED NO_DEFAULT_PATH)

message (INFO "${INCLUDE_DIR_DHT11}")

if(INCLUDE_DIR_DHT11)
	add_library(dht11 INTERFACE)
	target_include_directories(dht11 INTERFACE ${INCLUDE_DIR_DHT11})
else()
	message(FATAL "dht11 not found")
endif()
