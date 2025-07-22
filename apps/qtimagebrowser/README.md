## 项目设置

### 构建
- 构建命令使用
```
cmake --preset multi-windows
cmake --build --preset multi-windows
```
- 使用 powershell 运行

### 项目要求
- 需要使用 来自libs中的 simpleimageviewer 组件，作为显示组件
- cmake 生成exe时，只需要链接 libs 库，不需要包含 simpleimageviewer 所在目录

### 项目功能

- 需要构造100 张图片，主界面显示网格布局，支持上下浏览
- 点击图片时，将查看改图片的细节图，使用 simpleimageviewer
