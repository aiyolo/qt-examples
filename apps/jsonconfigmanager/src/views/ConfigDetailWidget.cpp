#include "ConfigDetailWidget.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QDebug>

ConfigDetailWidget::ConfigDetailWidget(QWidget* parent)
    : QWidget(parent)
    , m_currentConfigName("")
    , m_currentFilePath("")
    , m_titleLabel(nullptr)
    , m_field1Edit(nullptr)
    , m_field2Edit(nullptr)
    , m_saveButton(nullptr)
    , m_isModified(false)
{
    setupUI();
    clearContent();
}

ConfigDetailWidget::~ConfigDetailWidget()
{
}

void ConfigDetailWidget::setupUI()
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    // 标题
    m_titleLabel = new QLabel(this);
    m_titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333;");
    layout->addWidget(m_titleLabel);

    // 表单布局
    auto* formLayout = new QFormLayout();
    formLayout->setSpacing(10);
    formLayout->setLabelAlignment(Qt::AlignRight);

    // 字段1
    m_field1Edit = new QLineEdit(this);
    connect(m_field1Edit, &QLineEdit::textChanged,
            this, &ConfigDetailWidget::onField1Changed);
    formLayout->addRow("字段1:", m_field1Edit);

    // 字段2
    m_field2Edit = new QLineEdit(this);
    connect(m_field2Edit, &QLineEdit::textChanged,
            this, &ConfigDetailWidget::onField2Changed);
    formLayout->addRow("字段2:", m_field2Edit);

    layout->addLayout(formLayout);

    // 保存按钮
    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    m_saveButton = new QPushButton("保存", this);
    m_saveButton->setEnabled(false);
    connect(m_saveButton, &QPushButton::clicked, this, &ConfigDetailWidget::onSaveClicked);
    buttonLayout->addWidget(m_saveButton);
    layout->addLayout(buttonLayout);

    layout->addStretch();
}

void ConfigDetailWidget::clearContent()
{
    m_currentConfigName.clear();
    m_currentFilePath.clear();
    m_titleLabel->setText("请选择配置文件");
    m_field1Edit->clear();
    m_field2Edit->clear();
    m_field1Edit->setEnabled(false);
    m_field2Edit->setEnabled(false);
    m_saveButton->setEnabled(false);
    m_isModified = false;
}

void ConfigDetailWidget::loadConfig(const QString& configName, const QString& filePath)
{
    m_currentConfigName = configName;
    m_currentFilePath = filePath;

    // 更新标题
    m_titleLabel->setText(QString("配置: %1").arg(configName));

    // 读取JSON文件
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        m_field1Edit->clear();
        m_field2Edit->clear();
        m_field1Edit->setEnabled(false);
        m_field2Edit->setEnabled(false);
        m_saveButton->setEnabled(false);
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        m_field1Edit->clear();
        m_field2Edit->clear();
        m_field1Edit->setEnabled(false);
        m_field2Edit->setEnabled(false);
        m_saveButton->setEnabled(false);
        return;
    }

    QJsonObject obj = doc.object();

    // 设置字段值
    m_field1Edit->setText(obj.value("field1").toString());
    m_field2Edit->setText(obj.value("field2").toString());

    m_field1Edit->setEnabled(true);
    m_field2Edit->setEnabled(true);
    m_saveButton->setEnabled(false);
    m_isModified = false;
}

void ConfigDetailWidget::saveConfig()
{
    if (m_currentFilePath.isEmpty() || !m_isModified) {
        return;
    }

    QFile file(m_currentFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    QJsonObject obj;
    obj["field1"] = m_field1Edit->text();
    obj["field2"] = m_field2Edit->text();

    QJsonDocument doc(obj);
    file.write(doc.toJson());
    file.close();

    m_isModified = false;
    m_saveButton->setEnabled(false);
}

void ConfigDetailWidget::onField1Changed(const QString& text)
{
    Q_UNUSED(text);
    m_isModified = true;
    m_saveButton->setEnabled(true);
}

void ConfigDetailWidget::onField2Changed(const QString& text)
{
    Q_UNUSED(text);
    m_isModified = true;
    m_saveButton->setEnabled(true);
}

void ConfigDetailWidget::onSaveClicked()
{
    saveConfig();
}
