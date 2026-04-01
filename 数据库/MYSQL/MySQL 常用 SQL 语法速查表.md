# MySQL常用SQL语法速查表
## 一、数据库操作
| 操作                | 语法                                                                 |
|---------------------|----------------------------------------------------------------------|
| 创建数据库          | `CREATE DATABASE 数据库名 [CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci];` |
| 查看所有数据库      | `SHOW DATABASES;`                                                     |
| 切换数据库          | `USE 数据库名;`                                                       |
| 修改数据库字符集    | `ALTER DATABASE 数据库名 CHARACTER SET utf8mb4;`                      |
| 删除数据库          | `DROP DATABASE 数据库名;`                                             |


## 二、表结构操作（DDL）
### 1. 表创建与删除
```sql
-- 创建表
CREATE TABLE 表名 (
  字段1 类型 [约束],
  字段2 类型 [约束],
  ...
  [PRIMARY KEY (字段名)] -- 主键约束（可单独写）
);

-- 查看当前库所有表
SHOW TABLES;

-- 查看表结构
DESC 表名; 或 SHOW COLUMNS FROM 表名;

-- 修改表名
ALTER TABLE 旧表名 RENAME TO 新表名;

-- 删除表
DROP TABLE 表名;

-- 清空表（自增主键重置）
TRUNCATE TABLE 表名;
```

### 2. 字段操作
```sql
-- 添加字段
ALTER TABLE 表名 ADD 字段名 类型 [约束] [AFTER 已有字段名]; -- AFTER指定位置，默认末尾

-- 修改字段类型/约束
ALTER TABLE 表名 MODIFY 字段名 新类型 [新约束];

-- 修改字段名+类型
ALTER TABLE 表名 CHANGE 旧字段名 新字段名 新类型 [约束];

-- 删除字段
ALTER TABLE 表名 DROP 字段名;
```

### 3. 约束操作
```sql
-- 添加主键约束
ALTER TABLE 表名 ADD PRIMARY KEY (字段名);

-- 添加唯一约束
ALTER TABLE 表名 ADD UNIQUE (字段名);

-- 添加外键约束（关联主表）
ALTER TABLE 从表名 ADD FOREIGN KEY (从表字段) REFERENCES 主表名 (主表字段);

-- 删除主键约束
ALTER TABLE 表名 DROP PRIMARY KEY;

-- 删除唯一约束
ALTER TABLE 表名 DROP INDEX 约束名; -- 约束名默认是字段名
```


## 三、数据操作（DML）
### 1. 插入数据
```sql
-- 指定字段插入
INSERT INTO 表名 (字段1, 字段2, ...) VALUES (值1, 值2, ...);

-- 全字段插入（字段顺序与表结构一致）
INSERT INTO 表名 VALUES (值1, 值2, ...);

-- 批量插入
INSERT INTO 表名 (字段1, 字段2, ...) 
VALUES (值1-1, 值1-2, ...), (值2-1, 值2-2, ...), ...;
```

### 2. 更新数据
```sql
-- 条件更新（必须加WHERE，否则更新全表）
UPDATE 表名 SET 字段1=值1, 字段2=值2, ... WHERE 条件;
```

### 3. 删除数据
```sql
-- 条件删除
DELETE FROM 表名 WHERE 条件;

-- 清空表（无法回滚，比DELETE快）
TRUNCATE TABLE 表名;
```


## 四、数据查询（DQL）
### 1. 基础查询
```sql
-- 查询指定字段
SELECT 字段1, 字段2, ... FROM 表名;

-- 查询所有字段（不推荐生产环境）
SELECT * FROM 表名;

-- 字段起别名
SELECT 字段1 AS 别名1, 字段2 别名2 FROM 表名; -- AS可省略

-- 去重查询
SELECT DISTINCT 字段 FROM 表名;
```

### 2. 条件查询（WHERE）
```sql
-- 比较条件：=, !=, >, >=, <, <=
SELECT * FROM 表名 WHERE 字段 = 值;

-- 范围条件：BETWEEN...AND, IN
SELECT * FROM 表名 WHERE 字段 BETWEEN 最小值 AND 最大值;
SELECT * FROM 表名 WHERE 字段 IN (值1, 值2, ...);

-- 模糊查询：LIKE（%匹配任意字符，_匹配单个字符）
SELECT * FROM 表名 WHERE 字段 LIKE '%关键词%'; -- 包含关键词
SELECT * FROM 表名 WHERE 字段 LIKE '关键词_'; -- 关键词后跟1个字符

-- 空值判断：IS NULL, IS NOT NULL
SELECT * FROM 表名 WHERE 字段 IS NULL;

-- 逻辑条件：AND（且）, OR（或）, NOT（非）
SELECT * FROM 表名 WHERE 条件1 AND 条件2;
```

### 3. 排序与分页
```sql
-- 排序：ORDER BY 字段 [ASC（升序，默认）| DESC（降序）]
SELECT * FROM 表名 ORDER BY 字段1 DESC, 字段2 ASC; -- 先按字段1降序，再按字段2升序

-- 分页：LIMIT 偏移量, 条数（偏移量从0开始）
SELECT * FROM 表名 LIMIT 0, 10; -- 第1页，10条数据
SELECT * FROM 表名 LIMIT 10 OFFSET 10; -- MySQL8.0+，第2页，10条数据（偏移量10）
```

### 4. 聚合与分组
```sql
-- 聚合函数：COUNT(字段)（计数）、SUM(字段)（求和）、AVG(字段)（平均）、MAX(字段)（最大）、MIN(字段)（最小）
SELECT COUNT(*) 总条数, SUM(金额) 总金额 FROM 表名;

-- 分组：GROUP BY（分组字段），HAVING（分组后过滤，替代WHERE）
SELECT 分组字段, COUNT(*) 数量 FROM 表名 GROUP BY 分组字段 HAVING 数量 > 1;
```

### 5. 多表关联查询（JOIN）
```sql
-- 内连接：只查两表匹配的数据
SELECT a.字段, b.字段 FROM 表A a INNER JOIN 表B b ON a.关联字段 = b.关联字段;

-- 左连接：查左表所有数据，右表匹配不到为NULL
SELECT a.字段, b.字段 FROM 表A a LEFT JOIN 表B b ON a.关联字段 = b.关联字段;

-- 右连接：查右表所有数据，左表匹配不到为NULL
SELECT a.字段, b.字段 FROM 表A a RIGHT JOIN 表B b ON a.关联字段 = b.关联字段;
```


## 五、索引操作
```sql
-- 创建普通索引
CREATE INDEX 索引名 ON 表名 (字段名);

-- 创建唯一索引
CREATE UNIQUE INDEX 索引名 ON 表名 (字段名);

-- 创建联合索引（多字段）
CREATE INDEX 索引名 ON 表名 (字段1, 字段2, ...);

-- 查看表所有索引
SHOW INDEX FROM 表名;

-- 删除索引
DROP INDEX 索引名 ON 表名;
```


## 六、事务操作
```sql
-- 开启事务
START TRANSACTION; 或 BEGIN;

-- 提交事务（成功则生效）
COMMIT;

-- 回滚事务（失败则撤销操作）
ROLLBACK;

-- 保存点（部分回滚）
SAVEPOINT 保存点名; -- 设置保存点
ROLLBACK TO SAVEPOINT 保存点名; -- 回滚到保存点
```


## 七、视图操作
```sql
-- 创建视图
CREATE VIEW 视图名 AS SELECT 字段1, 字段2 FROM 表名 [WHERE 条件];

-- 查看视图
SELECT * FROM 视图名;

-- 修改视图
ALTER VIEW 视图名 AS 新查询语句;

-- 删除视图
DROP VIEW 视图名;
```


## 八、常用函数
### 1. 字符串函数
```sql
CONCAT(字符串1, 字符串2) -- 拼接字符串
LENGTH(字符串) -- 获取字节长度
CHAR_LENGTH(字符串) -- 获取字符长度
UPPER(字符串) -- 转大写
LOWER(字符串) -- 转小写
SUBSTRING(字符串, 起始位置, 长度) -- 截取字符串（起始从1开始）
```

### 2. 日期时间函数
```sql
NOW() -- 获取当前日期时间（YYYY-MM-DD HH:MM:SS）
CURDATE() -- 获取当前日期（YYYY-MM-DD）
CURTIME() -- 获取当前时间（HH:MM:SS）
DATE_FORMAT(日期字段, '%Y-%m-%d') -- 日期格式化
DATEDIFF(日期1, 日期2) -- 计算两个日期差值（天数）
```

### 3. 数值函数
```sql
ROUND(数值, 小数位数) -- 四舍五入
FLOOR(数值) -- 向下取整
CEIL(数值) -- 向上取整
MOD(数值1, 数值2) -- 取余
```


---

要不要我帮你整理一份**MySQL语法速查PDF版**？把这些核心语法按使用场景分类排版，方便你离线查阅和打印。