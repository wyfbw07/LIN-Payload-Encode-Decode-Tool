//
//  frame.cpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#include <limits>
#include <iomanip>
#include <fstream>
#include <iostream>
#include "frame.hpp"

std::ostream& operator<<(std::ostream& os, const Frame& frm) {
	std::cout << "[Frame] " << frm.name << ": " << std::endl;
	std::cout << "\t" << std::left << std::setw(20) << "id: " << frm.id << std::endl;
	std::cout << "\t" << std::left << std::setw(20) << "size: " << frm.messageSize << std::endl;
	std::cout << "\t" << std::left << std::setw(20) << "publisher: " << frm.publisher << std::endl;
	if (frm.connectedSignals.size() != 0) {
		for (size_t i = 0; i < frm.connectedSignals.size(); i++) {
			if (i == 0) {
				std::cout << "\t" << std::left << std::setw(20)
					<< "signals(start bit): "
					<< frm.connectedSignals[i]->getName() << " ("
					<< frm.connectedSignals[i]->getstartBit() << ") " << std::endl;
			}
			else {
				std::cout << "\t" << std::left << std::setw(20) << ""
					<< frm.connectedSignals[i]->getName() << " ("
					<< frm.connectedSignals[i]->getstartBit() << ") " << std::endl;
			}
		}
	}
	else {
		std::cout << "\tNo signals";
	}
	return os;
}
