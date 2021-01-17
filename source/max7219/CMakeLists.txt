cmake_minimum_required(VERSION 3.7)

project(max7219)

find_path(INCLUDE_DIR_MAX7219 NAMES max7219.h PATHS /usr/local/include/max7219 REQUIRED NO_DEFAULT_PATH)
find_library(LIBRARY_MAX7219 NAMES libmax7219.a PATHS /usr/local/lib REQUIRED NO_DEFAULT_PATH)

message (INFO "${INCLUDE_DIR_MAX7219} ${LIBRARY_MAX7219}")

if(INCLUDE_DIR_MAX7219 AND LIBRARY_MAX7219)
	add_library(max7219 INTERFACE)
	target_include_directories(max7219 INTERFACE ${INCLUDE_DIR_MAX7219})
	target_link_libraries(max7219 INTERFACE ${LIBRARY_MAX7219})
else()
	message(FATAL "max7219 not found")
endif()
