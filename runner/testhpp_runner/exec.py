'''
    exec
    ~~~~

    Entry point for executing a single test

    :copyright: Copyright (c) 2013 Vlad Riscutia
    :license: MIT License
'''
import argparse
import interop



def run_single_test(module, test):
    '''
    Runs a single test
    '''
    m = interop.Module(module)
    group, test = test.split('/') 
    return m.run_test(group, test)



def main(argv=None):
    '''
    Parses command line
    '''
    parser = argparse.ArgumentParser()
    parser.add_argument('-m', '--module', type=str, required=True,
        help='module containing test')
    parser.add_argument('-t', '--test', type=str, required=True,
        help='group/test to run (eg: -t foo/bar)')

    command = parser.parse_args(argv)
    return run_single_test(command.module, command.test)
