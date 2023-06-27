//
//  signal.hpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#ifndef SIGNAL_H
#define SIGNAL_H

#include <map>
#include <string>
#include <vector>
#include "signal_encoding_type.hpp"

constexpr int MAX_MSG_LEN = 8;
constexpr int MAX_BIT_INDEX_uint64_t = (sizeof(uint64_t) * CHAR_BIT) - 1;

class Signal {

public:

    // Getters
    std::string getName() const { return name; }
    unsigned int getInitValue() const { return initValue; }
    unsigned int getSignalSize() const { return signalSize; }
    // Setters
    void setName(const std::string& name) { this->name = name; }
    void setStartBit(const int& startBit) { this->startBit = startBit; }
    void setInitValue(const int& initValue) { this->initValue = initValue; }
    void setSignalSize(const int& signalSize) { this->signalSize = signalSize; }
    void setPublisher(const std::string& publisher) { this->publisher = publisher; }
    void setEncodingType(SignalEncodingType* encodingType) {this->encodingType = encodingType; }
    void setSubscribers(const std::vector<std::string>& subscribers) { this->subscribers = subscribers; }
    double decodeSignal(unsigned char rawPayload[MAX_MSG_LEN], int messageSize);
    // Operator overload, allows parsing of signals info
    friend std::istream& operator>>(std::istream& in, Signal& sig);
    friend std::ostream& operator<<(std::ostream& os, const Signal& sig);

private:

    // Initialize with invalid values
    int startBit = -1;
    int initValue{};
    int signalSize{};
    std::string name{};
    std::string publisher{};
    SignalEncodingType* encodingType{};
    std::vector<std::string> subscribers;
    
};

#endif /* SIGNAL_H */
