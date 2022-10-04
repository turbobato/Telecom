#ifndef _LINKED_LISTS_H
#define _LINKED_LISTS_H

#include "link-types.h"

void read_file_content(Student_t * array, FILE * file);

Link_t * new_link(Student_t student);

Link_t * chain(Link_t * beginning, Link_t * new_link);

void display_linked_list(Link_t * list);

Link_t * search (Link_t * beginning, char * name_to_search);

Link_t * insert (Link_t * beginning, Link_t * new_link);

Link_t * sort (Link_t * beginning);

void free_list(Link_t * list);

#endif