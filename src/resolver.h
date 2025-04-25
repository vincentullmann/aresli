#pragma once

// IMPORT THIRD-PARTY LIBRARIES
#include <pxr/usd/ar/defaultResolver.h>

#include <string>


PXR_NAMESPACE_OPEN_SCOPE


class Aresli : public ArDefaultResolver {
    public:
    Aresli() = default;
    ~Aresli() override = default;

    // bool IsRelativePath(const std::string& path) override;

    protected:
    ArResolvedPath _Resolve(const std::string &path) const;
};

PXR_NAMESPACE_CLOSE_SCOPE
