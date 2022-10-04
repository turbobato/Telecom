#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked-list.h"
#include "link-types.h"

extern int number_of_students;

/********************   read_file_content   ********************
* read file and extract information to store it in array
**************************************************************/

void read_file_content(Student_t * array, FILE * file){

  int ret;
  do {
    ret = fscanf(file, "%s %s %d",
    array[number_of_students].lastname,
    array[number_of_students].firstname,
    &array[number_of_students].group);

    if (ret == 3)
      number_of_students++;
    else
      printf ("Line  %d : error or end of file \n", number_of_students);
  } while (ret != EOF );

  number_of_students--;

}

/********************    new_link   *******************
* Create a new link of type Link_t: the field next is initialized to NULL
* Returns the adress of the new link.
********************************************************/
Link_t * new_link(Student_t student){
  Link_t *Ptr;

  Ptr = malloc(sizeof(Link_t));
	if (Ptr == NULL ) perror("Error occured when using malloc");
	Ptr->student = student;
  Ptr->next = NULL;

  return Ptr;
}

/********************    chain   *******************
* Add a new link (new_link) at the beginning of an existing
* linked list (beginning).
* Returns the address of the first element of the list.
********************************************************/
Link_t * chain(Link_t * beginning, Link_t * new_link){
  Link_t * Ptr;

  Ptr = new_link;
  Ptr->next = beginning;

  return Ptr;
}

/********************   display_linked_list   *******************
* Display (using printf) the information contained in the
* linked list.
********************************************************/
void display_linked_list(Link_t * list){
  Link_t * Ptr = list;
  int i=0;
  while (Ptr != NULL){
    printf ("Name %s, firstname %s, group %d, (index %d), adress of next link : (0x)%p\n",
        Ptr->student.lastname, Ptr->student.firstname, Ptr->student.group,
         i, (int *)Ptr->next);
    Ptr=Ptr->next;
    i = i+1;
  }
}

/********************   search   *******************
* Search for a link corresponding to a student last
* name
* beginning: first link of the list used for searching
* name_to_search: last name of the student to search
* returns the firts link for which the name equals
* name_to_search
********************************************************/
Link_t * search (Link_t * beginning, char * name_to_search)
{
  Link_t * found_link = beginning;

  while (found_link != NULL){
    if (strcmp(found_link->student.lastname, name_to_search) == 0)
      break;
    found_link=found_link->next;
  }

  return found_link;
}


/********************   insert   *******************
* Insert a link in a linked list
* beginning: first link of the original list
* new_link: link to insert
* returns the firts link of the list after insertion
********************************************************/
Link_t * insert (Link_t * beginning, Link_t * new_link)
{
  Link_t * list_after_insertion;

	Student_t s = new_link->student;
	if  ( beginning == NULL || ( strcmp ( s.lastname, beginning->student.lastname ) <= 0 ) )
		  list_after_insertion = chain(beginning, new_link);
	else
	{
    list_after_insertion = beginning;
    Link_t * p, * q;
    p = beginning;
		q = beginning -> next;
		while ( q != NULL  && ( strcmp(s.lastname, q->student.lastname) >  0) )
		{
			p = q;
			q = q -> next;
		}
		p->next = chain(q, new_link);
	}

  return list_after_insertion;
}

/********************   sort   *******************
* Sort the content of the linked list.
********************************************************/
Link_t * sort (Link_t * beginning)
{
  Link_t * sorted_list=NULL;
  Link_t * ptr = beginning;
  Link_t * new_beginning;

	while ( ptr != NULL)
	{
    new_beginning = ptr;
		ptr = ptr -> next;
		sorted_list = insert (sorted_list, new_beginning);
	}

  return sorted_list;
}

/********************   free_list   *******************
* Free the memory used by the linked list.
********************************************************/
void free_list(Link_t * list){

  Link_t *Ptr1, *Ptr2;
	Ptr1=list;

	while ( Ptr1->next != NULL) {
		Ptr2=Ptr1->next;
		free(Ptr1);
		Ptr1=Ptr2;
	}

}
