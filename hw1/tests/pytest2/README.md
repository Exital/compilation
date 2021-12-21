Changes:
	1. The tester checks for some errors now.
	2. Bug fixes.

How to run it:
	1. open the config file in the tests directory.
	2. change the values according to your preferences.
		NUM_OF_TOKENS=num of generated tokens at each test
		NUM_OF_TESTS=num of generated tests
		EXEC_FILE=name of your execution file
		COMP_CMD_1=first compilation command
		COMP_CMD_2=second compilation command
		TOKEN_TYPES=what types of tokens the tester should add to the tests
		
		When testing cases of error, you should set:
			NUM_OF_TOKENS=~100
			NUM_OF_TESTS=~300
		When testing regular cases, you should set:
			NUM_OF_TOKENS=~1000
			NUM_OF_TESTS=~20
		For brute testing you should set:
			NUM_OF_TOKENS=~10000
			NUM_OF_TESTS=~50
	3. run:
		chmod +x tests/tester.py
	4.1. for tests without errors in the input files:
		tests/tester.py
	4.2. for tests with errors in the input files:
		tests/tester.py -e
	5. check the input/output/expected/diff files in the following directory:
		tests/test_files

Flags:
	I highly recommend using the built-in flags of the tester.
	You can see which flags available by running:
		tests/tester.py -h
	Here are some examples for running the tester with flags:
		1. generating tests with errors:
			tests/tester.py -e
		2. compiling before running the tests:
			tests/tester.py -c
		3. generating tests with errors and compiling before running the tests:
			tests/tester.py -c -e
		3. running specific test which was previously generated:
			tests/tester.py -r <test_num>
		4. compiling and then running specific test which was previously generated:
			tests/tester.py -c -r <test_num>
	Flags names as -e, -r, etc are abbreviation for --errors, --compile, etc.
	Of course you can use the full name of the flags.
	Also, you can run the tester with more than one flag at once as shown in the examples.

***IMPORTANT NOTE***
The tester still doesn't check all posible cases!
If you have any questions about which cases are not being tested, feel free to send me a message.
Good luck!
