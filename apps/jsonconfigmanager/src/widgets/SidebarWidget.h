#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QAction>
#include <QtCore/QStringList>

class EditableListWidget;

class SidebarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SidebarWidget(QWidget* parent = nullptr);
    ~SidebarWidget();

    void setConfigs(const QStringList& configNames);

signals:
    void itemSelected(const QString& configName);
    void itemCreated(const QString& configName);
    void itemDeleted(const QString& configName);
    void itemRenamed(const QString& oldName, const QString& newName);

private slots:
    void onItemClicked(QListWidgetItem* item);
    void onItemDoubleClicked(QListWidgetItem* item);
    void onCreateConfig();
    void onCopyConfig();
    void onDeleteConfig();
    void onRenameConfig();
    void onItemCopied(const QString& originalText, const QString& newText);

private:
    void setupUI();
    void updateActions();

    EditableListWidget* m_listWidget;
    QToolBar* m_toolBar;
    QAction* m_createAction;
    QAction* m_copyAction;
    QAction* m_deleteAction;
    QAction* m_renameAction;
};
