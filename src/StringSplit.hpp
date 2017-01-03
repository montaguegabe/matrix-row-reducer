//
//  StringSplit.hpp
//  Matrix Row Reducer
//
//  Created by Gabe Montague on 12/31/16.
//  Copyright © 2016 Gabe Montague. All rights reserved.
//

#ifndef StringSplit_h
#define StringSplit_h

#include <string>
#include <sstream>
#include <vector>

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

#endif /* StringSplit_h */
