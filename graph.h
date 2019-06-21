/*
Класс Plot дочерний для  public QwtPlot
Будет рисовать гистограммы
*/

#ifndef GRAPH
#define GRAPH

#include <qlayout.h>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_point_data.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_text.h>
#include <qwt_symbol.h>
#include <qwt_math.h>
#include <math.h>

#include <qwt_scale_engine.h>
#include <qwt_scale_widget.h>

#include <qwt_plot_histogram.h>
#include <QtGui>
#include <QList>

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Отрисовка гистограммы
class Plot : public QwtPlot
{
    Q_OBJECT

public:
    Plot( QWidget *parent, const char *w_title, double x_min, double x_max, double y_min, double y_max, BOOL isTrigonometric ); //= NULL. Теперь не NULL, т.к. будет родитель - Альфа, fission виджет
    QwtPlotMagnifier    *Magnifier_param; //СССУУУУУУУУУУУККККАААААААААААА !!!!!!
                                          // НЕ ПУТАТЬ QwtPlotMagnifier И QwtMagnifier !!!!!!!!!
 //   void Redraw_Histogram();

    QwtScaleWidget *fiss_scale;
    QwtScaleWidget *hist_scale;
    double fis_magn;     //Переменная передает умножитель зума в главное окно, для тестов, вывода на экран.

    QwtPlotHistogram *hystogram;
    QVector<QwtIntervalSample> *intervals;
    UINT16 hData[4096];
    double Ch_Data; //Тестовые данные для 1 канала.
    double Ch_Data_2; //Тестовые данные для другого канала.

    double Y_MAX_Current; // Текущая верхня граница графика

    int N_number;       //Номер станции глобальный
    UINT8 command_type;

    // void Draw_Spectral_N_Chan (QByteArray *qbuf, UINT8 N_number, UINT8 Chan_number); //Выборка данных для заданной станции и канала в этой станции,
                                                                                       //добавление этих данных к гистограмме


 //   void ek_drawLines(void);





public slots:
  double return_param ();
//  void onTimer_1(void);  //Обработчик таймера
//  void onTimer_2(void);  //Обработчик таймера 2
  // Слот-обработка массива qbuf на выборку данных для заданной станции N и канала Chan_number и отрисовку гистограммы  по этим данным
  void Draw_Spectral_N_Chan_Slot (QByteArray *);
  void pass_N_SPECTRAL_slot (UINT8 * st_number, UINT8 comm_type);

signals  :
          void fis_resized();
          void Show_data_signal(QString sss);          // Сигнал для отображения данных в Log

protected:
    virtual void resizeEvent( QResizeEvent *);
 //   virtual void widgetWheelEvent (QWheelEvent *);

private:
  //  void populate_trigonometric();
    void populate_histogram();
    void updateGradient();
    void add_Point(UINT16 chan_num);
};

#endif // GRAPH

