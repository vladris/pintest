'''
    interop
    ~~~~~~~

    Interop with native test modules

    :copyright: Copyright (c) 2013 Vlad Riscutia
    :license: MIT License
'''
from ctypes import cdll, c_char_p, c_int

class Module:
    '''
    Wrapper over a native test module
    '''

    def __init__(self, filename):
        '''
        Creates a new Module instance. This function raises an error if
        the module is not a valid test module (doesn't export the expected
        functions)
        '''
        self.__module = cdll.LoadLibrary(filename)

        self.__get_group = self.__module.get_group
        self.__get_test = self.__module.get_test
        self.__run_test = self.__module.run_test

        self.tests = dict()

    def load(self):
        '''
        Loads the tests from the module into a dictionary
        '''
        group = self.__get_group(None)
        while group:
            self.tests[c_char_p(group).value] = []
            test = self.__get_test(group, None)
            while test:
                self.tests[c_char_p(group).value].append(c_char_p(test).value)
                test = self.__get_test(group, test)
            group = self.__get_group(group)

    def run_test(self, group, test):
        '''
        Runs the given test from the given group
        '''
        return c_int(self.__run_test(c_char_p(group), c_char_p(test))).value
