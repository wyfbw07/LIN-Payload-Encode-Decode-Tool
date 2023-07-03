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

// Display LDF info
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

void LdfParser::resetParsedContent() {
	// Reset Libraries
	framesLibrary = std::map<int, Frame>{};
	signalsLibrary = std::map<std::string, Signal>{};
	sigEncodingTypeLibrary = std::map<std::string, SignalEncodingType>{};
	// Reset flags
	isEmptyLibrary = true;
	isEmptyFramesLibrary = true;
	isEmptySignalsLibrary = true;
	isEmptySigEncodingTypeLibrary = true;
}

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
		loadAndParseFromFile(ldfFile);  // Parse file content
	}
	else {
		throw std::invalid_argument("Could not open LDF database file.");
		return false;
	}
	// Parse operation failed
	if (isEmptyFramesLibrary || isEmptySignalsLibrary || isEmptySigEncodingTypeLibrary) {
		if (isEmptySignalsLibrary) {
			std::cerr << "Cannot find signals infomation in LDF file." << std::endl;
			std::cerr << "Cannot parse signals representation infomation due to missing signals infomation in LDF file."
				<< " Check LDF validity and parse again." << std::endl;
		}
		if (isEmptyFramesLibrary) {
			std::cerr << "Cannot parse frames infomation due to missing signals infomation in LDF file." << std::endl;
			std::cerr << "Cannot parse signals representation infomation due to missing signals and frames infomation in LDF file."
				<< " Check LDF validity and parse again." << std::endl;
		}
		if (isEmptySigEncodingTypeLibrary) {
			std::cerr << "Cannot find signal encoding types infomation in LDF file."
				<< " Check LDF validity and parse again. " << std::endl;
		}
		resetParsedContent();
		ldfFile.close();
		return false;
	}

	// Data Integrity check: all signals should have corresponding encoding type
	for (auto signal : signalsLibrary) {
		if (signal.second.getEncodingType() == NULL) {
			std::cerr << "Signal \"" << signal.second.getName()
				<< "\" does not have a corresponding encoding type. Parse Failed." << std::endl;
			resetParsedContent();
			ldfFile.close();
			return false;
		}
	}
	// All operations and data integrity check passed, parse success
	isEmptyLibrary = false;
	ldfFile.close();
	return true;

}

// Actual implementation of parser
void LdfParser::loadAndParseFromFile(std::istream& in) {
	std::string preconditionContent;
	std::string conditionName;
	// Read until a curly bracket
	while (getline(in, preconditionContent, '{')) {
		conditionName = utils::lastTokenOf(preconditionContent);
		if (conditionName == "Signal_encoding_types") {
			std::string singleSigEncodingType = utils::getline(in, '}');
			while (singleSigEncodingType != "") {
				// Parse signal encoding type
				std::stringstream singleSigEncodingTypeStream(singleSigEncodingType);
				std::string sigEncodingTypeName = utils::getline(singleSigEncodingTypeStream, '{');
				SignalEncodingType sigEncodingType;
				sigEncodingType.setName(sigEncodingTypeName);
				singleSigEncodingTypeStream >> sigEncodingType;
				// Store signal encoding type
				sigEncodingTypeLib_iterator data_itr = sigEncodingTypeLibrary.find(sigEncodingType.getName());
				if (data_itr == sigEncodingTypeLibrary.end()) {
					// Uniqueness check passed, store the signal encoding type
					sigEncodingTypeLibrary.insert(std::make_pair(sigEncodingType.getName(), sigEncodingType));
				}
				else {
					throw std::invalid_argument("Signal encoding type \"" + sigEncodingType.getName()
						+ "\" has a duplicate. Parse Failed.");
				}
				// Get next signal encoding type
				singleSigEncodingType = utils::getline(in, '}');
			}
			isEmptySigEncodingTypeLibrary = false;
		}
		else if (conditionName == "Signals") {
			// Get all signal representations
			std::string allSignals = utils::getline(in, '}');
			std::stringstream allSignalsStream(allSignals);
			// Loop through each signal representation
			std::string singleSignal = utils::getline(allSignalsStream, ';');
			while (singleSignal != "") {
				// Parse signal
				std::stringstream singleSignalStream(singleSignal);
				Signal sig;
				singleSignalStream >> sig;
				// Store signal
				signalsLib_iterator data_itr = signalsLibrary.find(sig.getName());
				if (data_itr == signalsLibrary.end()) {
					// Uniqueness check passed, store the signal
					signalsLibrary.insert(std::make_pair(sig.getName(), sig));
				}
				else {
					throw std::invalid_argument("Signal \"" + sig.getName() + "\" has a duplicate. Parse Failed.");
				}
				// Get next signal
				singleSignal = utils::getline(allSignalsStream, ';');
			}
			isEmptySignalsLibrary = false;
		}
		else if ((conditionName == "Frames") && (!isEmptySignalsLibrary)) {
			// Loop through each frame
			std::string singleFrame = utils::getline(in, '}');
			while (singleFrame != "") {
				std::stringstream singleFrameStream(singleFrame);
				// Get name, id, publisher and frame size
				std::string frameName = utils::getline(singleFrameStream, ':');
				int id = utils::stoi(utils::getline(singleFrameStream, ','));
				std::string publisher = utils::getline(singleFrameStream, ',');
				int messageSize = utils::stoi(utils::getline(singleFrameStream, '{'));
				Frame frm;
				frm.setId(id); frm.setName(frameName);
				frm.setPublisher(publisher); frm.setMessageSize(messageSize);
				// Loop through its connected signals
				std::string singleSignal = utils::getline(singleFrameStream, ';');
				while (singleSignal != "") {
					// Get signal name and start bit
					std::stringstream singleSignalStream(singleSignal);
					std::string sigName = utils::getline(singleSignalStream, ',');
					int startBit = utils::stoi(utils::getline(singleSignalStream, ';'));
					// Store start bit and connect the signal with current frame
					signalsLib_iterator data_itr = signalsLibrary.find(sigName);
					if (data_itr == signalsLibrary.end()) {
						throw std::invalid_argument("Cannot find signal \"" + sigName
							+ "\" under frame \"" + frm.getName() + "\". Parse Failed.");
					}
					else {
						data_itr->second.setStartBit(startBit);
						frm.addSignalInfo(&(data_itr->second));
					}
					// Go to next signal
					singleSignal = utils::getline(singleFrameStream, ';');
				}
				// Store frame info into container
				framesLib_iterator data_itr = framesLibrary.find(frm.getId());
				if (data_itr == framesLibrary.end()) {
					framesLibrary.insert(std::make_pair(frm.getId(), frm));
				}
				else {
					throw std::invalid_argument("Frame \"" + frm.getName() + "\" has a duplicate. Parse Failed.");
				}
				// Get next frame
				singleFrame = utils::getline(in, '}');
			}
			isEmptyFramesLibrary = false;
		}
		else if ((conditionName == "Signal_representation")
			&& (!isEmptySignalsLibrary) && (!isEmptySigEncodingTypeLibrary)) {
			// Get all signal representations
			std::string sigRepresentations = utils::getline(in, '}');
			std::stringstream sigRepresentationsStream(sigRepresentations);
			// Loop through each signal representation
			std::string singleSigRepresentation = utils::getline(sigRepresentationsStream, ';');
			while (singleSigRepresentation != "") {
				std::stringstream singleSigRepresentationStream(singleSigRepresentation);
				std::string encodingTypeName = utils::getline(singleSigRepresentationStream, ':');
				// Loop through each subscriber
				std::string subscriber = utils::getline(singleSigRepresentationStream, ',');
				while (subscriber != "") {
					// Get subscriber name. Remove semi colon if there exists one
					char lastCharOfSubscriber = subscriber.back();
					if ((lastCharOfSubscriber == ';') && (!subscriber.empty())) {
						// Remove trailling colon and white spaces
						subscriber.pop_back();
						utils::trim(subscriber);
					}
					// Find the corresponding signal encoding type in sigEncodingTypeLibrary
					sigEncodingTypeLib_iterator data_itr_encode_type = sigEncodingTypeLibrary.find(encodingTypeName);
					if (data_itr_encode_type == sigEncodingTypeLibrary.end()) {
						throw std::invalid_argument("Cannot find signal encoding type \"" + encodingTypeName
							+ "\". Parse Failed.");
					}
					// Link the signal encoding type to the signal
					SignalEncodingType* encodingType_ptr = &(data_itr_encode_type->second);
					signalsLib_iterator data_itr = signalsLibrary.find(subscriber);
					if (data_itr == signalsLibrary.end()) {
						throw std::invalid_argument("Cannot find signal \"" + subscriber
							+ "\" under signal encoding type \""
							+ encodingTypeName + "\". Parse Failed.");
					}
					else {
						data_itr->second.setEncodingType(encodingType_ptr);
					}
					// Get next subscriber
					subscriber = utils::getline(singleSigRepresentationStream, ',');
				}
				// Get next signal representation
				singleSigRepresentation = utils::getline(sigRepresentationsStream, ';');
			}
		}
	}
}

std::map<std::string, std::tuple<double, std::string, ValueType> > LdfParser::decode(int& frameId,
	unsigned char payLoad[MAX_FRAME_LEN],
	int& dlc) {
	std::map<std::string, std::tuple<double, std::string, ValueType> > results, emptyResult;
	// Check if parser has info
	if (isEmptyLibrary) {
		std::cout << "Parse LDF file first before decoding frames." << std::endl;
		return emptyResult;
	}
	framesLib_iterator data_itr_frm = framesLibrary.find(frameId);
	// Find the frame that needs to be decoded
	if (data_itr_frm == framesLibrary.end()) {
		std::cerr << "No matching frame found. Decode failed. An empty result is returned." << std::endl;
		return emptyResult;
	}
	else {
		// Check input payload's dlc
		if (dlc != data_itr_frm->second.getDlc()) {
			std::cerr << "The data length of the input payload does not match with LDF info."
				<< " Decode failed. An empty result is returned." << std::endl;
			return emptyResult;
		}
		// Decode each signal under the frame
		std::vector<Signal*> connectedSignals = framesLibrary[frameId].getConnectedSignals();
		for (size_t i = 0; i < connectedSignals.size(); i++) {
			std::tuple<double, std::string, ValueType> decodedSignalValue = connectedSignals[i]->decodeSignal(payLoad);
			results.insert(std::make_pair(connectedSignals[i]->getName(), decodedSignalValue));
		}
	}
	return results;
}

int LdfParser::encode(int& frameId,
	std::vector<std::pair<std::string, double> > signalsToEncode,
	unsigned char encodedPayload[MAX_FRAME_LEN]) {
	// Check if parser has info
	if (isEmptyLibrary) {
		std::cout << "Parse LDF file first before encoding frames." << std::endl;
		return -1;
	}
	// Find corresponding frame to encode
	framesLib_iterator data_itr_frm = framesLibrary.find(frameId);
	if (data_itr_frm == framesLibrary.end()) {
		std::cout << "No matching frame found. Encode failed. An empty result is returned.\n" << std::endl;
		return -1;
	}
	// Get all signals under the frame
	int dlc = data_itr_frm->second.getDlc();
	std::vector<Signal*> signalsName = framesLibrary[frameId].getConnectedSignals();
	// Check input validity
	bool validInput = false;
	for (size_t i = 0; i < signalsToEncode.size(); i++) {
		for (size_t j = 0; j < signalsName.size(); j++) {
			if (signalsName[j]->getName() == signalsToEncode[i].first) {
				validInput = true;
			}
		}
		if (!validInput) {
			throw std::invalid_argument("Cannot find signal: " + signalsToEncode[i].first
				+ "\" under Frame \"" + data_itr_frm->second.getName() + "\". Encode failed.");
		}
	}
	// In LIN, recessive bit is 1. Initialize encoded value with all 1s.
	double physicalValue = 0;
	uint64_t encodedValue = 0;
	encodedValue = ~encodedValue;
	// Retrive physical value for each signal
	for (size_t i = 0; i < signalsName.size(); i++) {
		bool hasPhysicalValue = false;
		for (size_t j = 0; j < signalsToEncode.size(); j++) {
			if (signalsName[i]->getName() == signalsToEncode[j].first) {
				physicalValue = signalsToEncode[j].second;
				hasPhysicalValue = true;
			}
		}
		// If no physical value is provided upon encoding, use the intial value
        // Then encode this signal
        uint64_t singleEncodedValue;
		if (!hasPhysicalValue) {
			physicalValue = signalsName[i]->getInitValue();
            singleEncodedValue = signalsName[i]->encodeSignal(physicalValue, true);
		}
        else {
            singleEncodedValue = signalsName[i]->encodeSignal(physicalValue, false);
        }
		// Add single encoded value to encoded value
		encodedValue &= singleEncodedValue;
	}
	// Split the 64-bit encoded value into a byte array
	for (short i = 8 - 1; i >= 0; i--) {
		encodedPayload[i] = encodedValue % 256; // get the last byte
		encodedValue /= 256; // get the remainder
	}
	return dlc;
}
