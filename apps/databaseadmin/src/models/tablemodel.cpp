#include "tablemodel.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QCheckBox>
#include <QPushButton>
#include <QDebug>

TableModel::TableModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
}

TableModel::~TableModel()
{
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // Handle checkbox display for boolean columns
    if (isBooleanColumn(index.column())) {
        if (role == Qt::CheckStateRole) {
            QVariant value = QSqlTableModel::data(index, Qt::DisplayRole);
            return value.toBool() ? Qt::Checked : Qt::Unchecked;
        }
        if (role == Qt::DisplayRole) {
            return QVariant(); // Don't display text for checkboxes
        }
    }

    // Handle action buttons
    if (isActionColumn(index.column())) {
        if (role == Qt::DisplayRole) {
            return "操作"; // Button text
        }
        if (role == Qt::UserRole) {
            return "button"; // Custom role to identify button cells
        }
    }

    // Default behavior
    return QSqlTableModel::data(index, role);
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    // Handle checkbox editing
    if (isBooleanColumn(index.column()) && role == Qt::CheckStateRole) {
        bool checked = (value.toInt() == Qt::Checked);
        return QSqlTableModel::setData(index, checked, Qt::EditRole);
    }

    // Handle regular data editing
    return QSqlTableModel::setData(index, value, role);
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = QSqlTableModel::flags(index);

    // Make boolean columns checkable
    if (isBooleanColumn(index.column())) {
        flags |= Qt::ItemIsUserCheckable;
    }

    // Make action columns enabled but not editable
    if (isActionColumn(index.column())) {
        flags &= ~Qt::ItemIsEditable;
        flags |= Qt::ItemIsEnabled;
    }

    return flags;
}

QString TableModel::getColumnType(int column) const
{
    if (column >= 0 && column < m_columnTypes.size()) {
        return m_columnTypes.at(column);
    }
    return QString();
}

bool TableModel::isBooleanColumn(int column) const
{
    QString type = getColumnType(column);
    return type.contains("bool", Qt::CaseInsensitive) ||
           type.contains("boolean", Qt::CaseInsensitive);
}

bool TableModel::isActionColumn(int column) const
{
    QString columnName = this->headerData(column, Qt::Horizontal).toString();
    return columnName.contains("action", Qt::CaseInsensitive) ||
           columnName.contains("操作", Qt::CaseInsensitive) ||
           getColumnType(column).contains("action", Qt::CaseInsensitive);
}

void TableModel::setTable(const QString &tableName)
{
    QSqlTableModel::setTable(tableName);
    detectColumnTypes();
}

void TableModel::detectColumnTypes()
{
    m_columnTypes.clear();

    if (!this->database().isOpen() || this->tableName().isEmpty()) {
        return;
    }

    // Query column information
    QSqlQuery query(this->database());
    QString sql = QString("SELECT column_name, data_type FROM information_schema.columns "
                         "WHERE table_name = '%1' ORDER BY ordinal_position")
                  .arg(this->tableName());

    if (query.exec(sql)) {
        while (query.next()) {
            QString columnName = query.value(0).toString();
            QString dataType = query.value(1).toString();
            m_columnTypes.append(dataType);
        }
    } else {
        qDebug() << "Failed to query column types:" << query.lastError().text();
        // Fallback: create empty types list
        for (int i = 0; i < this->columnCount(); ++i) {
            m_columnTypes.append("");
        }
    }
}
