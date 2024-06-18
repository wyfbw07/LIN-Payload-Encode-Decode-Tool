# Payload Encoder and Decoder based on LIN Description File(LDF)

A C++ program that can parse LDF files, and decode or encode frame payloads.

The parser supports LIN2.0, LIN 2.1 and LIN 2.2(A).

The parser does not currently support diagnostic and sporadic frames.

The parser does not currently support byte-array signals (Initial values in BCD/ASCII).



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

#### Use Case

To load and parse a LDF file, given the file path in string

#### Input Parameters

**filePath**

The file path of the LDF file.

#### Return value

Returns a bool to indicate whether parsing succeeds (true) or not (false).

#### Sample usage of this function

```c++
LdfParser ldfFile;
ldfFile.parse("/Users/filelocation/ldf_test.ldf");
```

#### Description

A instance of the class LdfParser must be created first, and use the parse function to load and parse the LDF file. All frames, signals and signal encoding types info will then be stored. 

Repeated calls of this function are not allowed and will be ignored. There is little use cases that you would want to re-parse the file. However if you do want to re-parse, destroy the existing class instance and create a new one to parse again.

For frame classes, these information will be parsed: 
- Frame name
- Frame ID
- Frame size (payload length)
- Name of publisher node
- Signal classes that belong to this frame

For signal classes, these information will be parsed: 
- Signal name
- Signal start bit
- Signal size (data length)
- Name of publisher node
- The name of the signal encoding type it uses
- Name(s) of subscriber node

For signal encoding type classes, these information will be parsed: 

- Encoding type name
- Encoding value type (Logical value, Physical value)
- Factor
- Offset
- Maximum value
- Minimum value
- Unit (for physical value)/Descripton (for logical value)



### Print LDF File Info

```c++
friend std::ostream& operator<<(std::ostream& os, const LdfParser& ldfFile);
```

#### Use Case

To display all info in the LIN database File

#### Input Parameters

**ldfFile**

A LdfParser class instance that has already parsed an LDF file.

#### Return value

Output stream

#### Sample usage of this function

```c++
std::cout << ldfFile << std::endl;
```

#### Description

Use this function to display LDF file info once a LDF file is parsed. Frames, signals and signal encoding types info would appear in terminal.



### Decode a frame

```c++
std::map<std::string, std::tuple<double, std::string, LinSigEncodingValueType> >
LdfParser::decode(
	int const frameId,
	int const frmSize,
	unsigned char payLoad[MAX_FRAME_LEN]
);
```

#### Use Case

To decode an entire LIN frame payload. Check main.cpp for function call examples.

#### Input Parameters

**frameId**

The frame's identifier.

**frmSize**

Size of the frame.

**payload**

The frame payload that need to be decoded.

#### Return value

The function will return an map: 

<Signal name, <Decoded value, Unit\|Description of decoded value, Valuetype: Physical Value\|Logical value> > 

The first is signal name, and the second is a tuple that consists of decoded value, unit(or description) of decoded value, and an enum LinSigEncodingValueType. LinSigEncodingValueType can be either Physical value or Logical value.

#### Sample usage of this function

```c++
unsigned char rawPayload[8] = { 0xa0, 0x0, 0x0, 0x08, 0x00, 0x00, 0x00, 0x00 };
std::map<std::string, std::tuple<double, std::string, LinSigEncodingValueType> > result;
result = ldfFile.decode(10, 4, rawPayload);
```



### Encode a frame

```c++
int LdfParser::encode(
	int const frameId,
	std::vector<std::pair<std::string, double> >& signalsToEncode,
	unsigned char encodedPayload[MAX_FRAME_LEN]
);
```

#### Use Case

To encode a LIN frame payload. Check main.cpp for function call examples.

#### Input Parameters

**frameId**

The frame's identifier.

**signalsToEncode**

An vector of pairs that contains signal names and physical values. In each pair the first is signal name, and the second is the physical value for that signal. Signals under the frame will encode will initial values if no physical values are provided.

#### Output Parameters

**encodedPayload**

Contains the encoded frame payload. The MAX_FRAME_LEN is 8.

#### Return value

Returns an int that specifies the encoded frame size.

#### Sample usage of this function

```c++
// Prepare data
unsigned char encodedPayload[8];
std::vector<std::pair<std::string, double> > signalsToEncode;
signalsToEncode.push_back(std::make_pair("EngSpeed", 50));
signalsToEncode.push_back(std::make_pair("EngSpeed_Second", 1535));
// Encode
int encodedfrmSize = ldfFile.encode(168, signalsToEncode, encodedPayload);
```



## Resources

This tool is developed with XL-Driver-Library in mind. The free-of-charge XL-Driver-Library is a universal programming interface you can use to create your own applications while accessing Vector’s powerful hardware interfaces. 

[User Manual](https://cdn.vector.com/cms/content/products/XL_Driver_Library/Docs/XL_Driver_Library_Manual_EN.pdf) of [XL-Driver-Library](https://www.vector.com/int/en/products/products-a-z/libraries-drivers/xl-driver-library/#) by Vector Informatik GmbH.

Reference PDF for LIN bus [LIN Specification Package Revision 2.2A](https://cdn.vector.com/cms/content/know-how/lin/LIN-Spec_2.2_Rev_A.PDF) by LIN Consortium.

Reference PDF for LIN bus [LIN Specification Package Revision 2.1](https://lin-cia.org/fileadmin/microsites/lin-cia.org/resources/documents/LIN-Spec_Pac2_1.pdf) by LIN Consortium.



## What's New

- Added LDF examples
- Small adjustments in calculating operating time.



## Known Issues

- The parser does not currently support parsing LDF with comments. Try removing comments as a workaround.



## Upcoming Updates

- Allow parsing LDF that contains comments.
