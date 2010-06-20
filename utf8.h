#ifndef __UTF8_H__
#define __UTF8_H__

int utf8_pos(char *s, int len);
int utf8_is_valid(char *s);
int next_utf8(const char *s, unsigned int *c);

#endif
