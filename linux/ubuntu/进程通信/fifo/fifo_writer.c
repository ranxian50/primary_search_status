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
