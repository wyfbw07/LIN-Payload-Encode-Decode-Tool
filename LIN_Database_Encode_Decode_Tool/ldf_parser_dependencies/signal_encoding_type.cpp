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
#include "ldf_parser_helper.hpp"

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

std::istream& operator>>(std::istream& in, SignalEncodingType& sigEncodingType) {
    // Loop through each value type
    std::string singleValueType = utils::getline(in, ';');
    while (singleValueType != "") {
        std::stringstream singleValueTypeStream(singleValueType);
        std::string valueTypeName = utils::getline(singleValueTypeStream, ',');
        if (valueTypeName == "physical_value") {
            int min = utils::stoi(utils::getline(singleValueTypeStream, ','));
            int max = utils::stoi(utils::getline(singleValueTypeStream, ','));
            int factor = utils::stoi(utils::getline(singleValueTypeStream, ','));
            int offset = utils::stoi(utils::getline(singleValueTypeStream, ','));
            std::string unit = utils::getline(singleValueTypeStream, ';');
            // Remove quotation marks if unit exists
            if (unit != "") {
                std::stringstream unitStream(unit);
                unitStream >> std::quoted(unit);
            }
            // Store physical value info
            std::array<int, 4> tmpArray = {min, max, factor, offset};
            sigEncodingType.physicalValues.push_back(std::make_pair(tmpArray, unit));
        }
        else if (valueTypeName == "logical_value") {
            int value = utils::stoi(utils::getline(singleValueTypeStream, ','));
            std::string valueDescription = utils::getline(singleValueTypeStream, ';');
            // Remove quotation marks
            if (valueDescription != "") {
                std::stringstream unitStream(valueDescription);
                unitStream >> std::quoted(valueDescription);
            }
            sigEncodingType.logicalValues.push_back(std::make_pair(value, valueDescription));
        }
        // Get next value type
        singleValueType = utils::getline(in, '}');
    }
    return in;
}

std::pair<std::array<int, 4>, std::string> SignalEncodingType::getPhysicalValuesForRawValue(int64_t& rawValue) const {
    if (physicalValues.size() != 0) {
        for (size_t i = 0; i < physicalValues.size(); i++) {
            int min = (physicalValues[i].first)[0];
            int max = (physicalValues[i].first)[1];
            if ((rawValue > min) && (rawValue < max)) {
                return physicalValues[i];
            }
        }
    }
    std::pair<std::array<int, 4>, std::string> emptyResult{};
    return emptyResult;
}
