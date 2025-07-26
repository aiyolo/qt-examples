#include "mainwindow.h"
#include "widgets/SidebarWidget.h"
#include "views/ConfigDetailWidget.h"
#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QtCore/QDebug>
#include <QtCore/QFileInfoList>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_sidebarWidget(nullptr)
    , m_detailWidget(nullptr)
    , m_watcher(new QFileSystemWatcher(this))
{
    setupUI();
    loadConfigs();

    // 连接信号槽
    connect(m_sidebarWidget, &SidebarWidget::itemSelected,
            this, &MainWindow::onConfigSelected);
    connect(m_sidebarWidget, &SidebarWidget::itemCreated,
            this, &MainWindow::onConfigCreated);
    connect(m_sidebarWidget, &SidebarWidget::itemDeleted,
            this, &MainWindow::onConfigDeleted);
    connect(m_sidebarWidget, &SidebarWidget::itemRenamed,
            this, &MainWindow::onConfigRenamed);

    // 监听配置文件夹变化
    QString configPath = getConfigPath();
    if (QDir(configPath).exists()) {
        m_watcher->addPath(configPath);
        connect(m_watcher, &QFileSystemWatcher::directoryChanged,
                this, &MainWindow::loadConfigs);
    }

    setWindowTitle("JSON配置管理器");
    resize(800, 600);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto* layout = new QHBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // 创建分割器
    auto* splitter = new QSplitter(Qt::Horizontal, centralWidget);
    layout->addWidget(splitter);

    // 创建侧边栏
    m_sidebarWidget = new SidebarWidget(splitter);
    m_sidebarWidget->setMinimumWidth(200);
    m_sidebarWidget->setMaximumWidth(300);

    // 创建详情视图
    m_detailWidget = new ConfigDetailWidget(splitter);
    m_detailWidget->setMinimumWidth(400);

    splitter->addWidget(m_sidebarWidget);
    splitter->addWidget(m_detailWidget);
    splitter->setSizes({250, 550});
}

void MainWindow::loadConfigs()
{
    QString configPath = getConfigPath();
    QDir configDir(configPath);

    if (!configDir.exists()) {
        // 创建配置文件夹
        QDir().mkpath(configPath);
        return;
    }

    // 获取所有JSON文件
    QStringList filters;
    filters << "*.json";
    QFileInfoList fileList = configDir.entryInfoList(filters, QDir::Files);

    QStringList configNames;
    for (const QFileInfo& fileInfo : fileList) {
        QString fileName = fileInfo.baseName();
        configNames.append(fileName);
    }

    m_sidebarWidget->setConfigs(configNames);
}

QString MainWindow::getConfigPath() const
{
    // 使用应用程序数据目录
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (appDataPath.isEmpty()) {
        // 如果获取不到，使用当前目录下的configs文件夹
        appDataPath = QApplication::applicationDirPath();
    }

    return appDataPath + "/configs";
}

void MainWindow::onConfigSelected(const QString& configName)
{
    m_detailWidget->loadConfig(configName, getConfigPath() + "/" + configName + ".json");
}

void MainWindow::onConfigCreated(const QString& configName)
{
    QString configPath = getConfigPath();
    QDir().mkpath(configPath);

    QString filePath = configPath + "/" + configName + ".json";

    // 创建默认的JSON文件
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QString defaultContent = "{\n    \"field1\": \"\",\n    \"field2\": \"\"\n}";
        file.write(defaultContent.toUtf8());
        file.close();
    }

    loadConfigs();
}

void MainWindow::onConfigDeleted(const QString& configName)
{
    QString filePath = getConfigPath() + "/" + configName + ".json";
    QFile::remove(filePath);
    loadConfigs();
}

void MainWindow::onConfigRenamed(const QString& oldName, const QString& newName)
{
    QString configPath = getConfigPath();
    QString oldPath = configPath + "/" + oldName + ".json";
    QString newPath = configPath + "/" + newName + ".json";

    QFile::rename(oldPath, newPath);
    loadConfigs();
}
