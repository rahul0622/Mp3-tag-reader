#include<stdio.h>

#ifndef HEADER_H
#define HEADER_H

void View_mp3(FILE *fp);
void Edit_mp3(FILE *fp, char cmd, char *new_data, char *filename);

#endif