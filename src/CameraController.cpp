#include "CameraController.hpp"

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

    applyResolution(width_, height_);
    redoHistory_.clear();
    timer_.start(33);
    setPlayingState(true);
    return true;
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

    if (frame.cols != width_ || frame.rows != height_) {
        width_ = frame.cols;
        height_ = frame.rows;
        emit warningOccurred(QString("Camera is delivering %1 x %2 frames.")
                                .arg(width_).arg(height_));
        emit resolutionChanged(width_, height_);
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
    int framesAdvanced = 0;
    while (framesAdvanced < frameCount && !redoHistory_.isEmpty()) {
        lastFrame_ = redoHistory_.takeFirst().clone();
        history_.append(lastFrame_);
        if (history_.size() > 300) {
            history_.removeFirst();
        }
        ++framesAdvanced;
    }

    if (framesAdvanced < frameCount && !capture_.isOpened()) {
        emit errorOccurred("Start playback before using Forward.");
    } else {
        for (int i = framesAdvanced; i < frameCount; ++i) {
            cv::Mat frame;
            if (capture_.read(frame) && !frame.empty()) {
                lastFrame_ = frame.clone();
                history_.append(lastFrame_);
                if (history_.size() > 300) {
                    history_.removeFirst();
                }
            }
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
    if (!capture_.isOpened()) {
        width_ = width;
        height_ = height;
        emit resolutionChanged(width_, height_);
        return true;
    }

    const bool wasPlaying = timer_.isActive();
    timer_.stop();
    capture_.release();
    if (!capture_.open(0)) {
        setPlayingState(false);
        emit errorOccurred("Unable to reopen the camera to change resolution.");
        return false;
    }

    applyResolution(width, height);
    if (wasPlaying) {
        timer_.start(33);
        setPlayingState(true);
    }
    return true;
}

void CameraController::applyResolution(int requestedWidth, int requestedHeight)
{
    capture_.set(cv::CAP_PROP_FRAME_WIDTH, requestedWidth);
    capture_.set(cv::CAP_PROP_FRAME_HEIGHT, requestedHeight);

    const double negotiatedWidth = capture_.get(cv::CAP_PROP_FRAME_WIDTH);
    const double negotiatedHeight = capture_.get(cv::CAP_PROP_FRAME_HEIGHT);
    width_ = negotiatedWidth > 0 ? static_cast<int>(negotiatedWidth) : requestedWidth;
    height_ = negotiatedHeight > 0 ? static_cast<int>(negotiatedHeight) : requestedHeight;

    if (width_ != requestedWidth || height_ != requestedHeight) {
        emit warningOccurred(QString("Requested resolution %1 x %2 is unavailable; using %3 x %4.")
                                 .arg(requestedWidth).arg(requestedHeight)
                                 .arg(width_).arg(height_));
    }
    emit resolutionChanged(width_, height_);
}
