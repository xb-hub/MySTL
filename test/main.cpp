#include <iostream>
#include <cstdlib>

union FreeList
{
    union FreeList* free_list_link;
    char* address[1];
};

int main()
{
    FreeList* head = (FreeList*) malloc(5);
    FreeList* node = (FreeList*) malloc(5);
    head->free_list_link = node;
    printf("%x\n%x\n", (FreeList*)head, (char*)head);
    printf("%x\n%x\n", (FreeList*)node, (char*)node);
    printf("%x\n%x\n", (FreeList*)head->free_list_link, (char*)head->free_list_link);
}
