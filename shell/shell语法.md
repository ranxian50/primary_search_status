### Shell脚本语言详解：从入门到精通


#### 一、什么是Shell脚本？
Shell是操作系统的命令解释器，负责接收用户输入的命令并执行。**Shell脚本**（Shell Script）是由一系列Shell命令组成的文本文件，通过解释器（如`bash`）批量执行，实现自动化任务。

常见的Shell解释器：
- `bash`（Bourne Again SHell）：Linux默认Shell，兼容`sh`，功能最丰富。
- `sh`（Bourne Shell）：早期标准Shell，功能简单。
- `zsh`、`ksh`等：增强版Shell，兼容`bash`，增加更多特性。

本文以`bash`为例讲解，因为它是Linux系统中最常用的Shell。


#### 二、入门：第一个Shell脚本
##### 1. 脚本格式
Shell脚本以`.sh`为扩展名（非必须），首行需指定解释器：
```bash
#!/bin/bash  # 声明使用bash解释器
```
`#!`称为“shebang”，告诉系统用哪个程序执行脚本。

##### 2. 编写第一个脚本
创建文件`hello.sh`：
```bash
#!/bin/bash
echo "Hello, Shell Script!"  # echo用于输出文本
```

##### 3. 执行脚本
- 方法1：赋予执行权限后直接运行（推荐）：
  ```bash
  chmod +x hello.sh  # 赋予执行权限
  ./hello.sh         # 执行脚本（./表示当前目录）
  ```
- 方法2：通过解释器运行（无需执行权限）：
  ```bash
  bash hello.sh  # 或 sh hello.sh（若兼容sh）
  ```

输出结果：
```
Hello, Shell Script!
```


#### 三、基础语法
##### 1. 注释
- 单行注释：以`#`开头（除首行`#!/bin/bash`）。
  ```bash
  # 这是一行注释
  echo "执行命令"  # 行尾也可以加注释
  ```
- 多行注释：无原生语法，可用`:`（空命令）配合`<<`（here-document）实现：
  ```bash
  : <<'COMMENT'
  这是多行注释
  第二行
  COMMENT
  ```


##### 2. 变量
Shell变量是**弱类型**（无需声明类型），默认存储字符串，可参与数值运算。

###### （1）定义变量
```bash
name="Alice"  # 等号两侧不能有空格！
age=20
```

###### （2）使用变量
用`$变量名`或`${变量名}`引用（`{}`用于区分变量边界）：
```bash
echo "Name: $name"       # 输出 Name: Alice
echo "Age: ${age}岁"     # 输出 Age: 20岁（{}避免歧义）
```

###### （3）只读变量
用`readonly`声明，无法修改：
```bash
readonly pi=3.14
pi=3.1415  # 报错：pi: readonly variable
```

###### （4）删除变量
用`unset`（不能删除只读变量）：
```bash
unset age  # 删除age变量
echo $age  # 输出空（变量已不存在）
```

###### （5）环境变量
系统预定义的变量，用于配置环境，如：
- `$PATH`：命令搜索路径。
- `$HOME`：当前用户主目录。
- `$USER`：当前用户名。
- `$PWD`：当前工作目录。
- `$?`：上一条命令的退出状态（0表示成功，非0表示失败）。
- `$$`：当前脚本的进程ID（PID）。

示例：
```bash
echo "当前用户：$USER"   # 输出当前用户名
echo "当前目录：$PWD"   # 输出当前路径
echo "上条命令状态：$?"  # 若上条命令成功，输出0
```


##### 3. 字符串
Shell字符串可用单引号`''`或双引号`""`包围，区别：
- 单引号：内部字符原样输出，不解析变量和转义符。
- 双引号：解析变量和转义符（如`\n`换行）。

###### （1）字符串操作
```bash
str1='单引号内$name不解析'
str2="双引号内$name会解析"  # 输出 双引号内Alice会解析

# 拼接字符串
greeting="Hello, "
echo $greeting$name  # 输出 Hello, Alice

# 获取长度
echo ${#name}  # 输出 5（Alice有5个字符）

# 截取子串（从索引0开始，取3个字符）
echo ${name:0:3}  # 输出 Ali
```


##### 4. 数组
Shell仅支持一维数组，元素用空格分隔，下标从0开始。

###### （1）定义数组
```bash
fruits=("apple" "banana" "cherry")
# 或单独定义元素
fruits[0]="apple"
fruits[1]="banana"
```

###### （2）访问数组
```bash
echo ${fruits[0]}   # 输出 apple（访问单个元素）
echo ${fruits[@]}   # 输出所有元素（apple banana cherry）
echo ${#fruits[@]}  # 输出数组长度（3）
```

###### （3）遍历数组
```bash
for fruit in "${fruits[@]}"; do
  echo $fruit
done
```


##### 5. 输入输出
###### （1）输出：`echo`
- 基本用法：`echo "文本"`
- 不换行输出：`echo -n "文本"`
- 解析转义符：`echo -e "Hello\nWorld"`（`\n`换行）

###### （2）输入：`read`
从终端读取用户输入，存入变量：
```bash
echo "请输入姓名："
read username  # 等待用户输入，存入username
echo "你好，$username！"
```
选项：
- `-p`：直接提示信息，无需单独`echo`：
  ```bash
  read -p "请输入年龄：" age
  ```
- `-t`：超时时间（秒），超时后退出：
  ```bash
  read -t 5 -p "5秒内输入：" input  # 5秒未输入则继续执行
  ```


#### 四、流程控制
##### 1. 条件判断（`if`语句）
语法：
```bash
if 条件; then
  命令1
elif 条件2; then
  命令2
else
  命令3
fi  # 结束标志
```

###### （1）条件表达式
条件判断用`[ 表达式 ]`（注意括号内外的空格！）或`[[ 表达式 ]]`（`bash`增强语法，支持模式匹配）。

常用判断：
- 字符串比较：
  - `[ "$a" == "$b" ]`：相等（`==`在`[ ]`中需加引号，`[[ ]]`中可省略）。
  - `[ "$a" != "$b" ]`：不等。
  - `[ -z "$a" ]`：字符串为空。
  - `[ -n "$a" ]`：字符串非空。
- 数值比较：
  - `[ $a -eq $b ]`：等于（equal）。
  - `[ $a -ne $b ]`：不等于（not equal）。
  - `[ $a -gt $b ]`：大于（greater than）。
  - `[ $a -lt $b ]`：小于（less than）。
  - `[ $a -ge $b ]`：大于等于。
  - `[ $a -le $b ]`：小于等于。
- 文件判断：
  - `[ -f "file" ]`：是否为普通文件。
  - `[ -d "dir" ]`：是否为目录。
  - `[ -e "path" ]`：路径是否存在。
  - `[ -r "file" ]`：是否有读权限。

示例：判断数值大小
```bash
read -p "请输入一个数字：" num
if [ $num -gt 10 ]; then
  echo "大于10"
elif [ $num -eq 10 ]; then
  echo "等于10"
else
  echo "小于10"
fi
```


##### 2. 循环
###### （1）`for`循环
语法1：遍历列表
```bash
for 变量 in 列表; do
  命令
done
```
示例：遍历数组
```bash
colors=("red" "green" "blue")
for color in "${colors[@]}"; do
  echo $color
done
```

语法2：C语言风格（`bash`支持）
```bash
for ((i=1; i<=5; i++)); do
  echo "计数：$i"
done
```

###### （2）`while`循环
语法：条件为真时重复执行
```bash
while 条件; do
  命令
done
```
示例：循环5次
```bash
i=1
while [ $i -le 5 ]; do
  echo $i
  i=$((i+1))  # 数值自增
done
```

###### （3）`until`循环
与`while`相反，条件为假时执行（直到条件为真）：
```bash
i=1
until [ $i -gt 5 ]; do
  echo $i
  i=$((i+1))
done
```

###### （4）循环控制
- `break`：跳出当前循环。
- `continue`：跳过本次循环剩余部分，进入下一次。


##### 3. 选择结构（`case`语句）
多分支判断，类似`switch-case`：
```bash
case $变量 in
  模式1)
    命令1
    ;;  # 类似break
  模式2)
    命令2
    ;;
  *)  # 默认分支（匹配所有未命中的模式）
    命令3
    ;;
esac  # 结束标志
```
示例：判断输入的字符
```bash
read -p "输入一个字符：" char
case $char in
  [a-z]|[A-Z])
    echo "是字母"
    ;;
  [0-9])
    echo "是数字"
    ;;
  *)
    echo "是特殊字符"
    ;;
esac
```


#### 五、函数
Shell函数用于封装可复用的代码块。

##### 1. 定义函数
```bash
函数名() {
  命令
  [return 数值]  # 可选，返回状态（0-255，默认0）
}
```
或用`function`关键字：
```bash
function 函数名 {
  命令
}
```

##### 2. 调用函数
直接用函数名调用：
```bash
# 定义函数
greet() {
  echo "Hello, $1!"  # $1表示第一个参数
}

# 调用函数（传递参数）
greet "Bob"  # 输出 Hello, Bob!
```

##### 3. 函数参数
函数内用`$n`（n=1,2,...）获取参数，`$#`获取参数个数，`$@`获取所有参数：
```bash
sum() {
  echo "参数个数：$#"
  echo "所有参数：$@"
  echo "总和：$(( $1 + $2 ))"  # 计算前两个参数的和
}

sum 3 5  # 输出：参数个数：2；所有参数：3 5；总和：8
```

##### 4. 函数返回值
- 用`return`返回状态码（0-255），通过`$?`获取。
- 若需返回数值/字符串，可通过`echo`输出，调用时用`$(函数名)`捕获：
  ```bash
  get_name() {
    echo "Alice"  # 输出返回值
  }

  name=$(get_name)  # 捕获输出作为返回值
  echo $name  # 输出 Alice
  ```


#### 六、高级特性
##### 1. 命令替换
将命令的输出作为变量值，用`$(命令)`或`` `命令` ``（反引号）：
```bash
current_date=$(date +%Y-%m-%d)  # 获取当前日期
echo "今天是：$current_date"

files=`ls`  # 获取当前目录文件列表
echo "文件：$files"
```
推荐用`$(命令)`，支持嵌套：
```bash
echo "当前目录文件数：$(ls | wc -l)"  # 嵌套调用
```


##### 2. 管道（`|`）
将前一个命令的输出作为后一个命令的输入：
```bash
ls -l | grep ".sh"  # 列出所有.sh文件
cat file.txt | wc -l  # 统计file.txt的行数
```


##### 3. 重定向
- `>`：覆盖输出到文件（如`echo "test" > file.txt`）。
- `>>`：追加输出到文件（如`echo "new line" >> file.txt`）。
- `<`：从文件读取输入（如`read var < file.txt`）。
- `2>`：重定向错误输出（如`ls error_dir 2> error.log`）。
- `&>`：重定向所有输出（正确+错误）到文件。


##### 4. 通配符
- `*`：匹配任意个任意字符（如`*.sh`匹配所有.sh文件）。
- `?`：匹配单个任意字符（如`file?.txt`匹配file1.txt、fileA.txt等）。
- `[]`：匹配括号内的任意一个字符（如`[0-9]`匹配数字，`[a-zA-Z]`匹配字母）。


##### 5. 数组进阶
- 切片：`${数组名[@]:起始索引:长度}`（如`${fruits[@]:1:2}`取从索引1开始的2个元素）。
- 替换：`${数组名[@]/旧值/新值}`（批量替换元素中的字符串）。


#### 七、实战案例
##### 案例1：批量重命名文件
将当前目录所有`.txt`文件重命名为`file_1.txt`、`file_2.txt`...
```bash
#!/bin/bash
i=1
for file in *.txt; do
  if [ -f "$file" ]; then  # 确保是文件
    mv "$file" "file_$i.txt"
    echo "重命名：$file -> file_$i.txt"
    i=$((i+1))
  fi
done
```

##### 案例2：备份指定目录到压缩包
```bash
#!/bin/bash
read -p "请输入要备份的目录：" dir
if [ ! -d "$dir" ]; then  # 判断目录是否存在
  echo "错误：$dir 不是有效目录！"
  exit 1  # 退出脚本，状态码1表示错误
fi

backup_name="backup_$(date +%Y%m%d_%H%M%S).tar.gz"
tar -czf "$backup_name" "$dir"  # 压缩目录

if [ $? -eq 0 ]; then  # 判断压缩是否成功
  echo "备份成功：$backup_name"
else
  echo "备份失败！"
  exit 1
fi
```


#### 八、进阶学习资源
1. **内置命令帮助**：`help 命令`（如`help cd`）查看bash内置命令帮助。
2. **man手册**：`man bash`查看bash完整文档。
3. **在线教程**：
   - [GNU Bash手册](https://www.gnu.org/software/bash/manual/)
   - [Shell脚本教程 - 菜鸟教程](https://www.runoob.com/linux/linux-shell.html)
4. **工具**：使用`shellcheck`检查脚本语法错误（`sudo apt install shellcheck`安装）。


通过以上内容，你已掌握Shell脚本的核心语法和实战技巧。多写脚本（如自动化部署、日志分析、文件处理等）是提升的关键，遇到问题时结合手册和调试工具（如`set -x`开启命令跟踪）逐步排查。