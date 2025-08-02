#include "mainwindow.h"
#include <QApplication>
#include <QSplitter>
#include <QPushButton>
#include <QMessageBox>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_tableList(nullptr)
    , m_tableView(nullptr)
    , m_toolBar(nullptr)
    , m_statusBar(nullptr)
    , m_progressBar(nullptr)
    , m_statusLabel(nullptr)
    , m_databaseManager(new DatabaseManager(this))
    , m_tableModel(nullptr)
    , m_checkBoxDelegate(nullptr)
    , m_buttonDelegate(nullptr)
    , m_queryWatcher(new QFutureWatcher<QSqlQuery>(this))
    , m_booleanWatcher(new QFutureWatcher<bool>(this))
{
    setupUI();
    setupActions();
    setupToolBar();
    setupStatusBar();

    // 连接信号槽
    connect(m_databaseManager, &DatabaseManager::connectionStatusChanged,
            this, &MainWindow::handleDatabaseConnected);
    connect(m_databaseManager, &DatabaseManager::queryError,
            this, &MainWindow::handleQueryError);
    connect(m_queryWatcher, &QFutureWatcher<QSqlQuery>::finished,
            this, &MainWindow::onAsyncQueryFinished);
    connect(m_booleanWatcher, &QFutureWatcher<bool>::finished,
            this, &MainWindow::onAsyncQueryFinished);

    updateUIState();
    setWindowTitle("Database Admin");
    resize(1000, 700);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(5);

    // 创建分割器
    auto* splitter = new QSplitter(Qt::Horizontal, centralWidget);
    mainLayout->addWidget(splitter);

    // 左侧表格列表
    auto* leftWidget = new QWidget(splitter);
    leftWidget->setMinimumWidth(200);
    leftWidget->setMaximumWidth(300);

    auto* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    auto* tableLabel = new QLabel("数据库表", leftWidget);
    tableLabel->setStyleSheet("font-weight: bold;");

    m_tableList = new QListWidget(leftWidget);
    m_tableList->setSelectionMode(QAbstractItemView::SingleSelection);

    leftLayout->addWidget(tableLabel);
    leftLayout->addWidget(m_tableList);

    // 右侧表格视图
    auto* rightWidget = new QWidget(splitter);
    auto* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    m_tableView = new QTableView(rightWidget);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->verticalHeader()->setVisible(false);

    rightLayout->addWidget(m_tableView);

    splitter->addWidget(leftWidget);
    splitter->addWidget(rightWidget);
    splitter->setSizes({250, 750});

    // 连接表格列表选择信号
    connect(m_tableList, &QListWidget::itemClicked,
            [this](QListWidgetItem *item) {
                if (item) {
                    selectTable(item->text());
                }
            });
}

void MainWindow::setupActions()
{
    // 在工具栏设置中实现具体动作
}

void MainWindow::setupToolBar()
{
    m_toolBar = addToolBar("工具栏");
    m_toolBar->setObjectName("MainToolBar");
    m_toolBar->setMovable(false);

    // 连接数据库按钮
    auto* connectAction = m_toolBar->addAction("连接数据库");
    connectAction->setToolTip("连接到数据库");
    connect(connectAction, &QAction::triggered, this, &MainWindow::connectToDatabase);

    // 断开连接按钮
    auto* disconnectAction = m_toolBar->addAction("断开连接");
    disconnectAction->setToolTip("断开数据库连接");
    connect(disconnectAction, &QAction::triggered, this, &MainWindow::disconnectFromDatabase);

    m_toolBar->addSeparator();

    // 刷新按钮
    auto* refreshAction = m_toolBar->addAction("刷新");
    refreshAction->setToolTip("刷新表格列表");
    connect(refreshAction, &QAction::triggered, this, &MainWindow::refreshTables);

    m_toolBar->addSeparator();

    // 添加记录按钮
    auto* addAction = m_toolBar->addAction("添加");
    addAction->setToolTip("添加新记录");
    connect(addAction, &QAction::triggered, this, &MainWindow::addRecord);

    // 删除记录按钮
    auto* deleteAction = m_toolBar->addAction("删除");
    deleteAction->setToolTip("删除选中记录");
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteRecord);

    // 保存更改按钮
    auto* saveAction = m_toolBar->addAction("保存");
    saveAction->setToolTip("保存更改");
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveChanges);
}

void MainWindow::setupStatusBar()
{
    m_statusBar = statusBar();
    m_statusLabel = new QLabel("就绪", m_statusBar);
    m_progressBar = new QProgressBar(m_statusBar);
    m_progressBar->setVisible(false);
    m_progressBar->setMaximumWidth(200);

    m_statusBar->addWidget(m_statusLabel, 1);
    m_statusBar->addWidget(m_progressBar);
}

void MainWindow::connectToDatabase()
{
    m_progressBar->setVisible(true);
    m_progressBar->setRange(0, 0); // Indeterminate progress
    m_statusLabel->setText("正在连接数据库...");

    // 使用默认配置连接数据库
    bool success = m_databaseManager->connectToDatabase(
        "127.0.0.1",    // host
        "learnsql",       // database
        "zhu",     // username
        "123",     // password
        5432           // port
    );

    if (success) {
        m_statusLabel->setText("数据库连接成功");
        refreshTables();
    } else {
        m_statusLabel->setText("数据库连接失败");
        m_progressBar->setVisible(false);
    }
}

void MainWindow::disconnectFromDatabase()
{
    m_databaseManager->disconnect();
    m_tableList->clear();
    if (m_tableModel) {
        m_tableModel->clear();
    }
    m_currentTable.clear();
    m_statusLabel->setText("数据库已断开连接");
}

void MainWindow::refreshTables()
{
    if (!m_databaseManager->isConnected()) {
        return;
    }

    m_progressBar->setVisible(true);
    m_progressBar->setRange(0, 0);
    m_statusLabel->setText("正在加载表格列表...");

    QStringList tables = m_databaseManager->getTableNames();
    m_tableList->clear();
    m_tableList->addItems(tables);

    m_progressBar->setVisible(false);
    m_statusLabel->setText(QString("加载了 %1 个表格").arg(tables.size()));
}

void MainWindow::selectTable(const QString &tableName)
{
    if (!m_databaseManager->isConnected() || tableName.isEmpty()) {
        return;
    }

    m_progressBar->setVisible(true);
    m_progressBar->setRange(0, 0);
    m_statusLabel->setText(QString("正在加载表格 '%1'...").arg(tableName));

    loadTableData(tableName);

    m_currentTable = tableName;
    m_progressBar->setVisible(false);
    m_statusLabel->setText(QString("已加载表格 '%1'").arg(tableName));
}

void MainWindow::loadTableData(const QString &tableName)
{
    if (!m_databaseManager->isConnected() || tableName.isEmpty()) {
        return;
    }

    // 创建新的表格模型
    if (m_tableModel) {
        delete m_tableModel;
    }

    m_tableModel = new TableModel(this, m_databaseManager->database());
    m_tableModel->setTable(tableName);
    m_tableModel->select();

    m_tableView->setModel(m_tableModel);

    // 设置委托
    if (m_checkBoxDelegate) {
        delete m_checkBoxDelegate;
    }
    m_checkBoxDelegate = new CheckBoxDelegate(this);

    if (m_buttonDelegate) {
        delete m_buttonDelegate;
    }
    m_buttonDelegate = new ButtonDelegate(this);
    connect(m_buttonDelegate, &ButtonDelegate::buttonClicked,
            this, &MainWindow::handleButtonClicked);

    // 为布尔列和操作列设置委托
    for (int column = 0; column < m_tableModel->columnCount(); ++column) {
        if (m_tableModel->isBooleanColumn(column)) {
            m_tableView->setItemDelegateForColumn(column, m_checkBoxDelegate);
        } else if (m_tableModel->isActionColumn(column)) {
            m_tableView->setItemDelegateForColumn(column, m_buttonDelegate);
        }
    }

    // 调整列宽
    m_tableView->resizeColumnsToContents();
}

void MainWindow::addRecord()
{
    if (!m_tableModel || m_currentTable.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一个表格");
        return;
    }

    m_tableModel->insertRow(m_tableModel->rowCount());
    QModelIndex index = m_tableModel->index(m_tableModel->rowCount() - 1, 0);
    m_tableView->scrollTo(index);
    m_tableView->setCurrentIndex(index);
}

void MainWindow::deleteRecord()
{
    if (!m_tableModel || m_currentTable.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一个表格");
        return;
    }

    QModelIndex currentIndex = m_tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的记录");
        return;
    }

    int row = currentIndex.row();
    if (QMessageBox::question(this, "确认删除", "确定要删除选中的记录吗？",
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        m_tableModel->removeRow(row);
        m_tableModel->submitAll();
    }
}

void MainWindow::saveChanges()
{
    if (!m_tableModel || m_currentTable.isEmpty()) {
        QMessageBox::warning(this, "警告", "没有可保存的更改");
        return;
    }

    m_progressBar->setVisible(true);
    m_progressBar->setRange(0, 0);
    m_statusLabel->setText("正在保存更改...");

    if (m_tableModel->submitAll()) {
        m_statusLabel->setText("更改已保存");
    } else {
        m_statusLabel->setText(QString("保存失败: %1").arg(m_tableModel->lastError().text()));
    }

    m_progressBar->setVisible(false);
}

void MainWindow::handleDatabaseConnected(bool connected)
{
    updateUIState();
    if (connected) {
        refreshTables();
    }
}

void MainWindow::handleQueryError(const QString &error)
{
    m_progressBar->setVisible(false);
    m_statusLabel->setText("错误: " + error);
    QMessageBox::critical(this, "数据库错误", error);
}

void MainWindow::handleButtonClicked(const QModelIndex &index)
{
    if (!index.isValid() || m_currentTable.isEmpty()) {
        return;
    }

    int row = index.row();
    QString message = QString("按钮在表格 '%1' 的第 %2 行被点击").arg(m_currentTable).arg(row + 1);
    QMessageBox::information(this, "操作按钮", message);

    // 这里可以实现具体的按钮操作，比如编辑、删除等
}

void MainWindow::onAsyncQueryFinished()
{
    m_progressBar->setVisible(false);
    m_statusLabel->setText("异步操作完成");
}

void MainWindow::updateUIState()
{
    bool connected = m_databaseManager->isConnected();

    // 更新工具栏按钮状态
    for (QAction *action : m_toolBar->actions()) {
        if (action->text() == "连接数据库") {
            action->setEnabled(!connected);
        } else if (action->text() == "断开连接") {
            action->setEnabled(connected);
        } else {
            action->setEnabled(connected);
        }
    }

    // 更新表格相关控件状态
    m_tableList->setEnabled(connected);
    m_tableView->setEnabled(connected);

    if (!connected) {
        m_tableList->clear();
        if (m_tableModel) {
            m_tableModel->clear();
        }
        m_currentTable.clear();
        m_statusLabel->setText("数据库未连接");
    }
}
