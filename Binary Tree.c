#include <stdlib.h>
#include <stdio.h>

typedef struct _node
{   
    /* add all needed data*/
    int data;
    /**/
    struct _node * left;
    struct _node * right;
} node;

typedef struct _tree 
{    
    int count;
    node * root;
} tree;

tree* create_tree()
{
    tree* result = (tree*) malloc(sizeof(tree));
    result->count = 0;
    result->root = NULL;
    return result;
}

node* create_node(int value) 
{
    node* result = (node*) malloc(sizeof(node));
    result->data = value;
    result->left = NULL;
    result->right = NULL;
    return result;
}

int add_node(tree* my_tree, node* value)
{
    if(my_tree == NULL)
        return -1;
    
    if(my_tree->root == NULL) 
    {
        my_tree->root = value;
        my_tree->count = 1;
        return 0;
    }

    node* item = my_tree->root;
    ++(my_tree->count);

    while (1)
    {
        if(item->data > value->data) 
        {
            if(item->right == NULL) 
            {
                item->right = value;
                return 0;
            }
            item = item->right;
        } 
        else 
        {
            if(item->left == NULL) 
            {
                item->left = value;
                return 0;
            }
            item = item->left;
        }
    }
}

int print_in_order(node* root) 
{
    if(root == NULL) 
        return -1;
    
    print_in_order(root->right);
    printf(" %d ", root->data);
    print_in_order (root->left);
    
    return 0;
}

int print_in_reverse_order(node* root) 
{
    if(root == NULL) 
        return -1;

    print_in_reverse_order (root->left);
    printf(" %d ", root->data);
    print_in_reverse_order(root->right);
    
    return 0;
}

node* get_node(node* root, int value) 
{
    if(root == NULL)
    {
        return NULL;
    }

    if(root->data == value)
    {
        return root;
    }

    if(value > root->data)
    {
        return get_node(root->left,value);
    }

    return get_node(root->right,value);
}

//return the node with the max value
node* get_max_node(node* root) 
{

    if(root == NULL)
    {
        return NULL;
    }

    while(root->left != NULL)
    {
        root = root->left;
    }

    return root;
}

node* getMaxNode(node* root)
{
    if(root ==NULL)
    {
        return NULL;
    }
    if(root->left==NULL)
    {
        return root;
    }

    return getMaxNode(root->left);
}

node* delete_node (node* root, int data){
    
    if(root == NULL)
    {
        return NULL;
    }

    node* item = root;
    node** pre = &root;

    while(item->data != data) 
    {
        if(item->data > data) 
        {
            pre = &item->right;
            item = item->right;
        }
        else 
        {
            pre = &item->left;
            item = item->left;
        }

        if(item == NULL)
            return root;
    }

    if((item->left == NULL) && (item->right == NULL)) 
    {
        *pre = NULL;
        free(item);
        return root;
    }
    if((item->left != NULL) && (item->right == NULL)) 
    {
        *pre = item->left;
        free(item);
        return root;
    }
    if((item->left == NULL) && (item->right != NULL)) 
    {
        *pre = item->right;
        free(item);
        return root;
    }

    node* max_node = item->right;
    node** pre_max = &item->right;
    while (max_node->left != NULL)
    {
        pre_max = &max_node->left;
        max_node = max_node->left;
    }
    
    *pre_max = max_node->right;
    *pre = max_node;
    max_node->left = item->left;
    max_node->right = item->right;

    free(item);

    return root;
}


int main(int argc, char** argv) 
{
    if(argc == 1)
        printf("\n%s\n", argv[0]);

    tree* my_tree = create_tree();
    add_node(my_tree, create_node(10));
    add_node(my_tree, create_node(8));
    add_node(my_tree, create_node(20));
    add_node(my_tree, create_node(5));
    add_node(my_tree, create_node(16));
    add_node(my_tree, create_node(9));
    add_node(my_tree, create_node(35));
    add_node(my_tree, create_node(12));
    add_node(my_tree, create_node(19));
    add_node(my_tree, create_node(45));
    add_node(my_tree, create_node(55));
    add_node(my_tree, create_node(11));
    add_node(my_tree, create_node(18));

    my_tree->root = delete_node(my_tree->root, 20);

    // print_in_order(my_tree->root);
    print_in_reverse_order(my_tree->root);
    return 0;
}
