#include "link-types.h"

#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

void read_file_content(Student_t * array, FILE * file);
Link_t * new_link(Student_t student);
Link_t * chain(Link_t * beginning, Link_t * new_link);
void display_linked_list(Link_t * list);
void free_list(Link_t * list);
Link_t * sort (Link_t * beginning);
Link_t * search (Link_t * beginning, char * name_to_search);
Link_t * insert (Link_t * beginning, Link_t * new_link);

#endif
