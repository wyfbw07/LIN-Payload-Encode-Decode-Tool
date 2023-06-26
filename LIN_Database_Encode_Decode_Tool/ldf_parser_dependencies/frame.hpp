//
//  frame.hpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#ifndef frame_hpp
#define frame_hpp

#include <map>
#include <string>
#include <vector>
#include "signal.hpp"

class Frame {

public:

    // Getters
    int getId() const { return id; }
    int getDlc() const { return messageSize; }
    std::string getName() const { return name; }
    std::vector<std::pair<std::string, int> > getSignalsName() const { return signalsName; }
    // Setters
    void setId(const int& id) { this->id = id; }
    void setName(const std::string& name) { this->name = name; }
    void setMessageSize(const int& messageSize) { this->messageSize = messageSize; }
    void setPublisher(const std::string& publisher) { this->publisher = publisher; }
    void addSignalInfo (const std::pair<std::string, int>& sigName) {this->signalsName.push_back(sigName); }
    // Overloads
    friend std::istream& operator>>(std::istream& in, Frame& frm);
    friend std::ostream& operator<<(std::ostream& os, const Frame& frm);

private:

    int id{};
    int messageSize{};
    std::string name{};
    std::string publisher;
    std::vector<std::pair<std::string, int> > signalsName;
    
};

#endif /* frame_hpp */
