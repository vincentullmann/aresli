# Asset Resolver Lite

A simple toy asset resolver for experimenting with custom path resolution logic in USD pipelines.

## Features

### Environment Variable Substitution

Supports dynamic substitution of environment variables within asset paths using the following syntax:

Syntax: `{env:VAR_NAME}`

```bash
export SHOT=sh010

@caches/{env:SHOT}_cache.usd@       # Resolves to: caches/sh010_cache.usd

# With default fallback value
@caches/{env:SHOT:sh020}_cache.usd@ # Resolves to: caches/sh010_cache.usd
```

## Build and Test

Tested on Ubuntu 22.04 with USD 24.11.

```bash
rez env usd pytest
make install test
```
