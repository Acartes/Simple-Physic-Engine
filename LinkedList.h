#ifndef _LINKEDLIST_h
#define _LINKEDLIST_h

#include "BulletData.h"

struct node
{
	BulletData data;
	node *next;
	node();
};

class list
{
private:
	node * head, *tail;
public:
	list();
	void createnode(BulletData value);
	void insert_start(BulletData value);
	void insert_position(int pos, BulletData value);
	void delete_first();
	void delete_last();
	void delete_position(int pos);
	node* get_position(int pos);
};


#endif
