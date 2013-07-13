'''
    cmdline
    ~~~~~~~

    Test runner command line

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
    Parses command line and triggers required action
    '''
    parser = argparse.ArgumentParser()
    parser.add_argument('-m', '--module', type=str,
        help='load the specified module')
    parser.add_argument('-t', '--test', type=str,
        help='runs the specified test ($GROUP/$TEST, eg: -t foo/bar)')

    command = parser.parse_args(argv)

    if command.module:
        if command.test:
            return run_single_test(command.module, command.test)
        else:
            # TODO
            pass
    else:
        # TODO
        pass
