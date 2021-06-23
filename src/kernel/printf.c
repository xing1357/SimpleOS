/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "printf.h"
#include "stdarg.h"
#include "drivers/screen/screen.c"


int vprintf(const char *fmt, va_list ap)
{
	const char *hex = "0123456789abcdef";
	char buf[11];
	char *s;
	unsigned u;
	int ret = 0, i, c;

	while ((c = *fmt++)) {
		if (c == '%') {
			c = *fmt++;
			switch (c) {
			case 'c':
				print_char(va_arg(ap, int));
				ret++;
				continue;
			case 's':
				for (s = va_arg(ap, char *); *s; s++) {
					print_char(*s);
					ret++;
				}
				continue;
			case 'd':
				i = va_arg(ap, int);
				u = (i < 0 ? -i : i);
				s = buf;
				do {
					*s++ = '0' + u % 10U;
				} while (u /= 10U);
				if (i < 0)
					*s++ = '-';
				goto dumpbuf;
			case 'u':
				u = va_arg(ap, unsigned);
				s = buf;
				do {
					*s++ = '0' + u % 10U;
				} while (u /= 10U);
				goto dumpbuf;
			case 'x':
				u = va_arg(ap, unsigned);
				s = buf;
				do {
					*s++ = hex[u & 0xfu];
				} while (u >>= 4);
			dumpbuf:
				while (--s >= buf) {
					print_char(*s);
					ret++;
				}
				continue;
			}
		}
		print_char(c);
		ret++;
	}

	return (ret);
}


int printf(const char *fmt,...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vprintf(fmt, ap);
	va_end(ap);

	return (ret);
}