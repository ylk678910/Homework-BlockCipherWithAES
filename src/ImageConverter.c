#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int ImageConverter(char *InputImage, char *OutputPath, char *Extension)
{
    char cmd[300];
    int res;

    res = sprintf(cmd, "%s %s%s %s %s %s", "python3", AES_Info.CurrentPath, "/src/ImageConverter.py", InputImage, OutputPath, Extension);
    if (res < 0)
    {
        fprintf(stderr, "Error input.\nCannot convert image to %s.\n", Extension);
        return -1;
    }

    res = system(cmd);
    if (res < 0)
    {
        fprintf(stderr, "Error at execute ImageConverter.py.\n");
        return -1;
    }

    return 0;
}
