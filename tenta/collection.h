#ifndef __COLLECTION_H__
#define	__COLLECTION_H__

struct COLLECTION_LINK {
	int			entry;
	struct COLLECTION_LINK	*next;
};

struct COLLECTION_DLINK {
	int			entry;
	struct COLLECTION_DLINK	*prev;
	struct COLLECTION_DLINK	*next;
};


typedef union {
	struct COLLECTION_DLINK	**link;
	int			**entries;
} COLLECTION_DDATA;

typedef union {
	struct COLLECTION_LINK	**link;
	int			*entries;
} COLLECTION_DATA;


#endif
