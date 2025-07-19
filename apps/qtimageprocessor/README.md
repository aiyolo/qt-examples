# Qt Image Processor (简化版)

基于 Qt 和 OpenCV 的简化图像处理应用，专注于核心功能和扩展性。

## 简化目标

本版本移除了与算法无关的复杂代码，保留核心图像处理功能，提高代码可读性和可维护性。

## 核心功能

- **图像显示**：原图和处理结果对比显示
- **算法选择**：支持多种图像处理算法
- **参数调整**：实时调整算法参数
- **结果保存**：保存处理后的图像

## 支持的算法

- 高斯模糊
- Canny边缘检测
- 亮度/对比度调节
- 灰度化

## 简化内容

### 移除的功能
- 复杂的错误处理和用户提示
- 防抖处理和状态管理
- 详细的参数验证
- 状态栏和进度显示
- 重置功能和占位符

### 保留的核心
- 基本图像处理流程
- 算法工厂模式（保持扩展性）
- 参数系统架构
- 简洁的用户界面

## 架构设计

```
ImageProcessorWindow (主窗口)
├── ImageProcessor (图像处理器)
├── AlgorithmFactory (算法工厂)
├── AlgorithmParameters (参数系统)
└── SimpleImageViewer (图像显示)
```

## 扩展性

### 添加新算法
1. 继承 `ImageAlgorithm` 类
2. 实现 `process()` 方法
3. 创建对应的参数类
4. 在 `AlgorithmFactory` 中注册

### 添加新参数
1. 继承 `AlgorithmParameters` 类
2. 实现参数控件创建
3. 处理参数变化信号

## 构建和运行

```bash
mkdir build && cd build
cmake .. && make
./qtimageprocessor
```

## 使用方法

1. 点击"打开图像"加载图片
2. 从下拉菜单选择算法
3. 调整参数（如有）
4. 点击"保存结果"保存图像

## 支持格式

PNG, JPEG, BMP, TIFF
