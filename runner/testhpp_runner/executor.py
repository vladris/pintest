'''
    executor
    ~~~~~~~~

    Entry point for executing a single test

    :copyright: Copyright (c) 2013 Vlad Riscutia
    :license: MIT License
'''
import argparse
from subprocess import Popen, PIPE
from testhpp_runner import codes, interop



def spawn_exec(module, test):
    '''
    Spawn a new process to execute the test
    '''
    proc = Popen(['runtest-exec', '-m', module, '-t', test], stdout=PIPE, stderr=PIPE)
    stdout, stderr = proc.communicate()
    return proc.returncode, stdout, stderr



def main(argv=None):
    '''
    Entry point
    '''
    try:
        module, test = parse(argv)

        # try loading the module
        try:
            module = interop.Module(module)
        except:
            return codes.INVALID_MODULE
        group, test = test.split('/')

        # run the test
        return module.run_test(group, test)
    except:
        return codes.RUNNER_EXCEPTION



def parse(argv):
    '''
    Parses command line
    '''
    parser = argparse.ArgumentParser()
    parser.add_argument('-m', '--module', type=str, required=True,
        help='module containing test')
    parser.add_argument('-t', '--test', type=str, required=True,
        help='group/test to run (eg: -t foo/bar)')

    command = parser.parse_args(argv)
    return command.module, command.test
