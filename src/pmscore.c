#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/linked_list.h"
#include "../include/xsql.h"
#include "../include/pmscore.h"

/*--------------------------------------------------
	Application methods.
--------------------------------------------------*/

/**
 * @brief Print welcome message.
 * 
 * @return int 
 */
int welcomeMessage() {

	linked_list* p_head = create_node();
	p_head->next = p_head;
	linked_list* p_cur = p_head;

	for (int i = 0; i < 10; i++)
	{
		char* p = (char*)malloc(sizeof(char)*MAX_SHORT_NAME);
		sprintf(p, "Test node %d", i);
		append_node(p_head, i, p);
	}

	X_LOG_DEBUG("Before created.");
	for (linked_list* p = p_head->next; p != p_head; p = p->next) {
		print_node(p);
	}

	X_LOG_DEBUG("After set the node.");
	set_node(p_head, 5, "Hahahaha........................");
	set_node(p_head, 9, "Hohohoho........................");
	for (linked_list* p = p_head->next; p != p_head; p = p->next) {
		print_node(p);
	}

	X_LOG_DEBUG("The last node is:");
	linked_list* last_node = get_last_node(p_head);
	print_node(last_node);
}

double calcCost(double payment) {
	payment *= 1.4;
	return payment;
}

int add(int a, int b) {
	return a+b;
}

int GenerateMailSecurityInfo(char* pSecurityInfo) {
	assert(pSecurityInfo != NULL);
	memset(pSecurityInfo, 0, strlen(pSecurityInfo)+1);
	const char originalPassword[] = "Dj1290!@()";
	strcpy(pSecurityInfo, originalPassword);
  	return strlen(pSecurityInfo);
}

/**
    Filter c in src string, make a new string to 
    hold the filtered.
    @param src the original string terminated with \0
    @param c character to be deleted.
    @author Martin Dong <martin.dong@139.com>
*/
char* strtrim(char *src, char c) 
{
    char* output = (char*)malloc( strlen(src)*sizeof(char)+1 );
    memset(output, 0, strlen(src));

    char* in = src;
    char* ot = output;
    
    while(*in) 
    {
        if (*in == c)
        {
            *in++;
            continue;
            /* code */
        }
        else
        {
            *ot = *in;
            *ot++;
            *in++;
        }
    }
    return output;
}

