import os, random, string


def generate_strings():
    with open("test\\in\\string.in", "w") as file:
        for i in range(500):
            name = ''.join(random.choices(string.ascii_lowercase, k=4))
            file.write(name + '\n')


def run_test():
    run_string = '"cmake-build-debug\\VLVector.exe"'
    os.system(run_string)
    int_diff_str = "diff test\\out\\int.out test\\cmp\\int.cmp"
    os.system(int_diff_str)
    str_diff_str = "diff test\\out\\str.out test\\cmp\\str.cmp"
    os.system(str_diff_str)


if __name__ == '__main__':
    generate_strings()
    run_test()
