#pragma once

#include <QObject>
#include <QTimer>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

class CameraController : public QObject
{
    Q_OBJECT

public:
    explicit CameraController(QObject *parent = nullptr);
    ~CameraController() override;

    bool play();
    void pause();
    void resume();
    void forward(int frameCount = 5);
    void rewind(int frameCount = 5);
    bool setResolution(int width, int height);

signals:
    void frameReady(const QImage &frame);
    void errorOccurred(const QString &message);
    void warningOccurred(const QString &message);
    void resolutionChanged(int width, int height);
    void stateChanged(const QString &state);

private slots:
    void captureFrame();

private:
    cv::VideoCapture capture_;
    QTimer timer_;
    cv::Mat lastFrame_;
    QList<cv::Mat> history_;
    QList<cv::Mat> redoHistory_;
    int width_ = 1280;
    int height_ = 720;
    bool playing_ = false;

    void setPlayingState(bool playing);
    void applyResolution(int requestedWidth, int requestedHeight);
};
