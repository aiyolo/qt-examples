#include "EditableListWidget.h"
#include <QtWidgets/QListWidgetItem>

EditableListWidget::EditableListWidget(QWidget* parent)
    : QListWidget(parent)
{
    connect(this, &QListWidget::itemDoubleClicked,
            this, &EditableListWidget::onItemDoubleClicked);
}

EditableListWidget::~EditableListWidget()
{
}

void EditableListWidget::setItems(const QStringList& items)
{
    clear();
    for (const QString& item : items) {
        addItem(item);
    }
}

QStringList EditableListWidget::items() const
{
    QStringList result;
    for (int i = 0; i < count(); ++i) {
        result.append(item(i)->text());
    }
    return result;
}

void EditableListWidget::createItem()
{
    bool ok;
    QString text = QInputDialog::getText(this, "新建项目", "请输入项目名称:",
                                       QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        // 检查名称是否已存在
        QStringList existingNames = items();
        if (existingNames.contains(text)) {
            QMessageBox::warning(this, "警告", "项目名称已存在！");
            return;
        }

        addItem(text);
        emit itemCreated(text);
    }
}

void EditableListWidget::copyItem()
{
    QListWidgetItem* currentItem = this->currentItem();
    if (!currentItem) {
        QMessageBox::information(this, "提示", "请先选择要复制的项目！");
        return;
    }

    QString originalText = currentItem->text();
    QStringList existingNames = items();

    // 生成唯一的复制名称
    QString newText = getUniqueName(originalText + "_副本", existingNames);

    bool ok;
    newText = QInputDialog::getText(this, "复制项目", "请输入新项目名称:",
                                  QLineEdit::Normal, newText, &ok);
    if (ok && !newText.isEmpty()) {
        if (existingNames.contains(newText)) {
            QMessageBox::warning(this, "警告", "项目名称已存在！");
            return;
        }

        addItem(newText);
        emit itemCopied(originalText, newText);
    }
}

void EditableListWidget::deleteItem()
{
    QListWidgetItem* currentItem = this->currentItem();
    if (!currentItem) {
        QMessageBox::information(this, "提示", "请先选择要删除的项目！");
        return;
    }

    QString text = currentItem->text();
    int ret = QMessageBox::question(this, "确认删除",
                                  QString("确定要删除项目 \"%1\" 吗？").arg(text));
    if (ret == QMessageBox::Yes) {
        // 保存当前索引
        int currentIndex = row(currentItem);
        delete currentItem;

        // 删除后自动选择其他条目
        int itemCount = count();
        if (itemCount > 0) {
            // 优先选择下一个条目，如果没有则选择上一个
            int newIndex = currentIndex < itemCount ? currentIndex : itemCount - 1;
            if (newIndex >= 0 && newIndex < itemCount) {
                setCurrentRow(newIndex);
            }
        }

        emit itemDeleted(text);
    }
}

void EditableListWidget::renameItem()
{
    QListWidgetItem* currentItem = this->currentItem();
    if (!currentItem) {
        QMessageBox::information(this, "提示", "请先选择要重命名的项目！");
        return;
    }

    QString oldText = currentItem->text();
    bool ok;
    QString newText = QInputDialog::getText(this, "重命名项目", "请输入新名称:",
                                          QLineEdit::Normal, oldText, &ok);
    if (ok && !newText.isEmpty() && newText != oldText) {
        // 检查新名称是否已存在
        QStringList existingNames = items();
        existingNames.removeOne(oldText); // 移除旧名称
        if (existingNames.contains(newText)) {
            QMessageBox::warning(this, "警告", "项目名称已存在！");
            return;
        }

        currentItem->setText(newText);
        emit itemRenamed(oldText, newText);
    }
}

void EditableListWidget::onItemDoubleClicked(QListWidgetItem* item)
{
    // 双击时只选择项目，不自动重命名
    Q_UNUSED(item);
}

QString EditableListWidget::getUniqueName(const QString& baseName, const QStringList& existingNames)
{
    if (!existingNames.contains(baseName)) {
        return baseName;
    }

    int counter = 1;
    QString newName;
    do {
        newName = QString("%1_%2").arg(baseName).arg(counter);
        counter++;
    } while (existingNames.contains(newName));

    return newName;
}
