cmake_minimum_required(VERSION 3.7)

project(wiringPi)

find_path(INCLUDE_DIR_WIRINGPI NAMES wiringPi.h PATHS /usr/local/include REQUIRED NO_DEFAULT_PATH)
find_library(LIBRARY_WIRINGPI NAMES libwiringPi.so PATHS /usr/local/lib REQUIRED NO_DEFAULT_PATH)

message (INFO "${INCLUDE_DIR_WIRINGPI} ${LIBRARY_WIRINGPI}")

if(INCLUDE_DIR_WIRINGPI AND LIBRARY_WIRINGPI)
	add_library(wiringPi INTERFACE)
	target_include_directories(wiringPi INTERFACE ${INCLUDE_DIR_WIRINGPI})
	target_link_libraries(wiringPi INTERFACE ${LIBRARY_WIRINGPI})
else()
	message(FATAL "wiringPi not found")
endif()
