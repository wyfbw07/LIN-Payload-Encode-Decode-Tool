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

class SignalEncodingType {

public:
    
    // Getters
    std::string getName() const { return name; }
    int getMinValue(int64_t rawValue) const { return getPhysicalValuesForRawValue(rawValue).first[0]; }
    int getMaxValue(int64_t rawValue) const { return getPhysicalValuesForRawValue(rawValue).first[1]; }
    int getFactor(int64_t rawValue) const { return getPhysicalValuesForRawValue(rawValue).first[2]; }
    int getOffset(int64_t rawValue) const { return getPhysicalValuesForRawValue(rawValue).first[3]; }
    std::string getUnit(int64_t& rawValue) const { return getPhysicalValuesForRawValue(rawValue).second; }
    // Setter
    void setName(const std::string& name) { this->name = name; }
    // Overloads
    friend std::istream& operator>>(std::istream& in, SignalEncodingType& sigEncodingType);
    friend std::ostream& operator<<(std::ostream& os, const SignalEncodingType& sigEncodingType);
    
private:

    std::string name;
    std::vector<std::pair<int, std::string> > logicalValues;
    std::vector<std::pair<std::array<int, 4>, std::string> > physicalValues;
    std::pair<std::array<int, 4>, std::string> getPhysicalValuesForRawValue(int64_t& rawValue) const;

};

#endif /* signal_encoding_type_hpp */
