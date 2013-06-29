'''
    interop
    ~~~~~~~

    Interop with native test modules

    :copyright: Copyright (c) 2013 Vlad Riscutia
    :license: MIT License
'''
from ctypes import cdll, c_char_p, c_int

# Mangled export names
GET_GROUP = '?get_group@Interop@test@@CAPBDPBD@Z'
GET_TEST = '?get_test@Interop@test@@CAPBDPBD0@Z'
RUN_TEST = '?run_test@Interop@test@@CAHPBD0@Z'

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

        self.__get_group = getattr(self.__module, GET_GROUP)
        self.__get_test = getattr(self.__module, GET_TEST)
        self.__run_test = getattr(self.__module, RUN_TEST)

        self.tests = dict()

        self.__load()

    def __load(self):
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
        return c_int(self.__run_test(c_char_p(group), c_char_p(test))).value
