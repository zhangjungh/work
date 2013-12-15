#include <stdlib.h> 
#include <stdio.h> 
 
struct ListNode { 
public: 
   struct ListNode *next; 
   ListNode(int _val) { value = _val, next = NULL; }; 
   int GetValue() { return value; }; 
   //...should have other function like create list...// 
private: 
   int value; //make it private 
}; 
 
typedef struct ListNode item; 
 
void SwapAdjacentElements(item **pphead, item *elem) 
{ 
    // validate 
    if (pphead == NULL || elem == NULL) return; 
    item *head = *pphead;
    if (head == NULL || elem->next == NULL) return; 

    if (head == elem)
    {
        head = elem->next;
        elem->next = head->next;
        head->next = elem;
        //list has a new head
        *pphead = head;             
    }
    else 
    {
        // find the position of the element         
        while (head->next != NULL && head->next != elem) 
            head = head->next; 
        if (head->next != elem) return; 
        //swap the pointer 
        item *temp = head;
        head = elem->next;
        elem->next = head->next;
        head->next = elem;
        temp->next = head;             
    }
}  
 
 
int main() 
{ 
  
    ListNode *head = NULL, *curr; 
 
    // create a list 
    for(int i = 0; i < 10; i++) { 
        curr = new ListNode(i); 
        curr->next  = head; 
        head = curr; 
    } 
 
    curr = head; 
 
    while(curr) { 
        printf("%d, ", curr->GetValue()); 
        curr = curr->next ; 
    } 
    printf("\n");
 
    //just a simple test; 
    SwapAdjacentElements(&head, head->next); 
 
    curr = head; 
 
    while(curr) { 
        printf("%d, ", curr->GetValue()); 
        curr = curr->next ; 
    } 
    printf("\n");
 
    //destroy the list 
    curr = head; 
 
    while(curr) { 
        head = curr; 
        curr = head->next ; 
        delete head; 
    } 
    system("pause");
    return 0; 
}
