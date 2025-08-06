#include "AddRecordDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>

AddRecordDialog::AddRecordDialog(const QString &tableName, const QSqlDatabase &database, QWidget *parent)
    : QDialog(parent)
    , m_formLayout(nullptr)
    , m_okButton(nullptr)
    , m_cancelButton(nullptr)
{
    setWindowTitle("添加新记录");
    setModal(true);
    resize(400, 500);

    setupUI();

    // 获取表结构信息
    QSqlRecord record = database.record(tableName);
    populateFields(record);
}

void AddRecordDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // 创建表单布局
    m_formLayout = new QFormLayout();
    mainLayout->addLayout(m_formLayout);

    // 创建按钮框
    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    mainLayout->addWidget(buttonBox);

    m_okButton = buttonBox->button(QDialogButtonBox::Ok);
    m_cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
}

void AddRecordDialog::populateFields(const QSqlRecord &record)
{
    for (int i = 0; i < record.count(); ++i) {
        QSqlField field = record.field(i);
        QString fieldName = field.name();
        QVariant::Type fieldType = field.type();

        // 跳过自增主键
        if (field.isAutoValue()) {
            continue;
        }

        QWidget *widget = nullptr;

        // 根据字段类型创建合适的输入控件
        switch (fieldType) {
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::LongLong:
        case QVariant::ULongLong: {
            auto *spinBox = new QSpinBox(this);
            spinBox->setRange(-999999, 999999);
            widget = spinBox;
            break;
        }
        case QVariant::Double: {
            auto *doubleSpinBox = new QDoubleSpinBox(this);
            doubleSpinBox->setRange(-999999.99, 999999.99);
            doubleSpinBox->setDecimals(2);
            widget = doubleSpinBox;
            break;
        }
        case QVariant::Bool: {
            auto *checkBox = new QCheckBox(this);
            checkBox->setChecked(false);
            widget = checkBox;
            break;
        }
        case QVariant::Date: {
            auto *dateEdit = new QDateEdit(this);
            dateEdit->setCalendarPopup(true);
            dateEdit->setDate(QDate::currentDate());
            widget = dateEdit;
            break;
        }
        case QVariant::DateTime: {
            auto *dateTimeEdit = new QDateTimeEdit(this);
            dateTimeEdit->setCalendarPopup(true);
            dateTimeEdit->setDateTime(QDateTime::currentDateTime());
            widget = dateTimeEdit;
            break;
        }
        default: {
            auto *lineEdit = new QLineEdit(this);
            lineEdit->setMaxLength(255);
            widget = lineEdit;
            break;
        }
        }

        if (widget) {
            m_fieldWidgets[fieldName] = widget;
            m_formLayout->addRow(fieldName + ":", widget);
        }
    }
}

QMap<QString, QVariant> AddRecordDialog::getRecordData() const
{
    QMap<QString, QVariant> data;

    for (auto it = m_fieldWidgets.constBegin(); it != m_fieldWidgets.constEnd(); ++it) {
        QString fieldName = it.key();
        QWidget *widget = it.value();

        QVariant value;

        if (auto *lineEdit = qobject_cast<QLineEdit*>(widget)) {
            value = lineEdit->text();
        } else if (auto *spinBox = qobject_cast<QSpinBox*>(widget)) {
            value = spinBox->value();
        } else if (auto *doubleSpinBox = qobject_cast<QDoubleSpinBox*>(widget)) {
            value = doubleSpinBox->value();
        } else if (auto *checkBox = qobject_cast<QCheckBox*>(widget)) {
            value = checkBox->isChecked();
        } else if (auto *dateEdit = qobject_cast<QDateEdit*>(widget)) {
            value = dateEdit->date();
        } else if (auto *dateTimeEdit = qobject_cast<QDateTimeEdit*>(widget)) {
            value = dateTimeEdit->dateTime();
        }

        data[fieldName] = value;
    }

    return data;
}
