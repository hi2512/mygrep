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