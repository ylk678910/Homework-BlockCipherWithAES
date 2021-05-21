#include "main.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int GetArg(int, char *[]);

char CurrentPath[100];
char InputImageName[20];

struct AES_Infomation AES_Info;

int main(int argc, char *argv[])
{
    if (GetArg(argc, argv))
    {
        return -1;
    }

    ImageConverter(AES_Info.InputImage, AES_Info.OutputPath, "ppm");
    AES_env(0);
    return 0;
}

static int GetArg(int argc, char *argv[])
{
    //Get arguments from argv.
    char *arg, m_ptr;
    for (int ptr = 1; ptr < argc; ptr++)
    {
        arg = argv[ptr];
        m_ptr = 0;

        while (arg[m_ptr] == '-')
        {
            m_ptr++;
        }

        arg += m_ptr;

        switch (m_ptr)
        {
        case 1:
            switch (*arg)
            {
            case 'f':
                AES_Info.InputImage = argv[ptr + 1];
                break;
            case 'o':
                AES_Info.OutputPath = argv[ptr + 1];
                break;
            }
            break;
        case 2:
            if (strcmp("mode", arg) == 0)
                AES_Info.Mode = argv[ptr + 1];
            break;
        }
    }
    //
    //
    getcwd(CurrentPath, sizeof(CurrentPath));
    AES_Info.CurrentPath = CurrentPath;

    char *tmp_ptr, found_flg = 0, tmp[100];
    tmp_ptr = strtok(AES_Info.InputImage, ".");
    while (1)
    {
        AES_Info.InputImageName = tmp_ptr;

        tmp_ptr = strtok(NULL, ".");
        if (tmp_ptr == NULL)
            break;
        strcpy(tmp, tmp_ptr);
        for (int i = 0; i < strlen(tmp); i++)
        {
            tolower(tmp[i]);
        }
        if (strcmp(tmp, "jpeg") == 0 || strcmp(tmp, "jpg") == 0)
        {
            AES_Info.InputImageExtension = tmp_ptr;
            found_flg = 1;
            break;
        }
    }

    if (!found_flg)
    {
        printf("Wrong image input.\n");
        return -1;
    }

    strcpy(InputImageName, AES_Info.InputImageName);
    AES_Info.InputImageName = strtok(InputImageName, "/");
    sprintf(AES_Info.InputImage, "%s.%s", AES_Info.InputImage, AES_Info.InputImageExtension);

    return 0;
}