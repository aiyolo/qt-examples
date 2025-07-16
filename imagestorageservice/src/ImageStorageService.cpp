#include "ImageStorageService.h"
#include <QDir>
#include <QDateTime>
#include <QFileInfo>
#include <qglobal.h>
#include <qdebug.h>

ImageSaveTask::ImageSaveTask(const cv::Mat& image, const QString& imagePath)
    : image_(image)
    , imagePath_(imagePath)
{}

void ImageSaveTask::run()
{
    QFileInfo fileInfo(imagePath_);
    QString saveFolder = fileInfo.absolutePath();

    QDir dir(saveFolder);
    if (!dir.exists()) {
        if (!dir.mkpath(saveFolder)) {
            qCritical() << "Failed to create directory:" << saveFolder;
            return;
        }
    }
    cv::imwrite(imagePath_.toStdString(), image_);
    qDebug() << "==Saved image:" << imagePath_;
}

ImageStorageService::ImageStorageService()
    : threadPool_(this)
{
    threadPool_.setMaxThreadCount(1);
}

ImageStorageService::~ImageStorageService()
{
    if (!threadPool_.waitForDone(5000)) {
        qWarning() << "Timeout expired while waiting for image save tasks to complete. Some tasks may not be saved.";
    }
    threadPool_.clear();
}

void ImageStorageService::release()
{
    if (instance_) {
        delete instance_;
        instance_ = nullptr;
    }
}

ImageStorageService* ImageStorageService::Ins()
{
    if (!instance_) {
        instance_ = new ImageStorageService();
    }
    return instance_;
}

void ImageStorageService::saveImageAsync(const QString& imagePath, const cv::Mat& image)
{
    if (image.empty()) {
        qCritical() << "Image is empty: " << imagePath;
        return;
    }
    ImageSaveTask* task = new ImageSaveTask(image, imagePath);
    threadPool_.start(task);
}
