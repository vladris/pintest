'''
    executor
    ~~~~~~~~

    Entry point for executing a single test

    :copyright: Copyright (c) 2016 Vlad Riscutia
    :license: MIT License
'''
from pintest import codes


def run(module, test):
    '''
    Run test in-proc
    '''
    try:
        return module.run_test(*test.split('::'))
    except:
        return codes.RUNNER_EXCEPTION
