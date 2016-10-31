'''
    setup
    ~~~~~

    Package setup script

    :copyright: Copyright (c) 2016 Vlad Riscutia
    :license: MIT License
'''
from setuptools import setup, find_packages
import pintest


long_desc = '''
Python PinTest test runner
'''


requires = ['argparse>=1.1']


setup(
    name='pintest',
    version=pintest.__version__,
    url='https://github.com/vladris/pintest',
    download_url='http://pypi.python.org/pypi/pintest',
    license='MIT',
    author='Vlad Riscutia',
    author_email='vladris@outlook.com',
    description='PinTest test runner',
    long_description=long_desc,
    classifiers=[
        'Development Status :: 4 - Beta',
        'Environment :: Console',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
        'Programming Language :: Python',
        'Topic :: Software Development :: Testing'
    ],
    platforms='any',
    packages=find_packages(),
    data_files=[
        ('inc', ['../pintest.hpp'])
    ],
    include_package_data=True,
    entry_points={
        'console_scripts': [
            'pintest = pintest.cmdline:main',
        ],
    },
    install_requires=requires,
)
