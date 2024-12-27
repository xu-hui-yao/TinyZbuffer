#include <algorithm>
#include <core/common.h>
#include <iomanip>
#include <sstream>

std::vector<std::string> tokenize(const std::string &string,
                                  const std::string &delim,
                                  bool include_empty) {
    std::string::size_type last_pos = 0,
                           pos      = string.find_first_of(delim, last_pos);
    std::vector<std::string> tokens;

    while (last_pos != std::string::npos) {
        if (pos != last_pos || include_empty)
            tokens.push_back(string.substr(last_pos, pos - last_pos));
        last_pos = pos;
        if (last_pos != std::string::npos) {
            last_pos += 1;
            pos = string.find_first_of(delim, last_pos);
        }
    }

    return tokens;
}

std::string to_lower(const std::string &value) {
    std::string result;
    result.resize(value.size());
    std::transform(value.begin(), value.end(), result.begin(), ::tolower);
    return result;
}

bool to_bool(const std::string &str) {
    std::string value = to_lower(str);
    if (value == "false")
        return false;
    else if (value == "true")
        return true;
    else
        throw std::runtime_error("Could not parse boolean value " + str);
}

int to_int(const std::string &str) {
    char *end_ptr = nullptr;
    auto result   = static_cast<int>(strtol(str.c_str(), &end_ptr, 10));
    if (*end_ptr != '\0')
        throw std::runtime_error("Could not parse integer value " + str);
    return result;
}

unsigned int to_uint(const std::string &str) {
    char *end_ptr = nullptr;
    auto result = static_cast<unsigned int>(strtoul(str.c_str(), &end_ptr, 10));
    if (*end_ptr != '\0')
        throw std::runtime_error("Could not parse unsigned int value " + str);
    return result;
}

float to_float(const std::string &str) {
    char *end_ptr = nullptr;
    auto result   = strtof(str.c_str(), &end_ptr);
    if (*end_ptr != '\0')
        throw std::runtime_error("Could not parse float value " + str);
    return result;
}

std::string time_string(double time, bool precise) {
    if (std::isnan(time) || std::isinf(time))
        return "inf";

    std::string suffix = "ms";
    if (time > 1000) {
        time /= 1000;
        suffix = "s";
        if (time > 60) {
            time /= 60;
            suffix = "m";
            if (time > 60) {
                time /= 60;
                suffix = "h";
                if (time > 12) {
                    time /= 12;
                    suffix = "d";
                }
            }
        }
    }

    std::ostringstream os;
    os << std::setprecision(precise ? 4 : 1) << std::fixed << time << suffix;

    return os.str();
}
