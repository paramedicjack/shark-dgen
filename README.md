# shark-dgen

shark-dgen is a Qt GUI program that allows you to create Lua dissectors in wireshark with no experience in Lua.

![alt text](https://github.com/paramedicjack/shark-dgen/blob/main/photos/v0.3/empty.PNG?raw=true)

![alt text](https://github.com/paramedicjack/shark-dgen/blob/main/photos/v0.2/output.PNG?raw=true)

## How to use

### Setup

1. Grab the the [latest release](https://github.com/paramedicjack/shark-dgen/releases)
2. Extract the zip and run shark-dgen.exe

### Add fields

1. Click the fields tab
2. Type a field name
3. Select options - endianness, type, encoding
4. Press ADD
5. Verify that the field appears in the table and the first four columns in the row match

You can select a row and press DELETE ROW to delete the row.

It is not recommended to edit cells directly as this could cause faults or break your dissector.

### Add enums

Enums have only been tested on uints  and with = so far.

1. Click the enums tab
2. Select a row in the table
3. Enter a key (name) and value (number) in the appropriate boxes
4. Select an operation in the dropdown
5. Press ADD
6. Ensure that the enum has been added to the dropdown

You can delete an enum value by selecting it in the dropdown and pressing DELETE.

### Add info and ports

1. Go to Info tab
2. Enter a name in the name section
3. Enter a 2-3 letter acronym in the acronym box
4. Go to ports tab
5. Type a port and press ADD
6. Ensure that the ports you want are in the dropdown

You can delete ports by selecting them in the dropdown and pressing the DELETE button.

### Generate 

1. Go to the Create tab
2. Press Create
3. Verify that the dissector looks how it should in the Dissector tab
4. Press Save As... to export the dissector. Place it in the wireshark Lua plugins directory.

### Export table as CSV

1. Create table using the field prompts
2. Click the export table button
3. Save as .csv file

The tables can now be edited in excel or a CSV editor and imported later.

### Import as CSV

Note: table must be in the same format it was exported in. I recommend exporting a table to use as a template

1. Click the import button
2. Open the .csv file

The table will be filled. The column headers will be overwritten with numbers - this is a known bug.

## Features

| Feature  | Status | Version |
| ------------- | ------------- | ------------- |
| UDP dissectors | implemented | v0.1 |
| uints, floats, doubles, chars, bools | implemented | v0.1 |
| little and big endian | implemented | v0.1 |
| DEC, HEX, OCT, and NONE encoding | implemented  | v0.1 |
| save dissector to file | implemented | v0.1 |
| multiple port binding | implemented | v0.2 |
| enum values | implemented | v0.2 |
| import and export from csv | implemented | v0.3 |
| more types | not implemented | - |
| text appending | not implemented | - |
| nested subtrees | not implemented | - |

## Development and release

shark-dgen is developed using Qt 6.4.0 in Qt Creator with CMake. It is released using windeployqt with MSVC2019. 
