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
    // Decode
    std::map<std::string, double> decode(int msgId, unsigned char payLoad[], int dlc);
    friend std::ostream& operator<<(std::ostream& os, const LdfParser& ldfFile);

private:

    typedef std::map<int, Frame>::iterator framesLibrary_iterator;
    typedef std::map<std::string, Signal>::iterator signalsLibrary_iterator;
    typedef std::map<std::string, SignalEncodingType>::iterator sigEncodingTypeLibrary_iterator;
    // A hash table that stores all info of frames. <Frame id, Frame object>
    std::map<int, Frame> framesLibrary{};
    // A hash table that stores all info of signals. <Signal name, Signal object>
    std::map<std::string, Signal> signalsLibrary{};
    // A hash table that stores all info of signal encoding types. <encoding type name, Signal Encoding type object>
    std::map<std::string, SignalEncodingType> sigEncodingTypeLibrary{};
    bool isEmptyLibrary = true;
    bool isEmptySignalsLibrary = true;
    // This vector contains all the frames which got parsed from the LDF-File
    std::vector<Frame*> framesInfo;
    // Function used to parse LDF file
    void loadAndParseFromFile(std::istream& in);
    Signal& findSignalInSignalsLibrary(std::string& sigName);

};


#endif /* ldf_parser_hpp */
