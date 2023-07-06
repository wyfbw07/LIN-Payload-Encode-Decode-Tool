# LIN Description File (LDF) Parse Tool



## Build and Run

### On macOS

Open the .xcodeproj project file, hit Build (Cmd+B).

No arguments is required to run. Edit properties in main to use this tool.

### On Other Operating Systems

You will need to use the source files and create a new project on your own to build. This tool has been tested with Visual Studio 17.6.4 on Windows 11 x64.



## Error Handling

This tool uses exceptions to eraise errors. As a result, running and calling functions should be wrapped into a try-catch block. If nothing is added to catch potential errors, it could cause unhandled excepetions thus program termination.



## Function Calls

### Load and Parse LDF File

```c++
bool LdfParser::parse(const std::string& filePath);
```

| About this function | Description                                                  |
| :------------------ | :----------------------------------------------------------- |
| Use case            | To load and parse a LDF file, given the file path in string  |
| Return values       | A bool to indicate whether parsing succeeds (true) or not (false) |

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



### Print LDF File Info

```c++
friend std::ostream& operator<<(std::ostream& os, const SignalEncodingType& sigEncodingType);
```

| About this function | Description                                  |
| :------------------ | :------------------------------------------- |
| Use case            | To display all info in the LIN database File |
| Return values       | std::cout in terminal                        |

Sample usage of this function:

```c++
std::cout << ldfFile << std::endl;
```

Use this function to display LDF file info once a LDF file is loaded and parsed. Frames, signals and signal encoding types info would appear in terminal or Xcode debugger terminal.



### Decode a frame

```c++
std::map<std::string, std::tuple<double, std::string, LinSignalEncodingValueType> > decode(int& frameId, unsigned char payLoad[MAX_FRAME_LEN], int& dlc);
```

| About this function | Description                                                  |
| :------------------ | :----------------------------------------------------------- |
| Use case            | To decode an entire LIN frame payload                        |
| Input parameters    | (Frame ID, An array of frame payload, Frame size)            |
| Return values       | <Signal name, <Decoded value, Unit\|Description of decoded value, Valuetype: Physical Value\|Logical value> > |

Sample usage of this function:

```c++
unsigned char rawPayload[8] = { 0xa0, 0x0, 0x0, 0x08, 0x00, 0x00, 0x00, 0x00 };
std::map<std::string, std::tuple<double, std::string, LinSignalEncodingValueType> > result;
result = ldfFile.decode(10, rawPayload, 4);
```

The function will return an map, where the first is signal name, and the second is a tuple that consists of decoded value, unit of decoded value, and an enum LinSignalEncodingValueType. LinSignalEncodingValueType can be either PhysicalValue or LogicalValue, and the previous slot of the tuple will be the unit or description of decoded value correspondingly. 



### Encode a frame

```c++
int encode(int& frameId, std::vector<std::pair<std::string, double> > signalsToEncode, unsigned char encodedPayload[MAX_FRAME_LEN]);
```

| About this function | Description                                                  |
| :------------------ | :----------------------------------------------------------- |
| Use case            | To encode a LIN frame payload                                |
| Input parameters    | (Frame ID, An vector of pair of signal name and physical value, A fixed size 8 slots array that contains the encoded payload) |
| Return values       | Frame size of the encoded payload                            |

Sample usage of this function:

```c++
// Prepare data
unsigned int encodedDlc = 0;
unsigned char encodedPayload[8];
std::vector<std::pair<std::string, double> > signalsToEncode;
signalsToEncode.push_back(std::make_pair("EngSpeed", 50));
signalsToEncode.push_back(std::make_pair("EngSpeed_Second", 1535));
// Encode
encodedDlc = ldfFile.encode(168, signalsToEncode, encodedPayload);
```

The function encodes one or more signals at once into a single frame payload. The fixed size encodedPayload array contains the encoded payload once the function has been called. An additional value is returned to specify the message size of the encoded payload. If one or more signal values are not provided upon encoding, initial values defined in LDF will be used to encode the frame.



## Resources

This tool is developed with XL-Driver-Library in mind. The free-of-charge XL-Driver-Library is a universal programming interface you can use to create your own applications while accessing Vector’s powerful hardware interfaces. 

[User Manual](https://cdn.vector.com/cms/content/products/XL_Driver_Library/Docs/XL_Driver_Library_Manual_EN.pdf) of [XL-Driver-Library](https://www.vector.com/int/en/products/products-a-z/libraries-drivers/xl-driver-library/#) by Vector Informatik GmbH.

Reference PDF for LIN bus [LIN Specification Package Revision 2.2A](https://cdn.vector.com/cms/content/know-how/lin/LIN-Spec_2.2_Rev_A.PDF) by LIN Consortium.

Reference PDF for LIN bus [LIN Specification Package Revision 2.1](https://lin-cia.org/fileadmin/microsites/lin-cia.org/resources/documents/LIN-Spec_Pac2_1.pdf) by LIN Consortium.



## Notice

- This tool does not currently support parsing, decoding and encoding of signals that has init_value_array as initial value since there are little usage of this type of signal.



## What's New

- This tool now supports encoding of LDF frames.
