#include "version_token.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <regex>
#include <tuple>
#include <utility>


namespace fs = std::filesystem;

PXR_NAMESPACE_OPEN_SCOPE


static const std::regex PATTERN = std::regex(R"(\{v(\d+)?([+-])?(\d+)?\})");


//*****************************************************************************
// VersionConstraint


/// Represents a version range constraint (e.g. {v}, {v2+}, {v2-5}, {v5-})
struct VersionConstraint {
    uint16_t min = 0;
    uint16_t max = std::numeric_limits<uint16_t>::max();

    /// Parses a version token (e.g. "{v4+}") into a VersionConstraint.
    static VersionConstraint parse(const std::string& token);

    /// Checks if a version is allowed by this constraint.
    bool matches(uint16_t version) const {
        return version >= this->min && version <= this->max;
    }
};


VersionConstraint VersionConstraint::parse(const std::string& token) {
    std::smatch match;
    std::regex_search(token, match, PATTERN);

    if (match[1].str().empty()) {
        return {};  // {v}
    }

    const uint16_t v1 = std::stoi(match[1]);
    const std::string symbol = match[2];


    if (match[3].matched) {
        const uint16_t v2 = std::stoi(match[3]);
        return {v1, v2};
    }

    if (symbol == "+") {
        return {v1};
    }

    if (symbol == "-") {
        return {0, v1};
    }

    return {v1, v1};  // exact version
    // throw std::runtime_error("Invalid token: " + token);
}


//*****************************************************************************
//

std::string resolve_version_tokens(const std::string& input) {
    fs::path path = input;
    std::string filename = path.filename().string();
    std::string folder = path.parent_path().string();

    if (!fs::exists(folder) || !fs::is_directory(folder)) {
        return input;
    }

    // matched entries and their version numbers
    std::vector<std::pair<fs::directory_entry, int>> candidates;

    const VersionConstraint constraint = VersionConstraint::parse(filename);

    // Convert version token to regex
    // const std::regex filename_pattern("geo_v(\\d+).usda");
    const std::regex filename_pattern(std::regex_replace(filename, PATTERN, "v(\\d+)"));

    for (const auto& entry : fs::directory_iterator(folder)) {
        std::string entryName = entry.path().filename().string();

        std::smatch match;
        if (std::regex_match(entryName, match, filename_pattern)) {
            // Extract the version number from the filename
            uint16_t version = 0;
            if (match[1].matched) {
                version = std::stoi(match[1].str());
            }

            std::cout << "version: " << version << std::endl;


            if (constraint.matches(version)) {
                candidates.emplace_back(entry, version);
            }
        }
    }  // for entry

    // no candidates found
    if (candidates.empty()) {
        return input;
    }

    // Sort candidates by version number
    std::sort(candidates.begin(), candidates.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    return candidates.front().first.path().string();
}

PXR_NAMESPACE_CLOSE_SCOPE
