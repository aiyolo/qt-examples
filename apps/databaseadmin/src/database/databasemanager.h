#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QThread>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    // Database connection
    bool connectToDatabase(const QString &host, const QString &database,
                          const QString &username, const QString &password, int port = 5432);
    bool isConnected() const;
    void disconnect();

    // Table operations
    QStringList getTableNames() const;
    QSqlQuery executeQuery(const QString &query);

    // Async operations
    QFuture<QSqlQuery> executeQueryAsync(const QString &query);
    QFuture<bool> insertRecordAsync(const QString &table, const QVariantMap &data);
    QFuture<bool> updateRecordAsync(const QString &table, const QVariantMap &data, const QString &condition);
    QFuture<bool> deleteRecordAsync(const QString &table, const QString &condition);

    // Table creation
    bool createTables();

    // Get database connection
    QSqlDatabase database() const;

signals:
    void queryExecuted(const QSqlQuery &query);
    void queryError(const QString &error);
    void connectionStatusChanged(bool connected);

private:
    QSqlDatabase m_database;
    QString m_connectionName;
    static int connectionCounter;
};

#endif // DATABASEMANAGER_H
