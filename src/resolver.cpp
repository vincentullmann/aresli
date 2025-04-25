// IMPORT THIRD-PARTY LIBRARIES
#include <pxr/usd/ar/defaultResolver.h>
#include <pxr/usd/ar/defineResolver.h>

// IMPORT STD LIBRARIES
#include <cstdlib>  // for std::getenv
#include <iostream>
#include <regex>
#include <string>
#include <vector>

// IMPORT LOCAL LIBRARIES
#include "env_vars.h"
#include "resolver.h"


PXR_NAMESPACE_OPEN_SCOPE


AR_DEFINE_RESOLVER(Aresli, ArResolver)


ArResolvedPath Aresli::_Resolve(const std::string &input) const {
    std::string path = input;

    path = replace_env_vars(path);
    return ArResolvedPath(path);
}

PXR_NAMESPACE_CLOSE_SCOPE
