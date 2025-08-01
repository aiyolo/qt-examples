#include "checkboxdelegate.h"
#include <QCheckBox>
#include <QApplication>
#include <QMouseEvent>
#include <QStyleOptionButton>
#include <QStyle>

CheckBoxDelegate::CheckBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
    , m_checkBox(new QCheckBox)
{
}

QWidget *CheckBoxDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    QCheckBox *editor = new QCheckBox(parent);
    return editor;
}

void CheckBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    bool value = index.model()->data(index, Qt::EditRole).toBool();
    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    checkBox->setChecked(value);
}

void CheckBoxDelegate::setModelData(QWidget *editor,
                                   QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    bool value = checkBox->isChecked();
    model->setData(index, value, Qt::EditRole);
}

void CheckBoxDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void CheckBoxDelegate::paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    if (index.data(Qt::UserRole).toString() == "button") {
        // This is a button cell, let the button delegate handle it
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    // Draw checkbox
    bool checked = index.model()->data(index, Qt::DisplayRole).toBool();

    QStyleOptionButton checkBoxStyleOption;
    checkBoxStyleOption.state |= QStyle::State_Enabled;
    if (checked) {
        checkBoxStyleOption.state |= QStyle::State_On;
    } else {
        checkBoxStyleOption.state |= QStyle::State_Off;
    }
    checkBoxStyleOption.rect = option.rect;
    checkBoxStyleOption.text = QString();

    QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkBoxStyleOption, painter);
}

QSize CheckBoxDelegate::sizeHint(const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    QStyleOptionButton checkBoxStyleOption;
    QRect checkBoxRect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &checkBoxStyleOption);
    return QSize(checkBoxRect.width(), checkBoxRect.height());
}
