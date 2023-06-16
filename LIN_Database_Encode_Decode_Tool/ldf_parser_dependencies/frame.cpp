//
//  frame.cpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#include <fstream>
#include <iostream>
#include "frame.hpp"

std::ostream& operator<<(std::ostream& os, const Frame& frm){
    std::cout << "[Frame] " << frm.name << ": " << std::endl;
    std::cout << "\t" << std::left << std::setw(20) << "id: " << frm.id << std::endl;
    std::cout << "\t" << std::left << std::setw(20) << "size: " << frm.messageSize << std::endl;
    std::cout << "\t" << std::left << std::setw(20) << "publisher: " << frm.publisher << std::endl;
    if (frm.signalsName.size() != 0) {
        std::cout << "\t" << std::left << std::setw(20) << "signals(start bit): " << std::endl;
        for (size_t i = 0; i < frm.signalsName.size(); i++) {
            std::cout << "\t" << std::left << std::setw(20) << "" << frm.signalsName[i].first
            << " (" << frm.signalsName[i].second << ") " << std::endl;
        }
    }
    else {
        std::cout << "\tNo signals";
    }
    return os;
}

std::istream& operator>>(std::istream& in, Frame& frm){
    char semi;
    // Read frame id, publisher, and frame size
    in >> frm.id >> semi >> frm.publisher >> frm.messageSize;
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    // Remove trailling comma
    if (!frm.publisher.empty()){
        frm.publisher.pop_back();
    }
    // Read signal info under the frame
    std::string sigName; int startBit;
    while (in >> sigName && sigName != "}") {
        // Remove trailling comma
        if (!sigName.empty()){
            sigName.pop_back();
        }
        in >> startBit;
        frm.signalsName.push_back(std::make_pair(sigName, startBit));
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return in;
}
