//
//  signal_encoding_type.hpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#ifndef signal_encoding_type_hpp
#define signal_encoding_type_hpp

#include <string>
#include <vector>

class SignalEncodingType {

public:
    
    // Getters
    std::string getName() const { return name; }
    int getMinValue(double& rawValue) const { return getMinAndMaxValue(rawValue).first; }
    int getMaxValue(double& rawValue) const { return getMinAndMaxValue(rawValue).second; }
    // Setter
    void setName(const std::string& name) { this->name = name; }
    // Overloads
    friend std::istream& operator>>(std::istream& in, SignalEncodingType& sigEncodingType);
    friend std::ostream& operator<<(std::ostream& os, const SignalEncodingType& sigEncodingType);
    
private:

    std::string name;
    std::pair<int, int> getMinAndMaxValue(double& rawValue) const;
    std::vector<std::pair<int, std::string> > logicalValues;
    std::vector<std::pair<std::array<int, 4>, std::string> > physicalValues;

};

#endif /* signal_encoding_type_hpp */
