
### 步骤 1：确保 WSL 和必要工具已安装

1.  **安装 WSL**：
    *   在 Windows 10 或 11 上，打开 “Microsoft Store”。
    *   搜索 “Ubuntu” (例如 “Ubuntu 22.04 LTS”)，选择一个版本并安装。
    *   安装后，首次启动它会要求你创建一个 Linux 用户和密码。

2.  **在 WSL Ubuntu 中安装 GCC 和 Make**：
    *   启动你的 Ubuntu 终端（可以从开始菜单找到）。
    *   更新包列表并安装构建工具：
        ```bash
        sudo apt update
        sudo apt install build-essential
        ```
        这个命令会安装 `gcc`, `g++`, `make` 等一系列编译和构建所需的工具。

### 步骤 2：在 VS Code 中安装必要的扩展

为了无缝地与 WSL 集成，你需要安装 Microsoft 官方的 WSL 扩展。

1.  打开 VS Code。
2.  进入扩展市场（快捷键 `Ctrl+Shift+X`）。
3.  搜索并安装 **“WSL - Windows Subsystem for Linux”** 扩展。安装后可能需要重启 VS Code。

### 步骤 3：在 VS Code 中打开 WSL 中的文件

有两种推荐的方法可以在 VS Code 中访问你的 WSL 文件：

#### 方法 A：通过 VS Code 远程连接（推荐）

这是最强大和推荐的方式。它会在 WSL 中运行一个 VS Code 服务器，让你感觉就像在本地编辑一样，但所有操作都在 Linux 环境中进行。

1.  在 VS Code 中，按 `Ctrl+Shift+P` 打开命令面板。
2.  输入并选择 **“WSL: Connect to WSL”**。
3.  它会列出你已安装的 WSL 发行版（例如 `Ubuntu-22.04`），选择它。
4.  连接成功后，VS Code 窗口的左下角会显示 `WSL: Ubuntu-22.04`。
5.  现在，你可以通过 `文件(File)` > `打开文件夹(Open Folder)` 来打开 WSL 中的任何目录。VS Code 的文件浏览器会显示 Linux 文件系统的内容。导航到你的 C 语言文件所在的目录并打开它。

#### 方法 B：通过文件路径直接访问

WSL 文件系统在 Windows 中被映射为网络驱动器。

1.  在 VS Code 中，选择 `文件(File)` > `打开文件(Open File)`。
2.  在文件路径栏中，输入 `\\wsl$` 并按回车。
3.  你会看到你的 WSL 发行版（例如 `Ubuntu-22.04`）。
4.  双击进入，然后导航到你的 Linux 用户目录（通常在 `/home/<你的用户名>`），找到并打开你的 C 文件。

**注意**：虽然方法 B 简单直接，但方法 A（远程连接）提供了更好的性能和更完整的 Linux 开发体验。

### 步骤 4：在 VS Code 终端中使用 Ubuntu 的 GCC

现在，你的 VS Code 已经连接到 WSL 环境，集成终端也应该自动切换到 WSL 终端。

1.  在 VS Code 中，打开一个集成终端（快捷键 `` Ctrl+` `` 或者 `终端(Terminal)` > `新建终端(New Terminal)`）。
2.  你应该会看到一个类似 `user@machine:~$` 的命令提示符，这表示你正在与 WSL Ubuntu 交互。
3.  在这个终端里，你可以像在普通的 Ubuntu 终端一样使用 `gcc` 命令。

**示例**：

假设你在 WSL 中的文件路径是 `/home/yourusername/projects/fifo_experiment/fifo_reader.c`。

*   **编译**：
    ```bash
    # 确保你在正确的目录下
    cd /home/yourusername/projects/fifo_experiment

    # 使用 gcc 编译
    gcc fifo_reader.c -o reader
    gcc fifo_writer.c -o writer
    ```

*   **运行**：
    ```bash
    # 运行读方程序
    ./reader
    ```
    然后打开**另一个新的终端**（同样是 WSL 终端）来运行写方程序：
    ```bash
    # 在新终端中运行写方程序
    cd /home/yourusername/projects/fifo_experiment
    ./writer
    ```

