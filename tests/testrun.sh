#!/bin/bash

help () {
echo "test script -help
Valid input: testrun -options
-h				displays this message
-init			creates test directory /tests/$USER
-test			runs tests located in /tests/$USER/
-new [name] 	creates new test from testTemplate
-make [name]	creates a test from input and 

Not yet implemented
-commit			commits your tests to my github repository
"
exit 0
}

init () {
	if [ ! -d "tests" ]; then
		mkdir "tests/$USER/"
	elif [ ! -d "$USER" ]; then
		cd tests
		mkdir "$USER"
	fi
	exit 0
}

test () {
	testNum=0

	for testFile in tests/$USER/*; do
		((testNum++))
		./ifj20 <"$testFile/input" >"$testFile/output"
		echo "$?" > "$testFile/errCode"

		outputCompare=$(diff "$testFile/correctOutput" "$testFile/output")
		errcodeCompare=$(diff "$testFile/correctErrCode" "$testFile/errCode") 
	
		if [ ! "$outputCompare" == "" ] || [ ! "$errcodeCompare" == "" ]; then
			echo -e "Test #$testNum $testFile		\e[31mFailed\e[0m"
			if [ -n "$outputCompare" ] && [ -n "$errcodeCompare" ]; then

				echo -e "Both \e[1moutput file\e[0m and \e[1merror code\e[0m are wrong."
				echo -e "\e[1mOutput file\e[0m comparison:"
				echo "$outputCompare"
				echo ""
				echo -e "\e[1mOutput error code\e[0m comparison:"
				echo "$errcodeCompare"

			elif [ -n "$outputCompare" ]; then
				echo -e "\e[1mOutput file\e[0m is not correct."
				echo "$outputCompare"

			elif [ -n "$errcodeCompare" ]; then
				echo -e "\e[1mError code\e[0m is not correct."
				echo "$errcodeCompare"
			fi

		else
			echo -e "Test $testNum $testFile		\e[32mPassed\e[0m"
		fi
	done
}

new () {
	mkdir "tests/$USER/$1"
	cp tests/testTemplate/* "tests/$USER/$1"
}

make () {
	mkdir "tests/$USER/$1"
	cp input "tests/$USER/$1/"
	./ifj20 <input >"tests/$USER/$1/correctOutput"
	echo "$?" > "tests/$USER/$1/correctErrCode"

	echo "Created test tests/$USER/$1/"
	echo "The input file contains:"
	cat  "tests/$USER/$1/input"
	echo ""
	echo "The correctOutput file contains:"
	cat  "tests/$USER/$1/correctOutput"
	echo ""
	echo "The correctErrCode file contains:"
	cat  "tests/$USER/$1/correctErrCode"
}

# argumeny parsing
while [ $# -gt 0 ]
do
	if [ "$1" = "-h" ]; then
		help
	elif [ "$1" = "-init" ]; then
		init
	elif [ "$1" = "-test" ]; then
		test
	elif [ "$1" = "-new" ]; then
		if [ $# -eq 2 ]; then
			new $2
			shift
		else
			echo "-new [name] requires a name argument!" >&2
			exit 1
		fi
	elif [ "$1" = "-make" ]; then
		if [ $# -eq 2 ]; then
			make $2
			shift
		else
			echo "-make [name] requires a name argument!" >&2
			exit 1
		fi
	else
		echo "Invalid argument $1!" >&2
		echo "Use argument -h for help" >&2
		exit 1
	fi
	shift
done

 