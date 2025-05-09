#include "env_vars.h"

#include <cstdlib>  // for std::getenv
#include <iostream>
#include <regex>
#include <vector>


PXR_NAMESPACE_OPEN_SCOPE


// matches {env:VAR} or {env:VAR:default}
static const std::regex pattern(R"(\{env:([a-zA-Z_]\w*)(?::([^}]*))?\})");


std::string replace_env_vars(const std::string &path) {
    std::string result = path;

    // find all matches
    std::sregex_iterator begin(path.begin(), path.end(), pattern), end;
    std::vector<std::smatch> matches(begin, end);

    // Iterate in reverse order to avoid offset issues when replacing strings
    // with different lenghts
    for (auto it = matches.rbegin(); it != matches.rend(); ++it) {
        const auto &match = *it;
        const std::string var_name = match[1].str();
        const char *value = std::getenv(var_name.c_str());

        std::string replacement;
        if (value) {
            replacement = value;
        } else if (match[2].matched) {
            replacement = match[2].str();
        } else {
            replacement = var_name + " NOT FOUND";
        }

        result.replace(match.position(), match.length(), replacement);
    }

    return result;
}

PXR_NAMESPACE_CLOSE_SCOPE
