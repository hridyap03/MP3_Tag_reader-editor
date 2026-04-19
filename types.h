/*
Name        :   Hridya P
Description :   MP3 Tag Reader
Date        :   23-12-2025
Input       :   ./a.out -v <file.mp3>
Output      :---------------SONG INFO---------------
            TITLE :
            ARTIST :
            ALBUM  :
            YEAR   :
            GENRE  :
            COMMENT:
Input      :  ./a.out -e <operation> <data_to_update> <file.mp3>
Output     :-------------Selected mode is editing-------------
            Mp3 File is present
            edit validation successful
            header byte copy successful

*/

#include <stdio.h>
#include <string.h>
#include "view.h"
#include "edit.h"
#include "types.h"

OperationType check_operation_type(char *argv[]);

int main(int argc, char *argv[])

{

    if (argc < 2)
    {
        printf("Error : Insufficient argument count\n");
        return e_failure;
    }
    OperationType option = check_operation_type(argv);

    if (option == e_view)
    {
        view doview;

        // printf("-------------Selected mode is view-------------\n");

        if (read_and_validate_view_args(argv, &doview) == e_success)
        {
            // printf("Veiw validation successful\n");

            if (do_view(&doview) == e_success)
            {
                return e_success;
            }
            else
            {
                return e_failure;
            }
        }

        else
        {
            printf("Read and validate unsuccessful\n");
            return e_failure;
        }
    }

    else if (option == e_edit)
    {

        printf("-------------Selected mode is editing-------------\n");
        EditInfo editinfo;

        if (read_and_validate_edit_args(argv, &editinfo, argc) == e_success)
        {

            printf("edit validation successful\n");
            if (do_edit_data(&editinfo) == e_success)
            {
                return e_success;
            }
            else
            {
                return e_failure;
            }
        }
        else
        {
            printf("edit validation unsuccesfull\n");
            return e_failure;
        }
    }
    else if (option == e_help)
    {
        printf("\nFor viewing  : ./a.out -v <file.mp3>\n");
        printf("\nFor editing  : ./a.out -e <operation> <data_to_update> <file.mp3>\n");
        printf("\n\t\t\t-t : To edit Title\n");
        printf("\t\t\t-y : To edit Year\n");
        printf("\t\t\t-a : To edit Album\n");
        printf("\t\t\t-A : To edit Artist\n");
        printf("\t\t\t-c : To edit Comment\n");
        printf("\t\t\t-g : To edit Genre\n");
        printf("\n(Note : Type the data to update in double quotes except for Year)\n");
    }
    else
    {
        printf("not supported\n");
        return e_failure;
    }

    return e_success;
}

OperationType check_operation_type(char *argv[])
{

    if ((strcmp(argv[1], "-v")) == 0)
    {
        return e_view;
    }
    else if (strcmp(argv[1], "-e") == 0)
    {
        return e_edit;
    }
    else if (strcmp(argv[1], "--h") == 0)
    {
        return e_help;
    }
    else
    {
        return e_unsupported;
    }
}
