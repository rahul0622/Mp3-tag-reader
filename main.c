#include<stdio.h>
#include<string.h>
#include"header.h"

int main(int argc, char *argv[])
{
    FILE *fp;
    char ck_mp3[4];
    // Information on handling input
    if(argc < 2 || strcmp(argv[1], "-h") == 0)
    {
        printf("Usage:\n");
        printf("./a.out -v <mp3_filename> to view details of mp3 file\n");
        printf("./a.out -e <frame_tag> <new_data> <mp3_filename> to edit the tags of mp3 file\n");
        printf("-t -> title\n-y -> year\n-a -> artist\n-A -> album\n-g -> genre\n-c -> comments\n");
        return 0;
    }
    // reading user-defined mp3 file
    char *mp3_file = argv[argc-1];
    // to open the mp3 file
    fp=fopen(mp3_file,"r");
    if(fp == NULL)
    {
        printf("File Not Found\n");
        return 0;
    }
    fread(ck_mp3,3,1,fp);
    // storing null manually beacuse fread reads byte by byte
    ck_mp3[3]='\0';

    // mp3 File Check
    if(strcmp(ck_mp3,"ID3")  != 0)
    {
        printf("Not MP3 File\n");
        fclose(fp);
        return 0;
    }

    // Operation Check
    if(strcmp(argv[1] ,"-v") == 0)
        View_mp3(fp); 
    else if(strcmp(argv[1] ,"-e") == 0)
    {
        if(argc < 5)
        {
            printf("Invalid edit command\n");
            return 0;
        }
        char cmd = argv[2][1];
        char *new_data = argv[3];

        Edit_mp3(fp, cmd, new_data, mp3_file);
    }
    else
    {
        printf("Invalid Operation\n");
        printf("Use -h for information\n");
    }

    fclose(fp);
    return 0;
}