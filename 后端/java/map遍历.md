以下是为所有遍历方式代码添加**详细注释**后的版本，注释覆盖代码逻辑、API 用途、注意事项等核心信息：

```java
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.stream.Stream;

/**
 * Map 遍历的全场景示例（带详细注释）
 * 核心知识点：
 * 1. Map 存储键值对，遍历核心是获取 keySet/values/entrySet
 * 2. 不同遍历方式适用于不同场景（简洁性、兼容性、删除操作、复杂处理）
 * 3. 遍历中修改/删除元素的安全规范
 */
public class MapTraversalComplete {
    public static void main(String[] args) {
        // ===================== 1. 初始化测试 Map =====================
        // 创建 HashMap 实例（非线程安全，允许键/值为null）
        Map<String, Integer> demoMap = new HashMap<>();
        // 向 Map 中添加键值对：key=字符串类型，value=整数类型
        demoMap.put("Apple", 10);    // 键：Apple，值：10
        demoMap.put("Banana", 20);   // 键：Banana，值：20
        demoMap.put("Cherry", 30);   // 键：Cherry，值：30
        demoMap.put("Durian", null); // 测试值为null的场景
        demoMap.put(null, 40);       // 测试键为null的场景（HashMap允许）

        // ===================== 2. 方式1：Java8+ 首选 - forEach + Lambda =====================
        System.out.println("===== 方式1：forEach + Lambda（Java8+ 简洁版） =====");
        /**
         * Map.forEach(Consumer)：Java8新增默认方法，直接遍历键值对
         * 入参是BiConsumer函数式接口，Lambda表达式简化写法：(key, value) -> {逻辑}
         * 优点：代码最简洁，无需手动获取entry/迭代器
         * 缺点：不兼容Java8以下版本；遍历中删除元素会抛ConcurrentModificationException
         */
        demoMap.forEach((key, value) -> {
            // 遍历过程中判空（避免NullPointerException，因为HashMap允许键/值为null）
            String safeKey = (key == null) ? "NULL_KEY" : key;
            Integer safeValue = (value == null) ? 0 : value;
            // 打印键值对
            System.out.println("键：" + safeKey + "，值：" + safeValue);

            // 【可选】修改值：通过map.put()（key不可修改，Map的key是不可变标识）
            // 注意：此处修改会直接影响原Map，且forEach中修改值是安全的（删除/添加不安全）
            if ("Apple".equals(safeKey)) {
                demoMap.put(key, safeValue + 5); // Apple的值从10改为15
            }
        });

        // 仅遍历键（keySet()返回所有键的集合）
        System.out.println("\n===== 仅遍历键 =====");
        demoMap.keySet().forEach(key -> {
            String safeKey = (key == null) ? "NULL_KEY" : key;
            System.out.println("键：" + safeKey);
        });

        // 仅遍历值（values()返回所有值的集合）
        System.out.println("\n===== 仅遍历值 =====");
        demoMap.values().forEach(value -> {
            Integer safeValue = (value == null) ? 0 : value;
            System.out.println("值：" + safeValue);
        });

        // ===================== 3. 方式2：经典兼容版 - entrySet() + 增强for循环 =====================
        System.out.println("\n===== 方式2：entrySet() + 增强for循环（兼容所有Java版本） =====");
        /**
         * entrySet()：返回Map中所有键值对（Map.Entry）的集合，是效率最高的传统遍历方式
         * 原因：一次获取所有键值对，无需通过key重复调用get(key)（避免哈希计算/红黑树遍历）
         * Map.Entry：表示一个键值对对象，提供getKey()/getValue()/setValue()方法
         */
        for (Map.Entry<String, Integer> entry : demoMap.entrySet()) {
            // 获取当前键（可能为null，需判空）
            String key = entry.getKey();
            String safeKey = (key == null) ? "NULL_KEY" : key;

            // 获取当前值（可能为null，需判空）
            Integer value = entry.getValue();
            Integer safeValue = (value == null) ? 0 : value;

            System.out.println("键：" + safeKey + "，值：" + safeValue);

            // 【高效修改值】通过entry.setValue()直接修改，比map.put()更高效（无需重新定位key）
            if ("Banana".equals(safeKey)) {
                entry.setValue(safeValue + 10); // Banana的值从20改为30
            }
        }

        // ===================== 4. 方式3：安全删除版 - Iterator遍历（支持遍历中删除） =====================
        System.out.println("\n===== 方式3：Iterator遍历（遍历中安全删除元素） =====");
        /**
         * 场景：遍历过程中需要删除元素（增强for/forEach删除会抛ConcurrentModificationException）
         * 原理：Iterator是唯一支持遍历中安全删除的方式，通过iterator.remove()实现
         */
        // 获取entrySet的迭代器（泛型指定为Map.Entry，避免类型转换）
        Iterator<Map.Entry<String, Integer>> iterator = demoMap.entrySet().iterator();

        // hasNext()：判断是否还有下一个元素
        while (iterator.hasNext()) {
            // next()：获取下一个键值对元素（必须在hasNext()之后调用，否则抛NoSuchElementException）
            Map.Entry<String, Integer> entry = iterator.next();

            String key = entry.getKey();
            String safeKey = (key == null) ? "NULL_KEY" : key;
            Integer value = entry.getValue();
            Integer safeValue = (value == null) ? 0 : value;

            // 条件删除：删除值为null的元素（Durian对应的值为null）
            if (value == null) {
                // 安全删除：通过iterator.remove()，而非map.remove(key)
                // 注意：iterator.remove()必须在next()之后调用，且只能调用一次
                iterator.remove();
                System.out.println("已删除键：" + safeKey + "（值为null）");
            } else {
                System.out.println("键：" + safeKey + "，值：" + safeValue);
            }
        }

        // ===================== 5. 不推荐方式：keySet() + get(key) =====================
        System.out.println("\n===== 不推荐方式：keySet() + get(key) =====");
        /**
         * 缺点：效率低！每次get(key)都会重新计算key的哈希值（HashMap）或遍历红黑树（TreeMap）
         * 场景：仅适用于极简单、数据量极小的场景，实际开发中避免使用
         */
        for (String key : demoMap.keySet()) {
            String safeKey = (key == null) ? "NULL_KEY" : key;
            // 多次调用get(key)，性能损耗大
            Integer value = demoMap.get(key);
            Integer safeValue = (value == null) ? 0 : value;
            System.out.println("键：" + safeKey + "，值：" + safeValue);
        }

        // ===================== 6. 高级处理：Stream API遍历（Java8+） =====================
        System.out.println("\n===== 方式4：Stream API遍历（复杂过滤/映射） =====");
        /**
         * 场景：需要对键值对进行过滤、映射、排序等复杂操作
         * 核心：将entrySet转为Stream，通过流式操作简化复杂逻辑
         */
        demoMap.entrySet().stream()
                // 过滤条件1：键不为null（排除之前的null键）
                .filter(entry -> entry.getKey() != null)
                // 过滤条件2：值大于15（仅保留Banana(30)、Apple(15)中>15的元素）
                .filter(entry -> entry.getValue() != null && entry.getValue() > 15)
                // 遍历符合条件的元素
                .forEach(entry -> {
                    System.out.println("过滤后 - 键：" + entry.getKey() + "，值：" + entry.getValue());
                });
    }
}
```

### 注释核心说明：
1. **结构化注释**：按「场景分类+核心原理+优缺点」分层注释，便于理解每种方式的适用场景；
2. **异常防护**：重点标注 `null` 处理、`ConcurrentModificationException` 等常见异常的规避方法；
3. **性能说明**：明确标注 `entrySet()` 比 `keySet()+get(key)` 高效的原因，以及 `entry.setValue()` 比 `map.put()` 更优的逻辑；
4. **API 解析**：对 `forEach`、`entrySet()`、`Iterator` 等核心 API 的参数、返回值、调用规范做详细说明；
5. **注意事项**：在关键操作（如删除、修改）处标注「安全规范」，比如 `iterator.remove()` 必须在 `next()` 之后调用。

### 执行结果说明：
运行代码后会按以下顺序输出：
1. Lambda 遍历所有键值对（含 null 处理、值修改）；
2. 仅遍历键 → 仅遍历值；
3. 增强 for 循环遍历 entrySet（含高效修改值）；
4. Iterator 遍历并删除值为 null 的元素（Durian）；
5. 不推荐方式的遍历结果；
6. Stream 过滤后仅输出值 >15 的元素（Banana=30）。

该注释版本既保留了代码的完整性，又通过注释解释了「为什么这么写」「优缺点」「注意事项」，便于理解每个遍历方式的设计思路和适用场景。