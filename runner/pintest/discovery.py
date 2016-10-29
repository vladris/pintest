'''
    discovery
    ~~~~~~~~~

    Test discovery

    :copyright: Copyright (c) 2016 Vlad Riscutia
    :license: MIT License
'''
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
    result = []
    for root, dirs, files in os.walk(path.abspath(root_dir)):
        for f in files:
            ext = path.splitext(f)[1].lower()
            # only consider modules (dll or so depending on OS)
            if ext == MODULE_EXT:
                try:
                    # constructor throws if not a valid test module
                    module = interop.Module(path.join(root, f))
                    result.append(module)
                except:
                    pass
    return result
