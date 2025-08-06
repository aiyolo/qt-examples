
## 下载依赖
```
conan install . -s build_type=Debug  --build=missing -pr=mac
conan install . -s build_type=Release  --build=missing -pr=mac
``

## 编译
```
cmake --preset macos-debug
cmake --build --preset macos-debug
```
