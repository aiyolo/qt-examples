#pragma once

#include <QObject>
#include <QMutex>
#include <QThreadPool>
#include <opencv2/opencv.hpp>
#include <QString>

class ImageSaveTask : public QRunnable
{
public:
    ImageSaveTask(const cv::Mat& image, const QString& imagePath);

    void run() override;

private:
    cv::Mat image_;
    QString imagePath_;
};

class ImageStorageService : public QObject
{
    Q_OBJECT

public:
    static ImageStorageService* Ins();
    static void release();

    ImageStorageService(const ImageStorageService&) = delete;
    ImageStorageService& operator=(const ImageStorageService&) = delete;

    void saveImageAsync(const QString& imagePath, const cv::Mat& image);

signals:
    void errorOccurred(const QString& error);

private:
    ImageStorageService();
    ~ImageStorageService();

    QMutex mutex_;
    QThreadPool threadPool_;
    static inline ImageStorageService* instance_ = nullptr;
};
