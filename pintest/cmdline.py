'''
    cmdline
    ~~~~~~~

    Test runner command line

    :copyright: Copyright (c) 2016 Vlad Riscutia
    :license: MIT License
'''
import argparse
import pintest
from pintest import codes, discovery, executor, interop


def list_tests(path, module):
    '''
    Test discovery
    '''
    tests = discovery.get_tests(path, module)

    for module in tests:
        print('--- Module %s ---' % (module.filename,))
        for test in tests[module]:
            print(test)
        print('--------------------')


def run_tests(path, module, test):
    '''
    Run tests
    '''
    # if test is specified, run just that test
    if module and test:
        result = executor.run(interop.Module(module), test)
        print(codes.RESULTS[result], end='')
        return

    # otherwise run discovery and run all tests
    tests = discovery.get_tests(path, module)

    for module in tests:
        print('%s:' % (module.filename,), end='')
        for test in tests[module]:
            result = executor.run(module, test)
            print(codes.RESULTS[result], end='')
        print()


def main(argv=None):
    '''
    Parses command line and triggers required action
    '''
    parser = argparse.ArgumentParser()
    group = parser.add_mutually_exclusive_group()

    group.add_argument(
        '-r', '--run', nargs='?', const='.',
        help='run test or tests (default is current directory)')
    group.add_argument(
        '-l', '--list', nargs='?', const='.',
        help='list test or tests (default is current directory)')

    parser.add_argument(
        '-m', '--module', type=str,
        help='load the specified module')
    parser.add_argument(
        '-t', '--test', type=str,
        help='specify test')

    command = parser.parse_args(argv)

    if command.list:
        list_tests(command.list, command.module)
        return 0
    elif command.run:
        return run_tests(command.run, command.module, command.test)
    else:
        return run_tests('.', '', '')
