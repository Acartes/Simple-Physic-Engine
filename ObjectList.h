#ifndef _OBJECTLIST_h
#define _OBJECTLIST_h

#include "Objects.h"

struct objectNode
{
	Object data;
	objectNode *next;
	objectNode();
};

class objectList
{
private:
	objectNode * head, *tail;
public:
	objectList();
	void createnode(Object value);
	void insert_start(Object value);
	void insert_position(int pos, Object value);
	void delete_first();
	void delete_last();
	void delete_position(int pos);
	objectNode* get_position(int pos);
	int size;
};


#endif
