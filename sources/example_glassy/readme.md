# Glassy Library

An experiment of creating small library wrapper for OpenSSL functions using modern C++ features and using externallibraries providing useful types and abstractions.

## Project Structure

```
.
├── application/     # Application code
├── library/         # Core library implementation
│   └── sources/     # Library source files
├── tests/           # Test suite
└── third_party/     # External dependencies
```

## Dependencies

- OpenSSL
- GSL (Guidelines Support Library)
- Outcome library

## Features

- Big number operations with OpenSSL integration
- ASN.1 integer handling
- Memory management helpers
- Numeric conversions utilities
- Not-null concepts implementation
- Error handling using Outcome library



## Components

### Library

The core library provides several key components:

- `BigNum`: A wrapper around OpenSSL's BIGNUM functionality
- `ASN1Integer`: ASN.1 integer implementation
- `MemoryHelpers`: Memory management utilities
- `NumericConversions`: Safe numeric conversion utilities
- `NotNullConcepts`: Implementation of not-null pointer concepts

### Tests

The project includes a comprehensive test suite to ensure reliability and correctness of the implementation.