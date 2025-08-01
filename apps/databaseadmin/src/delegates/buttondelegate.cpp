#include "buttondelegate.h"
#include <QPushButton>
#include <QApplication>
#include <QMouseEvent>
#include <QStyleOptionButton>
#include <QStyle>
#include <QDebug>

ButtonDelegate::ButtonDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
    , m_button(new QPushButton)
{
}

QWidget *ButtonDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    QPushButton *editor = new QPushButton(parent);
    editor->setText("操作");
    return editor;
}

void ButtonDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
    QPushButton *button = static_cast<QPushButton*>(editor);
    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    button->setText(text);
}

void ButtonDelegate::setModelData(QWidget *editor,
                                 QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QPushButton *button = static_cast<QPushButton*>(editor);
    QString text = button->text();
    model->setData(index, text, Qt::EditRole);
}

void ButtonDelegate::updateEditorGeometry(QWidget *editor,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void ButtonDelegate::paint(QPainter *painter,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    if (index.data(Qt::UserRole).toString() != "button") {
        // This is not a button cell, let the default delegate handle it
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    // Draw button
    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    if (text.isEmpty()) {
        text = "操作";
    }

    QStyleOptionButton buttonStyleOption;
    buttonStyleOption.state |= QStyle::State_Enabled;
    buttonStyleOption.rect = option.rect;
    buttonStyleOption.text = text;

    QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonStyleOption, painter);
}

QSize ButtonDelegate::sizeHint(const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    QStyleOptionButton buttonStyleOption;
    buttonStyleOption.text = "操作";
    QRect buttonRect = QApplication::style()->subElementRect(QStyle::SE_PushButtonContents, &buttonStyleOption);
    return QSize(buttonRect.width(), buttonRect.height());
}

bool ButtonDelegate::editorEvent(QEvent *event,
                                QAbstractItemModel *model,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (option.rect.contains(mouseEvent->pos())) {
            emit buttonClicked(index);
            return true;
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
