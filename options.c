#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "header.h"

void View_mp3(FILE *fp)
{
    char frame_id[5];
    unsigned char size_bytes[4];
    int frame_size;
    char buffer[100];
    int frame_count=0;

    rewind(fp);

    fseek(fp,10,SEEK_SET);                             // skip header

    while(frame_count != 6)
    {
        fread(frame_id,4,1,fp);                        // reading tag
        frame_id[4]='\0';

        fread(size_bytes,4,1,fp);                      // reading size of respective tag
        frame_size = (size_bytes[0]<<24 | size_bytes[1]<<16 | size_bytes[2]<<8 |size_bytes[3]);  // performing explicit conversion

        fseek(fp,3,SEEK_CUR);                          // skipping flag bytes

        fread(buffer,(frame_size-1),1,fp);             // reading the respective tag info
        buffer[frame_size-1]='\0';

        if(strcmp(frame_id,"TIT2") == 0)               // checking condtion to display which tag information according to read data
            printf("Title : %s\n",buffer);
        else if(strcmp(frame_id,"TYER") == 0)
            printf("Year : %s\n",buffer);
        else if(strcmp(frame_id,"TPE1") == 0)
            printf("Artist : %s\n",buffer);
        else if(strcmp(frame_id,"TALB") == 0)
            printf("Album : %s\n",buffer);
        else if(strcmp(frame_id,"TCON") == 0)
            printf("Genre : %s\n",buffer);
        else if(strcmp(frame_id,"COMM") == 0)
            printf("Comments : %s\n",buffer);

        frame_count++;                                 // increasing frame count
    }
}

void Edit_mp3(FILE *fp, char cmd, char *new_data, char *filename)
{
    FILE *temp;
    char target_frame[5];
    char buffer[1024];
    int count = 0;
    char frame_id[5];
    unsigned char size_bytes[4];
    int frame_size;

    rewind(fp);

    temp = fopen("temp.mp3","w+");                     // open temporary mp3 file
    if(temp == NULL)
    {
        printf("FILE NOT FOUND\n");
    }

    switch (cmd)                                       // choosing which tag according to user input
    {
        case 't':
            strcpy(target_frame,"TIT2");
            break;
        case 'y':
            strcpy(target_frame,"TYER");
            break;
        case 'a':
            strcpy(target_frame,"TPE1");
            break;
        case 'A':
            strcpy(target_frame,"TALB");
            break;
        case 'g':
            strcpy(target_frame,"TCON");
            break;
        case 'c':
            strcpy(target_frame,"COMM");
            break;
        default:
            printf("Invalid choice\n");
            fclose(temp);
            remove("temp.mp3");
        return;
    }

    fread(buffer,10,1,fp);                             // read 10 bytes of header from selected mp3 file to buffer
    fwrite(buffer,10,1,temp);                          // write the contents of buffer into temp mp3 file

    while(count != 6)                      
    {
        fread(frame_id,4,1,fp);                        // reading tags from original mp3 file
        frame_id[4]='\0';
        fread(size_bytes,4,1,fp);
        frame_size = (size_bytes[0]<<24 | size_bytes[1]<<16 | size_bytes[2]<<8 | size_bytes[3]);
        fread(buffer,2,1,fp);                          // reading flags and NULL

        if(strcmp(frame_id,target_frame) == 0)         // cheack if the tag is the tag to be edited
        {
            int new_size = strlen(new_data)+1;

            unsigned char encoding = 0x00;

            fwrite(frame_id,4,1,temp);
            unsigned char new_size_bytes[4] = {(new_size >> 24) & 0xFF, (new_size >> 16) & 0xFF, (new_size >> 8) & 0xFF, (new_size >> 0)};
            fwrite(new_size_bytes,4,1,temp);
            fwrite(buffer,2,1,temp);
            fwrite(&encoding,1,1,temp);
            fwrite(new_data,strlen(new_data),1,temp);

            fseek(fp,frame_size,SEEK_CUR);
        }
        else                                           // copying the other tags into temp mp3
        {
            fwrite(frame_id,4,1,temp);                 // writing frame id to temp
            fwrite(size_bytes,4,1,temp);               // writing frame size to temp
            fwrite(buffer,2,1,temp);                   // writing flag to temp
            fread(buffer,frame_size,1,fp);             // reading frame data from original mp3
            fwrite(buffer,frame_size,1,temp);          // writing frame data to temp mp3
        }
        count++;
    }

    int bytes; 

    while((bytes = fread(buffer,1,sizeof(buffer),fp)) > 0)   // copying the remaining data into temp mp3 file
    {
        fwrite(buffer,1,bytes,temp);
    }
    fclose(temp);

    remove(filename);                                  // removing the original mp3 file
    rename("temp.mp3",filename);                       // renaming the temp mp3 file as the original mp3 file name

    printf("Successfully edited\n");
}