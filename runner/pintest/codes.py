'''
    codes
    ~~~~~

    Execution result codes

    :copyright: Copyright (c) 2016 Vlad Riscutia
    :license: MIT License
'''

# Result codes matching those in test.hpp - these come from the module in
# witch tests are executed via interop
SUCCESS = 0             # Test succeeded
FAILED = 1              # Assertion failed
EXCEPTION = 2           # C++ exception
INVALID = 3             # Invalid test

# Runner error codes
RUNNER_EXCEPTION = 10   # Unhandled exception
INVALID_MODULE = 11     # Module couldn't be loaded

# Character representation for results
RESULTS = {
    SUCCESS: '.',
    FAILED: 'F',
    EXCEPTION: 'E',
    INVALID: 'I',
    RUNNER_EXCEPTION: 'C',
    INVALID_MODULE: 'M'
}
