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
    int getOffsetFromPhysicalValue(int64_t physicalValue) const { return std::get<0>(getTypeInfoFromPhysicalValue(physicalValue))[3]; }
    int getFactorFromPhysicalValue(int64_t physicalValue) const { return std::get<0>(getTypeInfoFromPhysicalValue(physicalValue))[2]; }
    int getMaxValueFromPhysicalValue(int64_t physicalValue) const { return std::get<0>(getTypeInfoFromPhysicalValue(physicalValue))[1]; }
    int getMinValueFromPhysicalValue(int64_t physicalValue) const { return std::get<0>(getTypeInfoFromPhysicalValue(physicalValue))[0]; }
    int getOffsetFromRawValue(int64_t rawValue) const { return std::get<0>(getTypeInfoFromRawValue(rawValue))[3]; }
    int getFactorFromRawValue(int64_t rawValue) const { return std::get<0>(getTypeInfoFromRawValue(rawValue))[2]; }
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
    std::vector<std::tuple<std::array<int, 4>, std::string, ValueType> > encodingTypes;
    std::tuple<std::array<int, 4>, std::string, ValueType> getTypeInfoFromRawValue(int64_t& rawValue) const;
    std::tuple<std::array<int, 4>, std::string, ValueType> getTypeInfoFromPhysicalValue(int64_t& physicalValue) const;

};

#endif /* signal_encoding_type_hpp */
