# LIN Description File (LDF) Parse Tool



## Build and Run

### On macOS

Open the .xcodeproj project file, hit Build (Cmd+B).

No arguments is required to run. Edit properties in main to use this tool.

### On Other Operating Systems

You will need to use the source files and create a new project on your own to build. This tool is still under development, and no tests were conducted on Windows machines.



## Error Handling

This tool uses exceptions to eraise errors. As a result, running and calling functions should be wrapped into a try-catch block. If nothing is added to catch potential errors, it could cause program termination.



## Function Calls

### Load and Parse LDF File

```c++
bool LdfParser::parse(const std::string& filePath);
```

| About this function | Description                                                  |
| :------------------ | :----------------------------------------------------------- |
| Use case            | To load and parse a LDF file, given the file path in string  |
| Return value        | A bool to indicate whether parsing succeeds (true) or not (false) |

Sample usage of this function:

```c++
LdfParser ldfFile;
ldfFile.parse("/Users/filelocation/ldf_test.ldf");
```

A instance of the class LdfParser must be created first, and use the parse function to load and parse the LDF file. All frames, signals and signal encoding types info will then be stored. 

<u>This function can be called only once.</u> Repeated calls will be ignored. There is little use cases that you would want to re-parse the file. However if you do want to re-parse, destroy the existing class instance and create a new one to parse again.

For frame classes, these information will be parsed: 
- Frame name
- Frame ID
- Frame size (payload length)
- Name of publisher node
- A std::vector of std::pair of signal name and start bit

For signal classes, these information will be parsed: 
- Signal name
- Signal start bit
- Signal size (data length)
- Name of publisher node
- The name of the signal encoding type it belongs to
- Name(s) of subscriber node

For signal encoding type classes, these information will be parsed: 

- Signal encoding type name
- Signal start bit
- A std::vector of logical values
- A std::vector of physical values



### Print DBC File Info

```c++
friend std::ostream& operator<<(std::ostream& os, const SignalEncodingType& sigEncodingType);
```

| About this function | Description                                  |
| :------------------ | :------------------------------------------- |
| Use case            | To display all info in the LIN database File |
| Return value        | std::cout in terminal                        |

Sample usage of this function:

```c++
std::cout << ldfFile << std::endl;
```

Use this function to display LDF file info once a LDF file is loaded and parsed. Frames, signals and signal encoding types info would appear in terminal or Xcode debugger terminal.



## Resources

This tool is developed with XL-Driver-Library in mind. The free-of-charge XL-Driver-Library is a universal programming interface you can use to create your own applications while accessing Vector’s powerful hardware interfaces. 

Reference PDF for LIN bus [LIN Specification Package Revision 2.2A](https://cdn.vector.com/cms/content/know-how/lin/LIN-Spec_2.2_Rev_A.PDF) by LIN Consortium.

[User Manual](https://cdn.vector.com/cms/content/products/XL_Driver_Library/Docs/XL_Driver_Library_Manual_EN.pdf) of [XL-Driver-Library](https://www.vector.com/int/en/products/products-a-z/libraries-drivers/xl-driver-library/#) by Vector Informatik GmbH.



## What's New

- This tool now supports decoding of LDF frames.
- Completely new LDF parser so that it can parse normally even if the file content structure is not exactly identical.



## To Dos

- Encoding of LIN frame payloads.
