#include "tab_camac_paint.h"
#include <QtGui>


camac_paint::camac_paint(QWidget *parent) : QWidget(parent)
  //,
 //   ui(new Ui::plane)
{
//    ui->setupUi(this);
    QPalette palette;
    KK_connected = FALSE;
    transfer_flashing = FALSE;
    N_X = FALSE;
    palette.setColor(QPalette::Light, Qt::gray);
    setPalette(palette);
    setBackgroundRole(QPalette::Light);
    setAutoFillBackground(true);
    setAttribute(Qt::WA_StaticContents);

 //   p_first.setX(250);
 //   p_first.setY(50);
//    d1 = 1;

}
//+++++++++++++++++++
/*
void camac_paint::paintEvent(QPaintEvent *event)
{
  //  QWidget::paintEvent(event);
    Q_UNUSED(event);
    QPainter ekPaint(this);
    QPen framepen;

     QPalette palette;


    if (KK_connected && !transfer_flashing)
    {

        palette.setColor(QPalette::Light, Qt::green);



    }

    if  (!KK_connected)
     {
        palette.setColor(QPalette::Light, Qt::red);


    }
    //QRect rec(180, 780,30,30);

    if (KK_connected && transfer_flashing )
    {
         palette.setColor(QPalette::Light, Qt::gray);
    }



//============== SET X palette
    if  ((N_X) && !transfer_flashing && !KK_connected)
     {
        palette.setColor(QPalette::Light, Qt::red);


    }
     if  ((!N_X) && !transfer_flashing && !KK_connected)
    {
       palette.setColor(QPalette::Light, Qt::black);


   }
//+++++++++++++++++++++++


    setPalette(palette);

    setBackgroundRole(QPalette::Light);
    setAutoFillBackground(true);
    setAttribute(Qt::WA_StaticContents);

    framepen.setWidth(10);
    framepen.setColor(Qt::green);

    ekPaint.setPen(framepen);

 //   ekPaint.drawEllipse(8, 8, 15,15 ); //кружок №1

    //+++++++++++
    framepen.setWidth(1);
    framepen.setColor(Qt::yellow);
    ekPaint.setPen(framepen);
    ekPaint.drawLine(0,0, width(), height());



    QPainter painter(this);
    static QPixmap *doubleBuffer = 0;
    if (!doubleBuffer)
    {
        doubleBuffer = new QPixmap(1,1);
    }
    *doubleBuffer = doubleBuffer->scaled(d1, d1, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    QRect rect;
    rect.setSize(QSize(d1, d1));
    QPainter dbPainter(doubleBuffer);
    doubleBuffer->fill(Qt::black);
    dbPainter.setRenderHint(QPainter::Antialiasing, true);
    dbPainter.setBrush(Qt::magenta);
    dbPainter.setPen(Qt::darkMagenta);
    dbPainter.drawEllipse(rect);
    painter.drawPixmap(p_first, *doubleBuffer);
*/
//}

