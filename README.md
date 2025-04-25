# Asset Resolver Lite

A simple toy asset resolver for experimenting with custom path resolution logic in USD pipelines.

## Features

### Version Number Matching

Support matching of version numbers with optional lower/upper bounds

```bash
# Resolves to the highest available version
@caches/geo_{v}.usd@

# Resolves to version 2 or higher (e.g., v2, v3, ...)
@caches/geo_{v2+}.usd@

# Resolves to version 5 or lower (e.g., v5, v4, ...)
@caches/geo_{v5-}.usd@

# Resolves to a version between 2 and 5 (inclusive)
@caches/geo_{v2-5}.usd@
```

### Environment Variable Substitution

Supports dynamic substitution of environment variables within asset paths using the following syntax: `{env:VAR_NAME:fallback}`

```bash
export SHOT=sh010
@caches/{env:SHOT}_cache.usd@        # Resolves to: caches/sh010_cache.usd

# With default fallback value
@caches/{env:SHOT:sh020}_cache.usd@  # Resolves to: caches/sh012_cache.usd
```

## Build and Test

Tested on Ubuntu 22.04 with USD 24.11.

```bash
rez env usd pytest
make install test
```
