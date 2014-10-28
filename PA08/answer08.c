#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"answer08.h"

List * List_createNode(const char * str)
{
	List * node = malloc(sizeof(List));
	node->str = strdup(str);
	node->next = NULL;
	return node;
}

void List_destroy(List * list)
{
	if(list != NULL) {
		List_destroy(list->next);
		free(list->str);
		free(list);
	}
}

int List_length(List * list)
{
	int count = 0;
	while(list != NULL) {
		count ++;
		list = list->next;
	}
	return count;
}

List * List_merge(List * lhs, 
                  List * rhs, 
		  		  int (*compar)(const char *, const char*))
{
	List * head;
	if(lhs == NULL)      return rhs;
	else if(rhs == NULL) return lhs;
	
	if(compar(lhs->str,rhs->str) < 0) {
		head = lhs;
		head->next = List_merge(lhs->next,rhs,compar);
	} else {
		head = rhs;
		head->next = List_merge(rhs->next,lhs,compar);
	}
	return head;
}

List * List_sort(List * list, int (*compar)(const char *, const char*))
{
	int i, n;
	n = List_length(list);
	List * lhs = list, * rhs = NULL;
	if(n == 0 || n == 1) return list;
	else {
		for(i = 0; i < n/2 - 1; i++) list = list->next;
		rhs = list->next;
		list->next = NULL;
	}
	lhs = List_sort(lhs,compar);
	rhs = List_sort(rhs,compar);
	list = List_merge(lhs,rhs,compar);
	return list;
}
