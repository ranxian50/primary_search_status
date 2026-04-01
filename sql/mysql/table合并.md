在 MySQL 中，“合并表格”通常指将多个结构相同或相似的表中的数据合并到一个表中（可能是新表或已有表），或通过查询将多个表的数据联合展示。根据场景不同，合并方式分为**数据合并（物理合并，将数据写入目标表）** 和**查询合并（逻辑合并，仅在查询结果中展示合并数据）**。


### 一、前提条件
合并表格的核心是确保参与合并的表**结构兼容**：
- 字段数量、字段名、数据类型需一致（至少用于合并的核心字段需匹配），否则可能导致数据丢失或错误。
- 若结构不同，需先通过 `ALTER TABLE` 调整表结构（如添加/删除字段、修改数据类型），使其一致后再合并。


### 二、查询合并（逻辑合并，不修改原表数据）
通过 SQL 查询将多个表的结果“临时合并”展示，不改变原表数据，适合需要一次性查看多表数据的场景。核心语法是 `UNION` 和 `UNION ALL`。

#### 1. `UNION`：合并结果并去重
`UNION` 会将多个 `SELECT` 语句的结果合并，并自动去除重复记录（需所有 `SELECT` 语句的字段数量和类型一致）。

**示例**：  
假设有两个表 `sales_2023` 和 `sales_2024`，结构相同（含 `id`、`product`、`amount` 字段），合并查询所有销售记录：
```sql
-- 合并 2023 和 2024 年的销售数据，自动去重
SELECT id, product, amount FROM sales_2023
UNION
SELECT id, product, amount FROM sales_2024;
```

#### 2. `UNION ALL`：合并结果但不去重
`UNION ALL` 仅合并结果，保留重复记录（效率比 `UNION` 高，因为无需去重），适合确认无重复数据或需要保留重复数据的场景。

**示例**：  
```sql
-- 合并数据，保留重复记录
SELECT id, product, amount FROM sales_2023
UNION ALL
SELECT id, product, amount FROM sales_2024;
```

#### 注意事项：
- 所有 `SELECT` 语句的字段顺序、数量、数据类型必须一致，否则会报错。
- 可对合并后的结果进行排序或筛选，例如：
  ```sql
  (SELECT id, product, amount FROM sales_2023)
  UNION ALL
  (SELECT id, product, amount FROM sales_2024)
  ORDER BY amount DESC; -- 按金额降序排列合并结果
  ```


### 三、数据合并（物理合并，将数据写入目标表）
将多个表的数据**永久合并**到一个目标表（新建表或已有表），会修改目标表的数据。常见场景：分表数据汇总（如按年份分表的销售数据合并为总表）。

#### 1. 合并到新表（创建并写入数据）
通过 `CREATE TABLE ... SELECT` 语句，创建一个新表，并将多个表的数据合并写入新表。

**示例**：  
创建 `sales_all` 表，合并 `sales_2023` 和 `sales_2024` 的数据：
```sql
-- 创建新表 sales_all，并插入合并后的数据（包括重复记录）
CREATE TABLE sales_all AS
SELECT id, product, amount FROM sales_2023
UNION ALL
SELECT id, product, amount FROM sales_2024;
```

- 新表 `sales_all` 的结构会自动匹配 `SELECT` 语句的字段（字段名、数据类型）。
- 若需去重，将 `UNION ALL` 改为 `UNION`。


#### 2. 合并到已有表（向现有表插入数据）
若目标表已存在（结构与源表一致），使用 `INSERT INTO ... SELECT` 语句将其他表的数据插入目标表。

**示例**：  
假设已存在 `sales_all` 表，向其插入 `sales_2023` 和 `sales_2024` 的数据：
```sql
-- 先插入 2023 年数据
INSERT INTO sales_all (id, product, amount)
SELECT id, product, amount FROM sales_2023;

-- 再插入 2024 年数据
INSERT INTO sales_all (id, product, amount)
SELECT id, product, amount FROM sales_2024;
```

- 若目标表有主键或唯一约束，需确保插入的数据不违反约束（否则会报错），可通过 `IGNORE` 忽略冲突记录：
  ```sql
  -- 忽略重复主键的记录，仅插入无冲突的数据
  INSERT IGNORE INTO sales_all (id, product, amount)
  SELECT id, product, amount FROM sales_2024;
  ```

- 若需要替换冲突数据（如主键重复时更新其他字段），可使用 `REPLACE` 或 `INSERT ... ON DUPLICATE KEY UPDATE`：
  ```sql
  -- 主键重复时，更新 product 和 amount 字段
  INSERT INTO sales_all (id, product, amount)
  SELECT id, product, amount FROM sales_2024
  ON DUPLICATE KEY UPDATE 
    product = VALUES(product), 
    amount = VALUES(amount);
  ```


#### 3. 跨数据库合并表
若表位于不同数据库（同一 MySQL 服务器），只需在表名前加数据库名前缀即可。

**示例**：  
将 `db1.sales` 和 `db2.sales` 合并到 `db3.sales_all`：
```sql
-- 假设 db3.sales_all 已存在
INSERT INTO db3.sales_all (id, product, amount)
SELECT id, product, amount FROM db1.sales
UNION ALL
SELECT id, product, amount FROM db2.sales;
```


### 四、特殊场景处理
#### 1. 表结构不同的合并
若表结构不同（如字段数量、名称不同），需先通过字段映射或补充默认值，使 `SELECT` 语句的输出结构一致。

**示例**：  
`table_a` 含 `id, name, age`，`table_b` 含 `id, username, birth_year`，合并时统一为 `id, name, age`：
```sql
-- table_b 的 username 映射为 name，用当前年份 - birth_year 计算 age
SELECT id, name, age FROM table_a
UNION ALL
SELECT 
  id, 
  username AS name, 
  YEAR(CURDATE()) - birth_year AS age  -- 计算年龄
FROM table_b;
```

#### 2. 大表合并优化
若表数据量极大（百万级以上），直接合并可能导致锁表或超时，可分批插入：
```sql
-- 每次插入 1000 条，循环执行直到完成
INSERT INTO sales_all (id, product, amount)
SELECT id, product, amount FROM sales_2023
LIMIT 1000 OFFSET 0; -- 偏移量从 0 开始，每次增加 1000

INSERT INTO sales_all (id, product, amount)
SELECT id, product, amount FROM sales_2023
LIMIT 1000 OFFSET 1000;
-- ... 以此类推
```

#### 3. 合并后删除原表
若合并后原表不再需要，可删除以释放空间（谨慎操作！）：
```sql
DROP TABLE IF EXISTS sales_2023, sales_2024;
```


### 五、总结
MySQL 合并表格的核心方式：
| 场景                | 方法                  | 特点                                  |
|---------------------|-----------------------|---------------------------------------|
| 临时查看多表数据    | `UNION`/`UNION ALL`   | 不修改原表，支持去重/保留重复         |
| 永久合并到新表      | `CREATE TABLE ... SELECT` | 自动创建目标表，结构与查询结果一致    |
| 永久合并到已有表    | `INSERT INTO ... SELECT` | 需目标表结构兼容，支持冲突处理        |

合并前务必确认表结构兼容性，大表合并建议分批处理并备份数据，避免数据丢失或业务中断。