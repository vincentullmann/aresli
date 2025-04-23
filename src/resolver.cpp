// IMPORT THIRD-PARTY LIBRARIES
#include <pxr/usd/ar/defaultResolver.h>
#include <pxr/usd/ar/defineResolver.h>

// IMPORT LOCAL LIBRARIES
#include "resolver.h"

// IMPORT STD LIBRARIES
#include <string>
#include <iostream>


PXR_NAMESPACE_OPEN_SCOPE


AR_DEFINE_RESOLVER(URIResolver, ArResolver)


// URIResolver::URIResolver() : ArDefaultResolver() {}
// URIResolver::~URIResolver() = default;

// bool URIResolver::IsRelativePath(const std::string& path) {
    //     return false;
    // }
    // Split string
    // std::vector<std::string> path = TfStringSplit(uri.substr(index + 1), "/");


ArResolvedPath URIResolver::_Resolve(const std::string& path) const {

    std::string JOB = std::getenv("JOB");

    std::cout << "URIResolver::Resolve: " << path << std::endl;
    std::cerr << "URIResolver::Resolve: " << path << std::endl;
    printf("Passed!\n");

    if (path == "foo:apple") {
        return ArResolvedPath(JOB + "/yeyeyeyeyey");
    }
    return {};
}


PXR_NAMESPACE_CLOSE_SCOPE
