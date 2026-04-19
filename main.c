#include <stdio.h>
#include "types.h"

typedef struct _Edit
{

    FILE *fptr_temp_mp3;
    char *temp_fname; // temperory audio file name
    FILE *fptr_mp3;
    char *audio_fname;  // audio file name;
    char frame_id[5];   // to store tag title
    char new_data[100]; // to store new data

} EditInfo;

/* Function declarations */
OperationType check_operation_type(char *argv[]);
Status read_and_validate_edit_args(char *argv[], EditInfo *editinfo, int argc);
Status open_edit_files(EditInfo *editinfo);
Status do_edit_data(EditInfo *editinfo);
Status copy_header_bytes(EditInfo *editinfo, int size);
Status do_edit_new_data(EditInfo *editinfo);
