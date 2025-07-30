#include "mainwindow.h"
#include <QDebug>
#include <qabstractsocket.h>
#include <qmessagebox.h>
#include <qtcpsocket.h>
#include "simpleimageviewer/SimpleImageViewer.h"
#include <qnetworkproxy.h>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
    ui.setupUi(this);

    m_socket = new QTcpSocket(this);
    m_socket->setProxy(QNetworkProxy::NoProxy);
    connect(
      m_socket, &QTcpSocket::connected, [this]() { qDebug() << "connected"; });

    connect(m_socket, &QTcpSocket::disconnected, [this]() {
        // qDebug() << "disconnected";
        QMessageBox::information(this, "提示", "已断开连接");
    });

    connect(ui.pushButton, &QPushButton::clicked, [this]() {
        int gray = ui.lineEdit->text().toInt();
        if (m_socket->state() != QAbstractSocket::ConnectedState) {
            QMessageBox::information(this, "提示", "请先连接服务器");
            return;
        }
        QString cmd = QString("gray %1%2").arg(gray).arg('\0') ;
        auto cmd1 = QString("gray %1%2").arg('\0').arg('\n').toUtf8() ;
        // auto bytes = cmd1.toUtf8();
        auto len = m_socket->write(cmd.toUtf8());
        if (len > 0) {
            qDebug() << "发送成功";
        } else {
            qDebug() << " === 发送失败";
            QMessageBox::information(this, "提示", "发送失败");
        }
    });

    m_socket->connectToHost("192.168.1.200", 7653);
    auto ok = m_socket->waitForConnected(1000);
    qDebug() << "ok:" << ok;

    // m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    // auto opt = m_socket->socketOption(QAbstractSocket::KeepAliveOption);
    // qDebug() << "opt:" << opt.toBool();
}
