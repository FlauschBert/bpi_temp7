cmake_minimum_required(VERSION 3.7)

project(bpi_temp7 VERSION 0.1)
set(CMAKE_CXX_STANDARD 14)

# use interface library for including wiringBP dht11 max7219
add_subdirectory(wiringPi)
add_subdirectory(dht11)
add_subdirectory(max7219)

# add test executable
add_executable(bpi_temp7 main.cpp)
target_link_libraries(bpi_temp7 wiringPi dht11 max7219)

# install
SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
install(
	TARGETS bpi_temp7
	RUNTIME DESTINATION /usr/local/bin
)
