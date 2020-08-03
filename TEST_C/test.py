import random, string, os

NUMBER_OF_FILES = 10
PEOPLE_NUM = 100
MEETINGS_NUM = 50


def gen_files():
	for i in range(NUMBER_OF_FILES):
		people = list()
		pathPeople = 'in-out-example/' + str(i + 4) + '_people.in'
		pathMeeting = 'in-out-example/' + str(i + 4) + '_meeting.in'
		# people file
		with open(pathPeople, 'w') as peopleFile:
			for j in range(PEOPLE_NUM):
				personId = random.randint(100000, 999999)
				if not people.__contains__(personId):
					people.append(personId)
					name = ''.join(random.choices(string.ascii_lowercase, k=8))
					age = random.uniform(0, 120)
					peopleFile.write(name + ' ' + str(personId) + ' ' + str(age) + '\n')
		# meetings file
		with open(pathMeeting, 'w') as meetingFile:
			spreader = random.choice(people)
			meetingFile.write(str(spreader) + '\n')
			people.remove(spreader)
			infections = [spreader]
			count = 0
			while count < MEETINGS_NUM:
				spreader = infections.pop(0)
				num = random.randint(1, 4)
				# this spreader infections
				for j in range(num):
					infected = random.choice(people)
					infections.append(infected)
					people.remove(infected)
					dist = random.uniform(1, 3)
					time = random.uniform(15, 30)
					line = "{0:d} {1:d} {2:f} {3:f}\n".format(spreader, infected, dist, time)
					meetingFile.write(line)
					count += 1


def run_code():
	for i in range(1, NUMBER_OF_FILES + 4):
		run_string = '"cmake-build-debug\\SpreaderDetector.exe in-out-example\\{0}_people.in in-out-example\\{0}_meeting.in"'.format(i)
		os.system(run_string)
		sol_str = "in-out-example\\{0}_sol".format(i)
		save_str = "cat SpreaderDetectorAnalysis.out > " + sol_str + '.out'
		os.system(save_str)
		diff_str = "FC " + sol_str + '.out ' + sol_str + '.cmp > ' + sol_str + '.diff'
		os.system(diff_str)
		diff_name = sol_str + '.diff'
		with open(diff_name) as diff_file:
			if not diff_file.readlines()[1].__contains__("no differences encountered"):
				print('error in file number: ' + str(i))
			else:
				os.remove(diff_name)
	run_string = '"cmake-build-debug\\SpreaderDetector.exe in-out-example\\empty in-out-example\\empty"'
	os.system(run_string)
	if os.path.getsize("../SpreaderDetectorAnalysis.out") != 0:
		print('error with empty files')


if __name__ == "__main__":
	#gen_files()
	run_code()
	print('^^^^^^^^^^^^test ended^^^^^^^^^^^')






