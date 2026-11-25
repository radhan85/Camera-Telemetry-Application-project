#include "CameraController.h"

#include <QImage>
#include <opencv2/imgproc.hpp>

namespace {
QImage matToQImage(const cv::Mat &source)
{
    cv::Mat rgba;
    cv::cvtColor(source, rgba, cv::COLOR_BGR2RGBA);
    return QImage(rgba.data, rgba.cols, rgba.rows, static_cast<int>(rgba.step),
                  QImage::Format_RGBA8888).copy();
}
}

CameraController::CameraController(QObject *parent)
    : QObject(parent)
{
    connect(&timer_, &QTimer::timeout, this, &CameraController::captureFrame);
    timer_.setTimerType(Qt::PreciseTimer);
}

CameraController::~CameraController()
{
    timer_.stop();
    capture_.release();
}

void CameraController::setPlayingState(bool playing)
{
    playing_ = playing;
    emit stateChanged(playing_ ? "Playing" : "Paused");
}

bool CameraController::play()
{
    if (!capture_.isOpened()) {
        capture_.open(0);
    }
    if (!capture_.isOpened()) {
        emit errorOccurred("Unable to access the camera. Check that it is connected and not in use.");
        setPlayingState(false);
        return false;
    }

    capture_.set(cv::CAP_PROP_FRAME_WIDTH, width_);
    capture_.set(cv::CAP_PROP_FRAME_HEIGHT, height_);
    redoHistory_.clear();
    timer_.start(33);
    setPlayingState(true);
    return true;
}

void CameraController::stop()
{
    timer_.stop();
    capture_.release();
    history_.clear();
    redoHistory_.clear();
    lastFrame_.release();
    playing_ = false;
    emit stateChanged("Stopped");
}

void CameraController::pause()
{
    timer_.stop();
    setPlayingState(false);
}

void CameraController::resume()
{
    if (!capture_.isOpened()) {
        if (!play()) {
            return;
        }
    } else {
        timer_.start(33);
        setPlayingState(true);
    }
}

void CameraController::captureFrame()
{
    cv::Mat frame;
    if (!capture_.read(frame) || frame.empty()) {
        timer_.stop();
        setPlayingState(false);
        emit errorOccurred("The camera stopped delivering frames.");
        return;
    }

    lastFrame_ = frame.clone();
    history_.append(lastFrame_);
    if (history_.size() > 300) {
        history_.removeFirst();
    }
    redoHistory_.clear();
    emit frameReady(matToQImage(frame));
}

void CameraController::forward(int frameCount)
{
    if (!capture_.isOpened()) {
        emit errorOccurred("Start playback before using Forward.");
        return;
    }
    for (int i = 0; i < frameCount; ++i) {
        cv::Mat frame;
        if (capture_.read(frame) && !frame.empty()) {
            lastFrame_ = frame.clone();
            history_.append(lastFrame_);
            if (history_.size() > 300) history_.removeFirst();
        }
    }
    if (!lastFrame_.empty()) emit frameReady(matToQImage(lastFrame_));
}

void CameraController::rewind(int frameCount)
{
    for (int i = 0; i < frameCount && history_.size() > 1; ++i) {
        redoHistory_.prepend(history_.takeLast());
    }
    if (!history_.isEmpty()) {
        lastFrame_ = history_.last().clone();
        emit frameReady(matToQImage(lastFrame_));
    }
}

bool CameraController::setResolution(int width, int height)
{
    if (width <= 0 || height <= 0) {
        emit errorOccurred("Invalid camera resolution.");
        return false;
    }
    width_ = width;
    height_ = height;

    if (capture_.isOpened()) {
        capture_.set(cv::CAP_PROP_FRAME_WIDTH, width_);
        capture_.set(cv::CAP_PROP_FRAME_HEIGHT, height_);
    }
    emit resolutionChanged(width_, height_);
    return true;
}
