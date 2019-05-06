/*written by - harel kahani
this program will give you an ordring list from highest value of item to lowest using greedy algorithem list. enter items by their
values and weights (the program connect each item as a node in linked list). The value's needs to be entered as 'a'-'j'
chars than it will convert into the right number when 'a'=0 ---> 'j'=9.
When finishing insert items press -1 (when insert weight appers),than enter the knapsack size
to get the list of item sorted from the biggest value to the lowest with a fraction of how much from the lowest item entered to the sack*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define	SIZE 256

typedef struct {
	char *value;
	int weight;
}Item1;

struct Node {
	Item1 data;
	struct Node* next;
	struct Node* prev;
};

typedef struct Node linked_list;
char* convert(char* str, int size);
linked_list* CreateNewNode(Item1 item);
void InsertAtEnd(linked_list** head_ptr, Item1 item);
linked_list* SortedMerge(linked_list* a, linked_list* b);
void FrontBackSplit(linked_list* source, linked_list** frontRef, linked_list** backRef);
void MergeSort(linked_list** headRef);
void Greedy(linked_list** head_ptr,int size);
void free_list(linked_list** head);

int main()
{
	int weight = 0, value = 0, knap_sack=0, i;
	char val[SIZE], *ptr, *ptr2;
	Item1 temp_item;
	linked_list *head = NULL;
									
	while (1) {
		printf("Please insert weight: ");
		int scanWeight = scanf("%d", &weight);
		if (weight == -1)
			break;

		else if (weight < 0 || scanWeight != 1)									//taking care of a negetive weight or a non int input
		{
			printf("Please enter a whole positive number!");
			return 1;
		}

		temp_item.weight = weight;

		printf("Please insert Value as chars: ");
		int scanVal = scanf("%s", val);
		if (scanVal != 1)														//taking care of a non char input
		{
			printf("Please enter chars sequences using only 'a' to 'j' letters!");
			return 1;
		}
		for (i = 0; val[i] != '\0'; i++)										//as the teacher - the input needs to be between 'a'->'j'
		{
			if (val[i]<'a' || val[i]>'j')
			{
				printf("Please enter chars sequences using only 'a' to 'j' letters!");
				return 1;
			}
		}
	
		ptr = (char*)malloc(sizeof(char)*(strlen(val) + 1));
		assert(ptr != NULL);
		strncpy(ptr, val, (strlen(val) + 1));
		ptr2 = convert(ptr, strlen(ptr));
													
		printf("value as num is: %s\n", ptr2);
		temp_item.value = ptr2;
		

		if (head == NULL){
			head = CreateNewNode(temp_item);									//creating the first node 
		}
		else if (head != NULL)  {
			InsertAtEnd(&head, temp_item);									// since header is not global, sending to func with &head	
		}
		scanVal = 0;
		scanWeight = 0;
		printf("\n");
	}
	printf("Please entere the weight of knapsack: ");
	int scanSack = scanf("%d", &knap_sack);
	if (knap_sack < 0 || scanSack != 1)									//taking care of a negetive weight or a non int input
	{
		printf("Please enter a whole positive number!");
		return 1;
	}

	Greedy(&head, knap_sack);
	free_list(&head);

	return 0;
}

/*the following converte a char string into (char)numbers string by ascii value*/

char* convert(char* str, int size)
{
	int i, counter = 0;											//counter will indicate me for the right location to place in the final char string (after converting)
	char* new_str = (char*)malloc(sizeof(char)*(size + 1));
	assert(new_str != NULL);
	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] != 'a' || counter > 0) {
			new_str[counter] = (char)((int)(str[i]) - 49);		 //reaching to the right ascci valu
			counter++;
		}
	}
	if (counter == 0) {											//taking care if all the chars are (or the only char is) 'a'
		new_str[counter] = '0';
		counter++;
	}

	new_str[counter] = '\0';									//"closing" the new char string at the right location
	return new_str;
}

linked_list* CreateNewNode(Item1 item) {
	linked_list* newNode = (linked_list*)malloc(sizeof(linked_list));
	assert(newNode != NULL);
	newNode->data = item;
    newNode->prev = NULL;
	newNode->next = NULL;

	return newNode;
}
void InsertAtEnd(linked_list** head_ptr,Item1 item) {
	linked_list* temp = *head_ptr;
	linked_list* newNode = CreateNewNode(item);
	if (*head_ptr == NULL) {
		*head_ptr = newNode;
		return;
	}

	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = newNode;
	newNode->prev = temp;
	}
/* sorts the linked list by changing next pointers (not data) */
void MergeSort(linked_list** headRef) {
	linked_list* head = *headRef;
	linked_list* a;
	linked_list* b;
/* Base case -- length 0 or 1 */
	if ((head == NULL) || (head->next == NULL))
	{
		return;
	}
	/* Split head into 'a' and 'b' sublists */
	FrontBackSplit(head, &a, &b);

	/* Recursively sort the sublists */
	MergeSort(&a);
	MergeSort(&b);

	/* answer = merge the two sorted lists together */
	*headRef = SortedMerge(a, b);
}
linked_list* SortedMerge(linked_list* a, linked_list* b) {
	linked_list* result = NULL;

	/* Base cases */
	if (a == NULL)
		return(b);
	else if (b == NULL)
		return(a);

	/* Pick either a or b, and recur */
	if (((double)atoi(a->data.value) / (double)a->data.weight) >= (double)atoi(b->data.value) / (double)b->data.weight)
	{
		result = a;
		result->next = SortedMerge(a->next, b);
	}
	else
	{
		result = b;
		result->next = SortedMerge(a, b->next);
	}
	return(result);
}
/* UTILITY FUNCTIONS */
/* Split the nodes of the given list into front and back halves,
	and return the two lists using the reference parameters.
	If the length is odd, the extra node should go in the front list.
	Uses the fast/slow pointer strategy. */

void FrontBackSplit(linked_list* source, linked_list** frontRef, linked_list** backRef) {
	linked_list* fast;
	linked_list* slow;
	slow = source;
	fast = source->next;
	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	/* 'slow' is before the midpoint in the list, so split it in two
	at that point. */
	*frontRef = source;
	*backRef = slow->next;
	slow->next = NULL;
}
void Greedy(linked_list** head_ptr, int size) {
	linked_list* temp = *head_ptr;
	char str[SIZE];
	int cur_weight = 0;
	double maximum_value = 0;

	MergeSort(&temp);
	while (temp!=NULL)
	{
		if (cur_weight+temp->data.weight <= size) {
			cur_weight += temp->data.weight;					//to sum the weight to know when to skip if statment ant culc the fraction 
			printf("Added to the knapsack: %s, with fraction of: 1, Remain space is: %d \n", temp->data.value, (size - cur_weight));
			strncpy(str, temp->data.value, (strlen(temp->data.value) + 1));
			maximum_value += atoi(str); //since max val is 256->O(256)=O(1), also (the same resaon) atoi()->O(1)- final of O(1)
			temp = temp->next;
		}
		else if (cur_weight + temp->data.weight > size){
			double remain = size - cur_weight;
			printf("Added to the knapsack: %s, with fraction of: %f, Remain space is: 0 \n", temp->data.value, (remain / (double)temp->data.weight));
			strncpy(str, temp->data.value, (strlen(temp->data.value) + 1));
			maximum_value += ((double)atoi(str)*(remain / (double)temp->data.weight));// divide the remain weight with the currnt item weight
			break;
		}
		else {
			break;
		}
		
	}
	printf("Maximum value Greedy solution: %g", maximum_value);
}

void free_list(linked_list** head) {
	linked_list* temp = *head;
	
	if (*head == NULL) {
		return;
	}
	while (*head != NULL) {
		*head = (*head)->next;
		free(temp->data.value);
		free(temp);
		temp = *head;
	}
}


