#include <stdio.h>
#include <stdlib.h>

int fillbuf(char * buf, int bufsize, char * source) {
	int i, c;
	for (i = 0; i < bufsize; i++) {
		if ((c = source[i]) == '\n') {
			return 0;
		}
		buf[i] = c;
	}
	return i;
}

int addbuf(char * buf, int bufsize, int c) {
	int i;
	for (i = 0; i < (bufsize - 1); i++) {
		buf[i] = buf[i + 1];
	}
	buf[bufsize - 1] = c;
	return 1;
}

int patternMatch(char * buf, int ** pattern, int length) {
	//printf("pattern matching %s\n", buf);
	int i;
	for (i = 0; i < length; i++) {
		if (!pattern[i][(int) buf[i]]) {
			//printf("i: %d, buf %d\n", i, (int) buf[i]);
			return 0;
		}
	}
	return 1;
}

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

