#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSplitter>
#include <QtCore/QFileSystemWatcher>

class SidebarWidget;
class ConfigDetailWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConfigSelected(const QString& configName);
    void onConfigCreated(const QString& configName);
    void onConfigDeleted(const QString& configName);
    void onConfigRenamed(const QString& oldName, const QString& newName);

private:
    void setupUI();
    void loadConfigs();
    QString getConfigPath() const;

    SidebarWidget* m_sidebarWidget;
    ConfigDetailWidget* m_detailWidget;
    QFileSystemWatcher* m_watcher;
};
