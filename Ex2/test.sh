#!/bin/bash

echo ==================================================================================================================================
echo
echo Compiling...
echo
echo ==================================================================================================================================

gcc RailWayPlanner.c -Wall -Wextra -Wvla -std=c99 -lm -o RailWayPlanner
declare result=$?
if [[ result -ne 0  ]];
then
	echo
	echo "Something went wrong during compilation!"
	echo
	exit 1
fi

function blahblah {
	echo
	echo ==================================================================================================================================
	echo
	echo Tarring...
	echo
	echo ==================================================================================================================================
	mkdir -p ex2tar
	tar -cvf ex2tar/ex2.tar RailWayPlanner.c
	echo
	echo ==================================================================================================================================
	echo
	echo  presubmitting...
	echo
	echo ==================================================================================================================================
	~labcc2/www/ex2/presubmit_ex2 ex2tar/ex2.tar
	echo
	echo ==================================================================================================================================
	echo
	echo Style Checking...
	echo
	echo ==================================================================================================================================
	~labcc2/www/codingStyleCheck ex2tar/ex2.tar
}

if [[ $1 != "-short" ]];
then
	blahblah
fi

echo
echo ==================================================================================================================================
echo
echo Testing...
echo
echo ==================================================================================================================================
mkdir -p outputs

declare printCheck
echo ================================================================================================================================== | tee outputs/userOutput.txt outputs/schoolOutput.txt >/dev/null
echo validFiles | tee -a outputs/userOutput.txt outputs/schoolOutput.txt >/dev/null
echo | tee -a outputs/userOutput.txt outputs/schoolOutput.txt >/dev/null
for file in ./validFiles/*;
do
	echo "testing file $file"
	echo "$file" | tee -a outputs/userOutput.txt outputs/schoolOutput.txt >/dev/null
	printCheck=$(RailWayPlanner $file)
	if [ "$printCheck" ];
	then
		echo
		echo "Something was printed. Aborting tests."
		echo
		exit
	fi
	cat railway_planner_output.txt >> outputs/userOutput.txt
	~labcc2/www/ex2/school_solution $file >> outputs/schoolOutput.txt
	echo | tee -a outputs/userOutput.txt outputs/schoolOutput.txt >/dev/null
	echo | tee -a outputs/userOutput.txt outputs/schoolOutput.txt >/dev/null
done
echo ================================================================================================================================== | tee -a outputs/userOutput.txt outputs/schoolOutput.txt >/dev/null
echo invalidFiles | tee -a outputs/userOutput.txt outputs/schoolOutput.txt >/dev/null
echo | tee -a outputs/userOutput.txt outputs/schoolOutput.txt >/dev/null
for file in ./invalidFiles/*;
do
	echo "testing file $file"
	echo "$file" | tee -a outputs/userOutput.txt outputs/schoolOutput.txt >/dev/null
	printCheck=$(RailWayPlanner $file)
	if [ "$printCheck" ];
	then
		echo
		echo "Something was printed. Aborting tests."
		echo
		exit
	fi
	cat railway_planner_output.txt >> outputs/userOutput.txt
	~labcc2/www/ex2/school_solution $file >> outputs/schoolOutput.txt
	echo | tee -a outputs/userOutput.txt outputs/schoolOutput.txt >/dev/null
	echo | tee -a outputs/userOutput.txt outputs/schoolOutput.txt >/dev/null
done
	echo "testing file no_file"
	echo "no_file" | tee -a outputs/userOutput.txt outputs/schoolOutput.txt >/dev/null
	printCheck=$(RailWayPlanner "no_file")
	if [ "$printCheck" ];
	then
		echo
		echo "Something was printed. Aborting tests."
		echo
		exit
	fi
	cat railway_planner_output.txt >> outputs/userOutput.txt
	~labcc2/www/ex2/school_solution "no_file" >> outputs/schoolOutput.txt
	echo | tee -a outputs/userOutput.txt outputs/schoolOutput.txt >/dev/null
	echo | tee -a outputs/userOutput.txt outputs/schoolOutput.txt >/dev/null
echo ================================================================================================================================== | tee -a outputs/userOutput.txt outputs/schoolOutput.txt >/dev/null
echo

if (diff outputs/schoolOutput.txt outputs/userOutput.txt > outputs/diff.txt)
then
	echo "You've successfully passed all tests!"
else
	echo "You've failed some tests!"
fi

echo "Check outputs/(schoolOutput.txt, userOutput.txt, diff.txt) to compare differences"
echo "Make sure you've passed presubmission and style check!"
echo
