//
//  ldf_parser.cpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#include <fstream>
#include <iostream>
#include <stdexcept>
#include "ldf_parser.hpp"

// Load file from path. Parse and store the content
// A returned bool is used to indicate whether parsing succeeds or not
bool LdfParser::parse(const std::string& filePath) {
    // Get file path, open the file stream
    std::ifstream ldfFile(filePath.c_str(), std::ios::binary);
    if (ldfFile) {
        // Parse file content
        loadAndParseFromFile(ldfFile);
    }
    else {
        throw std::invalid_argument("Could not open CAN database file.");
        return false;
    }
    // Parse complete, mark as successful
    ldfFile.close();
    isEmptyLibrary = false;
    return true;
}

void LdfParser::loadAndParseFromFile(std::istream& in) {
    std::string line;
    std::string lineInitial;
    // Read the file line by line
    while (in >> lineInitial) {
        // Look for signal encoding types
        if (lineInitial == "Signal_encoding_types") {
            // Go to next line
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::string sigEncodingTypeName;
            while (in >> sigEncodingTypeName && sigEncodingTypeName != "}") {
                // Parse signal encoding types
                SignalEncodingType sigEncodingType;
                sigEncodingType.setName(sigEncodingTypeName);
                in >> sigEncodingType;
                // Signal encoding type name uniqueness check. Names by definition need to be unqiue within the file
                sigEncodingTypeLibrary_iterator data_itr = sigEncodingTypeLibrary.find(sigEncodingType.getName());
                if (data_itr == sigEncodingTypeLibrary.end()) {
                    // Uniqueness check passed, store the signal encoding type
                    sigEncodingTypeLibrary.insert(std::make_pair(sigEncodingType.getName(), sigEncodingType));
                }
                else {
                    throw std::invalid_argument("Signal encoding type \"" + sigEncodingType.getName() + "\" has a duplicate. Parse Failed.");
                }
            }
        }
        else if (lineInitial == "Signals") {
            // Go to next line
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::string sigName;
            while (in >> sigName && sigName != "}") {
                // Parse signal
                Signal sig;
                // Remove trailling colon
                if (!sigName.empty()){
                    sigName.pop_back();
                }
                sig.setName(sigName);
                in >> sig;
                // Signal name uniqueness check. Names by definition need to be unqiue within the file
                signalsLibrary_iterator data_itr = signalsLibrary.find(sig.getName());
                if (data_itr == signalsLibrary.end()) {
                    // Uniqueness check passed, store the signal
                    signalsLibrary.insert(std::make_pair(sig.getName(), sig));
                }
                else {
                    throw std::invalid_argument("Signal \"" + sig.getName() + "\" has a duplicate. Parse Failed.");
                }
            }
            isEmptySignalsLibrary = false;
            std::cout << std::endl;
        }
        else if ((lineInitial == "Frames") && (!isEmptySignalsLibrary)) {
            // Go to next line
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::string frameName;
            while (in >> frameName && frameName != "}") {
                // Parse frame
                Frame frm;
                // Remove trailling colon
                if (!frameName.empty()){
                    frameName.pop_back();
                }
                frm.setName(frameName);
                in >> frm;
                // Link frame with its signals
                for (size_t i = 0; i < frm.getSignalsName().size(); i++) {
                    signalsLibrary_iterator data_itr = signalsLibrary.find(frm.getSignalsName()[i].first);
                    if (data_itr == signalsLibrary.end()) {
                        throw std::invalid_argument("Cannot find signal \"" + frm.getSignalsName()[i].first
                                                    + "\" under frame \"" + frm.getName() + "\". Parse Failed.");
                    }
                    else {
                        // Store signal start bit
                        data_itr->second.setStartBit(frm.getSignalsName()[i].second);
                    }
                }
                // Frame name uniqueness check. Names by definition need to be unqiue within the file
                framesLibrary_iterator data_itr = framesLibrary.find(frm.getId());
                if (data_itr == framesLibrary.end()) {
                    // Uniqueness check passed, store the signal
                    framesLibrary.insert(std::make_pair(frm.getId(), frm));
                }
                else {
                    throw std::invalid_argument("Frame \"" + frm.getName() + "\" has a duplicate. Parse Failed.");
                }
            }
        }
        else if ((lineInitial == "Signal_representation") && (!isEmptySignalsLibrary)) {
            
        }
        // Skip the rest of the line for uninterested data and make sure we can get a whole new line in the next iteration
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::ostream& operator<<(std::ostream& os, const LdfParser& ldfFile) {
    std::cout << "-----------------------------------------------" << std::endl;
    // Print frame info
    for (auto frame : ldfFile.framesLibrary) {
        std::cout << frame.second << std::endl;
    }
    std::cout << "-----------------------------------------------" << std::endl;
    // Print signal info
    for (auto signal : ldfFile.signalsLibrary) {
        std::cout << signal.second << std::endl;
    }
    std::cout << "-----------------------------------------------" << std::endl;
    // Print signal encoding types info
    for (auto sigEncodingType : ldfFile.sigEncodingTypeLibrary) {
        std::cout << sigEncodingType.second << std::endl;
    }
    return os;
}
