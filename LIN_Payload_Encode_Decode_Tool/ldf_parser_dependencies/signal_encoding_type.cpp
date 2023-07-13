//
//  signal_encoding_type.cpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#include <limits>
#include <iomanip>
#include <iostream>
#include "signal_encoding_type.hpp"
#include "ldf_parser_helper.hpp"

std::ostream& operator<<(std::ostream& os, const SignalEncodingType& sigEncodingType) {
	std::cout << "<Signal Encoding Type> " << sigEncodingType.name << ": " << std::endl;
	for (size_t i = 0; i < sigEncodingType.encodingTypes.size(); i++) {
		if (std::get<2>(sigEncodingType.encodingTypes[i]) == LinSignalEncodingValueType::PhysicalValue) {
			std::cout << "\t<Physical value> min: " << std::get<0>(sigEncodingType.encodingTypes[i])[0]
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
		else if (std::get<2>(sigEncodingType.encodingTypes[i]) == LinSignalEncodingValueType::LogicalValue) {
			std::cout << "\t<Logical value> \""
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
        // TODO: Check for byte array signals
		if (valueTypeName == "physical_value") {
			int min = utils::stoi(utils::getline(singleValueTypeStream, ','));
			int max = utils::stoi(utils::getline(singleValueTypeStream, ','));
			double factor = std::stod(utils::getline(singleValueTypeStream, ','));
			double offset = std::stod(utils::getline(singleValueTypeStream, ','));
			std::string unit = utils::getline(singleValueTypeStream, ';');
			// Remove quotation marks if unit exists
			if (unit != "") {
				std::stringstream unitStream(unit);
				unitStream >> std::quoted(unit);
			}
			// Store physical value info
			std::array<double, 4> tmpArray = { (double)min, (double)max, factor, offset };
			sigEncodingType.encodingTypes.push_back(std::make_tuple(tmpArray, unit, LinSignalEncodingValueType::PhysicalValue));
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
			std::array<double, 4> tmpArray = { (double)value, (double)value, 1, 0 };
			sigEncodingType.encodingTypes.push_back(std::make_tuple(tmpArray, valueDescription, LinSignalEncodingValueType::LogicalValue));
		}
		// Get next value type
		singleValueType = utils::getline(in, ';');
	}
	return in;
}

std::tuple<std::array<double, 4>, std::string, LinSignalEncodingValueType> SignalEncodingType::getTypeInfoFromRawValue(int64_t& rawValue) const {
	if (encodingTypes.size() != 0) {
		for (size_t i = 0; i < encodingTypes.size(); i++) {
			double min = std::get<0>(encodingTypes[i])[0];
			double max = std::get<0>(encodingTypes[i])[1];
			if ((rawValue >= min) && (rawValue <= max)) {
				return encodingTypes[i];
			}
		}
	}
	std::tuple<std::array<double, 4>, std::string, LinSignalEncodingValueType> emptyResult{};
	std::get<2>(emptyResult) = LinSignalEncodingValueType::NotSet;
	return emptyResult;
}

std::tuple<std::array<double, 4>, std::string, LinSignalEncodingValueType> SignalEncodingType::getTypeInfoFromPhysicalValue(double& physicalValue) const {
	double minForPhysicalValue;
	double maxForPhysicalValue;
	for (size_t i = 0; i < encodingTypes.size(); i++) {
		// min/max * factor + offset
		minForPhysicalValue = std::get<0>(encodingTypes[i])[0] * std::get<0>(encodingTypes[i])[2] + std::get<0>(encodingTypes[i])[3];
		maxForPhysicalValue = std::get<0>(encodingTypes[i])[1] * std::get<0>(encodingTypes[i])[2] + std::get<0>(encodingTypes[i])[3];
		if ((physicalValue >= minForPhysicalValue) && (physicalValue <= maxForPhysicalValue)) {
			return encodingTypes[i];
		}
	}
	throw std::invalid_argument("Undefined input value for signal encoding type \"" + name + "\". Encode Failed.");
}
