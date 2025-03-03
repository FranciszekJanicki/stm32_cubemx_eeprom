add_library(eeprom STATIC)

target_sources(eeprom PRIVATE 
    "eeprom.hpp"
    "eeprom.cpp"
)

target_include_directories(eeprom PUBLIC 
    "."
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(eeprom PRIVATE
    utility
    stm32cubemx
)

target_compile_options(eeprom PUBLIC
    -std=c++23
    -Wall
    -Wextra
    -Wconversion
    -Wshadow
    -Wpedantic
    -Wnarrowing
    -Waddress
    -pedantic
    -Wdeprecated
    -Wsign-conversion
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wnull-dereference
    -Wdouble-promotion
    -Wimplicit-fallthrough
    -Wcast-align
    -fconcepts
)
