#include "cam.h"

#include "Data/define.h"
#include "Sensor/cameraparams.h"

#include <QColor>
#include <QDebug>
#include <QTimer>
#include <QMutex>
#include <QThread>

std::atomic_bool Cam::streamCamEnabled;

// Konstruktor
Cam::Cam()
{
    streamCamEnabled = false;
    cp.updatePeriod = 25;
    cp.source = 0;

    timer = new QTimer;
    mutexVideoCapture = new QMutex;

    // open video capture
    mutexVideoCapture->lock();
    videoCapture.open(cp.source);
    if(!videoCapture.isOpened()){
        enabled = false;
        mutexVideoCapture->unlock();
        qWarning() << "Could not open Camera";
        return;
    }
    mutexVideoCapture->unlock();

    color = CamColor::NONE;

    connect(timer, SIGNAL(timeout()), this, SLOT(timerSendFrame()));
    timer->start(cp.updatePeriod);
}

// Destruktor
Cam::~Cam()
{
    delete timer;
    delete mutexVideoCapture;
    videoCapture.release();
}

CamColor Cam::getLastColor()
{
    return color;
}

void Cam::slotSetCameraParams(CameraParams cameraParams)
{

    if(cameraParams.source != cp.source){
        QMutexLocker lock(mutexVideoCapture);
        videoCapture.release();
        videoCapture.open(cp.source);
        if(!videoCapture.isOpened()){
            enabled = false;
            std::cout << "Cam::slotSetCameraParams(): Could not open camera #" << cameraParams.source << std::endl;
            return;
        }
        std::cout << "Cam::slotSetCameraParams(): Now using camera #" << cameraParams.source << std::endl;
    }

    if(videoCapture.isOpened() && cameraParams.height != cp.height) {
        QMutexLocker lock(mutexVideoCapture);
        videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, cp.height );
    }

    if(videoCapture.isOpened() && cameraParams.width != cp.width)
    {
        QMutexLocker lock(mutexVideoCapture);
        videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, cp.width );
    }

    if(cameraParams.updatePeriod != cp.updatePeriod){
        timer->setInterval(cp.updatePeriod);
        timer->start();
    }

    cp = cameraParams;

}

void Cam::timerSendFrame() {
    if(streamCamEnabled) {
        Q_EMIT signalDisplayFrame(grabFrameAndColor());
    }
}

void Cam::slotStartColorDetection()
{
    if(config::enableDebugCam)
        qDebug() << "Color Detection Signal received";

    if(!videoCapture.isOpened()){
        if(config::enableDebugCam)
            qCritical() << "Keine Kamera vorhanden mit der die Farbe bestimmt werden könnte";
        Q_EMIT signalColorDetected(CamColor::ERROR);
    } else {
        grabFrameAndColor();

        // Die letzte erkannte Farbe ist noch in der member variable color gespeichert, gebe einfach diese zurück

        switch(color){
        case CamColor::BLUE:
            if(config::enableDebugCam)
                qDebug() << "Erkannte Farbe: Blau";
            break;
        case CamColor::GREEN:
            if(config::enableDebugCam)
                qDebug() << "Erkannte Farbe: Grün";
            break;
        case CamColor::YELLOW:
            if(config::enableDebugCam)
                qDebug() << "Erkannte Farbe: Gelb";
            break;
        case CamColor::NONE:
            if(config::enableDebugCam)
                qDebug() << "Erkannte Farbe: Keine";
            break;
        default:
            color = CamColor::ERROR;
            if(config::enableDebugCam)
                qDebug() << "Erkannte Farbe: Fehler";
            return;
        }

        Q_EMIT signalColorDetected(color);
    }
}

cv::Mat Cam::grabFrameAndColor()
{
    cv::Mat frame;

    // Ein paar frames ins leere lesen, weil manchmal scheinbar alte Bilder gebuffert werden
    for(int i=0; i<10; ++i){
        mutexVideoCapture->lock();
        videoCapture >> frame;
        mutexVideoCapture->unlock();
    }

    // Experimental
    if(0) {
        // Grab three Frames and take the average
        cv::Mat frame1, frame2, frame3;
        mutexVideoCapture->lock();
        videoCapture >> frame1;
        videoCapture >> frame2;
        videoCapture >> frame3;
        mutexVideoCapture->unlock();

        cv::add(frame1, frame2, frame);
        cv::add(frame, frame3, frame);
        for(int y=0; y<frame.rows; ++y){
            for(int x=0; x<frame.cols; ++x){
                cv::Vec3b px = frame.at<cv::Vec3b>(y,x);
                frame.at<cv::Vec3b>(y,x) = cv::Vec3b(px[0] / 3, px[1] / 3, px[2] / 3);
            }
        }
    } else {
        // Take a picture
        mutexVideoCapture->lock();
        videoCapture >> frame;
        mutexVideoCapture->unlock();
    }

    // Find ROI
    int width = frame.cols * config::CAM_ROI_WIDTH_RELATIVE;
    int height = frame.rows * config::CAM_ROI_HEIGHT_RELATIVE;
    int left = frame.cols * config::CAM_ROI_OFFSET_HORIZONTAL_RELATIVE;
    int top = frame.rows * config::CAM_ROI_OFFSET_VERTICAL_RELATIVE;

    // Find mean value in ROI
    cv::Rect roi(left,top,width,height);
    cv::Mat frameRoi = frame(roi);

    // Old implmentation
    if(0) {
        cv::Scalar meanScalar = cv::mean(frameRoi);
        QColor meanColor(meanScalar[2], meanScalar[1], meanScalar[0]);

        // Check if mean color falls in color bounds
        color = getPixelColor(meanColor);

    // New implementation
    } else {
        // Alternative: ein OpenCV Lookup-Table benutzen?

        // Alle Pixel im ROI klassifizieren
        double numPixels = 0, numBlue = 0, numYellow = 0, numGreen = 0, numNone = 0;
        for(int y=0; y<frameRoi.rows; y++) {
            for(int x=0; x<frameRoi.cols; x++) {
                cv::Vec3b v = frameRoi.at<cv::Vec3b>(y,x);
                CamColor c = getPixelColor(QColor(v[2], v[1], v[0]));
                if(c == CamColor::BLUE)
                    numBlue++;
                else if(c == CamColor::YELLOW)
                    numYellow++;
                else if(c == CamColor::GREEN)
                    numGreen++;
                else if(c == CamColor::NONE)
                    numNone++;
                numPixels++;
            }
        }

        // Die Farbe mit den meisten Pixeln übernehmen
        if (numNone / numPixels > config::CAM_PERCENTAGE_NON_COLOR_DETECTION) {
            color = CamColor::NONE;
        } else {
            if(numBlue >= numYellow && numBlue >= numGreen)
                color = CamColor::BLUE;
            else if(numYellow >= numBlue && numYellow >= numGreen)
                color = CamColor::YELLOW;
            else if(numGreen >= numBlue && numGreen >= numYellow)
                color = CamColor::GREEN;
            else
                color = CamColor::NONE; // Kann das jemals passieren? Ich hoffe nicht.
        }

        // Wenn das Bild in der GUI angezeigt werden wird
        if(streamCamEnabled) {

            // draw ROI bounds
            cv::rectangle(frame, roi, cv::Scalar(0,0,255), 2);

            // Semitransparenter Schwarzer Block als Text Hintergrund
            cv::Mat black = frame.clone();
            cv::rectangle(black, cv::Rect(0, 0, 95, 100), cv::Scalar(0), -1);
            cv::addWeighted(frame, 0.5, black, 0.5, 1.0, frame);

            // Text
            std::string s;
            s.assign("N: ");
            s.append(std::to_string((int) floor(100*numNone/numPixels)));
            s.append("%");
            if(color == CamColor::NONE) s.append(" X");
            cv::putText(frame, s, cv::Point(5, 20), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255,255,255));
            s.assign("B: ");
            s.append(std::to_string((int) floor(100*numBlue/numPixels)));
            s.append("%");
            if(color == CamColor::BLUE) s.append(" X");
            cv::putText(frame, s, cv::Point(5, 40), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255,255,255));
            s.assign("Y: ");
            s.append(std::to_string((int) floor(100*numYellow/numPixels)));
            s.append("%");
            if(color == CamColor::YELLOW) s.append(" X");
            cv::putText(frame, s, cv::Point(5, 60), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255,255,255));
            s.assign("G: ");
            s.append(std::to_string((int) floor(100*numGreen/numPixels)));
            s.append("%");
            if(color == CamColor::GREEN) s.append(" X");
            cv::putText(frame, s, cv::Point(5, 80), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255,255,255));
        }

    }

    return frame;
}

CamColor Cam::getPixelColor(const QColor &pixel) const
{
    // Check if pixel color falls in color bounds
    if(      pixel.hue() >= cp.colorBlueMin.hue() && pixel.saturation() >= cp.colorBlueMin.saturation() && pixel.value() >= cp.colorBlueMin.value() &&
             pixel.hue() <= cp.colorBlueMax.hue() && pixel.saturation() <= cp.colorBlueMax.saturation() && pixel.value() <= cp.colorBlueMax.value() )
    {
        return CamColor::BLUE;
    }
    else if( pixel.hue() >= cp.colorYellowMin.hue() && pixel.saturation() >= cp.colorYellowMin.saturation() && pixel.value() >= cp.colorYellowMin.value() &&
             pixel.hue() <= cp.colorYellowMax.hue() && pixel.saturation() <= cp.colorYellowMax.saturation() && pixel.value() <= cp.colorYellowMax.value() )
    {
        return CamColor::YELLOW;
    }
    else if( pixel.hue() >= cp.colorGreenMin.hue() && pixel.saturation() >= cp.colorGreenMin.saturation() && pixel.value() >= cp.colorGreenMin.value() &&
             pixel.hue() <= cp.colorGreenMax.hue() && pixel.saturation() <= cp.colorGreenMax.saturation() && pixel.value() <= cp.colorGreenMax.value() )
    {
        return CamColor::GREEN;
    }
    return CamColor::NONE;
}
