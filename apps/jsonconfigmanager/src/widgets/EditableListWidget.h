#pragma once

#include <QtWidgets/QListWidget>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>
#include <QtCore/QStringList>

class EditableListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit EditableListWidget(QWidget* parent = nullptr);
    ~EditableListWidget();

    void setItems(const QStringList& items);
    QStringList items() const;

signals:
    void itemCreated(const QString& text);
    void itemCopied(const QString& originalText, const QString& newText);
    void itemDeleted(const QString& text);
    void itemRenamed(const QString& oldText, const QString& newText);

public slots:
    void createItem();
    void copyItem();
    void deleteItem();
    void renameItem();

private slots:
    void onItemDoubleClicked(QListWidgetItem* item);

private:
    QString getUniqueName(const QString& baseName, const QStringList& existingNames);
};
