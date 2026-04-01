#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// 定义消息结构
struct my_msgbuf {
    long mtype;     // 消息类型
    char mtext[100]; // 消息内容
};

#define PROJECT_ID 'B'
#define MSG_FILE "msg_queue_file" // 用于ftok生成key的文件

int main() {
    int msqid;
    key_t key;
    struct my_msgbuf buf;

    // 1. 生成key
    if ((key = ftok(MSG_FILE, PROJECT_ID)) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    printf("Receiver: Generated key: %d\n", key);

    // 2. 创建/获取消息队列
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    printf("Receiver: Message queue ID: %d\n", msqid);
    printf("Receiver: Waiting for messages...\n");
    printf("Receiver: Type 'exit' in sender to quit.\n");

    // 3. 循环接收消息
    while (1) {
        // 接收类型为1的消息
        ssize_t bytes_received = msgrcv(msqid, &buf, sizeof(buf.mtext), 1, 0);
        if (bytes_received == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        printf("Receiver: Received message (type: %ld, size: %zd bytes): %s\n",
               buf.mtype, bytes_received, buf.mtext);

        // 如果收到 "exit" 消息，则退出
        if (strcmp(buf.mtext, "exit") == 0) {
            printf("Receiver: Exit command received. Closing...\n");
            break;
        }
    }

    // 4. (可选) 删除消息队列。通常由最后一个退出的进程执行。
    // 这里让发送方决定何时退出，接收方收到exit后也退出，不删除队列。
    // if (msgctl(msqid, IPC_RMID, NULL) == -1) {
    //     perror("msgctl");
    //     exit(EXIT_FAILURE);
    // }
    // printf("Receiver: Message queue deleted.\n");

    return EXIT_SUCCESS;
}