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

std::ostream& operator<<(std::ostream& os, const Signal& sig){
    std::cout << "[Signal] " << sig.name << ": " << std::endl;
    std::cout << "\t" << std::left << std::setw(20) << "size: " << sig.signalSize << std::endl;
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
    else{
        std::cout << "\tNo subscribers";
    }
    return os;
}

double Signal::decodeSignal(unsigned char rawPayload[MAX_MSG_LEN], int messageSize){
    // Change endianness
    int64_t payload = 0;
    for(int i = MAX_MSG_LEN; i > 0; i--) {
        payload <<= 8;
        payload |= (uint64_t)rawPayload[i-1];
    }
    // Decode raw value
    int64_t decodedValue = 0;
    uint8_t* data = (uint8_t*)&payload;
    unsigned int currentBit = startBit;
    // Access the corresponding byte and make sure we are reading a bit that is 1
    for (unsigned short bitpos = 0; bitpos < signalSize; bitpos++) {
        if (data[currentBit / CHAR_BIT] & (1 << (currentBit % CHAR_BIT))) {
            // Add dominant bit
            decodedValue |= (1ULL << bitpos);
        }
        currentBit++;
    }
    // Apply linear transformation
    int factor, offset;
    factor = encodingType->getFactor(decodedValue);
    offset = encodingType->getOffset(decodedValue);
    return (double)decodedValue * factor + offset;
}
