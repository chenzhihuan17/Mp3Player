#ifndef __DEBUG_H__
#define __DEBUG_H__

#define ECHO_DEBUG

#ifdef CZH_DEBUG
#include <assert.h>
#define 	ASSERT(x)		assert(x)
#define	CZH_DBG(fmt, ...)	do {printf("[CZH-DBG]:<%s, %d> ",__func__, __LINE__); printf(fmt, ##__VA_ARGS__); putchar('\n'); fflush(stdout); } while (0)
#else
#define	CZH_DBG(fmt, ...) 	NULL
#define 	ASSERT(x)		NULL

#endif /* DEBUG */

#define	CZH_DBG(fmt, ...)	do {printf("[CZH-ERR]:<%s, %s, %d > ", __FILE__, __func__, __LINE__); printf(fmt, ##__VA_ARGS__); putchar('\n'); fflush(stdout); } while (0)

#endif   //DEBUG_H__

