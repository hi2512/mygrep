int addbuf(char * buf, int bufsize, int c) {
	int i;
	for (i = 0; i < (bufsize - 1); i++) {
		buf[i] = buf[i + 1];
	}
	buf[bufsize - 1] = c;
	return 1;
}