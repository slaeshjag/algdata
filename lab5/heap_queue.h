/* heap_queue.h - Steven Arnow <s@rdw.se>,  2013 */
#ifndef __HEAP_QUEUE_H__

typedef struct HEAP_QUEUE {
	struct HEAP_QUEUE	*left;
	struct HEAP_QUEUE	*right;
	int			prio;
	void			*data;
} HEAP_QUEUE;


typedef struct {
	HEAP			*heap;
} HEAP;



#endif
