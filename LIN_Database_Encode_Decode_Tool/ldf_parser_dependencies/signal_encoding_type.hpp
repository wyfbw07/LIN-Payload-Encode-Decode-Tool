//
//  signal_encoding_type.hpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#ifndef signal_encoding_type_hpp
#define signal_encoding_type_hpp

#include <array>
#include <string>
#include <vector>

enum class ValueType {
	LogicalValue,
	PhysicalValue,
	NotSet
};

class SignalEncodingType {

public:

	// Getters
	std::string getName() const { return name; }
	std::string getUnitFromRawValue(int64_t rawValue) const { return std::get<1>(getTypeInfoFromRawValue(rawValue)); }
    double getOffsetFromPhysicalValue(double physicalValue) const { return std::get<0>(getTypeInfoFromPhysicalValue(physicalValue))[3]; }
    double getFactorFromPhysicalValue(double physicalValue) const { return std::get<0>(getTypeInfoFromPhysicalValue(physicalValue))[2]; }
	int getMaxValueFromPhysicalValue(double physicalValue) const { return std::get<0>(getTypeInfoFromPhysicalValue(physicalValue))[1]; }
	int getMinValueFromPhysicalValue(double physicalValue) const { return std::get<0>(getTypeInfoFromPhysicalValue(physicalValue))[0]; }
	double getOffsetFromRawValue(int64_t rawValue) const { return std::get<0>(getTypeInfoFromRawValue(rawValue))[3]; }
	double getFactorFromRawValue(int64_t rawValue) const { return std::get<0>(getTypeInfoFromRawValue(rawValue))[2]; }
	int getMaxValueFromRawValue(int64_t rawValue) const { return std::get<0>(getTypeInfoFromRawValue(rawValue))[1]; }
	int getMinValueFromRawValue(int64_t rawValue) const { return std::get<0>(getTypeInfoFromRawValue(rawValue))[0]; }
	ValueType getValueTypeFromRawValue(int64_t rawValue) const { return std::get<2>(getTypeInfoFromRawValue(rawValue)); }

	// Setter
	void setName(const std::string& name) { this->name = name; }
	// Overloads
	friend std::istream& operator>>(std::istream& in, SignalEncodingType& sigEncodingType);
	friend std::ostream& operator<<(std::ostream& os, const SignalEncodingType& sigEncodingType);

private:

	std::string name;
	std::vector<std::tuple<std::array<double, 4>, std::string, ValueType> > encodingTypes;
	std::tuple<std::array<double, 4>, std::string, ValueType> getTypeInfoFromRawValue(int64_t& rawValue) const;
	std::tuple<std::array<double, 4>, std::string, ValueType> getTypeInfoFromPhysicalValue(double& physicalValue) const;

};

#endif /* signal_encoding_type_hpp */
