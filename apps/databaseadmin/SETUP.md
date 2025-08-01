# Database Admin Setup Guide

## PostgreSQL 数据库设置

### 1. 安装 PostgreSQL

首先确保已安装 PostgreSQL 数据库服务器。

### 2. 创建数据库

```sql
-- 创建数据库
CREATE DATABASE testdb;

-- 连接到数据库
\c testdb;
```

### 3. 创建表结构

```sql
-- 创建用户表
CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    email VARCHAR(100),
    is_active BOOLEAN DEFAULT true,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 创建项目表
CREATE TABLE projects (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    description TEXT,
    is_active BOOLEAN DEFAULT true,
    start_date DATE,
    end_date DATE,
    actions TEXT DEFAULT 'edit,delete'
);

-- 创建任务表
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

### 4. 插入测试数据

```sql
-- 插入测试用户
INSERT INTO users (username, email, is_active) VALUES
('admin', 'admin@example.com', true),
('user1', 'user1@example.com', true),
('user2', 'user2@example.com', false);

-- 插入测试项目
INSERT INTO projects (name, description, is_active, start_date, end_date) VALUES
('项目A', '这是一个测试项目', true, '2025-01-01', '2025-12-31'),
('项目B', '另一个测试项目', true, '2025-02-01', '2025-11-30'),
('项目C', '已完成的项目', false, '2024-01-01', '2024-12-31');

-- 插入测试任务
INSERT INTO tasks (title, description, is_completed, priority, user_id, deadline) VALUES
('任务1', '完成项目设计', false, 1, 1, '2025-08-31 17:00:00'),
('任务2', '编写文档', false, 2, 2, '2025-09-15 17:00:00'),
('任务3', '测试功能', true, 3, 1, '2025-07-31 17:00:00');
```

### 5. 配置应用程序

在 `mainwindow.cpp` 中修改数据库连接参数：

```cpp
bool success = m_databaseManager->connectToDatabase(
    "localhost",    // host
    "testdb",       // database
    "postgres",     // username
    "password",     // password
    5432           // port
);
```

### 6. 运行应用程序

```bash
cd build/apps/databaseadmin/Debug
./databaseadmin.exe
```

## 功能说明

### 主要功能
- **数据库连接**: 连接/断开 PostgreSQL 数据库
- **表浏览**: 显示数据库中的所有表
- **数据编辑**: 支持增删改查操作
- **异步操作**: 数据库操作不阻塞UI线程
- **自定义显示**:
  - 布尔字段显示为 checkbox
  - 操作字段显示为按钮

### 界面操作
1. 点击"连接数据库"连接到PostgreSQL
2. 从左侧列表选择要查看的表
3. 在表格中直接编辑数据
4. 使用工具栏按钮进行CRUD操作
5. 点击"保存更改"提交修改

## 注意事项

- 确保PostgreSQL服务正在运行
- 确保数据库用户有读写权限
- 应用程序使用异步操作避免UI阻塞
- checkbox字段会自动保存更改
- 操作按钮字段用于触发自定义操作
