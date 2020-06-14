#!/bin/bash

echo ==================================================================================================================================
echo
echo Valgrinding...
echo
echo ==================================================================================================================================

echo ================================================================================================================================== | tee valLog.txt >/dev/null
echo validFiles | tee -a valLog.txt >/dev/null
echo | tee -a valLog.txt >/dev/null

for file in validFiles/*;
do
	echo "testing file $file" | tee -a valLog.txt
	valgrind --leak-check=full --log-file="leakCheck.txt" RailWayPlanner $file
	grep -q "All heap blocks were freed -- no leaks are possible" leakCheck.txt
	if [[ $? -ne 0 ]];
	then
		cat leakCheck.txt | tee -a valLog.txt
	else
		echo "All heap blocks were freed - no leaks are possible"
		cat leakCheck.txt | tee -a valLog.txt >/dev/null
	fi
	echo | tee -a valLog.txt
done

echo ================================================================================================================================== | tee -a valLog.txt >/dev/null
echo invalidFiles | tee -a valLog.txt >/dev/null
echo | tee -a valLog.txt >/dev/null

for file in invalidFiles/*;
do
    echo "testing file $file" | tee -a valLog.txt
    valgrind --leak-check=full --log-file="leakCheck.txt" RailWayPlanner $file
	grep -q "All heap blocks were freed -- no leaks are possible" leakCheck.txt
    if [[ $? -ne 0 ]];
    then
            at leakCheck.txt | tee -a valLog.txt
    else
            echo "All heap blocks were freed - no leaks are possible"
	cat leakCheck.txt | tee -a valLog.txt >/dev/null
    fi
	echo | tee -a valLog.txt
done

echo "testing file no_file" | tee -a valLog.txt
valgrind --leak-check=full --log-file="leakCheck.txt" RailWayPlanner "no_file"
grep -q "All heap blocks were freed -- no leaks are possible" leakCheck.txt
if [[ $? -ne 0 ]];
then
        at leakCheck.txt | tee -a valLog.txt
else
        echo "All heap blocks were freed - no leaks are possible"
cat leakCheck.txt | tee -a valLog.txt >/dev/null
fi

echo | tee -a valLog.txt
echo ================================================================================================================================== | tee -a valLog.txt >/dev/null

rm leakCheck.txt
echo "Valgrinding done!"
echo "See valLog.txt to check your results!"
echo
