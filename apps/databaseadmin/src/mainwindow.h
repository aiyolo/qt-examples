#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QListWidget>
#include <QTableView>
#include <QToolBar>
#include <QAction>
#include <QStatusBar>
#include <QProgressBar>
#include <QLabel>
#include <QMessageBox>
#include <QFutureWatcher>

#include "database/databasemanager.h"
#include "models/tablemodel.h"
#include "delegates/checkboxdelegate.h"
#include "delegates/buttondelegate.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void connectToDatabase();
    void disconnectFromDatabase();
    void refreshTables();
    void selectTable(const QString &tableName);
    void addRecord();
    void deleteRecord();
    void saveChanges();
    void handleDatabaseConnected(bool connected);
    void handleQueryError(const QString &error);
    void handleButtonClicked(const QModelIndex &index);
    void onAsyncQueryFinished();

private:
    void setupUI();
    void setupActions();
    void setupToolBar();
    void setupStatusBar();
    void loadTableData(const QString &tableName);
    void updateUIState();

    // UI components
    QListWidget *m_tableList;
    QTableView *m_tableView;
    QToolBar *m_toolBar;
    QStatusBar *m_statusBar;
    QProgressBar *m_progressBar;
    QLabel *m_statusLabel;

    // Database components
    DatabaseManager *m_databaseManager;
    TableModel *m_tableModel;
    CheckBoxDelegate *m_checkBoxDelegate;
    ButtonDelegate *m_buttonDelegate;

    // Async operation
    QFutureWatcher<QSqlQuery> *m_queryWatcher;
    QFutureWatcher<bool> *m_booleanWatcher;

    QString m_currentTable;
};

#endif // MAINWINDOW_H
