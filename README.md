# shark-dgen

shark-dgen is a Qt GUI program that allows you to create Lua dissectors in wireshark with no experience in Lua.

![alt text](https://github.com/paramedicjack/shark-dgen/blob/main/photos/v0.1/filled.PNG?raw=true)

![alt text](https://github.com/paramedicjack/shark-dgen/blob/main/photos/v0.1/output.PNG?raw=true)

## Features

1. Many common types such as bools, uints, ints, floats, doubles
2. Little Endian and Big Endian order
3. Base encoding types DEC, HEX, OCT, and NONE
4. Custom acronym and naming conventions
5. Single port binding
6. File saving

## Planned features

1. More types
2. More base encoding types
3. Tighter naming and field control
4. Text appending
5. Reading CSV
6. Subtrees
7. Enumeration values

## Development and release

shark-dgen is developed using Qt 6.4.0 in Qt Creator with CMake. It is released using windeployqt with MSVC2019. 
