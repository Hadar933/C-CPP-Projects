import difflib
import os
import subprocess
import sys

PROGRAM_NAME = "RailWayPlanner"
CREATE_GCC = f"gcc -Wextra -Wall -Wvla -std=c99 -lm {PROGRAM_NAME}.c -o {PROGRAM_NAME}"
OUTPUT_FILE = "railway_planner_output.txt"
TESTS_FILE_NAMES = ['connections_count_negative', 'empty_file', 'many_connections', 'price_overflow',
                    'connections_count_non_numeric_prefix', 'first_unknown_connection', 'many_rails',
                    'railway_length_negative', 'connections_count_non_numeric_suffix', 'length_negative',
                    'middle_bad_line', 'railway_length_non_numeric_prefix', 'connections_count_non_numeric',
                    'length_non_numeric_prefix', 'no_valid_solution', 'railway_length_non_numeric_suffix',
                    'connections_count_zero', 'length_non_numeric_suffix', 'number_connection',
                    'railway_length_non_numeric', 'duplicate_connection', 'length_non_numeric', 'price_negative',
                    'railway_length_zero', 'length_zero', 'price_non_numeric_prefix',
                    'second_unknown_connection', 'long_connection_in_rail',
                    'price_non_numeric_suffix', 'standard_file', 'long_connection',
                    'price_non_numeric', 'price_zero', 'large_railway']

USE_VALGRIND = False


def cmp_files(cmp_file, out_file):
    with open(cmp_file, 'r') as fCmp:
        with open(out_file, 'r') as fOut:
            diff = difflib.ndiff(fOut.readlines(), fCmp.readlines())

            for i, line in enumerate(diff):
                if line.startswith("- ") or line.startswith("+ ") or line.startswith("? "):
                    print("\tOutput test - FAIL")
                    print("\tDifference found in '" + out_file + "' at line " + str(i))
                    return 0

            print("\tOutput test - PASS")
            return 1


def compile_program():
    compiled = subprocess.run(CREATE_GCC, text=True, shell=True)
    if compiled.returncode != 0:
        print("\nProgram failed compiling")
        return False
    else:
        print("\nProgram compiled successfully")
        return True


def check_valgrind_file(val_file):
    check_lines = -1

    with open(val_file, 'r') as fVal:
        for line in fVal.readlines():
            if "LEAK SUMMARY:" in line:
                check_lines = 0

            if 0 <= check_lines < 5:
                check_lines += 1
                if ": 0 bytes in 0 blocks" not in line:
                    print("\tValgrind test - FAIL")
                    print("\tMemory leak found, more info in '" + val_file + "'\n")
                    return 0

            if "ERROR SUMMARY:" in line and "ERROR SUMMARY: 0 errors from" not in line:
                print("\tValgrind test - FAIL")
                print("\tError found, more info in '" + val_file + "'")
                return 0

    print("\tValgrind test - PASS")
    return 1


def run_test(test_name):
    print(f"Test {test_name}:")
    in_file = f"in/{test_name}.txt"
    cmp_file = f"cmp/{test_name}.txt"
    val_file = f"val/{test_name}_val.txt"
    new_output_file = f"out/{test_name}_output.txt"

    program = f"./{PROGRAM_NAME} {in_file}"
    if USE_VALGRIND:
        program = rf"valgrind --leak-check=full --log-file='{val_file}' ./{PROGRAM_NAME} {in_file}"

    subprocess.run(program, stderr=subprocess.STDOUT, shell=True, timeout=5)

    try:
        os.rename(OUTPUT_FILE, new_output_file)
    except FileNotFoundError:
        print("\tOutput test - FAIL: Cannot find railway_planner_output.txt")
        return 0, 0
    test_result = cmp_files(cmp_file, new_output_file)

    val_result = 0
    if USE_VALGRIND:
        val_result = check_valgrind_file(val_file)

    return test_result, val_result


if __name__ == '__main__':
    if "-val" in sys.argv:
        USE_VALGRIND = True

    print("\n------- COMPILATION START -------")
    isSuccess = compile_program()

    num_test_success = 0
    num_val_success = 0

    if (isSuccess):
        print("\n------- TEST START -------")
        for test_name in TESTS_FILE_NAMES:
            (t, v) = run_test(test_name)
            num_test_success += t
            num_val_success += v
            print("\n")
        print("------- TEST END -------\n")
        print(f"Test Pass: {num_test_success}/{len(TESTS_FILE_NAMES)}")
        if USE_VALGRIND:
            print(f"Valgrind Pass: {num_val_success}/{len(TESTS_FILE_NAMES)}")
