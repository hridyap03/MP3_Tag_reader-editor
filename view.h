#include <stdio.h>
#include <string.h>
#include "view.h"
#include <stdlib.h>
Status open_files(view *doview)
{
    doview->fptr_src_mp3 = fopen(doview->src_audio_fname, "r");
    if (doview->fptr_src_mp3 == NULL)
    {
        return e_failure;
    }
    return e_success;
}
Status read_and_validate_view_args(char *argv[], view *doview)
{
    char *ext = strstr(argv[2], ".mp3");
    if (ext != NULL && (strcmp(ext, ".mp3")) == 0)
    {

        // printf(".mp3 present\n");
        strcpy(doview->src_audio_fname, argv[2]);
        return e_success;
    }
    else
    {
        printf(".mp3 not present\n");
        return e_failure;
    }
}

Status skip_header(FILE *fptr_src_mp3)
{
    fseek(fptr_src_mp3, 10, SEEK_SET);
    return e_success;
}
Status read_and_print_data(FILE *fptr_src_mp3)
{
    printf("---------------SONG INFO---------------\n");
    int count = 0;
    while (count < 6)
    {
        char tag[5];
        int size = 0;
        if (fread(tag, 4, 1, fptr_src_mp3) == 0)
        {
            break;
        }
        tag[4] = '\0';
        fread(&size, 4, 1, fptr_src_mp3);
        endian(&size, 4);
        if (size <= 1)
        {
            break;
        }
        fseek(fptr_src_mp3, 3, SEEK_CUR);

        char *text = malloc(size);
        if (!text)
        {
            return e_failure;
        }
        fread(text, size - 1, 1, fptr_src_mp3);
        text[size - 1] = '\0';
        if (print(tag, text) == e_success)
        {
            count++;
        }
        free(text);
    }
    return e_success;
}
void endian(void *data, int size)
{
    char *ptr = (char *)data;
    for (int i = 0; i < size / 2; i++)
    {
        char temp = ptr[i];
        ptr[i] = ptr[size - 1 - i];
        ptr[size - 1 - i] = temp;
    }
}
Status print(char *tag, char *text)
{
    if (strcmp(tag, "TIT2") == 0)
    {
        printf("TITLE\t\t:%s\n", text);
    }
    else if (strcmp(tag, "TYER") == 0)
    {
        printf("YEAR\t\t:%s\n", text);
    }
    else if (strcmp(tag, "TALB") == 0)
    {
        printf("ALBUM\t\t:%s\n", text);
    }
    else if (strcmp(tag, "TPE1") == 0)
    {
        printf("ARTIST\t\t:%s\n", text);
    }
    else if (strcmp(tag, "COMM") == 0)
    {
        printf("COMMENT\t\t:%s\n", text);
    }
    else if (strcmp(tag, "TCON") == 0)
    {
        printf("GENRE\t\t:%s\n", text);
    }
    else
    {
        return e_failure;
    }
    return e_success;
}
Status do_view(view *doview)
{
    if (open_files(doview) == e_failure)
    {
        printf("Open files unsuccessful\n");
        return e_failure;
    }
    // printf("opened succsfully\n");
    if (skip_header(doview->fptr_src_mp3) == e_failure)
    {
        printf("Skipping header bytes unsuccessful\n");
        return e_failure;
    }
    // printf("skipped successfully\n");
    if (read_and_print_data(doview->fptr_src_mp3) == e_failure)
    {
        fclose(doview->fptr_src_mp3);
        return e_failure;
    }
    fclose(doview->fptr_src_mp3);
    return e_failure;

    return e_success;
}