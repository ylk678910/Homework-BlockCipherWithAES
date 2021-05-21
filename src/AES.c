#include "main.h"
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PIPE_W "./tmp/pipe_0.tmp"
#define PIPE_R "./tmp/pipe_1.tmp"
#define BUFFER_SIZE 100

char buf[BUFFER_SIZE], pipe_exist = 0;
FILE *fd_w, *fd_r;
pthread_t Sender;
pthread_t Receiver;

int send_cmd(char *cmd);
int send_PixelKey(char R, char G, char B, char *key);
int receive_cmd(char cmd[]);
void *Sender_th(void *param);
void *Receiver_th(void *param);
static int InitPipe(void);
static int InitAESpyProcess(void);
static int ClosePipe(void);
static int CloseAESpyProcess(void);

pid_t pid;

void sigintHandler(int sig_num)
{
    char tmp[30];
    sprintf(tmp, "kill -9 %d", pid);
    system(tmp);
    exit(-1);
}

int AES_env(int test)
{
    signal(SIGINT, sigintHandler);
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Failed to encode image.\n");
        return -1;
    }
    else if (pid == 0)
    {
        if (InitAESpyProcess() == -1)
        {
            fprintf(stderr, "Failed to start AES process.\n");
            exit(-1);
        }
    }
    else
    {
        if (InitPipe() == -1)
            return -1;

        send_PixelKey('F', 'F', 'F', "12f4567890123456");

        char cmd[100];
        while (1)
        {
            receive_cmd(cmd);
            if (strcmp(cmd, "return") == 0)
            {
                printf("get ans\n");
                break;
            }
            else
            {
                printf("at c in thread get : %s\n", cmd);
                break;
            }
        }

        CloseAESpyProcess();
        ClosePipe();
    }
    return 0;
}

static int InitPipe(void)
{
    int res;

    unlink(PIPE_R);
    unlink(PIPE_W);
    mkfifo(PIPE_W, 0666 | S_IFIFO);
    mkfifo(PIPE_R, 0666 | S_IFIFO);

    fd_w = fopen(PIPE_W, "w");
    fd_r = fopen(PIPE_R, "r");

    res = pthread_create(&Sender, NULL, (void *)Sender_th, NULL);
    if (res < 0)
    {
        fprintf(stderr, "Error at create Sender thread.\n");
        return -1;
    }
    res = pthread_create(&Receiver, NULL, (void *)Receiver_th, NULL);
    if (res < 0)
    {
        fprintf(stderr, "Error at create Receiver thread.\n");
        return -1;
    }
    pipe_exist = 1;
    return 0;
}

static int InitAESpyProcess(void)
{
    char PyPath[100];

    sprintf(PyPath, "%s/src/AES.py", AES_Info.CurrentPath);

    char *args[7];
    args[0] = "python3";
    args[1] = PyPath;
    args[2] = "--pr";
    args[3] = PIPE_W;
    args[4] = "--pw";
    args[5] = PIPE_R;
    args[6] = NULL;

    execvp(args[0], args);

    return -1;
}

static int ClosePipe(void)
{
    if (pipe_exist == 0)
    {
        fprintf(stderr, "Pipe does not exsist.\n");
        return -1;
    }
    sprintf(buf, "%s", "");
    fclose(fd_w);
    fclose(fd_r);
    pthread_cancel(Sender);
    pthread_cancel(Receiver);
    remove(PIPE_W);
    remove(PIPE_R);
    pipe_exist = 0;
    return 0;
}

static int CloseAESpyProcess(void)
{
    send_cmd("exit");
    wait(NULL);
    return 0;
}

int receive_cmd(char cmd[])
{
    while (1)
    {
        if (buf[0] != 0)
        {
            strcpy(cmd, buf);
            sprintf(buf, "%s", "");
            break;
        }
    }
    return 0;
}

int send_cmd(char *cmd)
{
    fprintf(fd_w, "%s\n", cmd);
    fflush(fd_w);
    return 0;
}

int send_PixelKey(char R, char G, char B, char *key)
{
    if (strlen(key) > 16 || strlen(key) == 0)
    {
        fprintf(stderr, "Error key length.\n");
        return -1;
    }

    char cmd[21];
    sprintf(cmd, "%c%c%c,%s", 0x46, 0x46, 0x46, key);
    send_cmd(cmd);
    return 0;
}

void *Sender_th(void *param)
{
    if (fd_w == NULL)
    {
        fprintf(stderr, "Failed to open\n");
        pthread_exit((void *)-1);
    }
    fseek(fd_w, 0, SEEK_SET);
}

void *Receiver_th(void *param)
{
    if (fd_r == NULL)
    {
        fprintf(stderr, "Failed to open\n");
        pthread_exit((void *)-1);
    }
    fseek(fd_r, 0, SEEK_SET);

    int timer = 0;
    while (1)
    {
        fscanf(fd_r, "%s", buf);
        usleep(10000);
    }
}
