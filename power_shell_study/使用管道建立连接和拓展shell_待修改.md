在 PowerShell 中，**管道（Pipeline）** 是核心特性之一，它允许将一个命令的输出直接作为另一个命令的输入，实现命令间的无缝协作，极大增强了 shell 的灵活性和扩展性。以下是关于管道的详细讲解：


### **一、管道的基本概念**
- **符号**：管道通过竖线 `|` 表示
- **作用**：将左侧命令的输出（对象）传递给右侧命令作为输入
- **优势**：无需临时文件/变量存储中间结果，简化复杂操作，实现"流式处理"

**基本语法**：
```powershell
命令1 | 命令2 | 命令3
```

示例：
```powershell
Get-ChildItem | Where-Object Length -gt 100000 | Sort-Object Length
```
- `Get-ChildItem` 列出文件 → 传递给 `Where-Object` 筛选大于100KB的文件 → 再传递给 `Sort-Object` 按大小排序


### **二、管道的工作原理**
PowerShell 管道传递的是 **.NET 对象**（而非文本），这与传统 shell（如 cmd、bash）传递纯文本的管道有本质区别：
1. 左侧命令输出的每个对象（如文件、进程、服务）会被逐个传递给右侧命令
2. 右侧命令通过 `$_`（自动变量，代表当前对象）处理每个输入对象
3. 支持多阶段管道（多个 `|` 连接），形成处理流水线

示例：处理进程对象
```powershell
# 筛选内存占用超过100MB的进程，并显示名称和内存使用
Get-Process | Where-Object { $_.WorkingSet64 -gt 100MB } | Select-Object Name, WorkingSet64
```
- `Get-Process` 输出 `Process` 类型对象
- `Where-Object` 基于对象的 `WorkingSet64` 属性筛选
- `Select-Object` 提取对象的 `Name` 和 `WorkingSet64` 属性


### **三、常用管道操作命令**
以下命令专为管道设计，用于处理输入对象：

#### 1. `Where-Object`（别名 `?`）
- **功能**：按条件筛选对象
- **语法**：`Where-Object { 条件表达式 }`
- **示例**：
  ```powershell
  # 筛选C盘下修改时间在30天内的.txt文件
  Get-ChildItem C:\ -Filter *.txt -Recurse | ? { $_.LastWriteTime -gt (Get-Date).AddDays(-30) }
  ```

#### 2. `Select-Object`（别名 `select`）
- **功能**：选择对象属性、限制数量或创建计算属性
- **示例**：
  ```powershell
  # 获取前5个运行中的服务，并只显示名称和状态
  Get-Service | ? Status -eq Running | Select-Object -First 5 Name, Status
  ```

#### 3. `Sort-Object`（别名 `sort`）
- **功能**：按属性排序对象
- **示例**：
  ```powershell
  # 按文件大小降序排列，显示名称和大小
  Get-ChildItem | Sort-Object Length -Descending | Select-Object Name, Length
  ```

#### 4. `ForEach-Object`（别名 `%`）
- **功能**：对每个对象执行操作（循环处理）
- **示例**：
  ```powershell
  # 为所有.log文件添加".bak"后缀
  Get-ChildItem *.log | % { Rename-Item $_ -NewName "$($_.Name).bak" }
  ```

#### 5. `Measure-Object`（别名 `measure`）
- **功能**：统计对象属性（计数、求和、平均值等）
- **示例**：
  ```powershell
  # 统计当前目录文件总数和总大小
  Get-ChildItem -File | Measure-Object -Property Length -Sum -Count
  ```

#### 6. `Out-*` 系列命令
- 将管道结果输出到不同目标：
  - `Out-File`：保存到文件
  - `Out-GridView`：在图形窗口中显示（交互式筛选）
  - `Out-Printer`：发送到打印机
- **示例**：
  ```powershell
  # 将进程列表导出到CSV文件
  Get-Process | Select-Object Name, Id | Out-File processes.txt
  # 在网格视图中查看服务
  Get-Service | Out-GridView
  ```


### **四、管道的高级用法**

#### 1. 多阶段管道组合
通过多个管道阶段实现复杂逻辑：
```powershell
# 查找占用CPU最高的5个进程，计算总CPU使用率
Get-Process | 
  Where-Object CPU -gt 0 |  # 排除CPU为0的进程
  Sort-Object CPU -Descending |  # 按CPU降序
  Select-Object -First 5 Name, CPU |  # 取前5个
  Measure-Object -Property CPU -Sum |  # 计算总和
  Select-Object @{Name="TotalCPU"; Expression={$_.Sum}}  # 显示结果
```

#### 2. 自定义属性（计算属性）
使用 `Select-Object` 创建动态属性：
```powershell
# 显示文件名称和大小（转换为MB）
Get-ChildItem | Select-Object Name, @{
  Name = "SizeMB"
  Expression = { [math]::Round($_.Length / 1MB, 2) }
}
```

#### 3. 管道中的错误处理
结合通用参数处理错误：
```powershell
# 尝试删除所有.tmp文件，忽略错误
Get-ChildItem *.tmp | Remove-Item -ErrorAction SilentlyContinue
```

#### 4. 管道与函数/脚本结合
自定义函数接收管道输入：
```powershell
# 定义一个处理管道输入的函数
function Add-Prefix {
  param(
    [Parameter(ValueFromPipeline=$true)]  # 允许从管道接收输入
    [string]$InputObject,
    [string]$Prefix = "-"
  )
  process {  # process块确保逐个处理管道对象
    "$Prefix $InputObject"
  }
}

# 使用函数：为每一行添加前缀
Get-Content "notes.txt" | Add-Prefix -Prefix ">>"
```


### **五、管道的局限性与注意事项**
1. **对象类型匹配**：右侧命令必须能处理左侧命令输出的对象类型（可通过 `Get-TypeName` 查看类型）
2. **性能考量**：复杂管道可能影响性能，大量数据处理时可考虑分批处理
3. **非对象输出**：部分命令输出纯文本（如 `ipconfig`），管道处理时需先转换为对象
   ```powershell
   # 处理文本输出（需手动解析）
   ipconfig | Where-Object { $_ -match "IPv4" }
   ```
4. **避免过度管道**：过于冗长的管道可能降低可读性，适当拆分或使用变量


### **六、管道拓展 shell 能力的典型场景**
1. **系统监控与分析**：
   ```powershell
   # 监控内存使用前10的进程，并实时刷新
   while ($true) {
     Get-Process | Sort-Object WorkingSet64 -Descending | Select-Object -First 10 Name, WorkingSet64
     Start-Sleep -Seconds 5
     Clear-Host
   }
   ```

2. **批量文件处理**：
   ```powershell
   # 查找所有空文件夹并删除
   Get-ChildItem -Directory -Recurse | 
     Where-Object { (Get-ChildItem $_).Count -eq 0 } | 
     Remove-Item -WhatIf  # 测试无误后移除-WhatIf
   ```

3. **网络数据采集**：
   ```powershell
   # 检查多个网站的响应状态
   @("google.com", "github.com", "example.com") | 
     ForEach-Object {
       $status = (Invoke-WebRequest $_ -TimeoutSec 5 -ErrorAction SilentlyContinue).StatusCode
       [PSCustomObject]@{ Site = $_; Status = $status ?? "Down" }
     }
   ```


### **总结**
PowerShell 管道通过传递对象实现命令协作，是拓展 shell 能力的核心机制：
- 基础用法：用 `|` 连接命令，实现筛选、排序、转换等操作
- 进阶技巧：结合自定义函数、计算属性、错误处理实现复杂任务
- 优势：简化代码、提高效率、增强可读性

掌握管道的使用，可以让你从简单的命令执行提升到高效的自动化任务处理，充分发挥 PowerShell 的强大功能。