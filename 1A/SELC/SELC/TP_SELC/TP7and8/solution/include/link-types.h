#ifndef __LINK_TYPES_H
#define __LINK_TYPES_H

 typedef struct student {
 	char lastname[30];
 	char firstname[20];
 	int group;
 } Student_t;

 typedef struct link{
      Student_t  student;
      struct link * next;
} Link_t;

#endif
