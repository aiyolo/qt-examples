# Database Admin

一个基于Qt的PostgreSQL数据库管理工具。

## 功能特性

- 连接和管理PostgreSQL数据库
- 显示数据库中的多个表
- 支持表之间的关联关系
- 提供增删改查(CRUD)操作
- 异步数据库操作，不阻塞主线程
- 自定义单元格显示（checkbox和按钮）

## 数据库设计

### 用户表 (users)
```sql
CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    email VARCHAR(100),
    is_active BOOLEAN DEFAULT true,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

### 项目表 (projects)
```sql
CREATE TABLE projects (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    description TEXT,
    is_active BOOLEAN DEFAULT true,
    start_date DATE,
    end_date DATE,
    actions TEXT DEFAULT 'edit,delete'
);
```

### 任务表 (tasks)
```sql
CREATE TABLE tasks (
    id SERIAL PRIMARY KEY,
    title VARCHAR(200) NOT NULL,
    description TEXT,
    is_completed BOOLEAN DEFAULT false,
    priority INTEGER DEFAULT 1,
    user_id INTEGER REFERENCES users(id),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    deadline TIMESTAMP
);
```

## 使用说明

1. 确保PostgreSQL数据库服务正在运行
2. 在PostgreSQL中创建数据库和表结构
3. 修改`mainwindow.cpp`中的数据库连接参数
4. 运行应用程序

## 编译和构建

```bash
cd build
cmake ..
cmake --build . --target databaseadmin
```

## 依赖

- Qt 5.15+
- PostgreSQL开发库
- Qt SQL模块
- Qt Concurrent模块
