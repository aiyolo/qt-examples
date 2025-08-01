#include "databasemanager.h"
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QMessageBox>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

int DatabaseManager::connectionCounter = 0;

DatabaseManager::DatabaseManager(QObject* parent)
  : QObject(parent)
{
    m_connectionName =
      QString("DatabaseConnection_%1").arg(++connectionCounter);
}

DatabaseManager::~DatabaseManager()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

bool DatabaseManager::connectToDatabase(const QString& host,
                                        const QString& database,
                                        const QString& username,
                                        const QString& password,
                                        int port)
{
    m_database = QSqlDatabase::addDatabase("QODBC3", m_connectionName);
    qDebug() << "Driver:" << m_database.isValid();

#if 1
    m_database.setHostName(host);
    m_database.setDatabaseName(database);
    m_database.setUserName(username);
    m_database.setPassword(password);
    // m_database.setPort(port);
#else
// QString conn = "DRIVER={SQL Server};SERVER=%1;DATABASE=%2;UID=%3;PWD=%4";
// auto name = conn.arg(host).arg(database).arg(username).arg(password);
// m_database.setDatabaseName(name);
#endif

    bool success = m_database.open();
    emit connectionStatusChanged(success);

    if (!success) {
        emit queryError(m_database.lastError().text());
        qDebug() << "Database connection failed:"
                 << m_database.lastError().text();
    } else {
        // Create tables if they don't exist
        createTables();
    }

    return success;
}

bool DatabaseManager::isConnected() const
{
    return m_database.isOpen();
}

void DatabaseManager::disconnect()
{
    if (m_database.isOpen()) {
        m_database.close();
        emit connectionStatusChanged(false);
    }
}

QStringList DatabaseManager::getTableNames() const
{
    QStringList tables;
    if (m_database.isOpen()) {
        tables = m_database.tables(QSql::Tables);
    }
    return tables;
}

QSqlQuery DatabaseManager::executeQuery(const QString& query)
{
    QSqlQuery sqlQuery(m_database);
    sqlQuery.exec(query);

    if (sqlQuery.lastError().isValid()) {
        emit queryError(sqlQuery.lastError().text());
        qDebug() << "Query error:" << sqlQuery.lastError().text();
    }

    return sqlQuery;
}

QFuture<QSqlQuery> DatabaseManager::executeQueryAsync(const QString& query)
{
    return QtConcurrent::run([this, query]() -> QSqlQuery {
        QSqlDatabase db = QSqlDatabase::database(m_connectionName);
        QSqlQuery sqlQuery(db);
        sqlQuery.exec(query);
        return sqlQuery;
    });
}

QFuture<bool> DatabaseManager::insertRecordAsync(const QString& table,
                                                 const QVariantMap& data)
{
    return QtConcurrent::run([this, table, data]() -> bool {
        QSqlDatabase db = QSqlDatabase::database(m_connectionName);
        QSqlQuery query(db);

        QStringList fields;
        QStringList placeholders;
        QVariantList values;

        for (auto it = data.begin(); it != data.end(); ++it) {
            fields.append(it.key());
            placeholders.append(":" + it.key());
            values.append(it.value());
        }

        QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
                        .arg(table)
                        .arg(fields.join(", "))
                        .arg(placeholders.join(", "));

        query.prepare(sql);
        for (int i = 0; i < fields.size(); ++i) {
            query.bindValue(":" + fields[i], values[i]);
        }

        bool success = query.exec();
        if (!success) {
            qDebug() << "Insert error:" << query.lastError().text();
        }

        return success;
    });
}

QFuture<bool> DatabaseManager::updateRecordAsync(const QString& table,
                                                 const QVariantMap& data,
                                                 const QString& condition)
{
    return QtConcurrent::run([this, table, data, condition]() -> bool {
        QSqlDatabase db = QSqlDatabase::database(m_connectionName);
        QSqlQuery query(db);

        QStringList setClauses;
        QVariantList values;

        for (auto it = data.begin(); it != data.end(); ++it) {
            setClauses.append(it.key() + " = :" + it.key());
            values.append(it.value());
        }

        QString sql = QString("UPDATE %1 SET %2 WHERE %3")
                        .arg(table)
                        .arg(setClauses.join(", "))
                        .arg(condition);

        query.prepare(sql);
        for (auto it = data.begin(); it != data.end(); ++it) {
            query.bindValue(":" + it.key(), it.value());
        }

        bool success = query.exec();
        if (!success) {
            qDebug() << "Update error:" << query.lastError().text();
        }

        return success;
    });
}

QFuture<bool> DatabaseManager::deleteRecordAsync(const QString& table,
                                                 const QString& condition)
{
    return QtConcurrent::run([this, table, condition]() -> bool {
        QSqlDatabase db = QSqlDatabase::database(m_connectionName);
        QSqlQuery query(db);

        QString sql =
          QString("DELETE FROM %1 WHERE %2").arg(table).arg(condition);
        query.prepare(sql);

        bool success = query.exec();
        if (!success) {
            qDebug() << "Delete error:" << query.lastError().text();
        }

        return success;
    });
}

bool DatabaseManager::createTables()
{
    if (!m_database.isOpen()) {
        return false;
    }

    QSqlQuery query(m_database);

    // Create users table
    QString createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            id SERIAL PRIMARY KEY,
            username VARCHAR(50) UNIQUE NOT NULL,
            email VARCHAR(100),
            is_active BOOLEAN DEFAULT true,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    )";

    if (!query.exec(createUsersTable)) {
        qDebug() << "Error creating users table:" << query.lastError().text();
        return false;
    }

    // Create projects table
    QString createProjectsTable = R"(
        CREATE TABLE IF NOT EXISTS projects (
            id SERIAL PRIMARY KEY,
            name VARCHAR(100) NOT NULL,
            description TEXT,
            is_active BOOLEAN DEFAULT true,
            start_date DATE,
            end_date DATE,
            actions TEXT DEFAULT 'edit,delete'
        )
    )";

    if (!query.exec(createProjectsTable)) {
        qDebug() << "Error creating projects table:"
                 << query.lastError().text();
        return false;
    }

    // Create tasks table
    QString createTasksTable = R"(
        CREATE TABLE IF NOT EXISTS tasks (
            id SERIAL PRIMARY KEY,
            title VARCHAR(200) NOT NULL,
            description TEXT,
            is_completed BOOLEAN DEFAULT false,
            priority INTEGER DEFAULT 1,
            user_id INTEGER REFERENCES users(id),
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            deadline TIMESTAMP
        )
    )";

    if (!query.exec(createTasksTable)) {
        qDebug() << "Error creating tasks table:" << query.lastError().text();
        return false;
    }

    return true;
}

QSqlDatabase DatabaseManager::database() const
{
    return m_database;
}
