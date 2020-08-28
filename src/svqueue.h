/*
 * Name:        svqueue.h
 * Description: Queues interface.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0320170743W1106172030L00177
 *
 * The following text is copied from the source code of SQLite and padded
 * with a little bit addition to fit the goals for StoneValley project:
 *
 * The author disclaims copyright to this source code. In place of
 * a legal notice, here is a blessing:
 *   May you do good and not evil.
 *   May you find forgiveness for yourself and forgive others.
 *   May you share freely, never taking more than you give.
 *   Hope you never need to push yourself or other people too hard.
 */

#ifndef _SVQUEUE_H_
#define _SVQUEUE_H_

#include "svstring.h"

/* Array implementation of queues. */
typedef struct st_QUEUE_A {
	size_t  front; /* Index of the first element in the array. */
	size_t  rear;  /* Index of the last  element in the array. */
	ARRAY_Z arr;   /* Array buffer. */
} QUEUE_A, * P_QUEUE_A;

typedef struct st_QUEUE_L {
	P_NODE_S pfront; /* A pointer of header. */
	P_NODE_S prear;  /* A pointer of tail. */
} QUEUE_L, * P_QUEUE_L;

typedef struct st_DEQUE_DL {
	P_NODE_D pfirst; /* A pointer of the first element. */
	P_NODE_D plast;  /* A pointer of the last element. */
} DEQUE_DL, * P_DEQUE_DL;

/* Function declarations here. */
/* Functions for circular queues that use fixed-size arrays. */
void *     queInitAC        (P_QUEUE_A  pqueac, size_t       num,   size_t     size);
void       queFreeAC        (P_QUEUE_A  pqueac);
P_QUEUE_A  queCreateAC      (size_t     num,    size_t       size);
void       queDeleteAC      (P_QUEUE_A  pqueac);
BOOL       queIsInitialAC_O (P_QUEUE_A  pqueac);
size_t     queUsageAC_O     (P_QUEUE_A  pqueac);
void       queInsertAC_O    (P_QUEUE_A  pqueac, const void * pitem, size_t     size);
void       queRemoveAC_O    (void *     pitem,  size_t       size,  P_QUEUE_A  pqueac);
/* Functions for queues that implemented with single linked-lists. */
void       queInitL_O       (P_QUEUE_L  pquel);
void       queFreeL         (P_QUEUE_L  pquel);
P_QUEUE_L  queCreateL       (void);
void       queDeleteL       (P_QUEUE_L  pquel);
BOOL       queIsEmptyL_O    (P_QUEUE_L  pquel);
size_t     queUsageL_O      (P_QUEUE_L  pquel);
BOOL       queInsertL       (P_QUEUE_L  pquel,  const void * pitem, size_t     size);
BOOL       queRemoveL       (void *     pitem,  size_t       size,  P_QUEUE_L  pquel);
/* Functions for queues that implemented with doubly linked-lists. */
void       queInitDL_O      (P_DEQUE_DL pdeque);
void       queFreeDL        (P_DEQUE_DL pdeque);
P_DEQUE_DL queCreateDL      (void);
void       queDeleteDL      (P_DEQUE_DL pdeque);
BOOL       queIsEmptyDL_O   (P_DEQUE_DL pdeque);
size_t     queUsageDL_O     (P_DEQUE_DL pdeque);
void       queFirstDL_O     (void *     pitem,  size_t       size,  P_DEQUE_DL pdeque);
P_NODE_D   quePushDL        (P_DEQUE_DL pdeque, const void * pitem, size_t     size);
P_NODE_D   quePopDL         (void *     pitem,  size_t       size,  P_DEQUE_DL pdeque);
void       queLastDL_O      (void *     pitem,  size_t       size,  P_DEQUE_DL pdeque);
P_NODE_D   queInjectDL      (P_DEQUE_DL pdeque, const void * pitem, size_t     size);
P_NODE_D   queEjectDL       (void *     pitem,  size_t       size,  P_DEQUE_DL pdeque);

/* Macros for function inline to accelerate execution speed. */
/* Functions in svqueue.c. */
#define queIsInitialAC_M(pqueac_M) ((pqueac_M)->front == (pqueac_M)->rear)
#define queUsageAC_M(pqueac_M) ((pqueac_M)->front > (pqueac_M)->rear ? \
	(pqueac_M)->arr.num - (pqueac_M)->front + (pqueac_M)->rear : \
	(pqueac_M)->rear - (pqueac_M)->front)
#define queInsertAC_M(pqueac_M, pitem_M, size_M) { \
	memcpy((pqueac_M)->arr.pdata + (pqueac_M)->rear * (size_M), (pitem_M), (size_M)); \
	(pqueac_M)->rear = ((pqueac_M)->rear + 1) % (pqueac_M)->arr.num; \
}
#define queRemoveAC_M(pitem_M, size_M, pqueac_M) { \
	memcpy((pitem_M), (pqueac_M)->arr.pdata + (pqueac_M)->front * (size_M), (size_M)); \
	(pqueac_M)->front = ((pqueac_M)->front + 1) % (pqueac_M)->arr.num; \
}
/* Macros for single linked-list queues. */
#define queInitL_M(pquel_M) { \
	(pquel_M)->pfront = (pquel_M)->prear = NULL; \
}
#define queIsEmptyL_M(pquel_M) (!(pquel_M)->pfront)
#define queUsageL_M(pquel_M) (strLevelLinkedListSC((pquel_M)->pfront))
/* Macros for doubly linked-list queues. */
#define queInitDL_M(pdeque_M) { \
	(pdeque_M)->pfirst = (pdeque_M)->plast = NULL; \
}
#define queIsEmptyDL_M(pdeque_M) (!(pdeque_M)->pfirst)
#define queUsageDL_M(pdeque_M) (strLevelLinkedListDC((pdeque_M)->pfirst, FALSE))
#define queFirstDL_M(pitem_M, size_M, pdeque_M) { \
	memcpy((pitem_M), (pdeque_M)->pfirst->pdata, (size_M)); \
}
#define queLastDL_M(pitem_M, size_M, pdeque_M) { \
	memcpy((pitem_M), (pdeque_M)->plast->pdata, (size_M)); \
}

/* Library optimal switch. */
#if   SV_OPTIMIZATION == SV_OPT_MINISIZE
	#include <string.h> /* Using function memcpy. */
	/* Macros for circular queues. */
	#define queIsInitialAC queIsInitialAC_M
	#define queUsageAC     queUsageAC_M
	#define queInsertAC    queInsertAC_O
	#define queRemoveAC    queRemoveAC_O
	/* Macros for single linked-list queues. */
	#define queInitL       queInitL_M
	#define queIsEmptyL    queIsEmptyL_M
	#define queUsageL      queUsageL_M
	/* Macros for doubly linked-list queues. */
	#define queInitDL      queInitDL_M
	#define queIsEmptyDL   queIsEmptyDL_M
	#define queUsageDL     queUsageDL_M
	#define queFirstDL     queFirstDL_M
	#define queLastDL      queLastDL_M
#elif SV_OPTIMIZATION == SV_OPT_MAXSPEED
	#include <string.h> /* Using function memcpy. */
	/* Macros for circular queues. */
	#define queIsInitialAC queIsInitialAC_M
	#define queUsageAC     queUsageAC_M
	#define queInsertAC    queInsertAC_M
	#define queRemoveAC    queRemoveAC_M
	/* Macros for single linked-list queues. */
	#define queInitL       queInitL_M
	#define queIsEmptyL    queIsEmptyL_M
	#define queUsageL      queUsageL_M
	/* Macros for doubly linked-list queues. */
	#define queInitDL      queInitDL_M
	#define queIsEmptyDL   queIsEmptyDL_M
	#define queUsageDL     queUsageDL_M
	#define queFirstDL     queFirstDL_M
	#define queLastDL      queLastDL_M
#elif SV_OPTIMIZATION == SV_OPT_FULLOPTM
	#include <string.h> /* Using function memcpy. */
	/* Macros for circular queues. */
	#define queIsInitialAC queIsInitialAC_M
	#define queUsageAC     queUsageAC_M
	#define queInsertAC    queInsertAC_M
	#define queRemoveAC    queRemoveAC_M
	/* Macros for single linked-list queues. */
	#define queInitL       queInitL_M
	#define queIsEmptyL    queIsEmptyL_M
	#define queUsageL      queUsageL_M
	/* Macros for doubly linked-list queues. */
	#define queInitDL      queInitDL_M
	#define queIsEmptyDL   queIsEmptyDL_M
	#define queUsageDL     queUsageDL_M
	#define queFirstDL     queFirstDL_M
	#define queLastDL      queLastDL_M
#else /* Optimization has been disabled. */
	/* Macros for circular queues. */
	#define queIsInitialAC queIsInitialAC_O
	#define queUsageAC     queUsageAC_O
	#define queInsertAC    queInsertAC_O
	#define queRemoveAC    queRemoveAC_O
	/* Macros for single linked-list queues. */
	#define queInitL       queInitL_O
	#define queIsEmptyL    queIsEmptyL_O
	#define queUsageL      queUsageL_O
	/* Macros for doubly linked-list queues. */
	#define queInitDL      queInitDL_O
	#define queIsEmptyDL   queIsEmptyDL_O
	#define queUsageDL     queUsageDL_O
	#define queFirstDL     queFirstDL_O
	#define queLastDL      queLastDL_O
#endif

#endif
