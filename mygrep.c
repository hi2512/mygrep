/*
NAME
  mygrep

SYNOPSIS
  mygrep [-f FILE]
  mygrep [--file=FILE]

DESCRIPTION
  Mygrep reads from standard input and searches from
  the lines read for ones that match the given pattern.

  When mygrep is executed, a pattern must be given to the
  program, either directly or through a file if the -f option
  or the --file= option is given. Unlike grep, in all cases
  mygrep will read from standard input, and from there, will
  generate its output.

  The design of mygrep was created by taking homework
  assignment 5 and homework assignment 6 and combining them
  together. This mygrep.c file contains the main routine of
  mygrep. The files mypattern.c and myfilter.c contain the
  routines for processing the input pattern and for comparing
  input from standard input to the pattern respectively.

  In mygrep.c, the program first determines if a file
  containing the pattern needs to be opened, and either opens
  it or selects the pattern as one of the parameters given to
  the program. From there, the doPattern function from
  mypattern.c is called to create the 2 dimensional array
  as specified in homework. This 2 dimensional array will
  have its number or rows equal to amount of characters to
  match in the regular expression given. Its number of columns
  will always be 256, because it it is supposed to be a mapping
  of characters. The value of any entry will always be either
  0 or 1. For example if the entry in the array at [0][97] is
  1, this means that the first character in this expression can
  be the character 'a' and will be treated as a match if the
  character 'a' is found within the input received from standard
  input.

  After this 2 dimensional array is created, it is passed off to
  the doFilter function within the myfilter.c file. Within this
  routine, the same process for matching as done in homework 6
  will be done with some slight changes. From standard input, the
  input will be processed line by line, just like the original
  grep. From here, it will add characters from the line to a buffer
  and then it will check to see if it can match the regular
  expression given. Differing from homework 6, instead of counting
  all the possible times the line can be matched, mygrep will
  immediately print the line and move on to the next line. This is
  done to mimic the functionality of the original grep. Mygrep
  will continue to do this until it reaches the end of the input
  that was received from standard input.

  As required by the specification, this lab includes a makefile
  to create the executable file. The makefile has two commands,
  'mygrep' and 'clean'. 'make mygrep' will create the .o files
  and mygrep, and 'make clean' will delete the files created by
  'make mygrep'.

  -f, --file=<file>
    reads in a regular expression from a given file to use
    as a pattern

AUTHOR
  Eric Nguyen
  en4939

*/
#include <stdio.h>
#include <stdlib.h>
#include "mypattern.h"
#include "myfilter.h"


char * optarg;
char * opt;
int argInd;

int isoption(char * opt) {
	return opt[0] == '-';
}

//my version of getopt
int mygetopt(int argc, char * argv[], char * options) {
	int i, y;
	int res = -1;
	for (i = 0; i < argc; i++) {
		y = 0;
		//printf("cur argv is %s\n", argv[i]);
		if (isoption(argv[i])) {
			//check if it matches an option
			while (options[y] != '\0') {
				//printf("matching for %c to %c", argv[i][1], options[y]);
				if (argv[i][1] == options[y]) {
					//found match
					res = options[y];
					//set the optarg
					optarg = argv[i + 1];
					argInd = i + 1;
					opt = argv[i];
					//return
					argv[i] = "\0";
					argv[i + 1] = "\0";
					return res;
				}
				y++;
			}

		}

	}

	return res;
}



int main(int argc, char * argv[], char * env[]) {

	FILE * pattern = NULL;

	if ((argc != 2) && (argc != 3)) {
		printf("Wrong number of args: %d\n", argc);
		return 1;
	}

	//get options
	int op;
	while ((op = mygetopt(argc, argv, "f-")) != -1) {
		switch (op) {
		case 'f':
			pattern = fopen(optarg, "r");
			break;
		case '-':
			//skip --file=
			opt = opt + 7;
			pattern = fopen(opt, "r");
			break;
		}
	}

	//process the pattern
	int ** exprAr = doPattern(pattern, argv[1]);
	char * expr;
	//using file
	if (pattern != NULL) {
		expr = getPattern(pattern);
	} else {
		expr = argv[1];
	}
	printf("expr: %s\n", expr);
	int len = exprLen(expr);

	//filter and print
	doFilter(exprAr, len);


	return 0;
}
