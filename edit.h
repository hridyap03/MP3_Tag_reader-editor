#include <stdio.h>
#include <string.h>
#include "edit.h"
#include "view.h"
#include <stdlib.h>

Status read_and_validate_edit_args(char *argv[], EditInfo *editinfo, int argc)
{
    if (argc < 5)
    {
        printf("Error:insufficient argument count\n");
        return e_failure;
    }
    if (argv[2] == NULL)
    {
        /* Printing usage info */
        printf("\nFor viewing  : ./a.out -v <file.mp3>\n");
        printf("\nTo perform editing  : ./a.out -e <operation> <data_to_update> <file.mp3>\n");
        printf("\t\t-t : To edit Title\n");
        printf("\t\t-y : To edit Year\n");
        printf("\t\t-a : To edit Album\n");
        printf("\t\t-A : To edit Artist\n");
        printf("\t\t-c : To edit Comment\n");
        printf("\t\t-g : To edit Genre\n");
        return e_failure;
    }

    if (strcmp(argv[2], "-t") == 0)
    {
        strcpy(editinfo->frame_id, "TIT2");
    }
    else if (strcmp(argv[2], "-y") == 0)
    {
        strcpy(editinfo->frame_id, "TYER");
    }
    else if (strcmp(argv[2], "-a") == 0)
    {
        strcpy(editinfo->frame_id, "TALB");
    }
    else if (strcmp(argv[2], "-A") == 0)
    {
        strcpy(editinfo->frame_id, "TPE1");
    }
    else if (strcmp(argv[2], "-c") == 0)
    {
        strcpy(editinfo->frame_id, "COMM");
    }
    else if (strcmp(argv[2], "-g") == 0)
    {
        strcpy(editinfo->frame_id, "TCON");
    }
    if (argv[3] == NULL || (strstr(argv[3], ".mp3")) != NULL)
    {
        printf("new data not found\n");
    }
    else
    {
        strcpy(editinfo->new_data, argv[3]);
    }
    if (argv[4] == NULL)
    {
        printf("Audio file  not found\n");
        return e_failure;
    }
    else if (strstr(argv[4], ".mp3") != NULL)
    {
        printf("Mp3 File is present\n");
        editinfo->audio_fname = argv[4];
    }
    else
    {
        printf("Mp3 File is not present\n");
        return e_failure;
    }
    return e_success;
}
Status open_edit_files(EditInfo *editinfo)
{
    editinfo->fptr_mp3 = fopen(editinfo->audio_fname, "r");
    if (editinfo->fptr_mp3 == NULL)
    {
        return e_failure;
    }
    editinfo->temp_fname = "temp.mp3";
    editinfo->fptr_temp_mp3 = fopen(editinfo->temp_fname, "w");
    if (editinfo->fptr_temp_mp3 == NULL)
    {
        return e_failure;
    }
    return e_success;
}
Status do_edit_data(EditInfo *editinfo)
{
    if (open_edit_files(editinfo) == e_failure)
    {
        printf("opened files unsuccessful\n");
        return e_failure;
    }
    // printf("Opened files uccesfully\n");

    if (copy_header_bytes(editinfo, 10) == e_failure)
    {
        printf("header byte copy unsuccessful\n");
        return e_failure;
    }
    printf("header byte copy successful\n");
    if (do_edit_new_data(editinfo) == e_failure)
    {
        printf("editting unsuccessful\n");
        return e_failure;
    }
    printf("Edited successfully\n");
    return e_success;
}

Status copy_header_bytes(EditInfo *editinfo, int size)
{
    char buffer[size];
    fread(buffer, size, 1, editinfo->fptr_mp3);
    fwrite(buffer, size, 1, editinfo->fptr_temp_mp3);

    return e_success;
}
Status do_edit_new_data(EditInfo *editinfo)
{
    char tag[5];  // to store the tag
    int size;     // to store the size
    char flag[3]; // to store the flag

    while (1)
    {
        fread(tag, 4, 1, editinfo->fptr_mp3);
        tag[4] = '\0';
        int original_size;
        fread(&original_size, 4, 1, editinfo->fptr_mp3);
        size = original_size;
        endian(&size, 4);
        char *data = malloc(size);
        if (data == NULL)
        {
            printf("Memory allocation failed\n");
            return e_failure;
        }
        fread(flag, 3, 1, editinfo->fptr_mp3);
        fread(data, (size - 1), 1, editinfo->fptr_mp3);
        data[size - 1] = '\0';
        if (strcmp(tag, editinfo->frame_id) == 0)
        {
            fwrite(tag, 4, 1, editinfo->fptr_temp_mp3);
            int new_size = strlen(editinfo->new_data) + 1;
            int new_size_be = new_size;
            endian(&new_size_be, 4);
            fwrite(&new_size_be, 4, 1, editinfo->fptr_temp_mp3);
            fwrite(flag, 3, 1, editinfo->fptr_temp_mp3);
            fwrite(editinfo->new_data, new_size - 1, 1, editinfo->fptr_temp_mp3);
            free(data);
            break;
        }
        else
        {
            fwrite(tag, 4, 1, editinfo->fptr_temp_mp3);
            fwrite(&original_size, 4, 1, editinfo->fptr_temp_mp3);
            fwrite(flag, 3, 1, editinfo->fptr_temp_mp3);
            fwrite(data, size - 1, 1, editinfo->fptr_temp_mp3);
            free(data);
        }
    }
    char buff[1];
    while (fread(buff, 1, 1, editinfo->fptr_mp3) == 1)
    {
        fwrite(buff, 1, 1, editinfo->fptr_temp_mp3);
    }

    fclose(editinfo->fptr_temp_mp3);
    fclose(editinfo->fptr_mp3);

    remove(editinfo->audio_fname);
    rename("temp.mp3", editinfo->audio_fname);
    return e_success;
}
