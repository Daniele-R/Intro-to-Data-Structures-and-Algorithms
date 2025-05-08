#include <stdlib.h>            // to be able to use malloc functions
#include <stdio.h>

#define SIZE 6

typedef struct _node
{
    char name;                 // not required for a linked list
    unsigned long long phone_nr;
    struct _node * next;       // points to the next node in the list
} node;

typedef struct _linked_list 
{
    int count;
    node * head;
    node * tail;
} list;

list * create_list() 
{
    list* result = (list *) malloc(sizeof(list));
    result->head = NULL;
    result->tail = NULL;
    result->count = 0;
    return result;
}

node * create_node(char name, unsigned long long phone) 
{
    node * result = (node *)malloc(sizeof(node));
    result->next = NULL;
    result->name = name;
    result->phone_nr = phone;
    return result;
}

int add_node_front(list* my_list_arg, node* my_new_node) 
{
    if(my_list_arg == NULL)
        return -1;
    ++my_list_arg->count;
    my_new_node->next = my_list_arg->head;
    my_list_arg->head = my_new_node;
    if(my_list_arg->tail == NULL)
        my_list_arg->tail = my_new_node;

    return 0;
}

int erase_front(list* my_list) 
{
    if(my_list == NULL)
        return -1;
    if(my_list->head == NULL)
        return 0;

    node * item = my_list->head;
    my_list->head = item->next;
    if(my_list->head == NULL)
        my_list->tail = NULL;

    --my_list->count;
    free(item);

    return 0;
}

node* get_element_from_list(list* my_list, unsigned long long phone_number)
{

    if (my_list == NULL)
        return NULL;
    node* temp = my_list->head;
    
    // node a = {'F', 778899, NULL};
    // a.phone_nr = 552244;
    // node *b = &a;
    // (*b).phone_nr = 112244;
    

    while (temp != NULL)
    {
        //if ((*temp).phone_nr == phone_number)
            
        if (temp->phone_nr == phone_number)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

int hash_function(unsigned long long val) 
{
    //return (val & 0xABCD) % SIZE;
    return val % SIZE;
}

int add_element_to_hash_table(list* my_table[], node* my_new_node) 
{
    if(my_new_node == NULL)
        return -1;
    
    int ix = hash_function(my_new_node->phone_nr);
    add_node_front(my_table[ix], my_new_node);

    return 0;
}

node* get_element_from_hash_table(list* my_table[], unsigned long long phone_number) 
{
    int ix = hash_function(phone_number);
    return get_element_from_list(my_table[ix], phone_number);
}

int main() {
    list* my_hash_table[SIZE];

    for(int i = 0; i < SIZE; ++i)
        my_hash_table[i] = create_list();

unsigned long long phones[] = {  
                            5048451427,
                            8103749840,
                            8562644130,
                            9079212010,
                            5135494561,
                            4198006759,
                            7739248565,
                            4088131105,
                            6057944895,
                            4108044694,
                            2154228694,
                            6316773675,
                            3102543084,
                            4405797763};

    for(int ix = 0; ix < (int)(sizeof(phones) / sizeof(phones[0])) ; ++ix)
        add_element_to_hash_table(my_hash_table, create_node('A' + ix, phones[ix]));
    
    unsigned long long phone = 7739248565;
    node* item = get_element_from_hash_table(my_hash_table, phone);
    if(item != NULL)
        printf("\n%c has the phone number %llu\n", item->name, item->phone_nr);
    else
        printf("\nphone number %llu is new\n", phone);


    return 0;
}
