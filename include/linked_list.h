#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__


#include <stdlib.h>
/*--------------------------------------------------
	linked list methods.
--------------------------------------------------*/
#ifndef MAX_SHORT_NAME
#define MAX_SHORT_NAME 256
#endif

typedef struct linked_list_s {
	struct linked_list_s *next;
	int index;
	char name[MAX_SHORT_NAME];
} linked_list;

linked_list* create_node();

int set_node(const linked_list* head, int index, char* name);

linked_list* find(const linked_list* head, int index);

linked_list* get_last_node(linked_list* head);

int append_node(linked_list* head, int index, char* name);

int print_node(const linked_list* node);

#endif // _LINKED_LIST_H_