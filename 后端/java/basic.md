# Java 详细全面讲解（含示例代码）

---

## 一、Java 简介

### 1.1 什么是 Java？
Java 是由 Sun Microsystems（现为 Oracle 公司）于 1995 年发布的一种**面向对象的、跨平台的、强类型编程语言**。它具有“一次编写，到处运行”（Write Once, Run Anywhere）的特点。

### 1.2 Java 的特点
- **跨平台性**：通过 JVM（Java 虚拟机）实现。
- **面向对象**：支持封装、继承、多态。
- **自动内存管理**：垃圾回收机制（GC）。
- **安全性高**：无指针操作，防止内存泄漏。
- **多线程支持**：内置多线程机制。
- **丰富的类库**：JDK 提供大量 API。

---

## 二、Java 开发环境搭建

### 2.1 安装步骤
1. 下载并安装 JDK（Java Development Kit）
   - 推荐使用 **JDK 17** 或 **JDK 21**（LTS 版本）
2. 配置环境变量：
   - `JAVA_HOME`：指向 JDK 安装路径
   - `PATH`：添加 `%JAVA_HOME%\bin`
3. 验证安装：
```bash
java -version
javac -version
```

### 2.2 编写第一个 Java 程序

```java
// HelloWorld.java
public class HelloWorld {
    public static void main(String[] args) {
        System.out.println("Hello, World!");
    }
}
```

编译与运行：
```bash
javac HelloWorld.java
java HelloWorld
```

输出：
```
Hello, World!
```

---

## 三、Java 基础语法

### 3.1 标识符与关键字

- **标识符规则**：
  - 字母、数字、下划线、$ 组成，不能以数字开头
  - 区分大小写
  - 不能是关键字

- **常见关键字**（部分）：
  `public`, `class`, `static`, `void`, `int`, `if`, `else`, `for`, `while`, `new`, `return`

---

### 3.2 数据类型

| 类型 | 分类 | 示例 |
|------|------|------|
| 基本数据类型 | 整数型 | `byte`, `short`, `int`, `long` |
|               | 浮点型 | `float`, `double` |
|               | 字符型 | `char` |
|               | 布尔型 | `boolean` |
| 引用数据类型 | 类、接口、数组、字符串等 | `String`, `Object` |

#### 示例：变量声明与初始化

```java
public class DataTypeDemo {
    public static void main(String[] args) {
        // 整数类型
        int age = 25;
        long bigNumber = 1000000000L;

        // 浮点类型
        double price = 19.99;
        float weight = 65.5f;

        // 字符类型
        char grade = 'A';

        // 布尔类型
        boolean isPass = true;

        // 字符串（引用类型）
        String name = "张三";

        System.out.println("姓名：" + name);
        System.out.println("年龄：" + age);
        System.out.println("成绩等级：" + grade);
        System.out.println("是否及格：" + isPass);
    }
}
```

---

### 3.3 运算符

| 类型 | 运算符 |
|------|--------|
| 算术运算符 | `+`, `-`, `*`, `/`, `%`, `++`, `--` |
| 关系运算符 | `==`, `!=`, `>`, `<`, `>=`, `<=` |
| 逻辑运算符 | `&&`, `||`, `!` |
| 赋值运算符 | `=`, `+=`, `-=` |
| 三元运算符 | `条件 ? 值1 : 值2` |

#### 示例：运算符使用

```java
public class OperatorDemo {
    public static void main(String[] args) {
        int a = 10, b = 3;

        System.out.println("a + b = " + (a + b));
        System.out.println("a % b = " + (a % b));

        boolean result = (a > 5) && (b < 5);
        System.out.println("逻辑与结果：" + result);

        int max = (a > b) ? a : b;
        System.out.println("最大值：" + max);
    }
}
```

---

### 3.4 控制流程语句

#### 1. 条件语句

```java
public class IfElseDemo {
    public static void main(String[] args) {
        int score = 85;

        if (score >= 90) {
            System.out.println("优秀");
        } else if (score >= 80) {
            System.out.println("良好");
        } else if (score >= 60) {
            System.out.println("及格");
        } else {
            System.out.println("不及格");
        }
    }
}
```

#### 2. switch 语句

```java
public class SwitchDemo {
    public static void main(String[] args) {
        int day = 3;
        String dayName;

        switch (day) {
            case 1:
                dayName = "星期一";
                break;
            case 2:
                dayName = "星期二";
                break;
            case 3:
                dayName = "星期三";
                break;
            default:
                dayName = "未知";
        }

        System.out.println("今天是：" + dayName);
    }
}
```

#### 3. 循环语句

##### for 循环

```java
public class ForLoopDemo {
    public static void main(String[] args) {
        for (int i = 1; i <= 5; i++) {
            System.out.println("第 " + i + " 次循环");
        }
    }
}
```

##### while 循环

```java
public class WhileLoopDemo {
    public static void main(String[] args) {
        int i = 1;
        while (i <= 5) {
            System.out.println("当前数值：" + i);
            i++;
        }
    }
}
```

##### do-while 循环

```java
public class DoWhileDemo {
    public static void main(String[] args) {
        int i = 1;
        do {
            System.out.println("执行第 " + i + " 次");
            i++;
        } while (i <= 3);
    }
}
```

---

## 四、面向对象编程（OOP）

Java 是纯粹的面向对象语言（除了基本数据类型）。

### 4.1 类与对象

- **类（Class）**：模板或蓝图
- **对象（Object）**：类的实例

#### 示例：定义一个学生类

```java
// Student.java
public class Student {
    // 属性（成员变量）
    private String name;
    private int age;
    private String major;

    // 构造方法
    public Student() {}

    public Student(String name, int age, String major) {
        this.name = name;
        this.age = age;
        this.major = major;
    }

    // Getter 和 Setter 方法
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        if (age > 0) {
            this.age = age;
        } else {
            System.out.println("年龄必须大于0！");
        }
    }

    public String getMajor() {
        return major;
    }

    public void setMajor(String major) {
        this.major = major;
    }

    // 成员方法
    public void study() {
        System.out.println(name + " 正在学习 " + major);
    }

    @Override
    public String toString() {
        return "Student{name='" + name + "', age=" + age + ", major='" + major + "'}";
    }
}
```

#### 使用 Student 类

```java
// TestStudent.java
public class TestStudent {
    public static void main(String[] args) {
        // 创建对象
        Student s1 = new Student();
        s1.setName("李四");
        s1.setAge(20);
        s1.setMajor("计算机科学");

        Student s2 = new Student("王五", 22, "数学");

        // 调用方法
        s1.study();
        s2.study();

        // 输出对象信息
        System.out.println(s1);
        System.out.println(s2);
    }
}
```

---

### 4.2 封装（Encapsulation）

- 使用 `private` 修饰属性
- 提供公共的 getter/setter 方法
- 保护数据安全

> ✅ 上面的 `Student` 类已经体现了封装。

---

### 4.3 继承（Inheritance）

子类可以继承父类的属性和方法。

```java
// Person.java（父类）
public class Person {
    protected String name;
    protected int age;

    public void eat() {
        System.out.println(name + " 正在吃饭。");
    }

    public void sleep() {
        System.out.println(name + " 正在睡觉。");
    }
}

// Teacher.java（子类）
public class Teacher extends Person {
    private String subject;

    public void teach() {
        System.out.println(name + " 正在讲授 " + subject);
    }

    // 重写父类方法
    @Override
    public void sleep() {
        System.out.println(name + " 因为备课太累，睡得很香...");
    }
}

// TestInheritance.java
public class TestInheritance {
    public static void main(String[] args) {
        Teacher t = new Teacher();
        t.name = "刘老师";  // protected 可访问
        t.age = 35;
        t.subject = "Java 编程";

        t.eat();
        t.sleep();
        t.teach();
    }
}
```

---

### 4.4 多态（Polymorphism）

同一个行为具有多个不同表现形式。

```java
// Animal.java
abstract class Animal {
    abstract void makeSound();
}

// Dog.java
class Dog extends Animal {
    @Override
    void makeSound() {
        System.out.println("汪汪汪！");
    }
}

// Cat.java
class Cat extends Animal {
    @Override
    void makeSound() {
        System.out.println("喵喵喵！");
    }
}

// TestPolymorphism.java
public class TestPolymorphism {
    public static void main(String[] args) {
        Animal dog = new Dog();  // 向上转型
        Animal cat = new Cat();

        dog.makeSound();  // 输出：汪汪汪！
        cat.makeSound();  // 输出：喵喵喵！

        // 多态数组
        Animal[] animals = {new Dog(), new Cat()};
        for (Animal a : animals) {
            a.makeSound();
        }
    }
}
```

---

### 4.5 抽象类与接口

#### 抽象类（Abstract Class）

```java
abstract class Shape {
    abstract double area();  // 抽象方法
}

class Circle extends Shape {
    private double radius;

    public Circle(double radius) {
        this.radius = radius;
    }

    @Override
    double area() {
        return Math.PI * radius * radius;
    }
}
```

#### 接口（Interface）

```java
interface Flyable {
    void fly();  // 默认 public abstract
}

interface Swimmable {
    void swim();
}

class Duck implements Flyable, Swimmable {
    @Override
    public void fly() {
        System.out.println("鸭子飞起来了！");
    }

    @Override
    public void swim() {
        System.out.println("鸭子在游泳！");
    }
}
```

> 💡 **区别**：
> - 类只能单继承，但可实现多个接口
> - 接口中不能有具体实现（Java 8+ 支持默认方法）

---

## 五、常用类库

### 5.1 String 类

```java
public class StringDemo {
    public static void main(String[] args) {
        String str = "Hello Java";
        
        System.out.println(str.length());
        System.out.println(str.toUpperCase());
        System.out.println(str.substring(0, 5));
        System.out.println(str.contains("Java"));
        
        // 字符串不可变，使用 StringBuilder 提高性能
        StringBuilder sb = new StringBuilder();
        sb.append("Hello").append(" ").append("World");
        System.out.println(sb.toString());
    }
}
```

---

### 5.2 集合框架（Collections）

```java
import java.util.*;

public class CollectionDemo {
    public static void main(String[] args) {
        // List：有序可重复
        List<String> list = new ArrayList<>();
        list.add("Apple");
        list.add("Banana");
        list.add("Apple");
        System.out.println("List: " + list);

        // Set：无序不重复
        Set<Integer> set = new HashSet<>();
        set.add(1);
        set.add(2);
        set.add(1);
        System.out.println("Set: " + set);

        // Map：键值对
        Map<String, Integer> map = new HashMap<>();
        map.put("语文", 90);
        map.put("数学", 95);
        System.out.println("Map: " + map);

        // 遍历 Map
        for (Map.Entry<String, Integer> entry : map.entrySet()) {
            System.out.println(entry.getKey() + ": " + entry.getValue());
        }
    }
}
```

---

### 5.3 异常处理

```java
public class ExceptionDemo {
    public static void main(String[] args) {
        try {
            int result = 10 / 0;
        } catch (ArithmeticException e) {
            System.out.println("发生算术异常：" + e.getMessage());
        } finally {
            System.out.println("finally 块总会执行");
        }

        // 自定义异常示例
        try {
            validateAge(-5);
        } catch (InvalidAgeException e) {
            System.out.println("错误：" + e.getMessage());
        }
    }

    public static void validateAge(int age) throws InvalidAgeException {
        if (age < 0) {
            throw new InvalidAgeException("年龄不能为负数");
        }
    }
}

// 自定义异常类
class InvalidAgeException extends Exception {
    public InvalidAgeException(String message) {
        super(message);
    }
}
```

---

### 5.4 输入输出（I/O）

```java
import java.io.*;

public class FileIODemo {
    public static void main(String[] args) {
        String fileName = "test.txt";

        // 写入文件
        try (PrintWriter writer = new PrintWriter(new FileWriter(fileName))) {
            writer.println("这是第一行");
            writer.println("这是第二行");
            System.out.println("数据已写入文件");
        } catch (IOException e) {
            e.printStackTrace();
        }

        // 读取文件
        try (BufferedReader reader = new BufferedReader(new FileReader(fileName))) {
            String line;
            while ((line = reader.readLine()) != null) {
                System.out.println(line);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```

---

## 六、多线程编程

```java
// 方式1：继承 Thread 类
class MyThread extends Thread {
    @Override
    public void run() {
        for (int i = 1; i <= 3; i++) {
            System.out.println(Thread.currentThread().getName() + ": 执行第 " + i + " 次");
            try {
                Thread.sleep(500);  // 暂停 500ms
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}

// 方式2：实现 Runnable 接口（推荐）
class Task implements Runnable {
    @Override
    public void run() {
        System.out.println("任务正在执行...");
    }
}

// 测试多线程
public class ThreadDemo {
    public static void main(String[] args) {
        // 方法1
        MyThread t1 = new MyThread();
        t1.start();

        // 方法2
        Thread t2 = new Thread(new Task());
        t2.start();

        // 主线程
        System.out.println("主线程结束");
    }
}
```

---

## 七、Java 新特性（简要）

| 版本 | 重要特性 |
|------|----------|
| Java 8 | Lambda 表达式、Stream API、Optional、函数式接口 |
| Java 9 | 模块化系统（Jigsaw） |
| Java 17 | Sealed Classes、Pattern Matching |
| Java 21 | Virtual Threads（预览）、Record Classes |

#### 示例：Lambda 与 Stream（Java 8+）

```java
import java.util.Arrays;
import java.util.List;

public class LambdaStreamDemo {
    public static void main(String[] args) {
        List<String> names = Arrays.asList("Alice", "Bob", "Charlie", "David");

        // 使用 Lambda 和 Stream 过滤并打印
        names.stream()
             .filter(name -> name.length() > 4)
             .forEach(System.out::println);  // 方法引用
    }
}
```

---

## 八、总结

| 主题 | 内容 |
|------|------|
| 核心概念 | 面向对象、JVM、GC、跨平台 |
| 基础语法 | 变量、运算符、控制流 |
| OOP 三大特性 | 封装、继承、多态 |
| 关键技术 | 异常处理、集合、I/O、多线程 |
| 最佳实践 | 使用接口、避免空指针、资源关闭（try-with-resources） |

---

## 九、学习建议

1. **动手实践**：每学一个知识点都写代码验证
2. **阅读官方文档**：[Oracle Java Documentation](https://docs.oracle.com/en/java/)
3. **使用 IDE**：IntelliJ IDEA 或 Eclipse
4. **学习框架前先掌握基础**
5. **参与开源项目或刷题（如 LeetCode）**

---

✅ **结语**：Java 是一门成熟、稳定、应用广泛的编程语言，广泛用于企业级开发（Spring）、Android 应用、大数据（Hadoop）、Web 后端等领域。打好基础是迈向高级开发的第一步！

如果你希望了解某个具体方向（如 Spring、JDBC、网络编程等），欢迎继续提问！