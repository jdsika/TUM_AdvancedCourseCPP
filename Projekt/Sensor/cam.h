#ifndef CAM_H
#define CAM_H

#include <atomic>

#include "Sensor/cameraparams.h"
#include <opencv2/opencv.hpp>

//struct CameraParams;
class QTimer;
class QMutex;

namespace cv
{
    class VideoCapture;
    class Mat;
}

enum class CamColor
{
    GREEN,  /**< recognised color*/
    YELLOW, /**< recognised color*/
    BLUE,   /**< recognised color*/
    NONE,   /**< initital color*/
    ERROR   /**< error signalisation*/
};  /**< recognised color*/

Q_DECLARE_METATYPE(CamColor)

/**
 * @brief The Cam class will stream cam data to GUI-panal for recognition of poles color.
 */
class Cam : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Cam default constructor for setting up the video stream
     */
    Cam();
    /**
      * @brief ~Cam default destructor
      */
    ~Cam();

    static std::atomic_bool streamCamEnabled;

    /**
     * @brief getLastColor
     * @return the last recognised color
     */
    CamColor getLastColor();

public Q_SLOTS:
    /**
     * @brief slotSetCameraParams
     * @param[in] cameraParams which are the new cam parameter
     */
    void slotSetCameraParams(CameraParams cameraParams);

    /**
     * @brief timerGrabFrame will determine which color is in front of the cam (called with an timer heartbeat)
     */
    void timerSendFrame();

    /**
     * @brief slotGetColor
     * @return the last gathered color.
     */
    void slotStartColorDetection();

Q_SIGNALS:
    /**
     * @brief signalDisplayFrame
     */
    void signalDisplayFrame(cv::Mat);

    /**
     * @brief signalColorDetected
     */
    void signalColorDetected(CamColor);

private:
    CameraParams cp; /**< parameters of the cam*/
    bool enabled;       /**< should the cam stream be enabled*/
    cv::VideoCapture videoCapture; /** captured video*/
    QTimer *timer; /**< timer to gather a new frame*/
    CamColor color; /**< internal color Enum*/
    QMutex *mutexVideoCapture; /**< mutex for the video capture*/

    /**
     * @brief grabFrameAndColor
     */
    cv::Mat grabFrameAndColor();

    /**
     * @brief getPixelColor
     * @param[in] pixel (QColor)
     * @return
     */
    CamColor getPixelColor(const QColor &pixel) const;
};


#endif // CAM_H
