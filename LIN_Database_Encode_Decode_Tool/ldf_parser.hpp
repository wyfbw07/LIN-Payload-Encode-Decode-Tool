//
//  ldf_parser.hpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#ifndef ldf_parser_hpp
#define ldf_parser_hpp

#include <map>
#include <string>
#include "ldf_parser_dependencies/frame.hpp"
#include "ldf_parser_dependencies/signal.hpp"
#include "ldf_parser_dependencies/signal_encoding_type.hpp"

class LdfParser {

public:

    // Construct using either a File or a Stream of a LDF-File
    // A bool is used to indicate whether parsing succeeds or not
    bool parse(const std::string& filePath);
    // Encode
    int encode(int& frameId,
               std::vector<std::pair<std::string, double> > signalsToEncode,
               unsigned char encodedPayload[MAX_FRAME_LEN]);
    // Decode
    std::map<std::string, std::tuple<double, std::string, ValueType> > decode(int& frameId,
                                                                              unsigned char payLoad[MAX_FRAME_LEN],
                                                                              int& dlc);
    // Print LDF info
    friend std::ostream& operator<<(std::ostream& os, const LdfParser& ldfFile);

private:

    typedef std::map<int, Frame>::iterator framesLib_iterator;
    typedef std::map<std::string, Signal>::iterator signalsLib_iterator;
    typedef std::map<std::string, SignalEncodingType>::iterator sigEncodingTypeLib_iterator;
    bool isEmptyLibrary = true;
    bool isEmptyFramesLibrary = true;
    bool isEmptySignalsLibrary = true;
    bool isEmptySigEncodingTypeLibrary = true;
    // A hash table that stores all info of frames. <Frame id, Frame object>
    std::map<int, Frame> framesLibrary{};
    // A hash table that stores all info of signals. <Signal name, Signal object>
    std::map<std::string, Signal> signalsLibrary{};
    // A hash table that stores all info of signal encoding types. <encoding type name, Signal Encoding type object>
    std::map<std::string, SignalEncodingType> sigEncodingTypeLibrary{};
    // Function used to parse LDF file
    void resetParsedContent();
    void loadAndParseFromFile(std::istream& in);

};

#endif /* ldf_parser_hpp */
