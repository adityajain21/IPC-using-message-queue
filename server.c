#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXSIZE     128


void die(char *s)
{
  perror(s);
  exit(1);
}

struct msgbuf
{
    long    mtype;
    char    mtext[MAXSIZE];
};

main()
{
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    struct msgbuf sbuf;
    size_t buflen;

    key = 1234;

    if ((msqid = msgget(key, msgflg )) < 0)   //Get the message queue ID for the given key
      die("msgget");

    //Message Type
    sbuf.mtype = 1;

    int ad;
    printf("Enter Text for communication\n");
    scanf("%[^\n]",sbuf.mtext);
    getchar();
    while(1)
    {
        printf("WRITE MODE\n");
        ad=0;
        while(1)
        {
            ad++;
            buflen = strlen(sbuf.mtext) + 1 ;

            if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0)
            {
                printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buflen);
                die("msgsnd");
                ad++;
            }

            else
            {
                printf("Message Sent\n");
                int time_to_run=1000000000;
                while(time_to_run--);
                ad++;
            }
            
            if(ad>25)
                break;
        }
        printf("READ MODE!\n");
        ad=0;
        while(1)
        {
            ad++;
            if (msgrcv(msqid, &sbuf, MAXSIZE, 1, 0) < 0)
            die("msgrcv"),ad++;

            printf("%s\n", sbuf.mtext);
            ad++;

            if(ad>25)
                break;
        }
    }
    exit(0);
}