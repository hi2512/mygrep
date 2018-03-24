#include <stdio.h>
#include <stdlib.h>

int prevValue;

int isPrintable(int c) {
	return (c > 31) && (c < 127);
}

void printRange(int * range) {
	int i;
	for (i = 0; i < 256; i++) {
		if (range[i]) {
			if (isPrintable(i)) {
				printf("%c", i);
			} else {
				printf("\\%03o", i);
			}
		}

	}
}

//is inclusive
void fillRange(int start, int end, int * range, int value) {
	int i;
	for (i = start; i != end; i++) {
		if (i == 256) {
			i = 0;
		}
		range[i] = value;
	}
}

int charIsNum(int c) {
	return (c >= '0') && (c <= '9');
}


int doSlash(char * exp, int index, int ** exprAr, int exprIndex) {
	//printf("START SLASH\n");
	index++;
	switch (exp[index]) {
	case '[':
	case ']':
	case '(':
	case ')':
	case '.':
	case '^':
	case '$':
	case '-':
	case '\\':
	case '*':
		prevValue = exp[index];
		/*
		 if (print) {
		 printf("%c", exp[index]);
		 }
		 */
		exprAr[exprIndex][(int) exp[index]] = 1;
		break;
		//for octal entry
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		prevValue = (exp[index] - '0') * 64;
		index++;
		prevValue += (exp[index] - '0') * 8;
		if (!charIsNum(exp[index])) {
			printf("\nBad slash usage\n");
		}
		index++;
		prevValue += (exp[index] - '0');
		if (!charIsNum(exp[index])) {
			printf("\nBad slash usage\n");
		}
		exprAr[exprIndex][prevValue] = 1;
		break;
	default:
		printf("\nBad slash usage\n");
		break;
	}
	//printf("END slash\n");
	return index + 1;
}

int isNum(int c) {
	return (c >= '0') && (c <= '9');
}

int doBracket(char * exp, int index, int ** exprAr, int exprIndex) {
	//printf("%s\n", exp);
	//skip over [
	index++;

	prevValue = -1;
	int c;
	//int range[256];
	int * range = exprAr[exprIndex];
	int startIndex;
	int inRange = 0;
	int complement = 0;
	int opValue = 1;

	//check if next is bracket
	if (exp[index] == ']') {
		printf("%c", exp[index]);
		index++;
	} else if (exp[index] == '^') {
		complement = 1;
		opValue = 0;
		index++;
	}

	int i;
	//fill range with 1s or 0s
	for (i = 0; i < 256; i++) {
		range[i] = complement;
	}

	//regular version
	while ((c = exp[index]) != ']') {
		//printf("c is %c\n", c);
		switch (c) {
		case '-':
			startIndex = prevValue + 1;
			index++;
			inRange = 1;
			break;
		case '^':
			printf("^ in wrong place\n");
			index++;
			break;
		case '\\':
			//printf("reached slash\n");
			if (isNum(exp[index + 1])) {
				startIndex = prevValue;
				index = doSlash(exp, index, exprAr, exprIndex);
				range[prevValue] = opValue;
				if (inRange) {
					fillRange(startIndex, prevValue, range, opValue);
					prevValue = -1;
					inRange = 0;
				}
				break;
			}
			//else fall through
		default:
			range[c] = opValue;
			index++;
			if (inRange) {
				fillRange(prevValue, c, range, opValue);
				prevValue = -1;
				inRange = 0;
				break;
			}
			prevValue = c;
			break;
		}
	}

	//printRange(range);
	//printf(" done\n");

	//return index of the next char
	return index + 1;

}

//function to retrieve regex stored in a file
char * getPattern(FILE * pattern) {
	int c, i, len;
	fseek(pattern, 0, SEEK_END);
	len = ftell(pattern);
	fseek(pattern, 0, SEEK_SET);

	char * res = (char *) malloc(len + 1);
	while ((c = fgetc(pattern)) != -1) {
		res[i] = (char) c;
		i++;
	}
	//null terminate
	res[i] = '\0';

	return res;
}

//function to retrieve the number of chars to match against
int exprLen(char * str) {
	int i = 0;
	int res = 0;
	int c;
	int inBracket = 0;
	int inSlash = 0;
	int numCount = 0;
	while ((c = str[i]) != '\0') {
		if (c == '[') {
			inBracket = 1;
			i++;
			continue;
		}
		if (c == '\\') {
			inSlash = 1;
			i++;
			continue;
		}
		if (inBracket) {
			if ((str[i - 1] != '[') && (c == ']')) {
				inBracket = 0;
			}
		}
		if (inSlash) {
			if (isNum(c)) {
				numCount++;
			}
			if ((!isNum(c)) || (numCount == 3)) {
				inSlash = 0;
				numCount = 0;
			}
		}
		if (!inBracket && !inSlash) {
			res++;
		}

		i++;
	}
	return res;
}

//changes to the regex to a 2d array to match against
int ** doPattern(FILE * pattern, char * name) {

	char * expr;
	//using file
	if (pattern != NULL) {
		expr = getPattern(pattern);
	} else {
		expr = name;
	}

	//printf("expression is: %s\n", expr);

	int i;
	int len = exprLen(expr);
	//printf("len is %d\n", len);
	int ** exprAr = (int **) calloc(len, sizeof(int *));
	for (i = 0; i < len; i++) {
		exprAr[i] = (int *) calloc(256, sizeof(int));
	}

	int c;
	int index = 0;
	int exprIndex = 0;
	while ((c = expr[index]) != '\0') {
		//printf("c is: %c\n", c);
		switch (c) {
		case ']':
			printf("Misplaced right bracket\n");
			break;
		case '[':
			index = doBracket(expr, index, exprAr, exprIndex);
			break;
		case '\\':
			index = doSlash(expr, index, exprAr, exprIndex);
			//printf("\n");
			break;
		case '(':
			break;
		default:
			//printf("%c\n", c);
			exprAr[exprIndex][c] = 1;
			index++;
			break;
		}
		exprIndex++;
	}

	return exprAr;
}
