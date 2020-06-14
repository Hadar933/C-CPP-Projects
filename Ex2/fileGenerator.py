from random import randint, choices

NUM_FILES = 20

LENGTH_IDX = 0
NUM_PART_TYPES_IDX = 1
PART_TYPES_IDX = 2
PARTS_IDX = 3

MIN_LENGTH, MAX_LENGTH = 0, 10000
MIN_PART_TYPES, MAX_PART_TYPES = 1, 128
MIN_PARTS, MAX_PARTS = 1, 500
MIN_PART_LEN_FACTOR, MAX_PART_LEN_FACTOR = 20, 4
MIN_PART_COST, MAX_PART_COST = 1, 10000

# ASCII chars, excluding first column & DEL & ,
char_list = [chr(i) for i in range(33, 127)]
char_list.remove(',')


def generate_data():
    length = randint(MIN_LENGTH, MAX_LENGTH)
    numparttypes = randint(MIN_PART_TYPES, MAX_PART_TYPES)
    parttypes = choices(char_list, k=numparttypes)
    numparts = randint(MIN_PARTS, MAX_PARTS)
    parts_list = []
    for i in range(numparts):
        # generate part's data
        s, e = choices(parttypes, k=2)
        part_length = randint(length // MIN_PART_LEN_FACTOR + 1, length // MAX_PART_LEN_FACTOR)
        part_cost = randint(MIN_PART_COST, MAX_PART_COST)
        parts_list.append((s, e, part_length, part_cost))
    return length, numparttypes, parttypes, parts_list


if __name__ == '__main__':
    for i in range(NUM_FILES):
        with open("validFiles/g{0}".format(i + 1), "w") as file:
            data = generate_data()
            file.write(str(data[LENGTH_IDX]) + '\n')
            file.write(str(data[NUM_PART_TYPES_IDX]) + '\n')
            file.write(",".join(str(x) for x in data[PART_TYPES_IDX]) + '\n')
            for part in data[PARTS_IDX]:
                file.write(",".join(str(x) for x in part) + '\n')
