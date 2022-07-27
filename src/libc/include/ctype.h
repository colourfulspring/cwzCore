#ifndef _CTYPE_H_
#define _CTYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define isupper(c) (c >= 'A' && c <= 'Z')
#define isalpha(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
#define isspace(c) (c == ' ' || c == '\t' || c == '\n' || c == '\12')
#define isdigit(c) (c >= '0' && c <= '9')

#ifdef __cplusplus
}
#endif

#endif /* _CTYPE_H_ */
