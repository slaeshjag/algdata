#ifndef __UPPG13_H__
#define	__UPPG13_H__

typedef void LIFO;

LIFO *lifo_new(int elements_max);
int lifo_pop(LIFO *l);
void lifo_push(LIFO *l, int element);
int lifo_size(LIFO *l);


#endif
