//
//  signal.hpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#ifndef SIGNAL_H
#define SIGNAL_H

#include <string>
#include <vector>
#include <limits>
#include <iosfwd>
#include <unordered_map>

constexpr int MAX_MSG_LEN = 8;
constexpr int MAX_BIT_INDEX_uint64_t = (sizeof(uint64_t) * CHAR_BIT) - 1;

// Respresents a signal. Contains all signal info.
class Signal {

public:

    // Getters
    std::string getName() const { return name; }
    unsigned int getInitValue() const { return initValue; }
    unsigned int getSignalSize() const { return signalSize; }
    // Setters
    void setName(const std::string& name) { this->name = name; }
    void setStartBit(const int& startBit) { this->startBit = startBit; }
    // Operator overload, allows parsing of signals info
    friend std::istream& operator>>(std::istream& in, Signal& sig);
    friend std::ostream& operator<<(std::ostream& os, const Signal& sig);

private:

    int initValue{};
    int startBit{};
    int signalSize{};
    std::string name{};
    std::string publisher{};
    std::string encodingType{};
    std::vector<std::string> subscribers;
    
};

#endif /* SIGNAL_H */
