
---

## 一、消息队列（Message Queue）通信详解

### 1. 什么是消息队列？

可以把消息队列想象成一个内核维护的“邮箱”或“消息缓冲区”。

*   **消息 (Message)**：进程间传递的数据单元。消息具有特定的格式，通常包括一个**消息类型**和一个**数据体**。
*   **队列 (Queue)**：消息按照“先进先出”（FIFO）的原则排队。但发送方可以指定消息类型，接收方可以根据类型来接收消息，这使得接收方可以有选择地获取特定类型的消息，而不是简单地按顺序接收。
*   **内核维护**：消息队列本身存在于内核空间，独立于发送和接收进程。这意味着即使发送进程退出，只要消息队列没有被删除，消息依然存在，等待接收进程读取。

### 2. 消息队列的特点

1.  **双向通信**：支持双向通信，进程既可以是发送方也可以是接收方。
2.  **异步通信**：发送方发送消息后即可返回，无需等待接收方立即接收。接收方可以在需要的时候再读取消息。
3.  **非阻塞通信（可选）**：可以设置为非阻塞模式。当消息队列为空时，接收操作不会阻塞；当消息队列满时，发送操作不会阻塞（会立即返回错误）。
4.  **消息类型**：每个消息都有一个类型标识。接收方可以：
    *   接收特定类型的消息。
    *   接收类型小于或等于某个值的消息。
    *   按消息到达的顺序接收（不指定类型）。
5.  **结构化数据**：消息可以是结构化的，方便传递复杂信息。
6.  **生命周期**：消息队列的生命周期随内核，直到被显式删除或系统重启。
7.  **与管道的区别**：
    *   **数据形式**：管道是字节流，消息队列是结构化的消息。
    *   **读取方式**：管道只能顺序读取，消息队列可以按类型随机读取。
    *   **阻塞行为**：管道的 `read`/`write` 可能阻塞。消息队列的 `msgsnd`/`msgrcv` 可以设置为非阻塞。
    *   **持久性**：管道的生命周期与进程相关。消息队列在内核中持久存在。

### 3. 消息队列的核心概念

*   **消息队列ID (msgid)**：每个消息队列都有一个唯一的ID，进程通过这个ID来标识和访问特定的消息队列。
*   **消息结构 (`struct msgbuf`)**：
    ```c
    struct msgbuf {
        long mtype;     // 消息类型，必须大于 0
        char mtext[1];  // 消息数据体，可以是任意大小的数组或其他数据结构
    };
    ```
    *   `mtype` 是必须的，且必须是正整数。
    *   `mtext` 是消息内容的起始地址。实际使用时，你可以定义一个更大的结构体，`mtext` 只是一个通用的表示。例如：
        ```c
        struct my_message {
            long mtype;
            char username[20];
            int age;
            char content[100];
        };
        ```

### 4. 消息队列的创建和使用

#### 核心系统调用：

1.  **`msgget()`**：创建一个新的消息队列或获取一个已存在的消息队列的ID。
    ```c
    #include <sys/msg.h>

    int msgget(key_t key, int msgflg);
    ```
    *   `key`：一个键值，用于标识消息队列。通常使用 `ftok()` 函数生成。
    *   `msgflg`：标志位，用于指定创建方式和权限。
        *   `IPC_CREAT`：如果消息队列不存在，则创建它。
        *   `IPC_EXCL`：与 `IPC_CREAT` 一起使用，如果消息队列已存在，则调用失败。
        *   权限位：如 `0666`，表示所有用户可读可写。
    *   **返回值**：成功返回消息队列ID，失败返回-1。

2.  **`msgsnd()`**：向消息队列发送一条消息。
    ```c
    #include <sys/msg.h>

    int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
    ```
    *   `msqid`：消息队列ID。
    *   `msgp`：指向要发送的消息结构体的指针。
    *   `msgsz`：消息数据体（`mtext`部分）的大小，不包括 `mtype` 的大小。
    *   `msgflg`：发送标志。
        *   `0`：默认行为，若队列满则阻塞。
        *   `IPC_NOWAIT`：非阻塞模式，若队列满则立即返回-1，并设置 `errno` 为 `EAGAIN`。
    *   **返回值**：成功返回0，失败返回-1。

3.  **`msgrcv()`**：从消息队列接收一条消息。
    ```c
    #include <sys/msg.h>

    ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
    ```
    *   `msqid`：消息队列ID。
    *   `msgp`：指向用于接收消息的结构体的指针。
    *   `msgsz`：消息数据体的最大长度。如果接收的消息数据体大于 `msgsz`，则消息会被截断（`msgtyp` 仍会被正确接收），且 `errno` 会被设置为 `E2BIG`。
    *   `msgtyp`：指定要接收的消息类型。
        *   `msgtyp == 0`：接收队列中的第一条消息（FIFO）。
        *   `msgtyp > 0`：接收类型为 `msgtyp` 的第一条消息。
        *   `msgtyp < 0`：接收类型小于或等于 `abs(msgtyp)` 的第一条消息。
    *   `msgflg`：接收标志。
        *   `0`：默认行为，若队列中没有符合条件的消息则阻塞。
        *   `IPC_NOWAIT`：非阻塞模式，若没有符合条件的消息则立即返回-1，并设置 `errno` 为 `ENOMSG`。
    *   **返回值**：成功返回接收到的消息数据体的大小，失败返回-1。

4.  **`msgctl()`**：对消息队列执行控制操作，如删除、获取状态信息等。
    ```c
    #include <sys/msg.h>

    int msgctl(int msqid, int cmd, struct msqid_ds *buf);
    ```
    *   `msqid`：消息队列ID。
    *   `cmd`：要执行的命令。
        *   `IPC_STAT`：获取消息队列的状态信息，存入 `buf` 指向的 `struct msqid_ds` 结构体。
        *   `IPC_SET`：根据 `buf` 中的信息设置消息队列的状态（如所有者、权限）。
        *   `IPC_RMID`：删除消息队列。这是最常用的命令。
    *   `buf`：指向 `struct msqid_ds` 结构体的指针，用于传递状态信息。
    *   **返回值**：成功返回0，失败返回-1。

5.  **`ftok()`**：将一个路径名和一个项目ID转换为一个 `key_t` 值，用于 `msgget()`。
    ```c
    #include <sys/types.h>
    #include <sys/ipc.h>

    key_t ftok(const char *pathname, int proj_id);
    ```
    *   `pathname`：一个已存在的、可访问的文件路径。
    *   `proj_id`：一个8位的非零整数（通常是 'a' 到 'z' 之间的字符）。
    *   **返回值**：成功返回一个 `key_t` 键值，失败返回-1。

---

## 二、源码示例

我们将创建两个程序：`mq_sender.c`（发送方）和 `mq_receiver.c`（接收方）。

### 1. mq_receiver.c (接收方程序)

```c
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
```

### 2. mq_sender.c (发送方程序)

```c
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
```

---
