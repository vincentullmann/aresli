#pragma once

#include <pxr/pxr.h>

#include <limits>
#include <optional>
#include <string>
#include <vector>

PXR_NAMESPACE_OPEN_SCOPE

/// Given a path pattern with one or more version tokens, returns the highest matching file path.
/// Version tokens are of the form:
/// - {v}     = any version
/// - {v2+}   = version 2 and above
/// - {v2-5}  = version 2 through 5
/// - {v5-}   = version 0 through 5
/// Multiple version tokens can appear in one path.
///
/// Example:
///   "some/path/cache_{v4+}/cachefile_{v4+}.abc"
std::string resolve_version_tokens(const std::string& pattern);


PXR_NAMESPACE_CLOSE_SCOPE
