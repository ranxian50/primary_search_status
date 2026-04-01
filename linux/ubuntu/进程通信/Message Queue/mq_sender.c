#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// 必须与接收方定义的消息结构一致
struct my_msgbuf {
    long mtype;     // 消息类型
    char mtext[100]; // 消息内容
};

#define PROJECT_ID 'B' // 必须与接收方一致
#define MSG_FILE "msg_queue_file" // 必须与接收方一致

int main() {
    int msqid;
    key_t key;
    struct my_msgbuf buf;

    // 确保用于ftok的文件存在
    FILE *fp = fopen(MSG_FILE, "w");
    if (fp) fclose(fp);

    // 1. 生成与接收方相同的key
    if ((key = ftok(MSG_FILE, PROJECT_ID)) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    printf("Sender: Generated key: %d\n", key);

    // 2. 获取消息队列 (如果不存在则创建)
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    printf("Sender: Message queue ID: %d\n", msqid);
    printf("Sender: Enter messages to send (type 'exit' to quit):\n");

    buf.mtype = 1; // 设置消息类型为1

    // 3. 循环从标准输入读取并发送消息
    while (1) {
        printf("> ");
        if (fgets(buf.mtext, sizeof(buf.mtext), stdin) == NULL) {
            perror("fgets");
            exit(EXIT_FAILURE);
        }

        // 去除fgets带来的换行符
        buf.mtext[strcspn(buf.mtext, "\n")] = '\0';

        // 4. 发送消息
        if (msgsnd(msqid, &buf, strlen(buf.mtext) + 1, 0) == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
        printf("Sender: Message sent.\n");

        // 如果输入 "exit"，则退出
        if (strcmp(buf.mtext, "exit") == 0) {
            printf("Sender: Exiting...\n");
            break;
        }
    }

    // 清理用于ftok的临时文件
    remove(MSG_FILE);

    return EXIT_SUCCESS;
}