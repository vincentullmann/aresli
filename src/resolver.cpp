// IMPORT THIRD-PARTY LIBRARIES
#include <pxr/usd/ar/defaultResolver.h>
#include <pxr/usd/ar/defineResolver.h>

// IMPORT LOCAL LIBRARIES
#include "resolver.h"

// IMPORT STD LIBRARIES
#include <cstdlib>  // for std::getenv
#include <iostream>
#include <regex>
#include <string>
#include <vector>

PXR_NAMESPACE_OPEN_SCOPE

AR_DEFINE_RESOLVER(MyAssetResolver, ArResolver)

/**
 * @brief Replace environment variable placeholders
 *
 * Replaces placeholders in the format `{env:VAR}` or `{env:VAR:default}` within
 * the input string with the corresponding environment variable value or a
 * fallback if the variable is not set.
 *
 * @param path The input string potentially containing environment variable
 * placeholders.
 * @return A new string with all placeholders replaced with their respective
 * values or fallbacks.
 */
std::string replace_env_vars(const std::string &path) {

    // static const std::regex pattern(R"(\{env:(\w+)(:\w*)?\})");
    static const std::regex pattern(R"(\{env:([a-zA-Z_]\w*)(?::([^}]*))?\})");

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


ArResolvedPath MyAssetResolver::_Resolve(const std::string &input) const {

    std::string path = input;

    path = replace_env_vars(path);
    return ArResolvedPath(path);
}

PXR_NAMESPACE_CLOSE_SCOPE
