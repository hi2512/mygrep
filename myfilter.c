#include <stdio.h>
#include <stdlib.h>
#include "addbuf.c"
#include "fillbuf.c"
#include "patternMatch.c"

void doFilter(int ** exprAr, int len) {

	//lines in the pattern is the length to check
	char * buf = (char *) calloc(1, len);
	char * line = NULL;
	size_t size;
	int pos, printed;

	//go line by line
	while (getline(&line, &size, stdin) != -1) {
		printed = 0;
		if (!(pos = fillbuf(buf, len, line))) {
			//printf("string shorter than pattern\n");
			continue;
		}
		//while not at the end of the line
		while (line[pos] != '\0') {
			//if it matches then print
			if (patternMatch(buf, exprAr, len)) {
				if (line[0] != '\n') {
					printf("%s", line);
				}
				printed = 1;
				break;
			}
			//get one more char, add to the buffer and see if it matches
			addbuf(buf, len, line[pos]);
			pos++;
		}
		if (((!printed) && patternMatch(buf, exprAr, len))) {
			if (line[0] != '\n') {
				printf("%s", line);
			}
		}
	}

}

