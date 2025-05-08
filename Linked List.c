#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

// Here we limiting the table to 100 pointers (#3)
#define SIZE 100

// user-defined data to limit the MOB n DOB
typedef enum 
{
    Default, January, February, March, April, May, June, July, August, September, October, November, December
} Month;

typedef enum 
{
    _default, _1st, _2nd, _3rd, _4th, _5th, _6th, _7th, _8th, _9th, _10th,
    _11th, _12th, _13th, _14th, _15th, _16th, _17th, _18th, _19th, _20th,
    _21st, _22nd, _23rd, _24th, _25th, _26th, _27th, _28th, _29th, _30th, _31st
} Date;

// Creation of the structure NODE, which we will be calling "student" (#1)
typedef struct _node
{
    int id;
    char name[30];
    int grade;
    Date dob;
    Month mob;
    int yob;
    struct _node* next;

} student;      // for this exercise, student will be a synonym of "node"

// Creation of the linked list, called "list" (#2)
typedef struct _linked_list
{
    int count;
    student *head;
} list;

// (#4) Create list function where we initialize the size of our list (malloc), head to NULL, and count to 0
list* create_list()
{
    list* result  = (list*)malloc(sizeof(list));
    result->head  = NULL;
    result->count = 0;

    return result;          // returns the pointer
}

// (#5) Create a function that returns a student info
student * create_student(int id, char* name, int grade, Date dob, Month mob, int yob)
{
    student* result = (student*)malloc(sizeof(student));
    result->id = id;
    strncpy(result->name, name, sizeof(result->name) - 1); // We copy the name safely by turning into a string
    result->name[sizeof(result->name) - 1] = '\0';         // We make sure to delete the null value of the prev array
    result->grade = grade;
    result->dob = dob;
    result->mob = mob;
    result->yob = yob;
    result->next = NULL;

    return result;

    /* 
    If you dont want to use string copy as you mentioend in class, we can do a for loop

    for (int i = 0; i < sizeof(result->name) - 1; ++i)      print the array 1 by 1
    {
        if (name[i] != '\0') {      
            result->name[i] = name[i];
        } 
        else {
            result->name[i] = '\0';                         breaks if found null space    
            break;
        }
    }
    */
}

// (#6) creation of hash function with ID as key
int hash_function(int id)
{
    return id % SIZE;   // For hash tables we distribute the key value (in this case id)
                        // among the size of the table, which in this case is 100
}

// (#7)
int add_student_tohash(list* hash_table[], student* new_student)
{
    // First we make sure the node we want to implement isnt empty
    if (new_student == NULL){
        return -1;
    }
    // Now we get the index of our hash table (id%SIZE)
    int index = hash_function(new_student->id);

    // Case to create list inside the hash table if necessary, return -1 if index doesnt exist
    if (hash_table[index] == NULL) 
    {
        hash_table[index] = create_list();

        if (hash_table[index] == NULL) {
            return -1;
        }
    }

    new_student->next = hash_table[index]->head;    // We move the head to next in that specific index
    hash_table[index]->head = new_student;          // We move the new node into the head of the index
    hash_table[index]->count++;                     // We increase the count of that linked list in the index
    
    return 0;
}

/* (#8) To create a function that prints all the student details, we can have 2 approaches,
1. We create a function that print the details of 1 student and we create another function
that prints the info of all students by reading 1 by 1 with a loop of our preference
2. We create a single function that prints all students that goes thru the hash table 
(if it exists), access to its index and prints the linked list in that index, and
continues reading the hash table indexes until it read entirely.
*/

int print_each_student(list* hash_table[])
{
    for (int i = 0; i < SIZE ; ++i)
    {
        if (hash_table[i] != NULL && hash_table[i]-> head != NULL)
        {
            printf ("\nStudents at the index %d are:\n", i);
            student* current = hash_table[i]->head;
            
            while (current != NULL)
            {
                printf("ID: %d, Name: %s, Grade: %d, DOB: %d/%d/%d\n",
                current->id, current->name, current->grade, 
                current->dob, current->mob, current->yob);

                current = current->next; // Move to the next student/node
            }
        }
    }
    return 0;
}

/* 
(#9) In order to delete a specific student by his ID we need to look thru the hash table
and then thru the linked list within that hash, and then correct the list.
*/
int erase_student_fromhash(list* hash_table[], int student_id)
{
    int index = hash_function(student_id);  // First we need to find the index in the table

    // Check for a list within the given index
    if (hash_table[index] == NULL){
        return -1;
    }

    // We create 2 pointers to travel around the list
    student* current  = hash_table[index]->head;
    student* previous = NULL;

    while (current != NULL && current->id != student_id)
    {
        previous = current;         // our previous head (current) becomes previous
        current = current->next;    // our head is now the next term in the list
    }

    // Case if not found
    if (current == NULL){
        return -1;
    }

    // If the student is the first in the list case
    if (previous == NULL){
        hash_table[index]->head = current->next;    //our next will become the head in the index
    }
    else{
        previous->next = current->next;             
    }

    hash_table[index]->count--;     // We reduce the count of the list within that index of the hash

    free(current);                  // We free the memory of the ID we found

    return 0;
}

// (#10) Very similar to 9. In here we return a pointer and instead of free, we point to NULL
student* return_and_remove_student(list* hash_table[], int student_id)
{
    int index = hash_function(student_id);  // First we must find what is the index of the table

    // Check if theres a list at INDEX
    if (hash_table[index] == NULL){
        return NULL;
    }

    student* current  = hash_table[index]->head;
    student* previous = NULL;

    while (current != NULL && current->id != student_id)
    {
        previous = current;
        current  = current->next;
    }

    if (current == NULL){
        return NULL;
    }

    if (previous == NULL){
        hash_table[index]->head = current->next;
    } 
    else{
        previous->next = current->next;    
    }

    hash_table[index]->count--;

    current->next = NULL;   // here is the main difference, where the pointer we previously free,
                            // in here we can simply specify is now pointing no nothing, AKA NULL
    return current;         // returns the student/node we removed
}

// (#11)
int validate_sizes(list* hash_table[])
{
    if(hash_table == NULL){
        return -1;
    }

    for (int i = 0; i < SIZE; ++i)
    {
        if (hash_table[i] != NULL)
        {
            int count = 0;
            student* current = hash_table[i]->head;

            while (current != NULL)     // this while loop will count up for each node in the list
            {
                count++;
                current = current->next;
            }
            if (count != hash_table[i]->count)
            {
                return -1;  // if the count we did doesnt match with the count of the table, it gives us false
            }
        }
    }
    return 0;   // if completed, it gives us true
}

// (#12) 
double find_fill_factor (list* hash_table[])
{
    int filled_count = 0;   // Initialize the count

    for (int i = 0; i < SIZE; ++i){
        if (hash_table[i] != NULL && hash_table[i]->head != NULL) {
            filled_count++;
        }
    }
    double fill_factor = (double)filled_count/SIZE; //we turn it to double so we can get decimals

    return fill_factor;
}

// (#13)
int find_max_equal_keys(list* hash_table[])
{
    int max_elements = 0;
    for (int i = 0; i < SIZE; ++i)
    {
        if (hash_table[i] != NULL)
        {
            int count = 0;
            student* current = hash_table[i]->head;

            // While loop to count elements in the current list
            while (current != NULL)
            {
                count++;
                current = current->next;
            }

            // Checks if the count is greater than the max
            if (count > max_elements){
                max_elements = count;
            }
        }
    }
    return max_elements;
}

// (#14) Same process to search as #13 but with a twist if inside the while loop
int students_with_4plus (list* hash_table[])
{
    int count = 0;
    for (int i = 0; i < SIZE; ++i)
    {
        if (hash_table[i] != NULL)
        {
            student* current = hash_table[i]->head;

            while (current != NULL)
            {
                if (current->grade >= 4){
                    count++;
                }
                current = current->next;
            }
        }
    }
    return count;
}

// (#15) Once again, same search method but without a if method inside as it only needs to count the elements
double average_elements_key(list* hash_table[]) 
{
    int total_elements = 0;

    for (int i = 0; i < SIZE; ++i) 
    {
        if (hash_table[i] != NULL) 
        {
            student *current = hash_table[i]->head;

            while (current != NULL)
            {
                total_elements++;
                current = current->next;
            }
        }
    }
    double average = (double)total_elements / SIZE;     // AVG calculation, note it must be in double for decimals

    return average;
}

// (#16)
int print_students_atkey(list *table[], int key) 
{
    int index = hash_function(key);     // First we find the index

    if (table[index] == NULL || table[index]->head == NULL)     // index does not exist OR head is NULL (empty list)
    {
        printf("There's no students found at key %d\n", key);
        return 0;
    }

    printf("Students at key %d:\n", key);
    student *current = table[index]->head;

    while (current != NULL) 
    {
        printf("ID: %d, Name: %s, Grade: %d, DOB: %d/%d/%d\n", 
        current->id, current->name, current->grade, current->dob, current->mob, current->yob);
        current = current->next;
    }
    return 0;
}


int main (int argc, char** argv) //not needed , can be main ()
{
    /* 
    (#3) We create a list with the size of our table (100), and we initialize it by calling
    all my future addresses to NULL, mainly to avoid collisions.

    list* hash_table[SIZE];
    for (int i = 0; i < SIZE; ++i)
    {
        hash_table[i] = NULL;
    }

    */
    list* hash_table[SIZE];
    for (int i = 0; i < SIZE; ++i)
    {
        hash_table[i] = create_list();
    }
    
    student* student_1 = create_student(10, "Daniele", 100, _29th, April, 1998);
    student* student_2 = create_student(20, "Ricciardelli", 3, _25th, February, 1994);
    student* student_3 = create_student(30, "Emergui", 50, _30th, January, 1995);
    student* student_4 = create_student(40, "Emilio", 2, _22nd, October, 1960);
    student* student_5 = create_student(50, "Esther", 69, _16th, November, 1960);

    add_student_tohash(hash_table, student_1);
    add_student_tohash(hash_table, student_2);
    add_student_tohash(hash_table, student_3);
    add_student_tohash(hash_table, student_4);
    add_student_tohash(hash_table, student_5);

    // Print details about each student in the hash table
    printf("All Students:\n");
    print_each_student(hash_table);

    // Erase a student from the hash table based on their ID & print again
    erase_student_fromhash(hash_table, 50);
    printf("\n --- After erase student 5---\n");
    print_each_student(hash_table);

    // Erase a student from the list n hash w/o free
    return_and_remove_student(hash_table,40);
    printf("\n --- After erase student 4 ---\n");
    print_each_student(hash_table);

    // Validation check after deleting 2 students
    printf("\n +++ VALIDATION CHECK +++ \n");
    if (validate_sizes(hash_table) == 0){
        printf("IT WORKS!!!\n\n");
    }
    else{
        printf("~~~Something's off~~~");
    }

    // Fill factor check
    printf("Fill factor is: %0.5f\n",find_fill_factor(hash_table)); // 5 decimals
    // 4+ check
    printf("Students with a grade 4 or below: %d\n", students_with_4plus(hash_table));

    // Studets at key check
    printf("\n");
    print_students_atkey(hash_table,20);

    return 0;
}
