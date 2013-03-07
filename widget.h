#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QTimer>
#include <QPainter>
#include "videodevice.h"

#include "cv.h"
#include "highgui.h"

extern int detect(IplImage* img,char * res);

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
private:
    Ui::Widget *ui;

    QPainter *painter;
        QLabel *label;
        QImage *frame;
        //QPixmap *frame;
        QTimer *timer;
        int rs;
        uchar *pp;
        uchar * p;
        unsigned int len;
        int convert_yuv_to_rgb_pixel(int y, int u, int v);
        int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);
        VideoDevice *vd;
        bool IsCapture;

    private slots:
        void paintEvent(QPaintEvent *);
        void display_error(QString err);
        void on_ButtonTake_clicked();
        void on_ButtonDetect_clicked();
};

#endif // WIDGET_H
