#ifndef _SWG_DEF_H
#define _SWG_DEF_H

#include "utGlobals.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#endif


#if 0
#if 0
#define TICKS_TIMER INT16U
TICKS_TIMER tmrSys;
TICKS_TIMER tmrATRet;
#define TMR_FRM(x)	((TICKS_TIMER)(tmrSys-x))
#endif

extern int sprintf  (char *, const char *, ...);
extern int scanf (const char *, ...);

extern char *strcat (char *s1, char *s2);
extern char *strncat (char *s1, char *s2, int n);

extern char strcmp (char *s1, char *s2);
extern char strncmp (char *s1, char *s2, int n);

extern char *strcpy (char *s1, char *s2);
extern char *strncpy (char *s1, char *s2, int n);

extern int strlen (char *);

extern char *strchr (const char *s, char c);
extern int strpos (const char *s, char c);
extern char *strrchr (const char *s, char c);
extern int strrpos (const char *s, char c);

extern int strspn (char *s, char *set);
extern int strcspn (char *s, char *set);
extern char *strpbrk (char *s, char *set);
extern char *strrpbrk (char *s, char *set);
extern char *strstr  (char *s, char *sub);
extern char *strtok  (char *str, const char *set);

extern char memcmp (void *s1, void *s2, int n);
extern void *memcpy (void *s1, void *s2, int n);
extern void *memchr (void *s, char val, int n);
extern void *memccpy (void *s1, void *s2, char val, int n);
extern void *memmove (void *s1, void *s2, int n);
extern void *memset  (void *s, char val, int n);
#endif

