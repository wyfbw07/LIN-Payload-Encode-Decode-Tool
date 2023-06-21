//
//  signal_encoding_type.cpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#include <limits>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "signal_encoding_type.hpp"

std::pair<std::array<int, 4>, std::string> SignalEncodingType::getPhysicalValuesForRawValue(int64_t& rawValue) const {
    int min, max;
    if (physicalValues.size() != 0) {
        for (size_t i = 0; i < physicalValues.size(); i++) {
            min = (physicalValues[i].first)[0];
            max = (physicalValues[i].first)[1];
            if ((rawValue > min) && (rawValue < max)) {
                return physicalValues[i];
            }
        }
    }
    std::pair<std::array<int, 4>, std::string> emptyResult{};
    return emptyResult;
}

std::istream& operator>>(std::istream& in, SignalEncodingType& sigEncodingType) {
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string valueType; char semi;
    while (in >> valueType && valueType != "}") {
        // Read info based on value types
        if (valueType == "physical_value,") {
            int min, max, factor, offset;  std::string unit;
            in >> min >> semi >> max >> semi >> factor >> semi >> offset >> semi;
            // if unit description exists, read it
            if (semi == ',') {
                in >> std::quoted(unit);
            }
            // Store physical value info
            std::array<int, 4> tmpArray = {min, max, factor, offset};
            sigEncodingType.physicalValues.push_back(std::make_pair(tmpArray, unit));
        }else if (valueType == "logical_value,") {
            int value; std::string valueDescription;
            in >> value >> semi >> std::quoted(valueDescription);
            sigEncodingType.logicalValues.push_back(std::make_pair(value, valueDescription));
        }
    }
    return in;
}

std::ostream& operator<<(std::ostream& os, const SignalEncodingType& sigEncodingType) {
    std::cout << "[Signal Encoding Type] " << sigEncodingType.name << ": " << std::endl;
    if (sigEncodingType.logicalValues.size() != 0) {
        for (size_t i = 0; i < sigEncodingType.logicalValues.size(); i++) {
            std::cout << "\t[Logical value] \"" << sigEncodingType.logicalValues[i].first
            << "\" means \"" << sigEncodingType.logicalValues[i].second << "\"" << std::endl;
        }
    }
    if (sigEncodingType.physicalValues.size() != 0) {
        for (size_t i = 0; i < sigEncodingType.physicalValues.size(); i++) {
            std::cout << "\t[Physical value] min: " << sigEncodingType.physicalValues[i].first[0]
            << " max: " << (sigEncodingType.physicalValues[i].first)[1]
            << " factor: " << (sigEncodingType.physicalValues[i].first)[2]
            << " offset: " << (sigEncodingType.physicalValues[i].first)[3];
            if (sigEncodingType.physicalValues[i].second != "") {
                std::cout << " unit: " << sigEncodingType.physicalValues[i].second << std::endl;
            }
            else {
                std::cout << std::endl;
            }
        }
    }
    return os;
}
