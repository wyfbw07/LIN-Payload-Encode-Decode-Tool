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
    for (size_t i = 0; i < sigEncodingType.encodingTypes.size(); i++) {
        if (std::get<2>(sigEncodingType.encodingTypes[i]) == ValueType::PhysicalValue) {
            std::cout << "\t[Physical value] min: " << std::get<0>(sigEncodingType.encodingTypes[i])[0]
            << " max: " << std::get<0>(sigEncodingType.encodingTypes[i])[1]
            << " factor: " << std::get<0>(sigEncodingType.encodingTypes[i])[2]
            << " offset: " << std::get<0>(sigEncodingType.encodingTypes[i])[3];
            if (std::get<1>(sigEncodingType.encodingTypes[i]) != "") {
                std::cout << " unit: " << std::get<1>(sigEncodingType.encodingTypes[i]) << std::endl;
            }
            else {
                std::cout << std::endl;
            }
        }
        else if (std::get<2>(sigEncodingType.encodingTypes[i]) == ValueType::LogicalValue) {
            std::cout << "\t[Logical value] \""
            << std::get<0>(sigEncodingType.encodingTypes[i])[0] << "\" means \""
            << std::get<1>(sigEncodingType.encodingTypes[i]) << "\"" << std::endl;
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
            sigEncodingType.encodingTypes.push_back(std::make_tuple(tmpArray, unit, ValueType::PhysicalValue));
        }
        else if (valueTypeName == "logical_value") {
            int value = utils::stoi(utils::getline(singleValueTypeStream, ','));
            std::string valueDescription = utils::getline(singleValueTypeStream, ';');
            // Remove quotation marks
            if (valueDescription != "") {
                std::stringstream unitStream(valueDescription);
                unitStream >> std::quoted(valueDescription);
            }
            // Store logical value info. Logical values always have a factor of 1 and an offset of 0
            std::array<int, 4> tmpArray = {value, value, 1, 0};
            sigEncodingType.encodingTypes.push_back(std::make_tuple(tmpArray, valueDescription, ValueType::LogicalValue));
        }
        // Get next value type
        singleValueType = utils::getline(in, '}');
    }
    return in;
}

std::tuple<std::array<int, 4>, std::string, ValueType> SignalEncodingType::getTypeInfoFromRawValue(int64_t& rawValue) const {
    if (encodingTypes.size() != 0) {
        for (size_t i = 0; i < encodingTypes.size(); i++) {
            int min = std::get<0>(encodingTypes[i])[0];
            int max = std::get<0>(encodingTypes[i])[1];
            if ((rawValue >= min) && (rawValue <= max)) {
                return encodingTypes[i];
            }
        }
    }
    std::tuple<std::array<int, 4>, std::string, ValueType> emptyResult{};
    std::get<2>(emptyResult) = ValueType::NotSet;
    return emptyResult;
}

std::tuple<std::array<int, 4>, std::string, ValueType> SignalEncodingType::getTypeInfoFromPhysicalValue(int64_t& physicalValue) const {
    int minForPhysicalValue;
    int maxForPhysicalValue;
    for (size_t i = 0; i < encodingTypes.size(); i++) {
        // min/max * factor + offset
        minForPhysicalValue = std::get<0>(encodingTypes[i])[0] * std::get<0>(encodingTypes[i])[2] + std::get<0>(encodingTypes[i])[3];
        maxForPhysicalValue = std::get<0>(encodingTypes[i])[1] * std::get<0>(encodingTypes[i])[2] + std::get<0>(encodingTypes[i])[3];
        if ((physicalValue >= minForPhysicalValue) && (physicalValue <= maxForPhysicalValue)) {
            return encodingTypes[i];
        }
    }
    std::tuple<std::array<int, 4>, std::string, ValueType> emptyResult{};
    std::get<2>(emptyResult) = ValueType::NotSet;
    return emptyResult;
}
