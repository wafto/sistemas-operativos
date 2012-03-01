#ifndef LIST_H
#define LIST_H

typedef struct ListNode {
	struct ListNode* prev;
	void* data;
	struct ListNode* next;
} ListNode;

typedef struct {
	ListNode* first;
	ListNode* last;
	unsigned long size;
} List;

typedef ListNode* iterator;

ListNode* createnode(void*);
void initlist(List*);
void emptylist(List*);
unsigned long sizelist(List);
int isemptylist(List);

int pushfrontlist(List*, void*);
int pushbacklist(List*, void*);
int pushatlist(List*, unsigned long, void*);

void* popfrontlist(List*);
void* popbacklist(List*);
void* popatlist(List*, unsigned long);

void* frontlist(List);
void* backlist(List);
void* atlist(List, unsigned long);



#endif