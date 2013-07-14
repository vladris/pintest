'''
    test_native
    ~~~~~~~~~~~

    Test native code by making sure native tests have expected results

    :copyright: Copyright (c) 2013 Vlad Riscutia
    :license: MIT License
'''
from testhpp_runner import discovery, executor, interop
import unittest



def test_discovery():
    modules = discovery.discover('.')        
    print(modules)
