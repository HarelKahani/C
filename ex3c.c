
/*written by - harel kahani
this program will creat a linked list from each char the user enter, than asked to
insert the amount of deliting jump that the user want to remove fron the list, the
program will print the final list without the chars after each requested jump*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>



typedef struct Node {
	char letter;
	struct Node* next;
} Node;

Node *head = NULL;

Node* CreateNewNode(char lett);
void InsertAtEnd(char lett);
Node* buildString();
Node* removeInJumps(Node * head, int k);
int getCount(struct Node* head);
void printList(Node * head);

int main() {
	int jumps;
	head= buildString();
	if (head == NULL) {															//at other cases free the list and return null
		Node *curr = head;
		while (curr != NULL) {
			head = head->next;
			free(curr);
			curr = head;
		}
		return NULL;
	}

	printf("Please the delation spaced (k)\n");
	int scanJump = scanf("%d", &jumps);
		if (scanJump != 1)									//taking care of a negetive jump or a non int input
		{
			printf("Please enter a whole positive number!");
			return 1;
		}
	head = removeInJumps(head, jumps);
	printList(head);
	return 0;
}
Node* buildString() {
	int i = 0;
	char c;
	printf("Please insert a list of chars: \n");
	while ((c=getchar()) != '\n') {

		if (head == NULL) {
			head = CreateNewNode(c);									//creating the first node 
		}
		else 
			InsertAtEnd(c);	
	}
	return head;
}
Node* removeInJumps(Node * head, int k) {
	int list_size = getCount(head);
	int counter = 1;									//=1 because at the first iteration if k=1 "if" statment must apply since the curr is at *head->next  
	if (k < 0) {
		printf("Please enter a whole positive number!");
		exit(-1);
	}
	else if (k==0) {
		return head;
	}
	else if (k == list_size) {
		return head;
	}
	/*the following using 2 pointers since its only one way list (without prev) so the head won't be lost*/
	else
	{
		Node *curr = head->next;
		Node *prev = head;
		while (curr != NULL)
		{
			if (counter == k)
			{
				Node* toFree = curr;
				curr = curr->next;
				free(toFree);
				prev->next = curr;
				counter = 0;
			}
			else {
				counter++;
				prev = curr;
				curr = curr->next;
			}
		} 
		
	}
	return head;

}

Node* CreateNewNode(char lett) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	assert(newNode != NULL);
	newNode->letter = lett;
	newNode->next = NULL;

	return newNode;
}

void InsertAtEnd(char lett) {
	Node *temp = head;
	struct Node* newNode = CreateNewNode(lett);
	if (head == NULL) {
		head = newNode;
		return;
	}
	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = newNode;
}
int getCount(struct Node* head)
{
	int count = 0;  // Initialize count 
	Node* current = head;  // Initialize current 
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return count;
}
void printList(Node * head) {
	while (head != NULL) {
		printf("%c->", head->letter);
		head = head->next;
	}
	printf("\n");
}