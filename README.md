
## 下载依赖
```
conan install . -s build_type=Debug -pr=mac --build=missing
conan install . -s build_type=Release -pr=mac --build=missing
``

## 编译
```
cmake --preset macos-debug
cmake --build --preset macos-debug
```
