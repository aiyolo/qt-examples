-- Database Admin Test Database Setup
-- This script creates a test database with sample data

-- Create test database (run this as superuser)
-- CREATE DATABASE databaseadmin_test;

-- Connect to the database
-- \c databaseadmin_test;

-- Create users table
CREATE TABLE IF NOT EXISTS users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    email VARCHAR(100),
    is_active BOOLEAN DEFAULT true,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Create projects table
CREATE TABLE IF NOT EXISTS projects (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    description TEXT,
    is_active BOOLEAN DEFAULT true,
    start_date DATE,
    end_date DATE,
    actions TEXT DEFAULT 'edit,delete'
);

-- Create tasks table
CREATE TABLE IF NOT EXISTS tasks (
    id SERIAL PRIMARY KEY,
    title VARCHAR(200) NOT NULL,
    description TEXT,
    is_completed BOOLEAN DEFAULT false,
    priority INTEGER DEFAULT 1,
    user_id INTEGER REFERENCES users(id),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    deadline TIMESTAMP
);

-- Insert test users
INSERT INTO users (username, email, is_active) VALUES
('admin', 'admin@example.com', true),
('developer', 'dev@example.com', true),
('tester', 'test@example.com', false),
('manager', 'manager@example.com', true)
ON CONFLICT (username) DO NOTHING;

-- Insert test projects
INSERT INTO projects (name, description, is_active, start_date, end_date) VALUES
('Web Application', 'Modern web application development', true, '2025-01-01', '2025-12-31'),
('Mobile App', 'Cross-platform mobile application', true, '2025-03-01', '2025-11-30'),
('Database Migration', 'Legacy system migration', false, '2024-01-01', '2024-06-30'),
('API Development', 'RESTful API services', true, '2025-02-01', '2025-08-31')
ON CONFLICT DO NOTHING;

-- Insert test tasks
INSERT INTO tasks (title, description, is_completed, priority, user_id, deadline) VALUES
('Design UI Mockups', 'Create user interface designs', false, 1, 1, '2025-08-15 17:00:00'),
('Setup Development Environment', 'Configure development tools', true, 2, 2, '2025-07-31 17:00:00'),
('Write Unit Tests', 'Create comprehensive test suite', false, 1, 3, '2025-09-30 17:00:00'),
('Deploy to Production', 'Final deployment procedures', false, 3, 4, '2025-12-15 17:00:00'),
('Code Review', 'Review pull requests', false, 2, 1, '2025-08-31 17:00:00')
ON CONFLICT DO NOTHING;

-- Verify data
SELECT 'Users table:' as table_name, COUNT(*) as row_count FROM users
UNION ALL
SELECT 'Projects table:' as table_name, COUNT(*) as row_count FROM projects
UNION ALL
SELECT 'Tasks table:' as table_name, COUNT(*) as row_count FROM tasks;

-- Sample queries to test relationships
SELECT u.username, COUNT(t.id) as task_count
FROM users u
LEFT JOIN tasks t ON u.id = t.user_id
GROUP BY u.id, u.username
ORDER BY task_count DESC;

SELECT p.name, p.is_active, t.title, t.is_completed
FROM projects p
LEFT JOIN tasks t ON t.description LIKE '%' || p.name || '%'
ORDER BY p.name;
