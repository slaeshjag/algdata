#ifndef __BINARY_TREE_H__
#define	__BINARY_TREE_H__

typedef struct TREE {
	struct TREE		*low;
	struct TREE		*high;
	int			val;
} TREE;

#include "uppg33.h"

struct TREE *btree_new();

#endif
