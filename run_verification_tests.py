#!/bin/env python3
#
# This is a script which will run the ray tracing application quite a few times for verification
# testing purpose.  To run correct, this script requires that the `idiff` program is installed,
# which comes from the `openimageio-tools` on ubuntu.

from os import path
from sys import exit
from argparse import ArgumentParser
from random import sample
from itertools import product
from csv import DictReader, DictWriter
from pathlib import Path
from shutil import copyfile
from subprocess import check_output, CalledProcessError

# Location of the ray tracing executable
PS_RAYTRACING_EXE = path.abspath(path.join('build', 'PSRayTracing'))

# The possible arguments that can be passed into the ray tracer
NUM_THREADS = (1, 2, 4)                                         # How many threads to render with
USE_COPY_PER_THREAD = (True, False)                             # Use the "deep copy per thread" feature
NUM_SAMPLES = (5, 10, 25)                                       # How many samples to use
RAY_DEPTHS = [10, 25, 50]                                       # How many times to bounce
RANDOM_SEEDS = ('9001', 'ASDF', "alvin's hot juicebox")         # Seeds for the RNG
SIZES = ('320x240', '400x400', '852x480')                       # Possible sizes

# Fields for the CSV files
FIELDS = ('id_num', 'scene_id', 'num_threads', 'use_copy_per_thread', 'num_samples', 'ray_depth', 'random_seed', 'size')


# This generates the tests to be run.  Needs to know how many tests per scene there should be, as well as what filename
# to save the CSV file to. To run successfully, the `PSRayTracing` binary must be found
def generate_test_cases(tests_per_scene, test_cases_filename):
    # Get the available scenes
    scenes_raw = check_output([PS_RAYTRACING_EXE, '--list-scenes']).decode('utf-8')     # Get list directly from the executable
    scenes = scenes_raw.splitlines()                                                    # Each scene is on its own line
    scenes = scenes[1:]                                                                 # First line is actually not a scene, the rest are
    scenes = [x.strip() for x in scenes]                                                # There is surrounding whitespace, clean it up

    # Generate all the possible rendering options (there's going to be a LOT)
    all_options = [NUM_THREADS, USE_COPY_PER_THREAD, NUM_SAMPLES, RAY_DEPTHS, RANDOM_SEEDS, SIZES]
    combinations = list(product(*all_options))

    # We only take a subset of the above for each scene (still quite a bit TBH)
    all_cases = []
    for scene in scenes:
        cases = sample(combinations, tests_per_scene)           # Take a sample of what cases to test
        cases = [[scene, *list(case)] for case in cases]        # Push the scene in front of each case

        all_cases += sorted(cases)

    # Figure out how many tests we have
    num_cases = len(all_cases)
    digit_count = len(str(num_cases))

    # Give them all an id number (zero padded to match the `digit_count`, e.g. 001, 002, ... 234, 235 etc)
    for i in range(0, num_cases):
        id_num = str(i + 1).zfill(digit_count)
        all_cases[i].insert(0, id_num)

    # Write the test cases to a CSV file
    with open(test_cases_filename, 'w') as csv_file:
        writer = DictWriter(csv_file, fieldnames=FIELDS)
        writer.writeheader()

        # Write each entry
        for case in all_cases:
            writer.writerow({
                'id_num':               case[0],
                'scene_id':             case[1],
                'num_threads':          case[2],
                'use_copy_per_thread':  case[3],
                'num_samples':          case[4],
                'ray_depth':            case[5],
                'random_seed':          case[6],
                'size':                 case[7]
            })

    print('Wrote %i test cases to `%s`' % (num_cases, test_cases_filename))


# Checks if two test cases should produce the same pixel-for-pixel output.  Returns a boolean
def should_have_same_output(case_a, case_b):
    # Looking at the feields we have, filter out the ones that shouldn't change the render output
    check_same = [x for x in FIELDS if x not in ('id_num', 'num_threads', 'use_copy_per_thread')]

    # Check the (values of) the fields that do change output
    is_same = True
    for field in check_same:
        is_same = is_same and (case_a[field] == case_b[field])

    return is_same


# Give a list of test cases, finds which cases should render the same image.  Returns a list of two-element tuples (of
# id strings).  E.g. `[('012', '015'), ('642', '654')]`
def find_matching_renders(test_cases):
    # Sort each test case into it's "scene bucket"
    cases_by_scene = {}
    for case in test_cases:
        s_id = case['scene_id']

        if s_id in cases_by_scene:
            cases_by_scene[s_id].append(case)       # scene already exists, append it alread
        else:
            cases_by_scene[s_id] = [case]           # New scene, make a new list

    # Find cases that are supposed to output the same renders
    matching_renders = []
    for scene_bucket in cases_by_scene.values():
        num_cases = len(scene_bucket)

        for i in range(0, num_cases):
            case_a = scene_bucket[i]

            # Look at the case after the first one we grab
            for j in range(i + 1, num_cases):
                case_b = scene_bucket[j]
                id_a = case_a['id_num']
                id_b = case_b['id_num']

                # first make sure they're the same test case
                if should_have_same_output(case_a, case_b):
                    matching_renders.append((id_a, id_b))

    return sorted(matching_renders)


# Uses the `idiff` program to see if two images are the same, pixel-for-pixel.  arguments must be filepaths to those images
def test_images_match(image_a_filepath, image_b_filepath):
    # If an error is thrown by `check_output`, then that means the renders don't match, else it's good
    try:
        check_output(['idiff', image_a_filepath, image_b_filepath])
        return True
    except CalledProcessError as e:
        return False


# This is the actual function that runs all of the tests, both reference and the real ones.
# `test_cases_filename` should be the name of the CSV file where the test cases live.  If
# the reference renders are being made `running_real_tests` should be set to `False`.  But
# When running the real tests, it should be `True`.
def run_test_cases(test_cases_filename, running_real_tests):
    #== Section 1: Setup (Files & Data) ==#
    # First determine where the renders live
    parts = [test_cases_filename, 'refernence', 'renders']
    references_folder = '_'.join(parts)

    if running_real_tests:
        parts = [test_cases_filename, 'renders']

    # build some file paths
    renders_destination = '_'.join(parts)
    results_csv_filename = path.join(renders_destination, 'results.csv')
    results_txt_filename = path.join(renders_destination, 'results.txt')
    cmake_cache_src = path.join('build', 'CMakeCache.txt')
    cmake_cache_dst = path.join(renders_destination, 'CMakeCache.txt')

    # Read in the test configurations
    test_cases = []
    with open(test_cases_filename, 'r') as csv_file:
        reader = DictReader(csv_file)
        test_cases = [row for row in reader]

    # Find the samesies for when we do the `idiff`
    matching_renders = find_matching_renders(test_cases)
    have_matching = (len(matching_renders) > 0)
    if have_matching:
        matching_msg = 'Verifying cases where renders should be the same:'
    else:
        matching_msg = 'WARNING: Not able to find any test cases that should produce the same result'

    # Save renders to a folder that's close to our filename
    Path(renders_destination).mkdir(exist_ok=True, parents=True)

    # Copy over the CMake build configuration, this way you know how the software was build for that test run
    copyfile(cmake_cache_src, cmake_cache_dst)

    # Create the results CSV file
    with open(results_csv_filename, 'w') as csv_file:
        fields = list(FIELDS)
        fields.append('render_time_ns')     # Add on the render time as another column

        # If we're running the real tests, we also need to list PASS/FAIL status
        if running_real_tests:
            fields.append('matches_reference')

        writer = DictWriter(csv_file, fieldnames=fields)
        writer.writeheader()

    #== Section 2: Running Tests ==#
    # Run the renders!
    total_render_time_ns = 0
    num_matches_reference = 0
    num_total_cases = len(test_cases)

    print('Running %i test cases:' % num_total_cases)
    for case in test_cases:
        #== Section 2a: Run the Render ==#
        # Build arguments to run with
        id_num = case['id_num']
        render_filename = '%s.png' % id_num
        render_filepath = path.join(renders_destination, render_filename)

        args = [
            '--testing-mode',
            '-o',               render_filepath,
            '--scene',          case['scene_id'],
            '--random-seed',    case['random_seed'],
            '--num-threads',    case['num_threads'],
            '--depth',          case['ray_depth'],
            '--num-samples',    case['num_samples'],
            '--size',           case['size'],
        ]

        # Do the render
        print('  Test %s/%s:' % (id_num,  num_total_cases), end='', flush=True)
        output = check_output([PS_RAYTRACING_EXE, *args]).decode('utf-8').strip()
        parts = output.split(' ')

        # Verify things were outputted correctly, if not, the quit testing
        if (len(parts) != 2) or (parts[1] != 'ns'):
            print('Error in the text output from test %s: %s' % (id_num, output))
            print("It's not as expected, quiting the test suite")
            exit(1)

        #== Section 2b: Check Render Results ==#
        # Test against the reference (maybe?)
        pass_fail_str = None
        if running_real_tests:
            ref_render_filepath= path.join(references_folder, '%s.png' % id_num)
            result = test_images_match(ref_render_filepath, render_filepath)

            if result:
                num_matches_reference += 1
                pass_fail_str = 'PASS'
            else:
                pass_fail_str = 'FAIL'

            # print the result of the pass/fail
            print(' %s' % pass_fail_str, end='', flush=True)

        #== Section 2c: Report/Save Metrics ==#
        # Get the time
        render_time_ns = int(parts[0])
        total_render_time_ns += render_time_ns
        render_time_s = render_time_ns / 1000000000.0
        print(' [%.3f s]' % render_time_s, end='', flush=True)

        # Newline
        print('')

        # Write results to CSV
        with open(results_csv_filename, 'a') as csv_file:
            # Add on the "render time (ns)" column
            case['render_time_ns'] = render_time_ns

            # And maybe the pass/fail
            if pass_fail_str:
                case['matches_reference'] = pass_fail_str

            DictWriter(csv_file, fieldnames=case.keys()).writerow(case)

    #== Section 3: Verification of Matching Renders ==#
    # Verify renders that should be the same
    print('')
    print(matching_msg)

    matching_renders_results_txt = ''
    for pair in matching_renders:
        # Build the arguments for the `idff` command
        render_a = path.join(renders_destination, '%s.png' % pair[0])
        render_b = path.join(renders_destination, '%s.png' % pair[1])
        result = test_images_match(render_a, render_b)
        result_str = 'PASS' if result else 'FAIL'

        # Format the message to print (and save to report)
        case = '  %s -- %s : %s' % (a, b, result_str)
        print(case)
        matching_renders_results_txt += '%s\n' % case

    #== Section 4: Metrics Info  ==#
    # Metrics
    total_time_str = 'Total render time was %.3f s' % (total_render_time_ns / 1000000000.0)
    print('')
    print(total_time_str)

    # Put some of those metrics in a file
    with open(results_txt_filename, 'w') as results_txt:
        if running_real_tests:
            results_txt.write('%s/%s tests passed\n' % (num_matches_reference, num_total_cases))

        results_txt.write('%s (or %i ns)\n' % (total_time_str, total_render_time_ns))
        results_txt.write('%s\n' % matching_msg)

        if have_matching:
            results_txt.write(matching_renders_results_txt)


def main():
    # Setup the arguments
    parser = ArgumentParser()
    parser.add_argument('-g', '--generate-test-cases', help='Generate a suite of tests to run',     action='store_true')
    parser.add_argument('-r', '--run-reference-test',  help='Runs the tests in "reference mode"',   action='store_true')
    parser.add_argument('-n', '--tests-per-scene',     help='If generating tests, how many to generate per scene (default is 10)', type=int, default=10)
    parser.add_argument('-f', '--test-cases-filename', help='CSV file where the test case configuration is stored',                type=str, default='test_cases.csv')
    args = parser.parse_args()

    # Make sure we can access the rendering executable
    if not path.exists(PS_RAYTRACING_EXE):
        print("ERROR: Not able to find `PSRayTracing` executable in the `build/` folder; can't run the tests without it.")
        exit(1)

    # Generate (only) or run the tests?
    if args.generate_test_cases:
        generate_test_cases(args.tests_per_scene, args.test_cases_filename)
    else:
        no_test_file = not path.exists(args.test_cases_filename)

        if args.run_reference_test and no_test_file:
            # If there is no test file on our reference run, genereate it
            generate_test_cases(args.tests_per_scene, args.test_cases_filename)
        elif no_test_file:
            # Else during a "real test", error out
            print('ERROR: Not able to find the test cases file `%s`.  Please run this with `-r` or `-g` before doing a actual test.' % args.test_cases_filename)
            exit(1)

        run_test_cases(args.test_cases_filename, not args.run_reference_test)


if __name__  == '__main__':
    main()
