# CSS 全面详解
CSS（Cascading Style Sheets，层叠样式表）是用于描述 HTML 或 XML 等文档呈现样式的语言，核心作用是将文档的**结构（HTML）** 与**表现（样式）** 分离，实现更灵活、高效的页面美化与布局控制。


## 一、CSS 基础核心
### 1. 引入方式（优先级：行内 > 内部 > 外部）
#### （1）外部样式表（推荐）
将 CSS 写在单独 `.css` 文件中，通过 `<link>` 标签引入 HTML，可实现样式复用。
```html
<!-- HTML 中引入 -->
<link rel="stylesheet" href="style.css">
```

#### （2）内部样式表
在 HTML 的 `<style>` 标签内写 CSS，仅作用于当前页面。
```html
<style>
  body { background: #fff; }
</style>
```

#### （3）行内样式
通过 HTML 元素的 `style` 属性直接写 CSS，仅作用于当前元素（耦合度高，慎用）。
```html
<div style="color: red; font-size: 16px;">行内样式</div>
```


### 2. 语法规则
CSS 由**选择器**和**声明块**组成，声明块包含多个 `属性: 值;` 键值对。
```css
/* 选择器 { 属性1: 值1; 属性2: 值2; } */
p {
  color: blue; /* 文本颜色 */
  font-size: 14px; /* 字体大小 */
}
```
- 注释：`/* 注释内容 */`（浏览器不解析）
- 单位：绝对单位（px）、相对单位（em/rem/vw/vh）等


### 3. 选择器（核心：精准选中要样式化的元素）
#### （1）基础选择器
| 选择器       | 语法       | 作用                     |
|--------------|------------|--------------------------|
| 元素选择器   | `div`      | 选中所有同名元素         |
| 类选择器     | `.box`     | 选中所有 `class="box"` 的元素 |
| ID 选择器    | `#header`  | 选中唯一 `id="header"` 的元素 |
| 通配符选择器 | `*`        | 选中页面所有元素         |
| 属性选择器   | `[type="text"]` | 选中带指定属性的元素 |

#### （2）复合选择器
- 后代选择器：`div p`（选中 div 内所有 p 元素）
- 子选择器：`div > p`（选中 div 直接子级的 p 元素）
- 相邻兄弟选择器：`div + p`（选中 div 后面紧邻的 p 元素）
- 并集选择器：`div, p`（选中所有 div 和 p 元素）
- 伪类选择器：`a:hover`（鼠标悬浮时的 a 元素）、`li:first-child`（第一个 li 元素）
- 伪元素选择器：`p::before`（在 p 元素前插入内容）、`p::after`（在 p 元素后插入内容）


### 4. 层叠与优先级
当多个样式作用于同一元素时，按以下规则生效：
1. **重要性**：`!important` 标记的样式（慎用，会打破优先级）
2. **优先级权重**（数值越大越优先）：
   - 行内样式：1000
   - ID 选择器：100
   - 类/伪类/属性选择器：10
   - 元素/伪元素选择器：1
3. **书写顺序**：权重相同时，后写的样式覆盖先写的
4. **继承性**：文本类属性（color、font-size）可继承，布局类属性（width、margin）不可继承


## 二、CSS 核心样式属性
### 1. 文本样式
- 颜色：`color: red;`（支持英文、十六进制 #ff0000、RGB rgb(255,0,0)、RGBA rgba(255,0,0,0.5)）
- 字体：`font-family: "微软雅黑", sans-serif;`（备选字体防止兼容问题）
- 字号：`font-size: 16px;`（默认 16px）
- 粗细：`font-weight: normal;`（normal/400、bold/700、100-900）
- 斜体：`font-style: italic;`（italic/oblique）
- 行高：`line-height: 1.5;`（无单位时为字号的 1.5 倍，常用于垂直居中）
- 文本对齐：`text-align: center;`（left/center/right/justify）
- 文本装饰：`text-decoration: none;`（none/underline/line-through，常用于清除 a 标签下划线）
- 文本缩进：`text-indent: 2em;`（首行缩进 2 个字符）


### 2. 背景样式
- 背景色：`background-color: #f5f5f5;`
- 背景图：`background-image: url("bg.jpg");`
- 背景重复：`background-repeat: no-repeat;`（repeat/repeat-x/repeat-y/no-repeat）
- 背景位置：`background-position: center center;`（方位词/百分比/像素值）
- 背景尺寸：`background-size: cover;`（cover：覆盖容器；contain：完整显示图片）
- 简写：`background: #f5f5f5 url("bg.jpg") no-repeat center center;`


### 3. 盒模型（页面布局的核心）
每个 HTML 元素都可视为一个“盒子”，由 4 部分组成（从内到外）：
- 内容区（content）：元素内容（文本、图片等），通过 `width`/`height` 设置大小
- 内边距（padding）：内容区与边框的距离，`padding: 上 右 下 左;`（可简写）
- 边框（border）：盒子的边框，`border: 1px solid #000;`（宽度 样式 颜色）
- 外边距（margin）：盒子与其他盒子的距离，`margin: 上 右 下 左;`（可简写）

#### 盒模型模式
- 标准盒模型（默认）：`box-sizing: content-box;` → 宽高 = 内容区宽高
- 怪异盒模型：`box-sizing: border-box;` → 宽高 = 内容区 + 内边距 + 边框（开发常用，避免计算麻烦）


### 4. 浮动（float）
用于实现元素“并排排列”，早期布局核心（现多被 Flex/Grid 替代）。
```css
.left { float: left; } /* 左浮动 */
.right { float: right; } /* 右浮动 */
```
#### 浮动问题：父元素高度塌陷（浮动元素脱离文档流，父元素无法被撑开）
#### 解决方法：清除浮动
1. 额外标签法：在浮动元素后加 `<div style="clear: both;"></div>`
2. 父元素加 `overflow: hidden;`（触发 BFC，自动包裹浮动元素）
3. 伪元素法（推荐）：
```css
.clearfix::after {
  content: "";
  display: block;
  clear: both;
}
```


### 5. 定位（position）
用于实现元素精准布局（如悬浮窗、导航栏固定等），通过 `top`/`bottom`/`left`/`right` 控制位置。
| 定位模式       | 语法               | 特点                                                                 |
|----------------|--------------------|----------------------------------------------------------------------|
| 静态定位（默认）| `position: static;`| 无定位，遵循文档流                                                   |
| 相对定位       | `position: relative;`| 不脱离文档流，相对于自身原有位置定位                                 |
| 绝对定位       | `position: absolute;`| 脱离文档流，相对于最近的已定位（非 static）祖先元素定位，无则相对于文档 |
| 固定定位       | `position: fixed;`  | 脱离文档流，相对于浏览器窗口定位（滚动时位置不变，如固定导航）         |
| 粘性定位       | `position: sticky;` | 滚动到指定位置前遵循文档流，之后变为固定定位（如吸顶导航）             |


## 三、现代布局方案（Flex + Grid）
### 1. Flex 布局（弹性布局）
用于实现“一维布局”（一行或一列），简单高效，是目前最常用的布局方式。
#### 用法：给父容器加 `display: flex;`，子元素自动成为“弹性项”
```css
.container {
  display: flex; /* 开启 Flex 布局 */
  width: 500px;
  height: 300px;
  background: #eee;
}
.item { width: 100px; height: 100px; background: red; }
```

#### 父容器核心属性
- `flex-direction`：主轴方向（row：水平/column：垂直）
- `justify-content`：主轴对齐方式（flex-start/center/flex-end/space-between/space-around）
- `align-items`：交叉轴对齐方式（flex-start/center/flex-end/stretch）
- `flex-wrap`：是否换行（nowrap：不换行/wrap：换行）
- `align-content`：多行时交叉轴整体对齐方式（仅换行时生效）

#### 子元素核心属性
- `flex: 1;`：子元素占满剩余空间（简写，等价于 `flex-grow: 1; flex-shrink: 1; flex-basis: 0%;`）
- `align-self`：单独设置子元素的交叉轴对齐方式（覆盖父容器 `align-items`）
- `order`：控制子元素顺序（默认 0，数值越小越靠前）


### 2. Grid 布局（网格布局）
用于实现“二维布局”（多行多列），适合复杂布局（如仪表盘、卡片网格）。
#### 用法：给父容器加 `display: grid;`，子元素自动成为“网格项”
```css
.container {
  display: grid; /* 开启 Grid 布局 */
  width: 500px;
  height: 300px;
  grid-template-columns: 100px 100px 100px; /* 3 列，每列 100px */
  grid-template-rows: 100px 100px; /* 2 行，每行 100px */
  gap: 10px; /* 网格项之间的间距 */
}
.item { background: red; }
```

#### 父容器核心属性
- `grid-template-columns`/`grid-template-rows`：定义列/行大小（支持 `fr` 单位，如 `1fr 2fr` 表示比例 1:2）
- `grid-template-areas`：通过命名定义网格区域（适合复杂布局）
- `justify-items`/`align-items`：网格项在单元格内的水平/垂直对齐
- `gap`：网格项之间的间距（`row-gap` + `column-gap`）

#### 子元素核心属性
- `grid-column`/`grid-row`：控制子元素跨列/跨行（如 `grid-column: 1 / 3;` 表示跨 1-3 列）
- `grid-area`：指定子元素所在的网格区域


## 四、CSS 高级特性
### 1. 媒体查询（响应式布局核心）
根据设备屏幕尺寸（宽度、高度等）切换不同样式，实现“一套代码适配多设备”。
```css
/* 屏幕宽度 ≤ 768px 时生效（移动端） */
@media (max-width: 768px) {
  .container { width: 100%; }
  .item { width: 50%; }
}

/* 屏幕宽度 > 768px 且 ≤ 1200px 时生效（平板） */
@media (min-width: 768px) and (max-width: 1200px) {
  .container { width: 90%; }
}
```


### 2. 动画与过渡
#### （1）过渡（transition）：实现样式“平滑变化”
```css
.btn {
  width: 100px;
  transition: width 0.3s ease; /* 宽度变化，耗时 0.3 秒，缓动效果 */
}
.btn:hover { width: 150px; } /* 鼠标悬浮时宽度平滑变为 150px */
```

#### （2）动画（animation）：实现更复杂的自定义动画
```css
/* 定义动画 */
@keyframes move {
  0% { transform: translateX(0); } /* 起始状态：无偏移 */
  100% { transform: translateX(200px); } /* 结束状态：向右偏移 200px */
}

/* 使用动画 */
.box {
  width: 100px;
  height: 100px;
  background: red;
  animation: move 2s ease infinite alternate; /* 动画名 时长 缓动 循环 反向 */
}
```


### 3. 变形（transform）
对元素进行旋转、缩放、平移、倾斜等操作。
```css
.box {
  transform: rotate(30deg); /* 旋转 30 度 */
  /* transform: scale(1.2); 缩放 1.2 倍 */
  /* transform: translateX(50px); 向右平移 50px */
  /* transform: skew(10deg); 倾斜 10 度 */
}
```


### 4. 变量（CSS Variables）
定义可复用的变量，方便样式维护（如主题色统一修改）。
```css
/* 定义变量（通常在 :root 伪类中，全局可用） */
:root {
  --main-color: #2c3e50;
  --font-size: 16px;
}

/* 使用变量 */
body {
  color: var(--main-color);
  font-size: var(--font-size);
}
.btn {
  background: var(--main-color);
}
```


### 5. 伪类与伪元素进阶
- 结构伪类：`nth-child(n)`（第 n 个子元素）、`nth-of-type(n)`（第 n 个同类型子元素）、`empty`（空元素）
- 状态伪类：`:focus`（元素获得焦点）、`:checked`（表单元素被选中）、`:disabled`（元素禁用）
- 伪元素：`::first-letter`（首字母）、`::first-line`（首行文本）、`::selection`（选中的文本）


## 五、CSS 工程化与优化
### 1. 工程化工具
- 预处理器：Less/Sass/Scss（扩展 CSS，支持变量、嵌套、混合等，需编译为原生 CSS）
- 后处理器：PostCSS（自动添加浏览器前缀、兼容处理，如 Autoprefixer）
- 构建工具：Webpack/Vite（打包 CSS，实现压缩、拆分等）


### 2. 性能优化
- 减少重绘回流：避免频繁修改 `width`/`height`/`top` 等属性，使用 `transform`/`opacity` 替代
- 样式压缩：打包时压缩 CSS（去除空格、注释）
- 合并样式文件：减少 HTTP 请求
- 使用 `link` 引入外部 CSS（避免 `@import`，会阻塞渲染）
- 懒加载非关键样式：对不影响首屏的样式进行延迟加载


---

要不要我帮你整理一份**CSS 常用属性速查表**？包含选择器、布局、动画等核心知识点，方便开发时快速查阅。