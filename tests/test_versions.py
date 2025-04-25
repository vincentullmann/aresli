# IMPORT STANDARD LIBRARIES
import os

# IMPORT THIRD PARTY LIBRARIES
from pxr import Ar


"""Folder containing test files.
>>> touch foo_v3.usda foo_v6.usda
>>> touch geo_v1.usda geo_v2.usda geo_v3.2.usda geo_v3.usda geo_v5.usda
>>> touch other.usda
"""
TMP_DIR = os.path.join(os.path.dirname(__file__), "files")


def resolve(value: str) -> str:
    """Resolve a string using the custom resolver."""

    path = os.path.join(TMP_DIR, value)
    resolved = str(Ar.GetResolver().Resolve(path))
    resolved = os.path.basename(resolved)
    return resolved


# === Basic Resolution ===


def test_no_placeholder_is_untouched():
    assert resolve("geo_v1.usda") == "geo_v1.usda"


def test_unrelated_file_not_resolved():
    assert resolve("other.usda") == "other.usda"


# === Placeholder Syntax: {v} - Match highest available version ===


def test_any_version_resolves_highest():
    assert resolve("geo_{v}.usda") == "geo_v5.usda"


def test_any_version_resolves_highest_foo():
    assert resolve("foo_{v}.usda") == "foo_v6.usda"


# === Placeholder Syntax: {vN+} - Match version N or higher ===


def test_version_or_higher_match():
    assert resolve("geo_{v2+}.usda") == "geo_v5.usda"


def test_version_or_higher_exact():
    assert resolve("geo_{v5+}.usda") == "geo_v5.usda"


def test_version_or_higher_too_high():
    assert resolve("geo_{v8+}.usda") == "geo_{v8+}.usda"  # No match


# === Placeholder Syntax: {vN-} - Match version N or lower ===


def test_version_or_lower_match():
    assert resolve("geo_{v5-}.usda") == "geo_v5.usda"


def test_version_or_lower_skips_higher():
    assert resolve("geo_{v2-}.usda") == "geo_v2.usda"


def test_version_or_lower_too_low():
    assert resolve("foo_{v2-}.usda") == "foo_{v2-}.usda"  # No match


# === Placeholder Syntax: {vN-M} - Match version in range ===


def test_version_in_range_middle():
    assert resolve("geo_{v2-5}.usda") == "geo_v5.usda"


def test_version_in_range_single_match():
    assert resolve("geo_{v1-1}.usda") == "geo_v1.usda"


def test_version_in_range_none_match():
    assert resolve("geo_{v6-7}.usda") == "geo_{v6-7}.usda"  # No match


# === Exact Version Placeholder: {vN} ===


def test_exact_version_match():
    assert resolve("geo_{v3}.usda") == "geo_v3.usda"


def test_exact_version_not_found():
    assert resolve("geo_{v4}.usda") == "geo_{v4}.usda"
