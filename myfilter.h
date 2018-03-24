/*
 * myfilter.h
 *
 *  Created on: Mar 5, 2018
 *      Author: Eric
 */

#ifndef MYFILTER_H_
#define MYFILTER_H_

int fillbuf(char * buf, int bufsize, char * source);
int addbuf(char * buf, int bufsize, int c);
int patternMatch(char * buf, int ** pattern, int length);
void doFilter(int ** exprAr, int len);

#endif /* MYFILTER_H_ */
