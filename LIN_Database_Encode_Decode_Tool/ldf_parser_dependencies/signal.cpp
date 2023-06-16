//
//  signal.cpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#include <fstream>
#include <iostream>
#include "signal.hpp"

std::istream& operator>>(std::istream& in, Signal& sig) {
    char semi; std::string subscriber; bool hasSubscriber = false;
    // Read signal size and initial value
    in >> sig.signalSize >> semi >> sig.initValue >> semi >> sig.publisher;
    while (in >> subscriber && subscriber != ";") {
        hasSubscriber = true;
        // Adapt to different format of a ldf file
        if (!subscriber.empty()) {
            char lastChar = subscriber.back();
            if (lastChar == ';') {
                subscriber.pop_back();
                sig.subscribers.push_back(subscriber);
                break;
            }
        }
        sig.subscribers.push_back(subscriber);
    }
    // Remove publisher's trailling colon
    if (!sig.publisher.empty() && hasSubscriber){
        sig.publisher.pop_back();
    }
    // Remove subscribers' extranious trailling comma
    if (sig.subscribers.size() > 1) {
        for (size_t i = 0; i < sig.subscribers.size()-1; i++) {
            sig.subscribers[i].pop_back();
        }
    }
    return in;
}

std::ostream& operator<<(std::ostream& os, const Signal& sig){
    std::cout << "[Signal] " << sig.name << ": " << std::endl;
    std::cout << "\t" << std::left << std::setw(20) << "size: " << sig.signalSize << std::endl;
    std::cout << "\t" << std::left << std::setw(20) << "initial value: " << sig.initValue << std::endl;
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
