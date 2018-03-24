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