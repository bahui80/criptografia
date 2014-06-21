#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>
#include <stdlib.h>

#define debug(fmt, ...)

#ifdef INFO
#define info(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define info(fmt, ...)
#endif
#ifndef NO_ERROR
#define error(fmt, ...) fprintf(stderr,fmt, ##__VA_ARGS__)
#else
#define error(fmt, ...)
#endif



#endif 
