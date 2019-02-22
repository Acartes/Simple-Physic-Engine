#include "ObjectList.h"

objectNode::objectNode() {

}

objectList::objectList() {
	head = NULL;
	tail = NULL;
}
void objectList::createnode(Object value)
{
	objectNode *temp = new objectNode;
	temp->data = value;
	temp->next = NULL;
	size++;
	if (head == NULL)
	{
		head = temp;
		tail = temp;
		temp = NULL;
	}
	else
	{
		tail->next = temp;
		tail = temp;
	}
}
void objectList::insert_start(Object value)
{
	objectNode *temp = new objectNode;
	temp->data = value;
	temp->next = head;
	head = temp;
	size++;
}
void objectList::insert_position(int pos, Object value)
{
	objectNode *pre = new objectNode;
	objectNode *cur = new objectNode;
	objectNode *temp = new objectNode;
	cur = head;
	for (int i = 1; i<pos; i++)
	{
		pre = cur;
		cur = cur->next;
	}
	temp->data = value;
	pre->next = temp;
	temp->next = cur;
	size++;
}
void objectList::delete_first()
{
	objectNode *temp = new objectNode;
	temp = head;
	head = head->next;
	delete temp;
	size--;
}
void objectList::delete_last()
{
	objectNode *current = new objectNode;
	objectNode *previous = new objectNode;
	current = head;
	while (current->next != NULL)
	{
		previous = current;
		current = current->next;
	}
	tail = previous;
	previous->next = NULL;
	delete current;
	size--;
}
void objectList::delete_position(int pos)
{
	objectNode *current = new objectNode;
	objectNode *previous = new objectNode;
	current = head;
	for (int i = 1; i<pos; i++)
	{
		previous = current;
		current = current->next;
	}
	previous->next = current->next;
	size--;
}
objectNode* objectList::get_position(int pos) {
	objectNode *current = head;
	objectNode *previous = head;
	current = head;
	for (int i = 0; i<pos; i++)
	{
		previous = current;
		current = current->next;
	}
	return current;
}
