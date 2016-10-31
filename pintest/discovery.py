'''
    discovery
    ~~~~~~~~~

    Test discovery

    :copyright: Copyright (c) 2016 Vlad Riscutia
    :license: MIT License
'''
import collections
import os
import os.path as path
from pintest import interop


# Module extension to look for based on OS
if os.name == 'nt':
    MODULE_EXT = '.dll'
else:
    MODULE_EXT = '.so'


def discover(root_dir):
    '''
    Walks the given directory and finds all modules
    '''
    for root, dirs, files in os.walk(path.abspath(root_dir)):
        for f in files:
            ext = path.splitext(f)[1].lower()
            # only consider modules (dll or so depending on OS)
            if ext == MODULE_EXT:
                try:
                    # constructor throws if not a valid test module
                    module = interop.Module(path.join(root, f))
                    yield module
                except OSError as e:
                    print(e)
                    pass


def enumerate_tests(path, module):
    '''
    Enumerate tests under the given path or in the given module
    '''
    if module:
        modules = [interop.Module(module)]
    else:
        modules = discover(path)

    for module in modules:
        module.load()
        for test_method in module.tests:
            yield module, test_method


def get_tests(path, module):
    '''
    Return tests under the given path or in the given module as a dictionary
    '''
    tests = collections.defaultdict(list)
    for module, test in enumerate_tests(path, module):
        if not test.endswith('__setup') and not test.endswith('__teardown'):
            tests[module].append(test)
    return tests
