#pragma once

#include <algorithm>
#include <string>

namespace sutils {

inline void ltrim(std::string &s) {
    s.erase(s.begin(),
            std::find_if(s.begin(),
                         s.end(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))));
}

inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(),
                         s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
            s.end());
}

inline void lrstrip(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

}
