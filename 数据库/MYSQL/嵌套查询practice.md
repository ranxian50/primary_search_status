以下是完整的环境准备语句（表结构创建）和测试数据插入语句，可直接复制执行，用于前面嵌套查询示例的实际操作：


### 一、创建数据库和表结构
```sql
-- 创建测试数据库（指定字符集支持中文）
CREATE DATABASE IF NOT EXISTS nested_query_demo 
CHARACTER SET utf8mb4 
COLLATE utf8mb4_unicode_ci;

-- 切换到该数据库
USE nested_query_demo;

-- 创建用户表（user）
CREATE TABLE IF NOT EXISTS user (
  id INT PRIMARY KEY AUTO_INCREMENT COMMENT '用户ID',
  username VARCHAR(50) NOT NULL UNIQUE COMMENT '用户名',
  age INT NOT NULL COMMENT '年龄',
  dept_id INT NOT NULL COMMENT '部门ID'
) COMMENT '用户表';

-- 创建订单表（order）
CREATE TABLE IF NOT EXISTS `order` (  -- 用反引号包裹，避免与关键字冲突
  id INT PRIMARY KEY AUTO_INCREMENT COMMENT '订单ID',
  user_id INT NOT NULL COMMENT '用户ID（关联user.id）',
  amount DECIMAL(10,2) NOT NULL COMMENT '订单金额',
  create_time DATETIME NOT NULL COMMENT '创建时间',
  FOREIGN KEY (user_id) REFERENCES user(id)  -- 外键关联用户表
) COMMENT '订单表';
```


### 二、插入测试数据
```sql
-- 向用户表插入数据（含不同部门、年龄的用户）
INSERT INTO user (username, age, dept_id) VALUES
('张三', 25, 1),   -- 部门1
('李四', 30, 1),   -- 部门1
('王五', 35, 1),   -- 部门1
('赵六', 28, 2),   -- 部门2
('孙七', 40, 2),   -- 部门2
('周八', 22, 3),   -- 部门3（无订单）
('吴九', 33, 3);   -- 部门3

-- 向订单表插入数据（关联用户ID，含不同金额和时间）
INSERT INTO `order` (user_id, amount, create_time) VALUES
-- 张三的订单
(1, 800.00, '2023-05-10 14:30:00'),
(1, 1200.00, '2023-06-15 09:45:00'),
-- 李四的订单
(2, 500.00, '2023-03-20 16:20:00'),
(2, 2000.00, '2023-11-01 11:10:00'),
-- 王五的订单
(3, 1500.00, '2023-09-05 10:00:00'),
-- 赵六的订单（2022年数据）
(4, 300.00, '2022-12-25 18:30:00'),
-- 孙七的订单
(5, 900.00, '2023-07-08 13:50:00'),
(5, 1800.00, '2023-08-22 15:15:00');
```


### 数据说明
1. **用户表（user）** 共7条数据，分布在3个部门：
   - 部门1：张三（25）、李四（30）、王五（35）
   - 部门2：赵六（28）、孙七（40）
   - 部门3：周八（22）、吴九（33）（其中周八无订单）

2. **订单表（order）** 共8条数据：
   - 包含2022年和2023年的订单
   - 金额从300到2000不等（部分订单金额>1000）
   - 赵六只有2022年的订单，无2023年订单

执行以上语句后，即可基于真实数据测试前面所有嵌套查询示例，验证查询结果是否符合预期。








以下是一份份覆盖嵌套查询核心知识的示例集，包含不同位置、类型的嵌套查询场景，每个示例均标注关键点和适用场景：


### 环境准备
假设有2张表：
1. `user`（用户表）：`id`(主键)、`username`(用户名)、`age`(年龄)、`dept_id`(部门ID)
2. `order`（订单表）：`id`(主键)、`user_id`(用户ID，外键关联`user.id`)、`amount`(金额)、`create_time`(创建时间)


### 一、WHERE子句中的嵌套查询
#### 1. 单行子查询（返回1行1列，搭配`=、>、<`）
**需求**：查询与“张三”同年龄的所有用户（不含张三）
```sql
SELECT * FROM user
WHERE age = (
  SELECT age FROM user WHERE username = '张三'  -- 子查询返回单行单列（如25）
) 
AND username != '张三';  -- 排除张三自己
```
**关键点**：子查询结果必须唯一，否则报错；适用于“基于单值匹配”的场景。


#### 2. 多行子查询（返回多行1列，搭配`IN、NOT IN、ANY、ALL`）
**需求1（IN）**：查询有订单的用户信息
```sql
SELECT * FROM user
WHERE id IN (
  SELECT DISTINCT user_id FROM `order`  -- 子查询返回多行用户ID（去重）
);
```

**需求2（ANY）**：查询年龄大于“部门1”任意用户的用户
```sql
SELECT * FROM user
WHERE age > ANY (  -- 大于子查询结果中的最小值
  SELECT age FROM user WHERE dept_id = 1
);
```

**需求3（ALL）**：查询年龄大于“部门1”所有用户的用户
```sql
SELECT * FROM user
WHERE age > ALL (  -- 大于子查询结果中的最大值
  SELECT age FROM user WHERE dept_id = 1
);
```
**关键点**：`IN`匹配“任意一个”，`ANY`/`ALL`用于范围比较；适用于“多值匹配”场景。


### 二、FROM子句中的嵌套查询（派生表）
**需求1**：统计每个部门的平均年龄，并筛选出平均年龄>30的部门
```sql
SELECT dept_id, avg_age FROM (
  -- 子查询作为派生表（临时表），必须加别名t
  SELECT dept_id, AVG(age) AS avg_age FROM user GROUP BY dept_id
) AS t
WHERE avg_age > 30;  -- 主查询筛选派生表结果
```

**需求2**：查询2023年订单金额前10的用户及其订单信息
```sql
SELECT u.username, o.amount, o.create_time FROM user u
JOIN (
  -- 子查询：先筛选2023年订单并按金额排序取前10
  SELECT user_id, amount, create_time 
  FROM `order` 
  WHERE YEAR(create_time) = 2023 
  ORDER BY amount DESC 
  LIMIT 10
) AS o  -- 派生表别名o
ON u.id = o.user_id;  -- 关联用户表
```
**关键点**：派生表必须加别名；适用于“需先处理数据再关联/筛选”的场景。


### 三、SELECT子句中的嵌套查询（字段级子查询）
**需求**：查询每个用户的用户名、年龄，以及该用户的最大订单金额（无订单则为NULL）
```sql
SELECT 
  username,
  age,
  -- 子查询依赖主查询的u.id，每行触发一次（相关子查询）
  (SELECT MAX(amount) FROM `order` o WHERE o.user_id = u.id) AS max_amount
FROM user u;
```
**关键点**：子查询需返回单行单列，且通常是相关子查询（依赖主表字段）；适用于“为每行数据添加一个计算字段”的场景。


### 四、EXISTS子查询（存在性判断）
**需求1（EXISTS）**：查询有“金额>1000”订单的用户
```sql
SELECT * FROM user u
WHERE EXISTS (
  -- 子查询判断是否存在满足条件的订单（相关子查询，依赖u.id）
  SELECT 1 FROM `order` o  -- 用1代替*，效率更高（EXISTS不关心具体值）
  WHERE o.user_id = u.id AND o.amount > 1000
);
```

**需求2（NOT EXISTS）**：查询没有任何订单的用户
```sql
SELECT * FROM user u
WHERE NOT EXISTS (
  SELECT 1 FROM `order` o WHERE o.user_id = u.id
);
```
**关键点**：`EXISTS`只关心“是否存在记录”，找到1条就停止查询，效率高于`IN`（尤其子查询结果量大时）；适用于“存在性判断”场景。


### 五、HAVING子句中的嵌套查询（分组后筛选）
**需求**：查询用户数超过“全公司平均部门人数”的部门
```sql
SELECT dept_id, COUNT(*) AS user_count FROM user
GROUP BY dept_id
HAVING COUNT(*) > (
  -- 子查询：先算每个部门的人数，再求平均值（子查询嵌套）
  SELECT AVG(dept_user_count) FROM (
    SELECT COUNT(*) AS dept_user_count FROM user GROUP BY dept_id
  ) AS t  -- 内层子查询的派生表
);
```
**关键点**：用于“分组后的数据需要和全局计算结果比较”的场景，支持多层嵌套。


### 六、相关子查询 vs 非相关子查询对比
#### 1. 非相关子查询（独立执行）
```sql
-- 子查询不依赖主查询，先执行子查询，再执行主查询
SELECT * FROM user
WHERE dept_id = (SELECT dept_id FROM user WHERE username = '张三');
```

#### 2. 相关子查询（依赖主查询）
```sql
-- 主查询每行的u.id会传给子查询，子查询随主查询逐行执行
SELECT u.username FROM user u
WHERE EXISTS (
  SELECT 1 FROM `order` o WHERE o.user_id = u.id
);
```


### 七、嵌套查询优化示例（JOIN替代相关子查询）
**场景**：查询每个用户的最新订单（原相关子查询效率低）
```sql
-- 低效写法（相关子查询，每行触发一次）
SELECT 
  u.username,
  (SELECT order_no FROM `order` o WHERE o.user_id = u.id ORDER BY create_time DESC LIMIT 1) AS latest_order
FROM user u;

-- 优化写法（JOIN替代，一次关联）
SELECT u.username, o.order_no AS latest_order
FROM user u
LEFT JOIN `order` o ON u.id = o.user_id
-- 子查询：先获取每个用户的最新订单ID
WHERE o.id IN (
  SELECT MAX(id) FROM `order` GROUP BY user_id  -- 假设id自增，MAX(id)即最新
);
```


### 总结
通过以上示例可覆盖嵌套查询的核心知识点：  
- 按位置：`WHERE`（单行/多行）、`FROM`（派生表）、`SELECT`（字段级）、`HAVING`、`EXISTS`  
- 按依赖关系：相关子查询（依赖主表）、非相关子查询（独立）  
- 优化原则：`EXISTS`替代`IN`、`JOIN`替代相关子查询、派生表加别名  

实际开发中需根据数据量和业务场景选择合适的嵌套方式，优先保证可读性和效率。