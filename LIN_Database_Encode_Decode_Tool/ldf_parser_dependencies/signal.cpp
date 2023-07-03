//
//  signal.cpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include "signal.hpp"
#include "ldf_parser_helper.hpp"

std::ostream& operator<<(std::ostream& os, const Signal& sig) {
	std::cout << "<Signal> " << sig.name << ": " << std::endl;
	std::cout << "\t" << std::left << std::setw(20) << "size: " << sig.signalSize << std::endl;
	std::cout << "\t" << std::left << std::setw(20) << "start bit: " << sig.startBit << std::endl;
	std::cout << "\t" << std::left << std::setw(20) << "initial value: " << sig.initValue << std::endl;
	std::cout << "\t" << std::left << std::setw(20) << "Encode type: " << sig.encodingType->getName() << std::endl;
	std::cout << "\t" << std::left << std::setw(20) << "publisher: " << sig.publisher << std::endl;
	if (sig.subscribers.size() != 0) {
		std::cout << "\t" << std::left << std::setw(20) << std::to_string(sig.subscribers.size()) + " subscriber(s): ";
		for (size_t i = 0; i < sig.subscribers.size(); i++) {
			std::cout << sig.subscribers[i] << ' ';
		}
		std::cout << std::endl;
	}
	else {
		std::cout << "\tNo subscribers" << std::endl;
	}
	return os;
}

std::istream& operator>>(std::istream& in, Signal& sig) {
	// Parse signal info
	std::string sigName = utils::getline(in, ':');
	int sigSize = utils::stoi(utils::getline(in, ','));
	int initValue = utils::stoi(utils::getline(in, ','));
	std::string publisher = utils::getline(in, ',');
	std::string subscriber = utils::getline(in, ',');
	std::vector<std::string> subscribers;
	while (subscriber != "") {
		// Remove semi colon if there exists one
		char lastCharOfSubscriber = subscriber.back();
		if ((lastCharOfSubscriber == ';') && (!subscriber.empty())) {
			// Remove trailling colon and white spaces
			subscriber.pop_back();
			utils::trim(subscriber);
		}
		subscribers.push_back(subscriber);
		subscriber = utils::getline(in, ',');
	}
	// Store signal info
	sig.setName(sigName);
	sig.setSignalSize(sigSize);
	sig.setInitValue(initValue);
	sig.setPublisher(publisher);
	sig.setSubscribers(subscribers);
	return in;
}

std::tuple<double, std::string, ValueType> Signal::decodeSignal(unsigned char rawPayload[MAX_FRAME_LEN]) {
	// Change endianness
	int64_t payload = 0;
	for (int i = MAX_FRAME_LEN; i > 0; i--) {
		payload <<= 8;
		payload |= (uint64_t)rawPayload[i - 1];
	}
	// Decode raw value
	int64_t decodedRawValue = 0;
	uint8_t* data = (uint8_t*)&payload;
	unsigned int currentBit = startBit;
	// Access the corresponding byte and make sure we are reading a bit that is 1
	for (unsigned short bitpos = 0; bitpos < signalSize; bitpos++) {
		if (data[currentBit / CHAR_BIT] & (1 << (currentBit % CHAR_BIT))) {
			decodedRawValue |= (1ULL << bitpos);
		}
		currentBit++;
	}
	// Apply linear transformation
	ValueType sigValueType = encodingType->getValueTypeFromRawValue(decodedRawValue);
	std::string unit = encodingType->getUnitFromRawValue(decodedRawValue);
	double factor = encodingType->getFactorFromRawValue(decodedRawValue);
	double offset = encodingType->getOffsetFromRawValue(decodedRawValue);
	double decodedPhysicalValue = (double)decodedRawValue * factor + offset;
	return std::make_tuple(decodedPhysicalValue, unit, sigValueType);
}

uint64_t Signal::encodeSignal(double physicalValue) {
	// Reverse linear conversion rule
	// to convert the signals physical value into the signal's raw value
	unsigned int currentBit = 0;
	uint64_t encodedValue = 0; encodedValue = ~encodedValue;
	// TODO: Need overhual, check for logical values
	int offset = encodingType->getOffsetFromPhysicalValue(physicalValue);
	int factor = encodingType->getFactorFromPhysicalValue(physicalValue);
	uint64_t rawValue = (uint64_t)(physicalValue - offset) / factor;
	uint8_t* rawPayload = (uint8_t*)&rawValue;
	// Encode
	for (unsigned short bitpos = 0; bitpos < signalSize; bitpos++) {
		// Access the corresponding byte and make sure we are reading a dominant bit 0
		if (!(rawPayload[currentBit / CHAR_BIT] & (1 << (currentBit % CHAR_BIT)))) {
			// Add dominant bit
			encodedValue &= ~(1ULL << (bitpos + startBit));
		}
		currentBit++;
	}
	// Change endianness
	unsigned char encodedPayload[MAX_FRAME_LEN];
	for (short i = 8 - 1; i >= 0; i--) {
		encodedPayload[i] = encodedValue % 256; // get the last byte
		encodedValue /= 256; // get the remainder
	}
	encodedValue = 0;
	for (int i = MAX_FRAME_LEN; i > 0; i--) {
		encodedValue <<= 8;
		encodedValue |= (uint64_t)encodedPayload[i - 1];
	}
	return encodedValue;
}
