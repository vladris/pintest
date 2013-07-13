'''
    setup
    ~~~~~

    Package setup script

    :copyright: Copyright (c) 2013 Vlad Riscutia
    :license: MIT License
'''
from setuptools import setup, find_packages
import testhpp_runner



long_desc = '''
Test runner for test.hpp
'''


requires = ["argparse>=1.2"]


setup(
    name = "test.hpp runner",
    version = testhpp_runner.__version__,
    url = "http://vladris.github.io/test.hpp",
    download_url = "http://pypi.python.org/pypi/test.hpp_runner",
    license = "MIT",
    author = "Vlad Riscutia",
    author_email = "vladris@outlook.com",
    description = "Test runner for test.hpp",
    long_description = long_desc,
    classifiers = [
        "Development Status :: 3 - Alpha",
        "Environment :: Console",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
        "Programming Language :: Python",
        "Topic :: Software Development :: Testing"
    ],
    platforms = "any",
    packages = find_packages(),
    include_package_data = True,
    entry_points = {
        "console_scripts": [
            "runtest = testhpp_runner.cmdline:main",
            "runtest-exec = testhpp_runner.executor:main"
        ],
    },
    install_requires = requires,
)
