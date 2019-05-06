/*written by - harel kahani
this program will give you the highest value from a items list. enter items by their
values and weights (the program connect each item as a node in linked list). The value's needs to be entered as 'a'-'j'
chars than it will convert into the right number when 'a'=0 ---> 'j'=9.
When finishing insert items press -1 (when insert weight appers),than enter the knapsack size
to get the matrix printing and the max value entered to the sack from your list*/
//atoi() function in converts string data type to int data type.
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
};

typedef struct Node linked_list;
char* convert(char* str, int size);
linked_list* CreateNewNode(Item1 item);
void InsertAtEnd(linked_list** head_ptr, Item1 item);
void dynamic(linked_list** head_ptr, int sack_size, int num_of_items);
void free_list(linked_list** head);

int main()
{
	int weight = 0, value = 0, knap_sack = 0, count_items=0, i;
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


		if (head == NULL) {
			head = CreateNewNode(temp_item);									//creating the first node 
		}
		else if (head != NULL) {
			InsertAtEnd(&head, temp_item);									// since header is not global, sending to func with &head	
		}
		count_items++;
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

	dynamic(&head, knap_sack,count_items);
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
	newNode->next = NULL;

	return newNode;
}
void InsertAtEnd(linked_list** head_ptr, Item1 item) {
	struct Node* temp = *head_ptr;
	struct Node* newNode = CreateNewNode(item);
	if (*head_ptr == NULL) {
		*head_ptr = newNode;
		return;
	}

	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = newNode;
	
}
/*the following giving the max value using dynamic code by keeping the correct max in a specific matrix cell
based on the next equasion reqartion: max{values[i-1,j],values[i-1,j-i.weight]*/
void dynamic(linked_list** head_ptr, int sack_size, int num_of_items) {
	linked_list* temp = *head_ptr;
	/*the following creates 2d array*/
	int i, j;
	char str[SIZE];
	int **values = (int**)malloc((num_of_items + 1) * sizeof(int*));		//creating a pointers array for 2d array with num_of_items+1 raws
	assert(values != NULL);
	for (i = 0; i < num_of_items+1 ; i++)
	{
		values[i] = (int*)malloc((sack_size + 1) * sizeof(int));					//creating array with sack_size+1 cols
		assert(values[i] != NULL);
	}

	/*the following fill the matrix with the correct max value depending on */

	for (i = 0; i < num_of_items + 1; i++)
	{
		values[i][0] = 0;
	}
	for (j = 0; j < sack_size + 1; j++)
	{
		values[0][j] = 0;
	}
	for (int i = 1; i < num_of_items + 1; i++)
	{
		strncpy(str, temp->data.value, (strlen(temp->data.value) + 1));
		
		int char_val_int = atoi(str);									//converting to int from string --> 0(1)
		
		for (int j = 1; j < sack_size + 1; j++)
		{
			if ((j - temp->data.weight) < 0)					    	 //taking care if j<0 -> out of bounds
			{
				values[i][j] = values[i - 1][j];						//copying the max from values[i-1] to values[i][j]
			}
			else {
				if (values[i - 1][j] < values[i - 1][j - (temp->data.weight)] + char_val_int) {		//taking the max value between those cells based on the requarsion equasion
					values[i][j] = values[i - 1][j - temp->data.weight] + char_val_int;
				}
				else {
					values[i][j] = values[i - 1][j];

				}
			}
		}
		if (temp->next != NULL)
		{
			temp = temp->next;		//taking care to get the next i.weight 

		}
	}
	
	for (i = 0; i <= num_of_items; i++)
	{
		printf("Row %d: ",i);
		for (j = 0; j < sack_size ; j++)
		{
			printf("%d,", values[i][j]);
		}
		printf("%d", values[i][sack_size]);
		printf("\n");
	}
	printf("\n\n");
	printf("Max value in Knapsack (solved with DP): %d", values[num_of_items][sack_size]);

	for (i = 0; i < num_of_items; i++) {
		free(values[i]);
	}
	free(values);
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


