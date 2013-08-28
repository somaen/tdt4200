#include <stdlib.h>
#include <stdio.h>


typedef struct linked_list{
    struct linked_list* next;
    struct linked_list* prev;
    int value;
} linked_list;


void print_array_stats(double* array, int size){
	double sum = 0;
	double max = array[0];
	int i = 0;
	if (size == 0) {
		printf("Array is 0-sized\n");
	}
	for (i = 0; i < size; i++) {
		sum += array[i];
		max = (array[i] > max ? array[i] : max);
	}
	printf("Sum: %f\n", sum);
	printf("Average: %f\n", sum/(double)size);
	printf("Max: %f\n", max);
}


linked_list* new_linked_list(int size, int value){
	linked_list *start = (linked_list*)malloc(sizeof(linked_list));
	start->value = value;
	start->prev = NULL;
	start->next = NULL;
	linked_list *curr = start;
	int i = 1;
	for (i = 1; i < size; i++) {
		linked_list *temp = (linked_list*)malloc(sizeof(linked_list));
		temp->value = value;
		curr->next = temp;
		temp->prev = curr;
		curr = temp;
	}
	curr->next = NULL;
	return start;
}


void print_linked_list(linked_list* ll, int horizontal, int direction){
	linked_list *curr = ll;
	if (!direction) {
		while (curr->next) {
			curr = curr->next;
		}
	}
	while (curr != NULL) {
		printf("%d", curr->value);
		if (!horizontal) {
			printf("\n");
		} else {
			printf(" ");
		}
		if (direction) {
			curr = curr->next;
		} else {
			curr = curr->prev;
		}
	}
	if (horizontal) {
		printf("\n");
	}
}


int sum_linked_list(linked_list* ll){
	int sum = 0;
	while (ll) {
		sum += ll->value;
		ll = ll->next;
	}
	return sum;
}


void insert_linked_list(linked_list* ll, int pos, int value){
	int i = 0;
	for (i = 0; i < pos - 1; i++) { // We need to find the element BEFORE
		ll = ll->next;
	}
	linked_list *new_node = (linked_list*)malloc(sizeof(linked_list));
	new_node->prev = ll;
	new_node->next = ll->next;
	ll->next->prev = new_node;
	ll->next = new_node;

	new_node->value = value;
}

void merge_linked_list(linked_list* a, linked_list* b){
	int index = 1;
	linked_list *tempA;
	linked_list *tempB;
	while (b) {
		tempA = a->next;
		tempB = b->next;
		a->next = b;
		b->prev = a;
		b->next = tempA;
		a = tempA;
		b = tempB;
	}
}

void destroy_linked_list(linked_list* ll){
	if (!ll) {
		return;
	}
	linked_list *next = ll->next;
	while (ll) {
		free(ll);
		ll = next;
		if (ll) {
			next = ll->next;
		}
	}
}

int main(int argc, char** argv){

    //Array statistics
    double array[5] = {2.0, 3.89, -3.94, 10.1, 0.88};
    print_array_stats(array, 5);

    //Creating liked list with 3 3s and 4 4s
    linked_list* ll3 = new_linked_list(3,3);
    linked_list* ll4 = new_linked_list(4,4);

    //Should print: "3 3 3"
    print_linked_list(ll3, 1, 1);

    //Inserting a 5 at the 1st position
    insert_linked_list(ll3, 1, 5);

    //Should print "3 5 3 3"
    print_linked_list(ll3, 1, 1);

    //Printing backwards, should print: "3 3 5 3"
    print_linked_list(ll3, 1, 0);

    //Merging the linked lists
    merge_linked_list(ll3, ll4);

    //Printing the result, should print: "3 4 5 4 3 4 3 4"
    print_linked_list(ll3, 1,1);

    //Summing the elements, should be 30
    printf("Sum: %d\n", sum_linked_list(ll3));

    //Freeing the memory of ll3
    destroy_linked_list(ll3);
}
