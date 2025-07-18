
#pragma once

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMutex>
#include <QSettings>
#include <QString>
#include <QTextStream>
#include <QThread>
#include <QtGlobal>
#include <qglobal.h>

// Log levels
enum LogLevel
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
};

// Global variables
static LogLevel MIN_LOG_LEVEL = LOG_DEBUG;          // Default to DEBUG
static QString LOG_FILE_PATH = "";                  // Will be set by setup_logger
static qint64 MAX_LOG_FILE_SIZE = 10 * 1024 * 1024; // Will be set by setup_logger, default 10MB
static int MAX_LOG_FILES = 3;                       // Will be set by setup_logger, default 3

// Function to read log level from config file
inline void initLogLevel(const QString& configFilePath)
{
    QFile configFile(configFilePath);
    if (!configFile.exists())
    {
        // Create default config file with DEBUG level
        QDir dir(QFileInfo(configFilePath).absolutePath());
        if (!dir.exists())
        {
            QDir().mkpath(dir.absolutePath());
        }
        QSettings settings(configFilePath, QSettings::IniFormat);
        settings.setValue("logging/loglevel", "DEBUG");
        MIN_LOG_LEVEL = LOG_DEBUG;
        return;
    }

    QSettings settings(configFilePath, QSettings::IniFormat);
    QString levelStr = settings.value("logging/loglevel", "Debug").toString().toUpper();

    if (levelStr == "Debug")
    {
        MIN_LOG_LEVEL = LOG_DEBUG;
    }
    else if (levelStr == "Info")
    {
        MIN_LOG_LEVEL = LOG_INFO;
    }
    else if (levelStr == "Warn")
    {
        MIN_LOG_LEVEL = LOG_WARN;
    }
    else if (levelStr == "Error")
    {
        MIN_LOG_LEVEL = LOG_ERROR;
    }
    else if (levelStr == "Fatal")
    {
        MIN_LOG_LEVEL = LOG_FATAL;
    }
    else
    {
        // Invalid level, default to DEBUG
        MIN_LOG_LEVEL = LOG_DEBUG;
        settings.setValue("logging/loglevel", "Debug");
    }
}

inline void outputMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    static QMutex mutex;
    mutex.lock();

    // Convert Qt message type to our log level
    LogLevel currentLevel;
    QString levelStr;
    switch (type)
    {
        case QtDebugMsg:
            currentLevel = LOG_DEBUG;
            levelStr = "debug";
            break;
        case QtInfoMsg:
            currentLevel = LOG_INFO;
            levelStr = "info ";
            break;
        case QtWarningMsg:
            currentLevel = LOG_WARN;
            levelStr = "warn ";
            break;
        case QtCriticalMsg:
            currentLevel = LOG_ERROR;
            levelStr = "error";
            break;
        case QtFatalMsg:
            currentLevel = LOG_FATAL;
            levelStr = "fatal";
            break;
    }

    // Skip messages below minimum log level
    if (currentLevel < MIN_LOG_LEVEL)
    {
        mutex.unlock();
        return;
    }

    // Format message
    QString tid = QString("%1").arg(quintptr(QThread::currentThreadId()));
    QString filePath = QString::fromUtf8(context.file);
    QString fileName = QFileInfo(filePath).fileName();
    QString formattedMessage;
    formattedMessage = QString("[%1] [%2] [%3] [%4:%5] %6")
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
                           .arg(levelStr)
                           .arg(tid)
                           .arg(fileName)
                           .arg(context.line)
                           .arg(msg);

    // Log file handling
    QDir dir(LOG_FILE_PATH);
    if (!dir.exists())
    {
        QDir().mkpath(LOG_FILE_PATH);
    }

    QString logFileName = QDateTime::currentDateTime().toString("yyyy-MM-dd") + QString(".log");
    QString fullPath = LOG_FILE_PATH + logFileName;

    // Check file size and rotate if necessary
    QFile file(fullPath);
    if (file.exists() && file.size() > MAX_LOG_FILE_SIZE)
    {
        // Rotate logs
        QString lastLogName = QString("%1%2.%3.log")
                                  .arg(LOG_FILE_PATH)
                                  .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"))
                                  .arg(MAX_LOG_FILES);
        QFile lastLogFile(lastLogName);
        if (lastLogFile.exists())
        {
            lastLogFile.remove();
        }
        for (int i = MAX_LOG_FILES - 1; i > 0; i--)
        {
            QString oldName = QString("%1%2.%3.log")
                                  .arg(LOG_FILE_PATH)
                                  .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"))
                                  .arg(i);
            QString newName = QString("%1%2.%3.log")
                                  .arg(LOG_FILE_PATH)
                                  .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"))
                                  .arg(i + 1);
            if (QFile::exists(oldName))
            {
                QFile::rename(oldName, newName);
            }
        }

        // Rename current log to .1
        QString firstBackup =
            QString("%1%2.1.log").arg(LOG_FILE_PATH).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
        if (QFile::exists(fullPath))
        {
            QFile::rename(fullPath, firstBackup);
        }
    }

    // Write to file
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << formattedMessage << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}

// Function to set up logger with specified log file path, max file size, and max log files
inline void setup_logger(const QString& logFilePath, qint64 maxLogFileSize, int maxLogFiles)
{
    // Ensure path ends with separator
    LOG_FILE_PATH = logFilePath;
    if (!LOG_FILE_PATH.endsWith(QDir::separator()))
    {
        LOG_FILE_PATH += QDir::separator();
    }

    // Set maximum log file size and number of log files
    MAX_LOG_FILE_SIZE = maxLogFileSize;
    MAX_LOG_FILES = maxLogFiles;

    // Initialize log level from config file
    QString configFilePath = LOG_FILE_PATH + "config.ini";
    initLogLevel(configFilePath);

    // Install message handler
    qInstallMessageHandler(outputMessage);
}
