#include <stdlib.h>
#include <stdio.h>
#include "list.h"

ListNode* createnode(void* data) {
	ListNode* node = (ListNode*) malloc(sizeof(ListNode));
	if (node != NULL) {
		node->prev = node->next = NULL;
		node->data = data;
	}
	return node;
}

void initlist(List* list) {
	list->first = list->last = NULL;
	list->size = 0;
}

void emptylist(List* list) {
	while (!isemptylist(*list)) free(popfrontlist(list));
}

unsigned long sizelist(List list) {
	return list.size;
}

int isemptylist(List list) {
	return list.size == 0 ? 1 : 0;
}

int pushfrontlist(List* list, void* data) {
	ListNode* node = createnode(data);
	if (node != NULL) {
		if (isemptylist(*list)) {
			list->first = list->last = node;
		} else {
			node->next = list->first;
			list->first->prev = node;
			list->first = node; 
		}
		list->size += 1;
		return 1;
	}
	return 0;
}

int pushbacklist(List* list, void* data) {
	ListNode* node = createnode(data);
	if (node != NULL) {
		if (isemptylist(*list)) {
			list->first = list->last = node;
		} else {
			node->prev = list->last;
			list->last->next = node;
			list->last = node;
		}
		list->size += 1;
		return 1;
	}
	return 0;
}

int pushatlist(List* list, unsigned long index, void* data) {
	unsigned long i = 0;
	ListNode *aux, *node;
	if (index == 0)
		return pushfrontlist(list, data);
	if (index == sizelist(*list)) 
		return pushbacklist(list, data);
	if (index > 0 && index < sizelist(*list)) {
		node = createnode(data);
		if (node != NULL) {
			for (aux = list->first; aux != NULL; aux = aux->next) {
				if (index == i) {
					node->next = aux;
					node->prev = aux->prev;
					aux->prev->next = node;
					aux->prev = node;
					list->size += 1;
					return 1;
				}
				i += 1;
			}
		}
	}
	return 0;
}

void* popfrontlist(List* list) {
	ListNode* node;
	void* data = NULL;
	if (!isemptylist(*list)) {
		node = list->first;
		if (sizelist(*list) == 1) {
			list->first = list->last = NULL;
		} else {
			list->first = list->first->next;
			list->first->prev = NULL;
		}
		data = node->data;
		free(node);
		list->size -= 1;
	}
	return data;
}

void* popbacklist(List* list) {
	ListNode* node;
	void* data = NULL;
	if (!isemptylist(*list)) {
		node = list->last;
		if (sizelist(*list) == 1) {
			list->last = list->first = NULL;
		} else {
			list->last = list->last->prev;
			list->last->next = NULL;
		}
		data = node->data;
		free(node);
		list->size -= 1;
	}
	return data;
}

void* popatlist(List* list, unsigned long index) {
	ListNode* node;
	unsigned long i = 0;
	void* data = NULL;
	if (!isemptylist(*list) && index >= 0 && index < sizelist(*list)) {
		if (index == 0)
			return popfrontlist(list);
		if (index == sizelist(*list) - 1)
			return popbacklist(list);
		for (node = list->first; node != NULL; node = node->next) {
			if (index == i) {
				node->next->prev = node->prev;
				node->prev->next = node->next;
				break;
			}
			i += 1;
		}
		data = node->data;
		free(node);
		list->size -= 1;
	}
	return data;
}

void* frontlist(List list) {
	return !isemptylist(list) ? list.first->data : NULL;
}

void* backlist(List list) {
	return !isemptylist(list) ? list.last->data : NULL;
}

void* atlist(List list, unsigned long index) {
	ListNode* node;
	unsigned long i = 0;
	void* data = NULL;
	if (!isemptylist(list) && index >= 0 && index < sizelist(list)) {
		if (index == 0)
			return frontlist(list);
		if (index == sizelist(list) - 1)
			return backlist(list);
		for (node = list.first; node != NULL; node = node->next) {
			if (index == i)
				break;
			i += 1;
		}
		data = node->data;
	}
	return data;
}
