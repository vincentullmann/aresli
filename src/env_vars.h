#pragma once

#include <pxr/pxr.h>

#include <string>

PXR_NAMESPACE_OPEN_SCOPE

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
std::string replace_env_vars(const std::string &path);


PXR_NAMESPACE_CLOSE_SCOPE
