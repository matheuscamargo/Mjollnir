#ifndef VIGRIDR_UTILS_STRING_UTILS
#define VIGRIDR_UTILS_STRING_UTILS

#include <vector>
#include <string>
#include <sstream>

namespace mjollnir { namespace vigridr { namespace utils {

inline std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

inline std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

}}}  // namespaces

#endif  // VIGRIDR_UTILS_STRING_UTILS
