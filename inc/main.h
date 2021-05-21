#define USER_ARG_NUM 3
#define TOTAL_ARG_NUM (USER_ARG_NUM + 1)

extern char CurrentPath[100];
extern char InputImageName[20];

extern struct AES_Infomation
{
    char *CurrentPath;
    char *InputImage;
    char *InputImageName;
    char *InputImageExtension;
    char *OutputPath;
    char *Mode;
} AES_Info;

int ImageConverter(char *InputImage, char *OutputPath, char *Extension);
int AES_env(int test);
