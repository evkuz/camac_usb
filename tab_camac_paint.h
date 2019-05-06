#ifndef TAB_CAMAC_PAINT
#define TAB_CAMAC_PAINT

#include <QWidget>
#include <QPainter>
#include <QPixmap>


class camac_paint : public QWidget
{
    Q_OBJECT

public:
    explicit camac_paint(QWidget *parent);
  //  ~plane();
    void set_coord(int d_1);
    QPoint p_first;
 //   int d1;
    bool pushed; // Флаг нажатия кнопки
    bool KK_connected; // Флаг подключения КК-устройства
    bool transfer_flashing; // Мигание передачи данных
    bool N_X; //Индикатор ответа Х от станции N

    QPalette palette;


protected:
// virtual   void paintEvent(QPaintEvent *event);

};

//===================




#endif // TAB_CAMAC_PAINT

