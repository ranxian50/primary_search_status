
---

## 一、命名管道（FIFO）通信详解

### 1. 什么是命名管道？

命名管道（Named Pipe），也被称为 FIFO（First-In-First-Out），是一种特殊类型的文件，它允许不相关的进程之间进行双向的数据通信。

- **“命名”的含义**：与匿名管道（Anonymous Pipe）不同，命名管道有一个可见的文件名，并存在于文件系统中。任何进程只要知道这个文件名，就可以打开它进行读写。
- **“管道”的含义**：数据的传输是基于先进先出的原则，如同一个管道，数据从一端流入，从另一端流出。
- **内核维护**：虽然它在文件系统中有一个入口，但命名管道的数据并不存放在磁盘上，而是像匿名管道一样，由内核在内存中维护一个缓冲区。

### 2. 命名管道的特点

1. **跨进程通信**：这是命名管道最核心的优势。它可以连接任何两个进程，无论它们是否有亲缘关系（父进程与子进程）。
2. **文件系统可见性**：通过 `ls -l` 命令可以看到 FIFO 文件，其文件类型标识为 `p` (pipe)。
3. **双向通信**：虽然在某些场景下会被用作单向通信，但 FIFO 本身支持双向读写。
4. **阻塞特性**：
    - **读操作 (open O_RDONLY)**：如果没有进程以写方式打开 FIFO，读操作会阻塞，直到有进程打开它进行写入。
    - **写操作 (open O_WRONLY)**：如果没有进程以读方式打开 FIFO，写操作会阻塞，直到有进程打开它进行读取。
    - **读写操作 (read/write)**：如果管道为空，`read()` 会阻塞；如果管道满了，`write()` 会阻塞。
5. **基于字节流**：数据以字节流的形式传输，没有消息边界。如果写入方分两次写入了 "Hello" 和 "World"，读取方可能一次就把 "HelloWorld" 全部读出来。

### 3. 命名管道的创建和使用

#### 创建方式：

1.  **命令行创建**：使用 `mkfifo` 命令。
    ```bash
    mkfifo my_pipe
    ```

2.  **程序中创建**：使用 `mkfifo()` 系统调用。
    ```c
    #include <sys/types.h>
    #include <sys/stat.h>

    int mkfifo(const char *pathname, mode_t mode);
    ```
    - `pathname`：FIFO 文件的路径名。
    - `mode`：文件权限，与 `open()` 或 `mkdir()` 的 `mode` 参数相同（如 `0666` 表示所有用户可读可写）。
    - 返回值：成功返回 0，失败返回 -1，并设置 `errno`。

#### 使用步骤：

1.  **打开 FIFO**：使用 `open()` 系统调用。
    - 读方：`int fd = open("my_pipe", O_RDONLY);`
    - 写方：`int fd = open("my_pipe", O_WRONLY);`
    - 读写：`int fd = open("my_pipe", O_RDWR);` (这种方式较少见，因为它会打破阻塞特性)

2.  **读写数据**：使用 `read()` 和 `write()` 系统调用。
    ```c
    // 写数据
    const char *msg = "Hello from writer!";
    write(fd, msg, strlen(msg) + 1); // +1 是为了把字符串结束符'\0'也写进去

    // 读数据
    char buffer[1024];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read > 0) {
        printf("Received: %s\n", buffer);
    }
    ```

3.  **关闭 FIFO**：使用 `close()` 系统调用。
    ```c
    close(fd);
    ```

4.  **删除 FIFO**：当不再需要时，使用 `unlink()` 系统调用或 `rm` 命令将其从文件系统中删除。
    ```bash
    rm my_pipe
    ```
    ```c
    unlink("my_pipe");
    ```

---

## 二、源码示例

我们将创建两个独立的程序：`fifo_writer.c` 和 `fifo_reader.c`。

### 1. fifo_reader.c (读方程序)

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_NAME "/tmp/my_test_fifo"
#define BUFFER_SIZE 1024

int main() {
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // 1. 检查并创建 FIFO
    if (access(FIFO_NAME, F_OK) == -1) {
        printf("FIFO '%s' does not exist. Creating it...\n", FIFO_NAME);
        if (mkfifo(FIFO_NAME, 0666) == -1) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    printf("Reader: Opening FIFO for reading...\n");
    // 2. 以只读方式打开 FIFO。如果没有写方，这里会阻塞
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    printf("Reader: FIFO opened successfully. Waiting for data...\n");

    // 3. 循环读取数据
    while (1) {
        bytes_read = read(fd, buffer, BUFFER_SIZE - 1); // 留一个字节给'\0'
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // 确保字符串正确结束
            printf("Reader: Received %zd bytes: %s\n", bytes_read, buffer);

            // 如果收到 "exit" 命令，则退出循环
            if (strcmp(buffer, "exit") == 0) {
                printf("Reader: Exit command received. Closing...\n");
                break;
            }
        } else if (bytes_read == 0) {
            // 写方关闭了FIFO，read()返回0
            printf("Reader: Writer has closed the FIFO. Exiting...\n");
            break;
        } else { // bytes_read == -1
            perror("read");
            break;
        }
    }

    // 4. 关闭 FIFO
    close(fd);
    printf("Reader: FIFO closed.\n");

    // 注意：通常由创建者或最后一个使用者删除FIFO
    // unlink(FIFO_NAME); 

    return EXIT_SUCCESS;
}
```

### 2. fifo_writer.c (写方程序)

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_NAME "/tmp/my_test_fifo"
#define BUFFER_SIZE 1024

int main() {
    int fd;
    char buffer[BUFFER_SIZE];

    printf("Writer: Opening FIFO for writing...\n");
    // 1. 以只写方式打开 FIFO。如果没有读方，这里会阻塞
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    printf("Writer: FIFO opened successfully. Ready to send data.\n");
    printf("Writer: Type your message (type 'exit' to quit):\n");

    // 2. 循环从标准输入读取数据并写入 FIFO
    while (1) {
        printf("> ");
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            perror("fgets");
            break;
        }

        // fgets 会包含末尾的换行符'\n'，我们可以去掉它
        buffer[strcspn(buffer, "\n")] = '\0';

        // 3. 写入数据
        ssize_t bytes_written = write(fd, buffer, strlen(buffer) + 1); // +1 for '\0'
        if (bytes_written == -1) {
            perror("write");
            break;
        }
        printf("Writer: Sent %zd bytes: %s\n", bytes_written, buffer);

        // 如果输入 "exit"，则退出循环
        if (strcmp(buffer, "exit") == 0) {
            printf("Writer: Exit command sent. Closing...\n");
            break;
        }
    }

    // 4. 关闭 FIFO
    close(fd);
    printf("Writer: FIFO closed.\n");

    return EXIT_SUCCESS;
}
```

---

## 三、学习实验报告

# Linux 命名管道（FIFO）通信实验报告

## 实验名称
Linux 命名管道（FIFO）跨进程通信实验

## 实验日期
[填写日期]

## 实验人
[填写姓名]

---

### 一、实验目的

1.  理解命名管道（FIFO）的概念、特点和工作原理。
2.  掌握在 Linux 环境下使用命令行和 C 语言创建命名管道的方法。
3.  学会编写利用命名管道进行数据交换的客户端/服务器程序。
4.  观察并验证命名管道的阻塞特性。
5.  对比命名管道与匿名管道的异同。

### 二、实验原理

1.  **命名管道定义**：命名管道是一种特殊的文件，它提供了一个在文件系统中的路径名，允许不相关的进程通过打开这个文件进行通信。数据在内核缓冲区中传输，遵循先进先出（FIFO）原则。
2.  **核心系统调用**：
    *   `mkfifo(const char *pathname, mode_t mode)`: 创建一个命名管道。
    *   `open(const char *pathname, int flags)`: 打开命名管道，`O_RDONLY` 为读，`O_WRONLY` 为写。
    *   `read(int fd, void *buf, size_t count)`: 从管道读取数据。
    *   `write(int fd, const void *buf, size_t count)`: 向管道写入数据。
    *   `close(int fd)`: 关闭文件描述符。
    *   `unlink(const char *pathname)`: 删除文件系统中的FIFO文件。
3.  **阻塞行为**：
    *   以读方式打开一个没有写方的FIFO，`open()` 会阻塞。
    *   以写方式打开一个没有读方的FIFO，`open()` 会阻塞。
    *   当管道为空时，`read()` 调用会阻塞，直到有数据写入。
    *   当管道满时，`write()` 调用会阻塞，直到有空间可用（数据被读取）。

### 三、实验环境

*   操作系统：[例如：Ubuntu 22.04 LTS / CentOS 7]
*   编译器：GCC (GNU Compiler Collection)
*   工具：终端 (Terminal)、文本编辑器 (Vim, VS Code等)

### 四、实验内容与步骤

#### 1. 准备工作

*   打开终端，创建一个新的工作目录。
    ```bash
    mkdir fifo_experiment
    cd fifo_experiment
    ```

#### 2. 使用命令行操作 FIFO

*   **创建 FIFO**：在终端中输入以下命令创建一个名为 `cmd_pipe` 的FIFO。
    ```bash
    mkfifo cmd_pipe
    ```
*   **查看 FIFO**：使用 `ls -l` 命令查看文件类型。你会看到类似 `-rw-rw-r--` 的权限，并且第一个字符是 `p`，代表这是一个管道文件。
    ```bash
    ls -l
    # 输出示例: prw-rw-r-- 1 user user 0 Aug 22 10:00 cmd_pipe
    ```
*   **单向通信测试**：
    *   **第一步**：打开一个新的终端窗口（终端A），用于读取 `cmd_pipe`。
    *   **第二步**：在终端A中，使用 `cat` 命令从FIFO读取数据。此时 `cat` 命令会阻塞，等待数据。
        ```bash
        cat < cmd_pipe
        ```
    *   **第三步**：在原来的终端窗口（终端B）中，向FIFO写入数据。
        ```bash
        echo "Hello, FIFO from command line!" > cmd_pipe
        ```
    *   **观察结果**：你会看到终端A中立即显示了从终端B发送过来的消息，然后 `cat` 命令退出。

#### 3. 编译并运行 C 语言程序

*   **创建源码文件**：使用文本编辑器创建 `fifo_reader.c` 和 `fifo_writer.c` 两个文件，并将上面提供的源码分别复制进去。
*   **编译程序**：
    ```bash
    gcc fifo_reader.c -o reader
    gcc fifo_writer.c -o writer
    ```
*   **运行读方程序**：
    *   打开一个新的终端（终端1）。
    *   进入工作目录并运行读方程序。
        ```bash
        cd fifo_experiment
        ./reader
        ```
    *   **观察**：程序会提示正在创建FIFO（如果不存在），然后在 `open()` 调用处阻塞，等待写方连接。
*   **运行写方程序**：
    *   打开另一个新的终端（终端2）。
    *   进入工作目录并运行写方程序。
        ```bash
        cd fifo_experiment
        ./writer
        ```
    *   **观察**：终端2的写方程序成功打开FIFO。同时，观察终端1，读方程序的 `open()` 调用解除阻塞，开始等待数据。
*   **进行通信**：
    *   在终端2（写方）的提示符 `>` 后输入任意字符串，然后按回车。
    *   **观察**：终端1（读方）会立即显示收到的数据。
    *   重复几次发送不同的消息。
*   **结束通信**：
    *   在终端2（写方）输入 `exit` 并回车。
    *   **观察**：终端1（读方）收到 "exit" 命令后，打印退出信息并关闭程序。终端2的写方程序在发送 "exit" 后也会关闭。
*   **清理**：检查 `/tmp` 目录下的 `my_test_fifo` 文件是否还存在。实验结束后可以手动删除它。
    ```bash
    rm /tmp/my_test_fifo
    ```