#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H

#include <QStyledItemDelegate>
#include <QPushButton>

class ButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ButtonDelegate(QObject *parent = nullptr);

    // Create editor widget
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const override;

    // Set editor data
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    // Set model data
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                     const QModelIndex &index) const override;

    // Update editor geometry
    void updateEditorGeometry(QWidget *editor,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const override;

    // Paint the button
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    // Get size hint
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    // Handle mouse events
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                    const QStyleOptionViewItem &option,
                    const QModelIndex &index) override;

signals:
    void buttonClicked(const QModelIndex &index);

private:
    QPushButton *m_button;
};

#endif // BUTTONDELEGATE_H
