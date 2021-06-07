#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


struct node
{
    void* data;
    struct node *next;
} typedef node;


struct llist {
    struct node* head;
} typedef llist;


//display the list
void print_list(llist *list)
{
    struct node *ptr = list->head;
    printf("[ ");

    //start from the beginning
    while (ptr != NULL)
    {
        char* data = ptr->data;
        printf("%s ", data);
        ptr = ptr->next;
    }

    printf("]\n");
}

llist* create_list() {
    //create a link
    llist *list = (struct llist *)malloc(sizeof(struct llist));

    list->head = NULL;

    return list;
}

//insert link at the first location
void insert_first(llist *list, void* data)
{
    //create a link
    struct node *link = (struct node *)malloc(sizeof(struct node));

    link->data = data;

    node* head = list->head;

    //point it to old first node
    link->next = head;

    //point first to new first node
    list->head = link;
}

//delete first item
struct node *delete_first(llist *list)
{
    node* head = list->head;

    //save reference to first link
    struct node *tempLink = head;

    //mark next to first link as first
    list->head = head->next;

    //return the deleted link
    return tempLink;
}

//is list empty
bool is_empty(llist *list)
{
    return list->head == NULL;
}

int length(llist *list)
{
    int length = 0;
    struct node *current;
    node* head = list->head;

    for (current = head; current != NULL; current = current->next)
    {
        length++;
    }

    return length;
}

//find a link with given data
node *find(llist *list, void* data)
{

    //start from the first link
    struct node *current = list->head;

    //if list is empty
    if (list->head == NULL)
    {
        return NULL;
    }

    //navigate through list
    while (current->data != data)
    {

        //if it is last node
        if (current->next == NULL)
        {
            return NULL;
        }
        else
        {
            //go to next link
            current = current->next;
        }
    }

    //if data found, return the current Link
    return current;
}

//delete a link with given data
node *delete (llist *list, void* data)
{

    //start from the first link
    struct node *current = list->head;
    struct node *previous = NULL;

    //if list is empty
    if (list->head == NULL)
    {
        return NULL;
    }

    //navigate through list
    while (current->data != data)
    {

        //if it is last node
        if (current->next == NULL)
        {
            return NULL;
        }
        else
        {
            //store reference to current link
            previous = current;
            //move to next link
            current = current->next;
        }
    }

    //found a match, update the link
    if (current == list->head)
    {
        //change first to point to next link
        list->head = list->head->next;
    }
    else
    {
        //bypass the current link
        previous->next = current->next;
    }

    return current;
}
