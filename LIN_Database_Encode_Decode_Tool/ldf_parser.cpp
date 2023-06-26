//
//  ldf_parser.cpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#include <limits>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "ldf_parser.hpp"
#include "ldf_parser_dependencies/ldf_parser_helper.hpp"

// Load file from path. Parse and store the content
// A returned bool is used to indicate whether parsing succeeds or not
bool LdfParser::parse(const std::string& filePath) {
    if (!isEmptyLibrary) {
        std::cerr << "LDF has already been parsed. Recalling of parse has no effect" << std::endl;
        return false;
    }
    // Get file path, open the file stream
    std::ifstream ldfFile(filePath.c_str(), std::ios::binary);
    if (ldfFile) {
        // Parse file content
        loadAndParseFromFile(ldfFile);
    }
    // File open failed
    else {
        throw std::invalid_argument("Could not open LDF database file.");
        return false;
    }
    // Integrity check passed, parse success
    if (!isEmptyFramesLibrary && !isEmptySignalsLibrary && !isEmptySigEncodingTypeLibrary) {
        isEmptyLibrary = false;
        ldfFile.close();
        return true;
    }
    // Integrity check failed, remove all previously parsed info
    framesLibrary = std::map<int, Frame>{};
    signalsLibrary = std::map<std::string, Signal>{};
    sigEncodingTypeLibrary = std::map<std::string, SignalEncodingType>{};
    // Display corresponding error
    if (isEmptySignalsLibrary) {
        std::cerr << "Cannot find signals infomation in LDF file."
        << " Cannot parse signals representation infomation due to missing signals infomation in LDF file."
        << " Check LDF validity and parse again." << std::endl;
    }
    if (isEmptyFramesLibrary) {
        std::cerr << "Cannot parse frames infomation due to missing signals infomation in LDF file."
        << " Cannot parse signals representation infomation due to missing signals and frames infomation in LDF file."
        << " Check LDF validity and parse again." << std::endl;
    }
    if (isEmptySigEncodingTypeLibrary) {
        std::cerr << "Cannot find signal encoding types infomation in LDF file."
        << " Check LDF validity and parse again. " << std::endl;
    }
    // Reset flags
    isEmptyFramesLibrary = true; isEmptySignalsLibrary = true; isEmptySigEncodingTypeLibrary = true;
    ldfFile.close();
    return false;
}

void LdfParser::loadAndParseFromFile(std::istream& in) {
    std::string line;
    std::string lineInitial;
    // Read the file line by line
    while (getline(in, line)) {
        std::stringstream lineStream(line);
        lineInitial = Utils::getLine(lineStream, '{');
        // Look for signal encoding types
        if (lineInitial == "Signal_encoding_types") {
            // Loop through each encoding type
            std::string sigEncodingTypeName, sigEncodingTypeNameLine;
            while(getline(in, sigEncodingTypeNameLine)) {
                std::stringstream sigEncodingTypeNameLineStream(sigEncodingTypeNameLine);
                sigEncodingTypeName = Utils::getLine(sigEncodingTypeNameLineStream, '{');
                // Stop condition
                if (sigEncodingTypeName == "}") {
                    break;
                }
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
                    throw std::invalid_argument("Signal encoding type \"" + sigEncodingType.getName()
                                                + "\" has a duplicate. Parse Failed.");
                }
            }
            isEmptySigEncodingTypeLibrary = false;
        }
        else if (lineInitial == "Signals") {
            std::string sigName, sigLine;
            // Parse each signal
            while(getline(in, sigLine)) {
                std::stringstream sigLineStream(sigLine);
                sigName = Utils::getLine(sigLineStream, ':');
                // Stop condition
                if (sigName == "}") {
                    break;
                }
                // Parse signal info
                int sigSize, initValue;
                std::string publisher, subscriber;
                std::vector<std::string> subscribers;
                sigSize = std::stoi(Utils::getLine(sigLineStream, ','));
                initValue = std::stoi(Utils::getLine(sigLineStream, ','));
                publisher = Utils::getLine(sigLineStream, ',');
                subscriber = Utils::getLine(sigLineStream, ',');
                while (subscriber != "") {
                    // Remove semi colon if there exists one
                    char lastCharOfSubscriber = subscriber.back();
                    if ((lastCharOfSubscriber == ';') && (!subscriber.empty())){
                        // Remove trailling colon and white spaces
                        subscriber.pop_back();
                        Utils::trim(subscriber);
                    }
                    subscribers.push_back(subscriber);
                    subscriber = Utils::getLine(sigLineStream, ',');
                }
                // Store signal info
                Signal sig;
                sig.setName(sigName);
                sig.setSignalSize(sigSize);
                sig.setInitValue(initValue);
                sig.setPublisher(publisher);
                sig.setSubscribers(subscribers);
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
        }
        else if ((lineInitial == "Frames") && (!isEmptySignalsLibrary)){
            
            // Loop through frames
            std::string frameName, frameNameLine;
            while(getline(in, frameNameLine)) {
                std::stringstream frameNameLineStream(frameNameLine);
                frameName = Utils::getLine(frameNameLineStream, ':');
                // Stop condition
                if (frameName == "}") {
                    break;
                }
                // Parse frame info
                int id, messageSize;
                std::string publisher;
                id = std::stoi(Utils::getLine(frameNameLineStream, ','));
                publisher = Utils::getLine(frameNameLineStream, ',');
                messageSize = std::stoi(Utils::getLine(frameNameLineStream, '{'));
                Frame frm;
                frm.setId(id);
                frm.setMessageSize(messageSize);
                frm.setName(frameName);
                frm.setPublisher(publisher);
                // Parse its connected signals
                std::string sigName, sigLine;
                while(getline(in, sigLine)) {
                    std::stringstream sigLineStream(sigLine);
                    sigName = Utils::getLine(sigLineStream, ',');
                    // Stop condition
                    if (sigName == "}") {
                        break;
                    }
                    int startBit = std::stoi(Utils::getLine(sigLineStream, ';'));
                    frm.addSignalInfo(std::make_pair(sigName, startBit));
                }
                // Store signals start bit
                for (size_t i = 0; i < frm.getSignalsName().size(); i++) {
                    signalsLibrary_iterator data_itr = signalsLibrary.find(frm.getSignalsName()[i].first);
                    if (data_itr == signalsLibrary.end()) {
                        throw std::invalid_argument("Cannot find signal \"" + frm.getSignalsName()[i].first
                                                    + "\" under frame \"" + frm.getName() + "\". Parse Failed.");
                    }
                    else {
                        data_itr->second.setStartBit(frm.getSignalsName()[i].second);
                    }
                }
                // Store frame info into container
                framesLibrary_iterator data_itr = framesLibrary.find(frm.getId());
                if (data_itr == framesLibrary.end()) {
                    // Uniqueness check passed, store the signal
                    framesLibrary.insert(std::make_pair(frm.getId(), frm));
                }
                else {
                    throw std::invalid_argument("Frame \"" + frm.getName() + "\" has a duplicate. Parse Failed.");
                }
                
            }
            isEmptyFramesLibrary = false;
        }
        else if ((lineInitial == "Signal_representation")
                 && (!isEmptySignalsLibrary) && (!isEmptySigEncodingTypeLibrary)) {
            
            // Get all signal representations
            std::string sigRepresentations = Utils::getLine(in, '}');
            std::string singleSigRepresentation;
            std::stringstream sigRepresentationsStream(sigRepresentations);
            // Loop through each signal representation
            singleSigRepresentation = Utils::getLine(sigRepresentationsStream, ';');
            while (singleSigRepresentation != "") {
                std::stringstream singleSigRepresentationStream(singleSigRepresentation);
                std::string encodingTypeName = Utils::getLine(singleSigRepresentationStream, ':');
                // Loop through each subscriber
                std::string subscriber = Utils::getLine(singleSigRepresentationStream, ',');
                while (subscriber != "") {
                    // Get subscriber name. Remove semi colon if there exists one
                    char lastCharOfSubscriber = subscriber.back();
                    if ((lastCharOfSubscriber == ';') && (!subscriber.empty())){
                        // Remove trailling colon and white spaces
                        subscriber.pop_back();
                        Utils::trim(subscriber);
                    }
                    // Find the corresponding signal encoding type in sigEncodingTypeLibrary
                    sigEncodingTypeLibrary_iterator data_itr_encode_type = sigEncodingTypeLibrary.find(encodingTypeName);
                    if (data_itr_encode_type == sigEncodingTypeLibrary.end()) {
                        throw std::invalid_argument("Cannot find signal encoding type \"" + encodingTypeName
                                                    + "\". Parse Failed.");
                    }
                    SignalEncodingType* encodingType_ptr = &(data_itr_encode_type->second);
                    // Link the signal encoding type to the signal
                    signalsLibrary_iterator data_itr = signalsLibrary.find(subscriber);
                    if (data_itr == signalsLibrary.end()) {
                        throw std::invalid_argument("Cannot find signal \"" + subscriber
                                                    + "\" under signal encoding type \""
                                                    + encodingTypeName + "\". Parse Failed.");
                    }
                    else {
                        data_itr->second.setEncodingType(encodingType_ptr);
                    }
                    // Get next subscriber
                    subscriber = Utils::getLine(singleSigRepresentationStream, ',');
                }
                // Get next signal representation
                singleSigRepresentation = Utils::getLine(sigRepresentationsStream, ';');
            }
            // TODO: Integrity check: all signals should have corresponding encoding type
            
        }
    }
    
}

std::ostream& operator<<(std::ostream& os, const LdfParser& ldfFile) {
    if (ldfFile.isEmptyLibrary) {
        std::cout << "Parse LDF file first before printing its info." << std::endl;
        return os;
    }
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

std::map<std::string, double> LdfParser::decode(int& msgId, unsigned char payLoad[], int& dlc){
    std::map<std::string, double> decodedValues, emptyResult;
    // Check if parser has info
    if (isEmptyLibrary) {
        std::cout << "Parse LDF file first before decoding frames." << std::endl;
        return emptyResult;
    }
    framesLibrary_iterator data_itr_frm = framesLibrary.find(msgId);
    // Find the frame that needs to be decoded
    if (data_itr_frm == framesLibrary.end()) {
        std::cerr << "No matching frame found. Decode failed. An empty result is returned." << std::endl;
        return emptyResult;
    }
    else {
        if (dlc != data_itr_frm->second.getDlc()) {
            std::cerr << "The data length of the input payload does not match with LDF info. Decode failed. An empty result is returned." << std::endl;
            return emptyResult;
        }
        std::vector<std::pair<std::string, int> > signalsName = framesLibrary[msgId].getSignalsName();
        // Decode each signal under the frame
        for (size_t i = 0; i < signalsName.size(); i++) {
            signalsLibrary_iterator data_itr_sig = signalsLibrary.find(signalsName[i].first);
            if (data_itr_sig == signalsLibrary.end()) {
                std::cerr << "No matching signal found. Decode failed. An empty result is returned." << std::endl;
                return emptyResult;
            }
            else { // Decode signal
                double decodedSignalValue = data_itr_sig->second.decodeSignal(payLoad, data_itr_frm->second.getDlc());
                decodedValues.insert(std::make_pair(data_itr_sig->first, decodedSignalValue));
            }
        }
    }
    return decodedValues;
}
