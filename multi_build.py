#!/bin/env python3
#
# This is a script that will build multiple configuration/permutations/combinations of a CMake project
# and with different compilers.


from pathlib import Path
from shutil import rmtree
from dataclasses import dataclass
from copy import deepcopy
from pprint import pprint
import subprocess


@dataclass
class Compiler:
    '''Compiler to use'''
    name: str           # General name of compiler
    CC_path: Path       # Path to C compiler
    CXX_path: Path      # PAth to C++ compiler


@dataclass
class ConfigurationOption:
    '''A option that can be configured/set at CMake's invocation time'''
    name: str               # General name of the configuration
    option_key: str         # The key for the option e.g. 'CMAKE_BUILD_TYPE'
    values: list[str]       # Possible values for the option (e.g. ['Release', 'Debug']);  Supply a single one for a static value


@dataclass
class Build:
    compiler:               Compiler    # Which compiler suite to use.
    names:                  list[str]   # List of the names for the build folder
    cmake_configuration:    list[str]   # List of what needs to be supplied to CMake
    folder_name:            Path        # Is inside of `multi_build/`


compilers = [
    Compiler('gcc14',   '/usr/bin/gcc-14',   '/usr/bin/g++-14'),
    Compiler('clang18', '/usr/bin/clang-18', '/usr/bin/clang++-18'),
]
configuration_options = [
    ConfigurationOption('build_type',              'CMAKE_BUILD_TYPE',            ['Release']),
    ConfigurationOption('sin_cos',                 'WITH_BOOK_SIN_COS',           ['ON']),
    ConfigurationOption('rejection_sampling',      'WITH_BOOK_RNG_UNIT_SAMPLING', ['ON', 'OFF']),
#    ConfigurationOption('book_bvh',                'WITH_BOOK_BVH',               ['ON', 'OFF']),      # These two are just here for testing
#    ConfigurationOption('use_final',               'WITH_FINAL',                  ['OFF'])
]
cmake_exe = Path('/usr/bin/cmake')
num_cores_to_use = 4


def generate_builds(compilers: list[Compiler], configs: list[ConfigurationOption]) -> list[Build]:
    # Initial empty build
    builds = [Build(None, [], [], None)]

    # Go through each configuration setting
    for option in configs:
        if (len(option.values) == 1):
            # Static option
            value = option.values[0]
            folder_name_part = f'{option.name}_{value}'
            cmake_config_string = f'-D{option.option_key}={value}'

            # Add it onto each
            for build in builds:
                build.names.append(folder_name_part)
                build.cmake_configuration.append(cmake_config_string)
        else:
            # Multiple values for the option
            branched_builds = []
            for build in builds:
                # Put a different one on each build we have thus far
                for value in option.values:
                    folder_name_part = f'{option.name}_{value}'
                    cmake_config_string = f'-D{option.option_key}={value}'

                    # Copy the existing build and add the new information
                    coped_build = deepcopy(build)
                    coped_build.names.append(folder_name_part)
                    coped_build.cmake_configuration.append(cmake_config_string)

                    branched_builds.append(coped_build)

            # Set the branch back to out current build
            builds = branched_builds

    # Finalize the configurations
    num = 1
    builds_with_compilers = []
    for build in builds:
        # Now do `compiler` x `build`
        for compiler in compilers:
            copied_build = deepcopy(build)

            # Assign a compiler and number to the build
            copied_build.compiler = compiler
            num_str = f'{num:02}'

            # Specify the compiler for cmake
            cmake_c_compiler = f'-DCMAKE_C_COMPILER={compiler.CC_path}'
            cmake_cxx_compiler = f'-DCMAKE_CXX_COMPILER={compiler.CXX_path}'
            copied_build.cmake_configuration.append(cmake_c_compiler)
            copied_build.cmake_configuration.append(cmake_cxx_compiler)

            # Make the actual folder names and build strings
            copied_build.folder_name = '-'.join([num_str, compiler.name] + build.names)

            builds_with_compilers.append(copied_build)
            num += 1

    return builds_with_compilers


def run_cmake(build: Build, root_build_folder: Path) -> None:
    # Configure the build inside of each folder
    cwd = root_build_folder.joinpath(build.folder_name)
    cmd = ' '.join([str(cmake_exe), '../../'] + build.cmake_configuration)
    subprocess.call(cmd, cwd=cwd, shell=True, text=True)


def perform_build(build: Build, root_build_folder: Path) -> None:
    cwd = root_build_folder.joinpath(build.folder_name)
    cmd = str(cmake_exe) + f' --build . -j{num_cores_to_use}'
    subprocess.call(cmd, cwd=cwd, shell=True, text=True)


def main():
    builds = generate_builds(compilers, configuration_options)

    # Create the folder for the builds (clear out the old)
    mb_path = Path('multi_build')
    if mb_path.exists():
        rmtree(mb_path)
    mb_path.mkdir()

    # Create each specific build
    for build in builds:
        print(f'Configuring {build.folder_name} ...')

        # Make folder and configure
        build_path = mb_path.joinpath(build.folder_name)
        build_path.mkdir()
        run_cmake(build, mb_path)

    # Do the build
    for build in builds:
        print(f'Building {build.folder_name} ...')
        perform_build(build, mb_path)

    print('\nAll Done.')


if __name__ == '__main__':
    main()
