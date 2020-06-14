import difflib
import subprocess
import sys

TESTS = ["EdgeCases", "GeneralTest"]
USE_VAL = False


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
                    print(
                        "\tMemory leak found, more info in '" + val_file + "'\n")
                    return 0

            if "ERROR SUMMARY:" in line and "ERROR SUMMARY: 0 errors from" not in line:
                print("\tValgrind test - FAIL")
                print("\tError found, more info in '" + val_file + "'")
                return 0

    print("\tValgrind test - PASS")
    return 1


def cmp_files(cmp_file, out_file):
    with open(cmp_file, 'r') as fCmp:
        with open(out_file, 'r') as fOut:
            diff = difflib.ndiff(fOut.readlines(), fCmp.readlines())

            for i, line in enumerate(diff):
                if line.startswith("- ") or line.startswith(
                        "+ ") or line.startswith("? "):
                    print("\tOutput test - FAIL")
                    print(
                        "\tDifference found in '" + out_file + "' at line " + str(
                            i))
                    return 0

            print("\tOutput test - PASS")
            return 1


def run_test(file_name):
    cmp_file = f"cmp/{file_name}.txt"
    val_file = f"val/{file_name}_val.txt"
    output_file = f"out/{file_name}_output.txt"
    try:
        print(f"Test {file_name}:")
        with open(output_file, 'w') as fOut:
            if USE_VAL:
                program = "valgrind --leak-check=full --log-file=" + val_file + " " + file_name
            else:
                program = file_name
            subprocess.run(program, stdout=fOut, stderr=subprocess.STDOUT,
                           shell=True, timeout=10, cwd="in")

    except subprocess.TimeoutExpired:
        print(file_name + "\tTest - FAIL")
        print("\tTIMEOUT Reached")

    test_result = cmp_files(cmp_file, output_file)
    val_result = 0
    if USE_VAL:
        val_result = check_valgrind_file(f"in/{val_file}")

    return test_result, val_result


def compile_c(file_name):
    CREATE_GCC_EDGE_CASE = f"gcc -Wextra -Wall -Wvla -std=c99 -Wno-unused-parameter -lm" \
                           f" {file_name}.c RBTree.c RBTree.h -o {file_name}"
    return subprocess.run(CREATE_GCC_EDGE_CASE, text=True, shell=True,
                          cwd="in").returncode


if __name__ == '__main__':
    if "-val" in sys.argv:
        USE_VAL = True
    print("\n------- COMPILATION START -------")
    for test in TESTS:
        if compile_c(test) != 0:
            print("\nProgram failed compiling")
            exit(1)
    print(
        "------- COMPILATION END - The section above should have no warnings nor errors -------")

    print("\n------- TEST START -------")
    num_test_success = 0
    num_val_success = 0
    for test in TESTS:
        (t, v) = run_test(test)
        num_test_success += t
        num_val_success += v
        print("\n")

    print("------- TEST END -------\n")
    print(
        f"Test Pass: {num_test_success}/{len(TESTS)}")
    if USE_VAL:
        print(
            f"Valgrind Pass: {num_val_success}/{len(TESTS)}")
