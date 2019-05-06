#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qt_windows.h>
#include <windows.h>
#include "ftd2xx.h"
#include "mythread.h"
#include <QTimer>
#include <QList>
#include <QRegExp>
//#include <QFile>
#include <QtGui>

#include <QDateTime>

//#include <QDataStream>
#include "ftdi_d2xx.h"
#include "graph.h"

#include "tab_camac_paint.h"

#include <limits>



//#define KK_version "KK_Tab_10"

#define mode_manual_KK "Режим \"РУЧНОЙ КОНТРОЛЛЕР\""

#define CAMAC_F27 27
#define CAMAC_F25 25
#define CAMAC_F17 17




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTimer      *timer; //Нужен для индикации, чтобы "лампочки" меняли цвет при смене подлкючения и т.д.
    QTimer      *timer_flash; //Таймер "мигания" передачи данных
    QString str;
    QString target_name;
//    void on_pushButton_Read_data_clicked();

    BOOL FTDI_Connected; //Флаг наличия связи с контроллером
    BOOL A_Open, B_Open; //Флаги открытия порта А, В
//+++++++++++++++++++++++++++++++++++++++++++++++++++ QWT +++++++++++++++++++++++
    Plot *Hyst_Single; // Виджет для Альфа-спектра одиночного, потом надо будет переименовать.
    Plot *ADP16_Alpha;  // Виджет для Альфа-спектра ADP16.
    Plot *ADP16_Fission;

    // Все же названия гистограмм должны соответствовать объектам, которые они отображают
    // Примерно так:
                // Hist_Single, Hist_Fission, Hist_Alpha, Hist_Time


    QwtScaleEngine * Fission_X_scaleEngine;
    QwtScaleEngine * Fission_Y_scaleEngine;
   // QList <QwtPlot *> widgetList;

    double sc_range; //scale range

    QTimer      *timer_1; // Следит за подключением FTDI
    QTimer      *timer_2; // Следит за буфером для вкладки SINLE
    QTimer      *timer_3; // Следит за буфером для вкладки ADP16 ALPHA
    QTimer      *timer_4; // Следит за буфером для вкладки ADP16 FISSION

  //  QWidgetList  *widgetList;
    QList <QWidget *> widgetList;

//    int imin ; // minimum value
    int imax ; // maximum int value [2 147 483 647]  for QwtInterval tracking

#define Fission_X_Limit 4100 // Fission Число точек по оси X - число каналов
#define Fission_Y_Limit 500

#define Probe_chan      0x07D0 // 2000 Номер канала отрисовки.
#define Probe_chan_2      0x07E9 // 2025 Номер канала отрисовки.

#define Spectral_Data_File   "./Camac_Thread_data_000.bin"
#define Log_File_Name        "./camac.log"
#define Camac_Commands_File  "./camac_commands.txt"

#define KK_Index 0
#define Single_Index 1
#define ADP16_Alpha_Index 2

#define COMMAND_b0 1
#define COMMAND_b2 3

#define Interval_Limit 0x186a0   //0x7a120 - 500 000 //0x4c4b40 // Это 5 000 000 - ограничение количества точек в интервале
//quint64 f_offset =0;
//quint64 f_size;

    //+++++++++++++++++++++++++++++ Threads +++++++++++++++
    // FT2232H has 2 channels A & B, So we create 2 Threads for A & B channel each one
    MyThread  *chan_A, *chan_B, *File_WR;
    QThread   *thread_A, *thread_B, *thread_F;
    FTDI_D2XX *KAN;
//++++++++++++++++++++++++++++++++++++++++
    void Init_Values(void); //задать начальные значения элементов интерфейса
    void CAMAC_make_command(void);  //На основе данных в окне пользователя формируем команду камак на отправку в USB
    void Update_Label(void);  //Обновляем значение 4 байт команды при изменении эл-тов управляения
    void Create_N_WidgetList(void);
    void ftdi_answer_parser(int); //Выдает сообщения в зависимости от результата работы ftdi_d2XX::ftdi_init()
    void Write_To_Log (int value, QString log_nessage); //Пишет в лог-файл номер ошибки value и сообщение message
    void Camac_SPEC_Command(UINT8 F_number, UINT16 F_DATA); // функция формирования спец-команд КК и отправки этой команды в КК.
    void Camac_Run_Always (void); // Запуск считывания из FTDI->ПК в потоке А, а не в ответ на отдельную команду.


    UINT8 N_CAMAC, A_CAMAC, F_CAMAC, Data_HB, Data_LB;
    UINT8 ADP16_TIME_HB, ADP16_TIME_LB;
    UINT8 ADP16_FISS_HB, ADP16_FISS_LB;
    UINT8 ADP16_ALPH_HB, ADP16_ALPH_LB;
    UINT8 N_RANGE_CAMAC[2]; //Массив или группа (Лист) выбранных станций для 1 команды КАМАК
    UINT8 N_SPECTRAL;         //Номер станции для отображения спектра в обычном блоке
    UINT8 N_SPECTRAL_CHANNEL; //Номер канала в станции для отображения спектра [для многоканальных блоков]


    UINT8 N_SPECTRAL_ADP16;         //Номер станции для отображения спектра ADP16
    UINT8 N_SPECTRAL_ADP16_CHANNEL; //Номер канала в станции для отображения спектра ADP16 [для многоканальных блоков]


#define N_MAXVALUE 23
#define A_MAXVALUE 15
#define F_MAXVALUE 31
#define N_MINVALUE  0 //[24-25] - занимает контроллер крейта.
#define A_MINVALUE  0
#define F_MINVALUE  0

    UINT16 ToDev_Bits; // Биты данных в команде для КАМАК
    UINT16 NAF_Bits;   // Биты  NAF в команде для КАМАК
    UINT32 CAMAC_WORD; // Команда КАМАК состоит из 4 байт.
    UINT16 cycle_counnter; // Счетчик циклов записи в файл, по нему выполняется команда flush

//+++++++++++++++++++++++++++++ PAINT EVENT STUFF ++++++++++++++++++++++++++++++++++++
    QPen framepen; //Лампочки сигналов X, Q
    BOOL X, Q;
    camac_paint *camac_tab;
    camac_paint *X_flag;
    camac_paint *Q_flag;
//++++++++++++++++++++++++++++++++ USB STAFF ++++++++++++
#define TRANSFER_SIZE 4
#define RECEIVE_SIZE  8 // 8 байт
#define EK_BUFFER_SIZE 64

    char byOutputBuffer[TRANSFER_SIZE]; // TRANSFER_SIZE Массив данных на отправку в USB, данные берем из GUI

    void Purge_IO_Buffers(void);
//    void Debug_Output(void);
    void Bits_To_Buttons(UINT16 data_bits);
    void Send_NAF (UINT8 N_num, UINT8 A_num, UINT8 F_num);
    int  Command_File_Execute (QString fname);
    int  parse_command_line(QString comstr);
//    int  Send_Inhibit();

public slots:
//    void on_GET_Stop_pushButton_clicked();
    void on_pushButton_stop_Sending_clicked();
    void Output_text_to_window(QString sss);
    void Device_Connected_Slot();
    void Device_NOT_connected_Slot();
    void On_Timer();
    void onTimer_1(void);  //Обработчик таймера 1 - запись в файл
//    void onTimer_2(void);  //Обработчик таймера 2 - отрисовка спектра-гистограмма

    void On_Transfer_Flashing();
    // Слот-обработка массива qbuf на выборку данных для заданной станции N и канала Chan_number и отрисовку гистограммы  по этим данным
//    void Draw_Spectral_N_Chan_Slot (QByteArray *); //, UINT8 N_number, UINT8 Chan_number QByteArray &qbuf

signals:

//    void Get_ftHandle_B();
//    void Stop_Sending_B();

//    void Get_ftHandle_A();
//    void Stop_Sending_A();

    void push_data_buffer(char * ); //OutBuffer Этот сигнал вызываем при формировании команды камак
    void Write_TO_File_Signal(); //Запись в файл по нажатию кнопки в GUI
//    void Get_Spectral_Points_Signal();
    void pass_N_SPECTRAL_signal (UINT8 *, UINT8);

private slots:

//++++++++++++++++++++++++++++++++++++++++++++++
    void on_SEND_TO_USB_pushButton_clicked();

    void on_N_spinBox_valueChanged(int arg1);
    void on_A_spinBox_valueChanged(int arg1);

    void on_F_spinBox_valueChanged(int arg1);

    void on_Clear_All_clicked();

    void on_Sel_Bit_16_toggled(bool checked);

    void on_Sel_Bit_15_toggled(bool checked);

    void on_Sel_Bit_14_toggled(bool checked);

    void on_Sel_Bit_13_toggled(bool checked);

    void on_Sel_Bit_12_toggled(bool checked);

    void on_Sel_Bit_11_toggled(bool checked);

    void on_Sel_Bit_10_toggled(bool checked);

    void on_Sel_Bit_09_toggled(bool checked);

    void on_Sel_Bit_08_toggled(bool checked);

    void on_Sel_Bit_07_toggled(bool checked);

    void on_Sel_Bit_06_toggled(bool checked);

    void on_Sel_Bit_05_toggled(bool checked);

    void on_Sel_Bit_04_toggled(bool checked);

    void on_Sel_Bit_03_toggled(bool checked);

    void on_Sel_Bit_02_toggled(bool checked);

    void on_Sel_Bit_01_toggled(bool checked);


    void on_Stop_ALL_pushButton_clicked();

 //   void on_Set_Data_pushButton_clicked();

 //   void on_Set_Data_pushButton_2_clicked();


    //+++++++++++++ camac concerned
    void Camac_N_Connected_Slot(int N_num); // Слот обработки Сигнала, что станция N на связи.
    void Camac_N_NOT_Connected_Slot(uint N_num); // Слот обработки Сигнала, что станция N !!! НЕ !!! на связи.

    void Camac_N_Q_Flag_Slot();
    void Camac_N_Q_NOT_Flag_Slot();

    void on_Start_Aquisition_pushButton_clicked();

    void on_Show_Selected_Spectral_pushButton_clicked();

    void on_Show_Spectral_Selected_From_File_pushButton_clicked();

    void Show_file_size_slot(QString sss, QString sss2);
    void Show_file_offset_slot (QString sss);

    void on_Load_commands_pushButton_clicked();

    void on_Show_Selected_Spectral_Single_pushButton_clicked();



    void on_N_Spectral_spinBox_Single_valueChanged(int arg1);

    void on_N_Spectral_Channel_spinBox_Single_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
protected:
//  virtual void paintEvent(QPaintEvent *e);

};

#endif // MAINWINDOW_H
