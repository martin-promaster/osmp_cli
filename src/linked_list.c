/*--------------------------------------------------
	linked list methods.
--------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/xdefine.h"
#include "../include/linked_list.h"

linked_list* create_node() {
	return (linked_list*)malloc(sizeof(linked_list));
}

linked_list* find(const linked_list* head, int index) {
	for (linked_list* p = head->next; p != head; p = p->next) {
		if (p->index == index)
		{
			return p;
		}
	}
}

int set_node(const linked_list* head, int index, char* name) {
	linked_list* p1 = find(head, index);
	strncpy(p1->name, name, MAX_SHORT_NAME);
	return 1;
}

linked_list* get_last_node(linked_list* head) 
{
	linked_list* last_node = head;
	for (linked_list* p = head->next; p != head; p = p->next)
		last_node = p;
	return last_node;
}

int append_node(linked_list* head, int index, char* name) {
	linked_list* last_node = get_last_node(head);
	linked_list* p_new = create_node();
	last_node->next = p_new;
	p_new->next = head;
	p_new->index = index;
	strncpy(p_new->name, name, sizeof(name));
	return 1;
}

int print_node(const linked_list* node) {
	return X_LOG_DEBUG("[%x] -> [%x] index:%d name:%s", node, node->next, node->index, node->name);
}