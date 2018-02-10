#ifndef KUWS_UTILS_H_INCLUDED
#define KUWS_UTILS_H_INCLUDED

#include <cctype>
#include <string>
#include <algorithm>
namespace r7 {
    inline std::string trim(const std::string &s)
    {
       auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
       auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
       return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
    }
}


#endif // KUWS_UTILS_H_INCLUDED
