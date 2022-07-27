#ifndef _STDDEF_H_
#define _STDDEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _PTRDIFF_T
#define _PTRDIFF_T
typedef long ptrdiff_t;
#endif

#ifndef _SIZE_T
#define _SIZE_T
#undef size_t
typedef unsigned int size_t;
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifdef __cplusplus
}
#endif

#ifndef __cplusplus
#define nullptr NULL
#endif

#endif /* _STDDEF_H_ */
