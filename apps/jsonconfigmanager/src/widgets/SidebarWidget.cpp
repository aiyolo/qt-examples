#include "SidebarWidget.h"
#include "EditableListWidget.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QListWidgetItem>

SidebarWidget::SidebarWidget(QWidget* parent)
    : QWidget(parent)
    , m_listWidget(nullptr)
    , m_toolBar(nullptr)
    , m_createAction(nullptr)
    , m_copyAction(nullptr)
    , m_deleteAction(nullptr)
    , m_renameAction(nullptr)
{
    setupUI();
    updateActions();

    // 连接信号槽
    connect(m_listWidget, &EditableListWidget::itemClicked,
            this, &SidebarWidget::onItemClicked);
    connect(m_listWidget, &EditableListWidget::itemDoubleClicked,
            this, &SidebarWidget::onItemDoubleClicked);
    connect(m_listWidget, &EditableListWidget::itemCreated,
            this, &SidebarWidget::itemCreated);
    connect(m_listWidget, &EditableListWidget::itemCopied,
            this, &SidebarWidget::onItemCopied);
    connect(m_listWidget, &EditableListWidget::itemDeleted,
            this, &SidebarWidget::itemDeleted);
    connect(m_listWidget, &EditableListWidget::itemRenamed,
            this, &SidebarWidget::itemRenamed);

    // 连接列表选择变化信号
    connect(m_listWidget, &EditableListWidget::itemSelectionChanged,
            this, &SidebarWidget::updateActions);
}

SidebarWidget::~SidebarWidget()
{
}

void SidebarWidget::setConfigs(const QStringList& configNames)
{
    m_listWidget->setItems(configNames);
    updateActions();
}

void SidebarWidget::setupUI()
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // 创建工具栏
    m_toolBar = new QToolBar(this);
    m_toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    m_createAction = m_toolBar->addAction("新建");
    m_copyAction = m_toolBar->addAction("复制");
    m_deleteAction = m_toolBar->addAction("删除");
    m_renameAction = m_toolBar->addAction("重命名");

    connect(m_createAction, &QAction::triggered, this, &SidebarWidget::onCreateConfig);
    connect(m_copyAction, &QAction::triggered, this, &SidebarWidget::onCopyConfig);
    connect(m_deleteAction, &QAction::triggered, this, &SidebarWidget::onDeleteConfig);
    connect(m_renameAction, &QAction::triggered, this, &SidebarWidget::onRenameConfig);

    layout->addWidget(m_toolBar);

    // 创建列表控件
    m_listWidget = new EditableListWidget(this);
    layout->addWidget(m_listWidget);
}

void SidebarWidget::updateActions()
{
    bool hasSelection = m_listWidget->currentItem() != nullptr;
    m_copyAction->setEnabled(hasSelection);
    m_deleteAction->setEnabled(hasSelection);
    m_renameAction->setEnabled(hasSelection);
}

void SidebarWidget::onItemClicked(QListWidgetItem* item)
{
    if (item) {
        emit itemSelected(item->text());
    }
}

void SidebarWidget::onItemDoubleClicked(QListWidgetItem* item)
{
    if (item) {
        emit itemSelected(item->text());
    }
}

void SidebarWidget::onCreateConfig()
{
    m_listWidget->createItem();
}

void SidebarWidget::onCopyConfig()
{
    m_listWidget->copyItem();
}

void SidebarWidget::onDeleteConfig()
{
    m_listWidget->deleteItem();
}

void SidebarWidget::onRenameConfig()
{
    m_listWidget->renameItem();
}

void SidebarWidget::onItemCopied(const QString& originalText, const QString& newText)
{
    Q_UNUSED(originalText);
    // 复制操作完成后，发出itemCreated信号通知MainWindow创建了新配置
    emit itemCreated(newText);
}
