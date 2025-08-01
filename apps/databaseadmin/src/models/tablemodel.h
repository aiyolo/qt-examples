#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QVariant>
#include <QModelIndex>

class TableModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit TableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    ~TableModel();

    // Override data method to handle custom cell types
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Override setData method to handle custom cell editing
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    // Override flags method to control cell editability
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Get column type information
    QString getColumnType(int column) const;
    bool isBooleanColumn(int column) const;
    bool isActionColumn(int column) const;

    // Set table with automatic column type detection
    void setTable(const QString &tableName) override;

private:
    QStringList m_columnTypes;
    void detectColumnTypes();
};

#endif // TABLEMODEL_H
