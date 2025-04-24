import os
import pytest
from pxr import Ar


@pytest.fixture(autouse=True)
def setup_env():
    """Clear and set relevant environment variables before each test."""
    os.environ["SHOT"] = "sh010"
    os.environ.pop("HELLO", None)
    os.environ.pop("FOO", None)
    os.environ.pop("JOB", None)
    yield
    # Optional: clean up after test
    os.environ.pop("SHOT", None)


def resolve(value):
    """Resolve a string using the custom resolver."""
    resolver = Ar.GetResolver()
    return str(resolver.Resolve(value))


# --- BASIC BEHAVIOR ---


def test_no_placeholder_is_untouched():
    assert resolve("project/char_cat/geo.usda") == "project/char_cat/geo.usda"


def test_single_existing_env_var():
    assert resolve("project/{env:SHOT}/geo.usda") == "project/sh010/geo.usda"


def test_missing_env_var_with_no_fallback():
    assert resolve("project/{env:HELLO}/geo.usda") == "project/HELLO NOT FOUND/geo.usda"


def test_missing_env_var_with_fallback():
    assert resolve("project/{env:FOO:sh020}/geo.usda") == "project/sh020/geo.usda"


def test_fallback_empty_string():
    assert resolve("project/geo{env:FOO:}.usda") == "project/geo.usda"


def test_multiple_env_vars_combined():
    assert resolve("/jobs/{env:JOB:some_job_name}/{env:SHOT}/geo_{env:SHOT}.usda") == "/jobs/some_job_name/sh010/geo_sh010.usda"


# --- EDGE CASES AND EXTRA TESTS ---


def test_multiple_occurrences_of_same_var():
    assert resolve("{env:SHOT}-{env:SHOT}+{env:SHOT}") == "sh010-sh010+sh010"


def test_var_name_with_invalid_characters_not_matched():
    # This won't be replaced unless your resolver supports dashes etc.
    assert resolve("test-{env:INVALID-NAME:default}") == "test-{env:INVALID-NAME:default}"


def test_colons_in_fallback():
    # Should preserve colons in fallback, e.g., in paths
    assert resolve("fallback={env:FOO:/tmp/cache:data}") == "fallback=/tmp/cache:data"


def test_trailing_text_after_env():
    assert resolve("geo{env:FOO:}_hi.usda") == "geo_hi.usda"


def test_only_placeholder():
    assert resolve("{env:SHOT}") == "sh010"


def test_placeholder_with_number_in_name():
    os.environ["VAR1"] = "xyz"
    assert resolve("foo_{env:VAR1}_bar") == "foo_xyz_bar"


def test_unclosed_placeholder_is_ignored():
    # Depending on regex used, this should be ignored or left untouched
    assert resolve("path/{env:SHOT") == "path/{env:SHOT"
