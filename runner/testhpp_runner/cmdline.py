'''
    cmdline
    ~~~~~~~

    Test runner command line

    :copyright: Copyright (c) 2013 Vlad Riscutia
    :license: MIT License
'''
import argparse
import discovery
import interop
import re



def list_tests(path, module, test):
    '''
    Test discovery
    '''
    # if module is specified, just load it
    if module:
        modules = [interop.Module(module)]
    # otherwise perform discovery
    else:
        modules = discovery.discover(path)

    # filter if we are looking for a particular test
    if test:
        filt = re.compile(test)
    else:
        filt = re.compile('.*')


    for module in modules:
        print("")
        print("*** Module: %s ***" % (module.filename, ))
        module.load()
        for group in sorted(module.tests):
            for test in module.tests[group]:
                name = "%s/%s" % (group, test)
                if filt.match(name):
                    print(name)



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
    parser.add_argument('-l', '--list', nargs='?', const='.',
        help='discover and list tests in LIST directory (default is current directory)')
    parser.add_argument('-m', '--module', type=str,
        help='load the specified module')
    parser.add_argument('-t', '--test', type=str,
        help='runs the specified test ($GROUP/$TEST, eg: -t foo/bar)')

    command = parser.parse_args(argv)

    if command.list:
        list_tests(command.list, command.module, command.test)
        return 0

    if command.module:
        if command.test:
            return run_single_test(command.module, command.test)
        else:
            # TODO
            pass
    else:
        # TODO
        pass
