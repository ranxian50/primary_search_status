在Windows 11上使用WSL安装Ubuntu 24并显示图形化界面，可以按照以下步骤进行：

### 安装WSL 2
1. **检查Windows版本**：确保你的Windows 11是最新版本。可以通过“设置”-“更新和安全”-“Windows更新”来检查更新。
2. **启用虚拟功能**：以管理员身份打开PowerShell，运行以下命令启用适用于Linux的Windows子系统和虚拟机平台功能：
```bash
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
```
3. **下载Linux内核更新程序包**：从[WSL 2内核更新包](https://wslstorestorage.blob.core.windows.net/wslblob/wsl_update_x64.msi)下载并安装适用于WSL 2的Linux内核更新程序包。
4. **设置WSL 2为默认版本**：在PowerShell中运行以下命令，查看已安装的WSL版本：
```bash
wsl -l -v
```
如果版本不是2，使用以下命令将其设置为WSL 2：
```bash
wsl --set-default-version 2
```

### 安装Ubuntu 24
1. **通过命令行安装**：打开PowerShell，运行以下命令安装Ubuntu 24.04，你可以使用`--location`参数指定安装目录：
```bash
wsl --install -d Ubuntu-24.04 --location "D:\wsl\ubuntu-24.04"
```
2. **启动Ubuntu**：安装完成后，可以在开始菜单中找到“Ubuntu 24.04 LTS”并点击启动，或者在PowerShell中运行以下命令启动：
```bash
wsl -d Ubuntu-24.04
```
首次启动时，系统会提示你设置用户名和密码。

### 安装图形化界面
1. **更新Ubuntu系统**：在Ubuntu终端中，运行以下命令更新软件包列表和已安装的软件包：
```bash
sudo apt update && sudo apt upgrade
```
2. **安装图形界面组件**：运行以下命令安装Ubuntu桌面环境和XWayland：
```bash
sudo apt install ubuntu-desktop xwayland
```
3. **创建和配置服务**：运行以下命令创建并编辑一个服务文件：
```bash
sudo systemctl edit --full --force wslg-fix.service
```
在编辑器中粘贴以下内容，然后按`Ctrl+S`保存，按`Ctrl+X`退出：
```
[Service]
Type=oneshot
ExecStart=-/usr/bin/umount /tmp/.X11-unix
ExecStart=/usr/bin/rm -rf /tmp/.X11-unix
ExecStart=/usr/bin/mkdir /tmp/.X11-unix
ExecStart=/usr/bin/chmod 1777 /tmp/.X11-unix
ExecStart=/usr/bin/ln -s /mnt/wslg/.X11-unix/X0 /tmp/.X11-unix/X0
ExecStart=/usr/bin/chmod 0777 /mnt/wslg/runtime-dir
ExecStart=/usr/bin/chmod 0666 /mnt/wslg/runtime-dir/wayland-0.lock
[Install]
WantedBy=multi-user.target
```
4. **启用服务**：运行以下命令启用刚刚创建的服务：
```bash
sudo systemctl enable wslg-fix.service
```

完成以上步骤后，你就可以在Windows 11上通过WSL运行带有图形化界面的Ubuntu 24了。你可以在Ubuntu终端中输入图形化应用程序的名称来启动它们，例如`gedit`或`firefox`，它们将以窗口形式显示在Windows桌面上。