#include "mainwindow.h"
#include <QLabel>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    statusBar()->showMessage("Database Admin Ready");
}

MainWindow::~MainWindow()
{
}

void MainWindow::connectToDatabase() {}
void MainWindow::disconnectFromDatabase() {}
void MainWindow::refreshTables() {}
void MainWindow::selectTable(const QString &tableName) {}
void MainWindow::addRecord() {}
void MainWindow::deleteRecord() {}
void MainWindow::saveChanges() {}
void MainWindow::handleDatabaseConnected(bool connected) {}
void MainWindow::handleQueryError(const QString &error) {}
void MainWindow::handleButtonClicked(const QModelIndex &index) {}
void MainWindow::onAsyncQueryFinished() {}
