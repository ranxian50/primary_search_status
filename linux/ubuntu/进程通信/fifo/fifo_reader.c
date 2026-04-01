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