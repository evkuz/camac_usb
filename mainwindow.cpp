/*
Вся суть окна пользователя - дать ему возможность сформировать команду КАМАК.
Далее эта команда отправляется в USB стандартными средствами FTDI.

*/


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QByteArray>
#include <stdlib.h>
#include <stdio.h>
#include <QDataStream>
#include <QTextStream>
#include "ftdi_d2xx.h"

#include <qglobal.h>


char Mini_ID_A[] = "Camac_021216A"; //FT112244EKA
char Mini_ID_B[] = "Camac_021216B"; //FT112244EKB

//QTextStream uin;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // ui->FTDI_List_textBrowser->locale();

   // ui->Data2Send_lineEdit->setText("AABBCCDD0102CE3F");

//++++++++++++++++++++++++++++++
    Init_Values();
    Update_Label();
    Create_N_WidgetList();
    ui->tabWidget->setCurrentIndex(KK_Index); //Начинаем на вкладке "KK_manual"

 //   target_name = QCoreApplication::applicationFilePath();
    target_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();   // target_name +="\"" ;


     //ui->tab_Single_NOT_ADP16->setStyleSheet("background-color: rgb(191, 153, 255);");


    update();
    //++++++++++++++++++++ Задаем частоты обмена с КАМАК ++++++++++
    QStringList Freq_List;
    Freq_List << "No cycle" << "5KHz" << "10KHz" << "20KHz" << "2KHz" << "333Hz";

    ui->Frequency_comboBox->addItems(Freq_List);
    ui->Frequency_comboBox->setCurrentIndex(0); // 0 means "No Cycle", 1 means "5kHz",

    ui->Show_Spectral_Selected_From_File_Single_pushButton->setVisible(false);

    ui->pushButton_stop_Sending->setVisible(false); //Кнопку "Stop Sending" делаем невидимой

   // ui->tabWidget->setCurrentIndex(2);

//+++++++++++++++++++++++++++++++++++++ Объект класса FTDI устройства ++++++++++++++

    KAN = new FTDI_D2XX();
// Нужна обработка исключения

//     KAN->Mini_ID_A = "Camac_021216A"; //FT112244EKA
//     KAN->Mini_ID_B = "Camac_021216B"; //FT112244EKB

//    KAN->ftdi_init();
    ftdi_answer_parser( KAN->Log_File_Open(Log_File_Name) );      // Open text log-file
    //uin(&KAN->LogFile);

    ftdi_answer_parser(0xf000); // Write to log-file first message
    ftdi_answer_parser( KAN->File_Open(Spectral_Data_File) ); //Open binary data file

    timer = new QTimer(this);
    connect(timer, SIGNAL( timeout() ),this , SLOT( On_Timer() ) );

    timer_flash = new QTimer(this);
    connect (timer_flash, SIGNAL (timeout()), this, SLOT(On_Transfer_Flashing()) );
  //  timer->start(500); //Проверяем наличие подключения. Запускаем таймер пока потоки не запустили.
    connect (KAN, SIGNAL(Show_data(QString)), this, SLOT(Output_text_to_window(QString)));
    connect (KAN, SIGNAL(Device_connected_Signal()), this, SLOT(Device_Connected_Slot()) );
    connect (KAN, SIGNAL(Device_NOT_connected_Signal()), this, SLOT(Device_NOT_connected_Slot()) );

    connect (KAN, SIGNAL(Camac_N_Connected_Signal(int )), this, SLOT(Camac_N_Connected_Slot(int)) );
    connect (KAN, SIGNAL(Camac_N_NOT_Connected_Signal(int)), this, SLOT(Camac_N_NOT_Connected_Slot(int)) );

    connect (KAN, SIGNAL(Camac_N_Q_Flag_Signal()), this, SLOT(Camac_N_Q_Flag_Slot()) );
    connect (KAN, SIGNAL(Camac_N_Q_NOT_Flag_Signal()), this, SLOT(Camac_N_Q_NOT_Flag_Slot()) );



    connect (KAN, SIGNAL(Show_file_size_signal(QString,QString )), this, SLOT(Show_file_size_slot(QString,QString )) );
    connect (KAN, SIGNAL(Show_file_offset_signal(QString)), this, SLOT(Show_file_offset_slot(QString)) );
//    connect (KAN, SIGNAL(Draw_Spectral_N_Chan_Signal(QByteArray *)), this, SLOT(Draw_Spectral_N_Chan_Slot(QByteArray *)) ); //QByteArray
                                                            //,UINT8,UINT8                                      //,UINT8,UINT8
//KAN->ftdi_init();
 //   Init_Values();
 //   Update_Label();

    // ============================================== Создаем поток 1 - Канал А
        thread_A = new QThread;
        chan_A = new MyThread();

        connect(thread_A, SIGNAL(started()), chan_A, SLOT(A_SLOT()) );
        connect(chan_A, SIGNAL(finished()), thread_A, SLOT(quit()));
        connect(chan_A, SIGNAL(Process_A()), KAN, SLOT(Read_AAA()), Qt::QueuedConnection); //Считываем из железки в ПК по каналу А
                                                                       // Qt::DirectConnection
                                                                       // Qt::QueuedConnection
                                                                       // Qt::BlockingQueuedConnection
                                                                       // Qt::DirectConnection
/*
 * "2)Direct Connection The slot is invoked immediately, when the signal is emitted. The slot is executed in the emitter's thread,
 *    which is not necessarily the receiver's thread.

3)Queued Connection The slot is invoked when control returns to the event loop of the receiver's thread.
  The slot is executed in the receiver's thread.

  Blocking Queued Connection The slot is invoked as for the Queued Connection,
  except the current thread blocks until the slot returns. Note: Using this type to connect objects in the same thread will cause deadlock."
*/
        chan_A->moveToThread(thread_A);
        chan_A->finthread = FALSE;

   // ============================================== Создаем поток 2 - Канал B
            thread_B = new QThread;
            chan_B = new MyThread();

            connect(thread_B, SIGNAL(started()), chan_B, SLOT(B_SLOT()) );
            connect(chan_B, SIGNAL(finished()), thread_B, SLOT(quit()));
            connect(chan_B, SIGNAL(Process_B()), KAN, SLOT(Write_B_Thread())); //Write_B_Thread
            chan_B->finthread = FALSE;                                         //Qt::DirectConnection
            chan_B->moveToThread(thread_B);                                    //Qt::BlockingQueuedConnection


           ui->SEND_TO_USB_pushButton->setFocus();

//++++++++++++++++++++++
           connect (this, SIGNAL (push_data_buffer(char *)), KAN, SLOT(Get_Buffer_Slot(char *)) ); //Вот это и есть "передать указатель в другой класс" ???!
// Разобрался. Нужно закрывать файл в деструкторе класса.
           connect (this, SIGNAL (Write_TO_File_Signal()), KAN, SLOT (Write_TO_File_Slot()) ); // Проверяем запись в файл.



//++++++++++++++++++++++++++++++++++++++++++++ QWT +++++++++++++++++++++
           //           widgetList.append(ui->Qwt_Widget_Alpha);  //Alpha widget
           //           widgetList.append(ui->Qwt_Histogram_Fission); //Fission widget

           //num = 1;//Выбор графика. Сначала №2, т.е. Qwt_Histogram_Fission
           //Hyst_Single = new Plot (ui->ALPHA_widget, "Histogram", 0.0, Fission_X_Limit, 0.0, 4096, FALSE); //4096
           Hyst_Single = new Plot (ui->SINGLE_widget, "Histogram", 0.0, Fission_X_Limit, 0.0, 4096, FALSE); //4096
           Hyst_Single->resize(800,400);

           Hyst_Single->Y_MAX_Current = 4096;
           Hyst_Single->intervals  = new QVector<QwtIntervalSample>;
           Hyst_Single->Ch_Data = 0.0;//1000000.0;// 0x0000;
           Hyst_Single->Ch_Data_2 = 0.0;//0x0000;

           // Следующая вкладка - Альфа-спектр  ADP16

           ADP16_Alpha = new Plot (ui->ALPHA_widget, "ADP16 Alpha", 0.0, Fission_X_Limit, 0.0, 4096, FALSE);
           ADP16_Alpha->resize(800, 400);

           ADP16_Alpha->Y_MAX_Current = 4096;
           ADP16_Alpha->intervals  = new QVector<QwtIntervalSample>;
           ADP16_Alpha->Ch_Data = 0.0;//0x0000;
           ADP16_Alpha->Ch_Data_2 = 0.0;//0x0000;


           // ADP16 Fission ADP16_Fission
           ADP16_Fission = new Plot (ui->FISSION_widget, "ADP16 Fission", 0.0, Fission_X_Limit, 0.0, 4096, FALSE);
           ADP16_Fission->resize(800, 400);

           ADP16_Fission->Y_MAX_Current = 4096;
           ADP16_Fission->intervals  = new QVector<QwtIntervalSample>;
           ADP16_Fission->Ch_Data = 0.0;//0x0000;
           ADP16_Fission->Ch_Data_2 = 0.0;//0x0000;



    connect (Hyst_Single, SIGNAL(Show_data_signal(QString)), this, SLOT(Output_text_to_window(QString)));
    connect (KAN, SIGNAL(Draw_Spectral_N_Chan_Signal(QByteArray *)), Hyst_Single, SLOT(Draw_Spectral_N_Chan_Slot(QByteArray *)) ); //QByteArray




//++++++++++++++++++++++++++++++++++++++++++++ End of QWT +++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++ timers signal/slot assignment

           timer_1 = new QTimer(this); // Таймер записи в файл
  //         connect(timer_1, SIGNAL( timeout() ), Hyst_Single, SLOT( onTimer_1() ) );
           connect(timer_1, SIGNAL( timeout() ), this, SLOT( onTimer_1() ) );
   //       timer_1->setInterval(20); timer_1->start();

          timer_2 = new QTimer(this); //Таймер отрисовки спектра onLine
  //        connect(timer_2, SIGNAL( timeout() ), Hyst_Single, SLOT( onTimer_2() ) );
          connect(timer_2, SIGNAL( timeout()), KAN, SLOT(Get_Spectral_Points_Slot()) );

          // Передача значения N  и типа команды в класс отрисовки спектра Plot.
          connect(this, SIGNAL(pass_N_SPECTRAL_signal(UINT8*, UINT8)), Hyst_Single, SLOT(pass_N_SPECTRAL_slot(UINT8*, UINT8)) );


    //     timer_2->setInterval(700); timer_2->start();

    //      connect (this, SIGNAL(Get_Spectral_Points_Signal()), KAN, SLOT(Get_Spectral_Points_Slot()) );

//++++++++++++++++++++++++++++++++++++++++++++ END of timers signal/slot assignment
//#########################################################################################
//++++++++++++++++++++++++++++++++++++++++++++  Threads signal/slot assignment


          connect (KAN, SIGNAL(Thread_Pause_Signal()), chan_A, SLOT(Thread_Pause_Slot()) );
          connect (KAN, SIGNAL(Thread_Pause_OFF_Signal()), chan_A, SLOT(Thread_Pause_OFF_Slot()) );

          connect(KAN, SIGNAL(Thread_Pause_Signal()), chan_B, SLOT(Thread_Pause_Slot()) );
          connect (KAN, SIGNAL(Thread_Pause_OFF_Signal()), chan_B, SLOT(Thread_Pause_OFF_Slot()) );


          connect (this, SIGNAL(make_io_file_signal()), KAN, SLOT(make_io_file_slot()) );



//ui->tab_camac->
camac_tab = new camac_paint(ui->tab_camac);
camac_tab->resize(30,30);
camac_tab->setGeometry(1050, 10, 30, 30);

X_flag = new camac_paint(ui->tab_camac);
X_flag->resize(30,30);
X_flag->setGeometry(20,270,30,30);

Q_flag = new camac_paint(ui->tab_camac);
Q_flag->resize(30,30);
Q_flag->setGeometry(70,270,30,30);



//Для отладочных целей, чтобы сократить вермя запуска теста
//emit on_Set_Data_pushButton_2_clicked();


//метка для быстрого попадания в эту точку кода #########proga-begin#######
int value = KAN->ftdi_init(Mini_ID_A, Mini_ID_B );

if (value == FTDI_OK) {camac_tab->KK_connected = TRUE;

   camac_tab->palette.setColor(QPalette::Light, Qt::green);
   camac_tab->setPalette(camac_tab->palette);

 //  Camac_Run_Always(); // Задаем массив [11202222]

   //Обновляем буффер в класее FTDI
  emit push_data_buffer(byOutputBuffer); // Копируем данные массива byOutputBuffer в массив внутри класса FTDI_D2XX

   //ОТправляем СПЕЦ-команду
   ftdi_answer_parser( 0x0400 + KAN->Write_B_Single(byOutputBuffer, TRANSFER_SIZE) ); // First send command


   Sleep(1);
   //Запускаем набор в потоке сразу при старте программы
   emit on_Start_Aquisition_pushButton_clicked();

   KAN->Purge_USB_Buffers();

} //Девайс подключен
//Выдаем в GUI результат работы функции ftdi_init(), список устрйоств
ftdi_answer_parser( value );
Sleep(1500);

//int imin = std::numeric_limits<int>::min(); // minimum value

// Пока не используем. Была нужна для определения максимального числа точек в одной переменной.
//int imax = std::numeric_limits<int>::max();


}

MainWindow::~MainWindow()
{
    Write_To_Log(0xffff, "Program is going to be closed");
    delete ui;
    //KAN->mfile.close();
    delete KAN;
}
//+++++++++++++++++++++++++++++++++++
/*
void MainWindow::paintEvent(QPaintEvent *e)
{
   Q_UNUSED(e);
   QPainter ekPaint(this);

//    QRect rec(180, 780,30,30);

   framepen.setWidth(20);
   if (X) framepen.setColor(Qt::green);
   else framepen.setColor(Qt::black); //Меняем цвет кружка red

   ekPaint.setPen(framepen);
   ekPaint.drawEllipse(40, 520, 20,20 ); //"X" кружок
 //  ekPaint.drawEllipse(180, 780, 20,20 );

   if (Q) framepen.setColor(Qt::green); //"Q"
   else framepen.setColor(Qt::red); //Меняем цвет кружка

   ekPaint.setPen(framepen);
   ekPaint.drawEllipse(120, 520, 20,20 );

   //http://www.evileg.ru/baza-znanij/qt/qpainter-znakomstvo-s-risovaniem-v-qt.html
   //https://www.youtube.com/watch?v=gqFxNBBi4Q0

   int i, x;
 //  UINT16 rdata;
  // UINT8 byte;


//   rdata = byInputBuffer[4]; rdata<<=8;
//   rdata += byInputBuffer[3];


//   x = 0; //y = 460;

//   for (i=15; i>=0; i--)//рисуем индикаторы слева направо, поэтому номера идут со старшего.
//   {
//      if ((rdata>>i) & 0x01) {framepen.setColor(Qt::red);}
//      else framepen.setColor(Qt::black);
//      ekPaint.setPen(framepen);
//      if  (!((i+1)%4)) {x+=20;}
//      x = x + 49;
//      ekPaint.drawEllipse(x, 460, 19,19 );

//   } //for


   //++++ индикатор связи
   if (FTDI_Connected) framepen.setColor(Qt::green); //
   else framepen.setColor(Qt::red); //Тревога

   ekPaint.setPen(framepen);

   ekPaint.drawEllipse(940, 18, 19,19 );
}

*/
//+++++++++++++++++++++++++++++++++++
//Очистка буферов, необходима чтобы очищались индикаторы данных.
void MainWindow::Purge_IO_Buffers(void)
{
memset(byOutputBuffer, 0, sizeof(byOutputBuffer));

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++

  void MainWindow::on_N_spinBox_valueChanged(int arg1)
{
    if (arg1 > N_MAXVALUE)
    {
        N_CAMAC = N_MAXVALUE;
        ui->N_spinBox->setValue(N_MAXVALUE);
        return;

    }
    if (arg1 < N_MINVALUE)
    {
        N_CAMAC = N_MINVALUE;
        ui->N_spinBox->setValue(N_MINVALUE);
        return;
    }

    else N_CAMAC = arg1;

    Update_Label();

    //++++++++++ Тут меняем подпись в строке состояние команды


}

//+++++++++++++++++++++++++++++++++++++++
// Обновить строку hex-данных-NAF, выбранных кнопками в окне
// Ф-ция вызывается каждый раз, когда пользователь что-то меняет в GUI
void MainWindow::Update_Label(void)
{

    CAMAC_make_command(); //Заполняем массив byOutputBuffer на передачу данных в USB

    QString str;
    QByteArray mystr = QByteArray(reinterpret_cast<char*>(byOutputBuffer), 16);
    mystr.resize(4);
    str = mystr.toHex().toUpper();
    str.insert(2,"       "); //+7
    str.insert(11,"       "); //+7
    str.insert(20,"         "); //+7
   // str.insert(26,"      "); //+8

    ui->NAF_Data_Label->clear();
    ui->NAF_Data_Label->setText(str);

}
//+++++++++++++++++
void MainWindow::Create_N_WidgetList(void)
{
    widgetList.append(ui->N_01_widget);
    widgetList.append(ui->N_02_widget);
    widgetList.append(ui->N_03_widget);
    widgetList.append(ui->N_04_widget);
    widgetList.append(ui->N_05_widget);
    widgetList.append(ui->N_06_widget);
    widgetList.append(ui->N_07_widget);
    widgetList.append(ui->N_08_widget);
    widgetList.append(ui->N_09_widget);
    widgetList.append(ui->N_10_widget);
    widgetList.append(ui->N_11_widget);
    widgetList.append(ui->N_12_widget);
    widgetList.append(ui->N_13_widget);
    widgetList.append(ui->N_14_widget);
    widgetList.append(ui->N_15_widget);
    widgetList.append(ui->N_16_widget);
    widgetList.append(ui->N_17_widget);
    widgetList.append(ui->N_18_widget);
    widgetList.append(ui->N_19_widget);
    widgetList.append(ui->N_20_widget);
    widgetList.append(ui->N_21_widget);
    widgetList.append(ui->N_22_widget);
    widgetList.append(ui->N_23_widget);

}
//+++++++++++++++++

void MainWindow::on_A_spinBox_valueChanged(int arg1)
{
    if (arg1 > A_MAXVALUE)
    {
        A_CAMAC = A_MAXVALUE;
        ui->A_spinBox->setValue(A_MAXVALUE);
        return;
    }

    if (arg1 < A_MINVALUE)
    {
        A_CAMAC = A_MINVALUE;
        ui->A_spinBox->setValue(A_MINVALUE);
        return;
    }


    else A_CAMAC = arg1;
    Update_Label();

}

void MainWindow::on_F_spinBox_valueChanged(int arg1)
{
    if (arg1 > F_MAXVALUE)
    {
        F_CAMAC = F_MAXVALUE;
        ui->F_spinBox->setValue(F_MAXVALUE);
        return;
    }
    if (arg1 < F_MINVALUE)
    {
        F_CAMAC = F_MINVALUE;
        ui->F_spinBox->setValue(F_MINVALUE);
        return;
    }


  else F_CAMAC = arg1;
    Update_Label();

}

//+++++++++++++++++++++++++++=
// Эту ф-ция формирует массив из 4 байт на отправку в FTDI.
// Данные массива составляются на основе заданных опций в GUI
// Вызываем эту ф-цию после каждого изменения в GUI из функции Update_Label(), которая вызывается
// прям в слоте обработке сигнала от элемента GUI.
void MainWindow::CAMAC_make_command(void)
{
    //N_CAMAC = 2..23
    //F_CAMAC = 0..31
    //A_CAMAC = 0..15
    UINT16 naf;
//++++++++++++++++++++++++++++ биты 7,8 не определены, не обрабатываем +++++++++++++++

    // Проверка на спецкоманду N ==0, A ==0  bit F1 == 1

    if ((N_CAMAC == 0) && (A_CAMAC==0) &&  (F_CAMAC & 0x0001) )  //(F_CAMAC & 0x0001) == 1 ) (F_CAMAC == 17)
    {//Формируем спецкоманду

        Camac_SPEC_Command(F_CAMAC, ToDev_Bits);
    }
   else
   {
    naf = N_CAMAC; //naf <<= 8; //naf & 0x1fff;
    NAF_Bits = naf & 0x001f; // N16,N8,N4,N2,N1 на 5-1 битах

    naf = F_CAMAC;
    NAF_Bits |=  ((naf<<1) & 0x0020);  //F16 на 6-м бите
    NAF_Bits |=  ((naf<<12) & 0xf000); //F8,F4,F2,F1 на 16-13 битах

    naf = A_CAMAC;
    NAF_Bits |=  ( (naf << 8) & 0x0f00); // A8,A4,A2,A1 на 12-09 битах

// Теперь NAF_Bits и ToDev_Bits объединяем в 1 32 битное слово
    CAMAC_WORD = ToDev_Bits; CAMAC_WORD <<=16;  CAMAC_WORD &= 0xffff0000;
    CAMAC_WORD += NAF_Bits; // NAF_Bits идут в 1-2 байтах команды.

    byOutputBuffer[0]= (CAMAC_WORD & 0x000000ff); // // '\x11';//
    byOutputBuffer[1]= (CAMAC_WORD & 0x0000ff00)>>8;// '\x20';//
    byOutputBuffer[2]= (CAMAC_WORD & 0xff000000)>>24; // Старший байт данных '\x22';//
    byOutputBuffer[3]= (CAMAC_WORD & 0x00ff0000)>>16; // Младший байт данных '\x22';//
    //byOutputBuffer[4]= 0x00;
   }
 //Обновляем буффер в класее FTDI
 emit push_data_buffer(byOutputBuffer); // Копируем данные массива byOutputBuffer в массив внутри класса FTDI_D2XX
}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Clear_All_clicked()
{
    //Начальные значения
    N_CAMAC = 2; //
    A_CAMAC = 0;
    F_CAMAC = 0;

    NAF_Bits = 0x0000;
    ToDev_Bits = 0;
    QString str = "00       00       00         00";

    ui->N_spinBox->setValue(N_CAMAC);
    ui->A_spinBox->setValue(A_CAMAC);
    ui->F_spinBox->setValue(F_CAMAC);

    ui->NAF_Data_Label->setText(str);

    X = FALSE; Q = FALSE;

    if (ui->Sel_Bit_01->isChecked()) ui->Sel_Bit_01->toggle();
    if (ui->Sel_Bit_02->isChecked()) ui->Sel_Bit_02->toggle();
    if (ui->Sel_Bit_03->isChecked()) ui->Sel_Bit_03->toggle();
    if (ui->Sel_Bit_04->isChecked()) ui->Sel_Bit_04->toggle();
    if (ui->Sel_Bit_05->isChecked()) ui->Sel_Bit_05->toggle();
    if (ui->Sel_Bit_06->isChecked()) ui->Sel_Bit_06->toggle();
    if (ui->Sel_Bit_07->isChecked()) ui->Sel_Bit_07->toggle();
    if (ui->Sel_Bit_08->isChecked()) ui->Sel_Bit_08->toggle();
    if (ui->Sel_Bit_09->isChecked()) ui->Sel_Bit_09->toggle();
    if (ui->Sel_Bit_10->isChecked()) ui->Sel_Bit_10->toggle();
    if (ui->Sel_Bit_11->isChecked()) ui->Sel_Bit_11->toggle();
    if (ui->Sel_Bit_12->isChecked()) ui->Sel_Bit_12->toggle();
    if (ui->Sel_Bit_13->isChecked()) ui->Sel_Bit_13->toggle();
    if (ui->Sel_Bit_14->isChecked()) ui->Sel_Bit_14->toggle();
    if (ui->Sel_Bit_15->isChecked()) ui->Sel_Bit_15->toggle();
    if (ui->Sel_Bit_16->isChecked()) ui->Sel_Bit_16->toggle();


update();

Init_Values();
update();
}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Sel_Bit_16_toggled(bool checked)
{
    if(checked) {
       ui->Sel_Bit_16->setStyleSheet("background-color: yellow");
       ToDev_Bits |= 0x8000; //16 bit set to '1'
    }
    else
    {  ui->Sel_Bit_16->setStyleSheet("background-color: lightgray");
       ToDev_Bits &= 0x7fff; //16 bit set to '0'
    }
  Update_Label();
}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Sel_Bit_15_toggled(bool checked)
{
    if(checked) {
       ui->Sel_Bit_15->setStyleSheet("background-color: yellow");
       ToDev_Bits |= 0x4000; //15 bit set to '1'
    }
    else
    {  ui->Sel_Bit_15->setStyleSheet("background-color: lightgray");
       ToDev_Bits &= 0xBfff; //15 bit set to '0'
    }
  Update_Label();

}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Sel_Bit_14_toggled(bool checked)
{
    if(checked) {
       ui->Sel_Bit_14->setStyleSheet("background-color: yellow");
       ToDev_Bits |= 0x2000; //14 bit set to '1'
    }
    else
    {  ui->Sel_Bit_14->setStyleSheet("background-color: lightgray");
       ToDev_Bits &= 0xDfff; //14 bit set to '0'
    }
  Update_Label();

}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Sel_Bit_13_toggled(bool checked)
{
    if(checked) {
       ui->Sel_Bit_13->setStyleSheet("background-color: yellow");
       ToDev_Bits |=0x1000; //13 bit set to '1'
    }
    else
    {  ui->Sel_Bit_13->setStyleSheet("background-color: lightgray");
       ToDev_Bits &= 0xEfff; //13 bit set to '0'
    }
  Update_Label();

}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Sel_Bit_12_toggled(bool checked)
{
    if(checked) {
       ui->Sel_Bit_12->setStyleSheet("background-color: yellow");
       ToDev_Bits |= 0x0800; //12 bit set to '1'
    }
    else
    {  ui->Sel_Bit_12->setStyleSheet("background-color: lightgray");
       ToDev_Bits &= 0xf7ff; //12 bit set to '0'
    }
    Update_Label();
}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Sel_Bit_11_toggled(bool checked)
{
    if(checked) {
       ui->Sel_Bit_11->setStyleSheet("background-color: yellow");
       ToDev_Bits |= 0x0400; //11 bit set to '1'
    }
    else
    {  ui->Sel_Bit_11->setStyleSheet("background-color: lightgray");
       ToDev_Bits &= 0xfBff; //11 bit set to '0'
    }
  Update_Label();

}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Sel_Bit_10_toggled(bool checked)
{
    if(checked) {
       ui->Sel_Bit_10->setStyleSheet("background-color: yellow");
       ToDev_Bits |= 0x0200; //10 bit set to '1'
    }
    else
    {  ui->Sel_Bit_10->setStyleSheet("background-color: lightgray");
       ToDev_Bits &= 0xfDff; //10 bit set to '0'
    }
  Update_Label();

}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Sel_Bit_09_toggled(bool checked)
{
    if(checked) {
       ui->Sel_Bit_09->setStyleSheet("background-color: yellow");
       ToDev_Bits |= 0x0100; //09 bit set to '1'
    }
    else
    {  ui->Sel_Bit_09->setStyleSheet("background-color: lightgray");
       ToDev_Bits &= 0xfEff; //09 bit set to '0'
    }
  Update_Label();

}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Sel_Bit_08_toggled(bool checked)
{
    if(checked) {
       ui->Sel_Bit_08->setStyleSheet("background-color: yellow");
       ToDev_Bits |= 0x0080; //08 bit set to '1'
    }
    else
    {  ui->Sel_Bit_08->setStyleSheet("background-color: lightgray");
       ToDev_Bits &= 0xff7f; //08 bit set to '0'
    }
  Update_Label();

}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Sel_Bit_07_toggled(bool checked)
{
    if(checked) {
       ui->Sel_Bit_07->setStyleSheet("background-color: yellow");
       ToDev_Bits |= 0x0040; //07 bit set to '1'
    }
    else
    {  ui->Sel_Bit_07->setStyleSheet("background-color: lightgray");
       ToDev_Bits &= 0xffBf; //07 bit set to '0'
    }
  Update_Label();

}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Sel_Bit_06_toggled(bool checked)
{
    if(checked) {
       ui->Sel_Bit_06->setStyleSheet("background-color: yellow");
       ToDev_Bits |= 0x0020; //06 bit set to '1'
    }
    else
    {  ui->Sel_Bit_06->setStyleSheet("background-color: lightgray");
       ToDev_Bits &= 0xffDf; //06 bit set to '0'
    }
  Update_Label();

}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Sel_Bit_05_toggled(bool checked)
{
    if(checked) {
       ui->Sel_Bit_05->setStyleSheet("background-color: yellow");
       ToDev_Bits |= 0x0010; //05 bit set to '1'
    }
    else
    {  ui->Sel_Bit_05->setStyleSheet("background-color: lightgray");
       ToDev_Bits &= 0xffEf; //05 bit set to '0'
    }
  Update_Label();

}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Sel_Bit_04_toggled(bool checked)
{
    if(checked) {
       ui->Sel_Bit_04->setStyleSheet("background-color: yellow");
       ToDev_Bits |= 0x0008; //04 bit set to '1'
    }
    else
    {  ui->Sel_Bit_04->setStyleSheet("background-color: lightgray");
       ToDev_Bits &= 0xfff7; //04 bit set to '0'
    }
  Update_Label();

}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Sel_Bit_03_toggled(bool checked)
{
    if(checked) {
       ui->Sel_Bit_03->setStyleSheet("background-color: yellow");
       ToDev_Bits |= 0x0004; //03 bit set to '1'
    }
    else
    {  ui->Sel_Bit_03->setStyleSheet("background-color: lightgray");
       ToDev_Bits &= 0xfffB; //03 bit set to '0'
    }
  Update_Label();

}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Sel_Bit_02_toggled(bool checked)
{
    if(checked) {
       ui->Sel_Bit_02->setStyleSheet("background-color: yellow");
       ToDev_Bits |= 0x0002; //02 bit set to '1'
    }
    else
    {  ui->Sel_Bit_02->setStyleSheet("background-color: lightgray");
       ToDev_Bits &= 0xfffD; //02 bit set to '0'
    }
  Update_Label();

}
//+++++++++++++++++++++++++++++++++++
void MainWindow::on_Sel_Bit_01_toggled(bool checked)
{
    if(checked) {
       ui->Sel_Bit_01->setStyleSheet("background-color: yellow");
       ToDev_Bits |= 0x0001; //01 bit set to '1'
    }
    else
    {  ui->Sel_Bit_01->setStyleSheet("background-color: lightgray");
       ToDev_Bits &= 0xfffE; //01 bit set to '0'
    }
  Update_Label();

}
//+++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++++++++
//++++++++++++++
// Задаем начальные значения элементов посылки в Camac в главном окне
void MainWindow::Init_Values(void)
{
    Purge_IO_Buffers();
    QString str;
    ui->N_spinBox->setAlignment(Qt::AlignCenter); ui->N_spinBox->setFocus();
    ui->A_spinBox->setAlignment(Qt::AlignCenter);
    ui->F_spinBox->setAlignment(Qt::AlignCenter);
    //Начальные значения
    N_CAMAC = 0; //
    A_CAMAC = 0;
    F_CAMAC = 0;
    ui->N_spinBox->setValue(0);
    ui->A_spinBox->setValue(0);
    ui->F_spinBox->setValue(0);

    NAF_Bits = 0x0000;
    ToDev_Bits = 0;

    FTDI_Connected = FALSE;
    X = FALSE;
    Q = FALSE;

    A_Open = FALSE;
    B_Open = FALSE;
    cycle_counnter = 0;
    update();


}
//+++++++++++++++++++++++


//+++++++++++++++++++++++
//Слот обработки сигнала Process_B - запись данных в канал B
// Выполняет отправку NAF в ручном режиме
/*
 * Т.к. это ручной режим, то в этой ф-ции добавляем сообщения в ЛОГ, с указанием деталей:
 * станция, канал, данные и т.д.
*/
void MainWindow::on_SEND_TO_USB_pushButton_clicked()
{

   // Проверить, открыт ли файл набора и если нет, то открыть.

   CAMAC_make_command(); //Заполняем буфер byOutputBuffer на отправку данных
    //Т.к. частота меняется, то в поток необходимо передавать параметр - частоту.
    int index, duration;

   index = ui->Frequency_comboBox->currentIndex();
    switch (index)
    {
      case 0 : duration = 0;
      case 1 : duration = 200;  break; //5kHZ
      case 2 : duration = 100;  break; //10kHZ
      case 3 : duration = 50;   break; //20kHZ
      case 4 : duration = 500;  break; //2kHZ
      case 5 : duration = 3000; break; //333HZ

    }

    chan_B->Duration_mks = duration; //ui->Frequency_comboBox->currentIndex();

//    char b_Data[MIN_Rx_BUFFER_SIZE]; //Сюда запишем принятые от ftdi данные

    // If "No cycle"  so make  1 cycle
    if  (index == 0) {

        //Выполняем команду камак для 1 станции.
          QString str;
          QByteArray mystr = QByteArray(reinterpret_cast<char*>(byOutputBuffer), 8);
          mystr.resize(TRANSFER_SIZE);
          str.sprintf("Отправляем Данные в FTDI : ");
          str.append(mystr.toHex().toUpper());
          str.append("\n");
          Write_To_Log(0x1000, str);
          ftdi_answer_parser( KAN->Write_B_Single(byOutputBuffer, TRANSFER_SIZE) ); // First send command
         //Т.к. режим "1-Разовый" то вводим паузу. Иначе в файл данные не пишутся.
         Sleep(1); // это 1мс. !!!!!!!!!!!, а не 1с.
         //KAN->Read_A_Single();
        // ftdi_answer_parser( KAN->Read_A_Single(b_Data, MIN_Rx_BUFFER_SIZE ) );
         ftdi_answer_parser( KAN->Read_A_Single(KAN->byInputBuffer, MIN_Rx_BUFFER_SIZE ) ); //MIN_Rx_BUFFER_SIZE
         KAN->Camac_Data_Parser();

        mystr = QByteArray((char*)KAN->byInputBuffer, 8);//b_Data MIN_Rx_BUFFER_SIZE
        mystr.resize(RECEIVE_SIZE);
        str.sprintf("SEND_TO_USB Считанны Данные из FTDI : ");
        str.append(mystr.toHex().toUpper());
        str.append("\n");
        Write_To_Log(0x1000, str);
        update();

    }
    else {//if  (index == 0) // Если циклически, то запускаем поток.

      //  if (timer->isActive()) {timer->stop();} //Стопорим таймер, т.к. щас мигать начнет конкретно

        emit push_data_buffer(byOutputBuffer); // Копируем данные массива byOutputBuffer в массив внутри класса FTDI_D2XX
        chan_B->finthread = FALSE;
        // Надо добавить проверку, если поток уже в рабочем состоянии, но на паузе, то только снять с паузы.
        // А если не в рабочем, то запустить
        if (!thread_B->isRunning()) {
        thread_B->start(QThread::HighPriority); //Пишем из ПК в Железо по каналу B
        }

 //       chan_A->finthread = FALSE;
 //       if (!thread_A->isRunning()) {
 //       thread_A->start(QThread::HighPriority); //Пишем из Железа в ПК по каналу A
 //       }
        Write_To_Log(0x1000, "Запущен постоянный набор\n");
        ui->Status_label->setText("ИДЕТ НАБОР ДАННЫХ !");
        ui->Status_label->setTextFormat(Qt::RichText);
        timer_flash->start(250); // таймер мигающий индикатором связи


    }


}
//+++++++++++++++++++++++++++++++++++
// Stop sending data, Channel B
void MainWindow::on_pushButton_stop_Sending_clicked()
{
    KAN->KK_Write_To_Log(0x3000, "Нажата кнопка **Stop Sending**\n");

    chan_B->finthread = TRUE;  //   emit Stop_Sending_B(); //

    chan_A->finthread = TRUE; //    emit Stop_Sending_A(); //

    // Мигалку остановить, надпись "Идет набор данных" поменять.
    if (timer_flash->isActive()) {timer_flash->stop();}

    ui->Status_label->setText(mode_manual_KK); //"НАБОР ДАННЫХ НЕ ИДЕТ !"
    camac_tab->palette.setColor(QPalette::Light, Qt::blue);
    camac_tab->setPalette(camac_tab->palette);
    ui->Frequency_comboBox->setCurrentIndex(0); // 0 means "No Cycle",



}
//+++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++
//+++++++++++++++++++
/*
 * Слот нажатия кнопки "Stop ALL".
 * Останавливаем потоки
 * Останавливаем мигалку
 * Останавливаем timer_2, занятый для отрисовки спектров.
 * Порты FTDI остаются открытыми, что дает возможность дальше рабоать с USB
 * в режиме "Ручной контроллер", однако непонятно, в какой файл при этому пишутся данные ...
*/
//+++++++++++++
void MainWindow::on_Stop_ALL_pushButton_clicked()
{
   if (timer_flash->isActive()) {timer_flash->stop();}
    camac_tab->KK_connected = FALSE;

    KAN->KK_Write_To_Log(0x3000, "Нажата кнопка **Stop ALL**\n");


    // Ставим на "холостой ход". сам поток остается рабочим
    chan_B->finthread = TRUE;  //   emit Stop_Sending_B(); //
    // Ставим на "холостой ход". сам поток остается рабочим
    chan_A->finthread = TRUE; //    emit Stop_Sending_A(); //

    // Надо дождаться окончания посылок в потоке.
    // После этого отправлять спец-команду. А так она отправляется 8-10 раз, пока поток не остановится.



    // Добавляем команду N[0]*A[0]*F[27]
   Camac_SPEC_Command(CAMAC_F25, 0x0000);
   //Обновляем буффер в класее FTDI
   emit push_data_buffer(byOutputBuffer); // Копируем данные массива byOutputBuffer в массив внутри класса FTDI_D2XX


    //ОТправляем СПЕЦ-команду
    ftdi_answer_parser( 0x0400 + KAN->Write_B_Single(byOutputBuffer, TRANSFER_SIZE) ); // First send command



    // Мигалку остановить, надпись "Идет набор данных" поменять.
    if (timer_flash->isActive()) {timer_flash->stop();}
    ui->Status_label->setText(mode_manual_KK); //"НАБОР ДАННЫХ НЕ ИДЕТ !"
    camac_tab->palette.setColor(QPalette::Light, Qt::blue);
    camac_tab->setPalette(camac_tab->palette);
    ui->Frequency_comboBox->setCurrentIndex(0); // 0 means "No Cycle",

    if (timer_2->isActive()) {timer_2->stop();} // Остановили таймер считывания из буфера/файла  и отрисовки спектра. В лог не пишется количество точек

//    ui->Status_label->setText("нет обмена данными");

    // Закрыть файл с данными.
    KAN->mfile.close();

    // И вот теперь добавляем открытие нового файла для записи, если остаёмся для работы в ручном режиме.

    emit make_io_file_signal();

}
//++++++++++++++++++++++++++++++++
// Это слот сигнала KAN->Show_data(str);
void MainWindow::Output_text_to_window(QString sss) { Write_To_Log(0, sss); }
//+++++++++++++++++++++++++++++++++++++++++++
void MainWindow::Bits_To_Buttons(UINT16 data_bits)
{

    if (data_bits & 0x0001) { ui->Sel_Bit_01->setChecked(true);}
    else   ui->Sel_Bit_01->setChecked(false);

   if  ((data_bits >>1) & 0x0001)  { ui->Sel_Bit_02->setChecked(true);} //ui->Sel_Bit_02->toggle();
   else   ui->Sel_Bit_02->setChecked(false);

   if  ((data_bits >>2) & 0x0001)  { ui->Sel_Bit_03->setChecked(true);}
   else   ui->Sel_Bit_03->setChecked(false);

   if  ((data_bits >>3) & 0x0001)  { ui->Sel_Bit_04->setChecked(true);}
   else   ui->Sel_Bit_04->setChecked(false);

   if  ((data_bits >>4) & 0x0001)  { ui->Sel_Bit_05->setChecked(true);}
   else   ui->Sel_Bit_05->setChecked(false);

   if  ((data_bits >>5) & 0x0001)  { ui->Sel_Bit_06->setChecked(true);}
   else   ui->Sel_Bit_06->setChecked(false);

   if  ((data_bits >>6) & 0x0001)  { ui->Sel_Bit_07->setChecked(true);}
   else   ui->Sel_Bit_07->setChecked(false);

   if  ((data_bits >>7) & 0x0001)  { ui->Sel_Bit_08->setChecked(true);}
   else   ui->Sel_Bit_08->setChecked(false);

   if  ((data_bits >>8) & 0x0001)  { ui->Sel_Bit_09->setChecked(true);}
   else   ui->Sel_Bit_09->setChecked(false);

   if  ((data_bits >>9) & 0x0001)  { ui->Sel_Bit_10->setChecked(true);}
   else   ui->Sel_Bit_10->setChecked(false);

   if  ((data_bits >>10) & 0x0001)  {ui->Sel_Bit_11->setChecked(true);}
   else   ui->Sel_Bit_11->setChecked(false);

   if  ((data_bits >>11) & 0x0001)  { ui->Sel_Bit_12->setChecked(true);}
   else   ui->Sel_Bit_12->setChecked(false);

   if  ((data_bits >>12) & 0x0001)  { ui->Sel_Bit_13->setChecked(true);}
   else   ui->Sel_Bit_13->setChecked(false);

   if  ((data_bits >>13) & 0x0001)  {ui->Sel_Bit_14->setChecked(true);}
   else   ui->Sel_Bit_14->setChecked(false);

   if  ((data_bits >>14) & 0x0001)  {ui->Sel_Bit_15->setChecked(true);}
   else   ui->Sel_Bit_15->setChecked(false);

   if  ((data_bits >>15) & 0x0001)  { ui->Sel_Bit_16->setChecked(true); }
   else   ui->Sel_Bit_16->setChecked(false);

update();
}

//+++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++ QWT ++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++ SLOT of timer_1
void MainWindow::onTimer_1()
{
 //Пишем файл данные от N17, канал 1.
    char N17_Data[8] = {'\xf0','\x00', '\x00', '\x00', '\x00', '\x07', '\xD0', '\x11'};

    if (KAN->mfile.isOpen())
     {
         //int written_bytes;
         QDataStream out(&KAN->mfile);
         out.writeRawData(N17_Data, 8); //RxBytes
     //    str.sprintf("SINGLE Записано %d байт \n", written_bytes); emit Show_data(str);

     }

}//onTimer_1()
//+++++++++++++++++++++++++++++++++++++++

//+++++++++++ SLOT of timer_2

//+++++++++++++++++++++++++++++++++++++++
Plot::Plot( QWidget *parent, const char *w_title, double x_min, double x_max, double y_min, double y_max, BOOL isTrigonometric ):
    QwtPlot( parent )
{

    setAutoFillBackground( true );
    setPalette( QPalette( QColor( 0, 0, 0 ) ) ); //QColor( 165, 193, 228 )
    //updateGradient();

    setTitle( w_title );
    insertLegend( new QwtLegend(), QwtPlot::RightLegend );

    // axes
    setAxisTitle( xBottom, "x -->" );
   // setAxisScale( xBottom, 0.0, 10.0 );
    setAxisScale( xBottom, x_min, x_max );

    setAxisTitle( yLeft, "y -->" );
    setAxisScale( yLeft, y_min, y_max );
   // setAxisAutoScale(yLeft, false);

    // canvas
    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setLineWidth( 1 );
    canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas->setBorderRadius( 15 );

   // QPalette canvasPalette( Qt::white );
    QPalette canvasPalette( Qt::black ); //=============== ВООООТ ЗАДАЕМ ЧЁРНЫЙ ФОН =====================
    canvasPalette.setColor( QPalette::Foreground, QColor( 255, 255, 0 ) ); //QColor( 133, 190, 232 )
    canvas->setPalette( canvasPalette );

    setCanvas( canvas );

    // panning with the left mouse button
    ( void ) new QwtPlotPanner( canvas );

    // zoom in/out with the wheel
  //  ( void ) new QwtPlotMagnifier( canvas );
    Magnifier_param = new QwtPlotMagnifier( canvas );

  //  fiss_scale = new QwtScaleWidget (canvas);
    fiss_scale = this->axisWidget(QwtPlot::xBottom); // Returns Scale widget of the specified axis,
                                                        // or NULL if axisId is invalid.


//if (isTrigonometric) populate_trigonometric();
//else populate_histogram();
if (!isTrigonometric) populate_histogram();

//График теперь будем рисовать не одной ф-цией populate_histogram(), а по таймеру.
}
//++++++++++++++++++++++++ Прицепляем кривую к виджету с готовой шкалой +++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++
double Plot::return_param ()
{
    this->fis_magn = 3.5; //Magnifier_param->wheelFactor();
    return fis_magn;
}
//++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++
void Plot::resizeEvent( QResizeEvent *event)
{
    QwtPlot::resizeEvent( event );

    // Qt 4.7.1: QGradient::StretchToDeviceMode is buggy on X11
    //updateGradient();

     emit fis_resized();
}
//++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++
void Plot::updateGradient()
{
    QPalette pal = palette();

    const QColor buttonColor = pal.color( QPalette::Button );

    QLinearGradient gradient( rect().topLeft(), rect().bottomLeft() );
    gradient.setColorAt( 0.0, Qt::white );
    gradient.setColorAt( 0.7, buttonColor );
    gradient.setColorAt( 1.0, buttonColor );

    pal.setBrush( QPalette::Window, gradient );
    setPalette( pal );
}
//++++++++++++++++++++++++++++++++++++++++++++++
void Plot::populate_histogram()
{
    //QwtPlotHistogram
    //int i;
    //float di = 0.05;

    hystogram   = new QwtPlotHistogram;
    intervals   = new QVector<QwtIntervalSample>;

/*    for (i=1; i<= Fission_X_Limit; i++)
    {
        if (i%10) hData[i] = 0; //Если деление нацело, то остаток будет 0
        else hData[i] = i;

        intervals->append(QwtIntervalSample(hData[i], i, i+di));
    }
*/

/*
    QPainter * painter;
  //  painter.drawLine(10, 20);
    painter->setPen(Qt::blue);
    QwtScaleMap x_chan, y_chan;

    double p1, p2;
    p1 = 1.0; p2 = 50.0;
    x_chan.setPaintInterval(p1, p2);
    y_chan.setPaintInterval(p1, p2);

*/
    hystogram->setStyle(QwtPlotHistogram::HistogramStyle::Lines);
    hystogram->setPen(Qt::yellow); //========================= ВООООТ ЗАДАЕМ ЦВЕТ ЛИНИИ ГИСТОГРАММЫ ============
    hystogram->setSamples(*intervals);

//    hystogram->drawLines(painter, x_chan, y_chan, 1, 50);

    hystogram->attach(this); //widget_number
   // this->replot();
    intervals->clear(); //очищаем массив точек

}
//+++++++++++++++++++++++++++++++++++++
//Флаг KK_connected устанавливается здесь по сигналу от ftdi_init();
void MainWindow::Device_Connected_Slot()
{
    FTDI_Connected = TRUE;
    camac_tab->KK_connected = TRUE;
   //Таймер не останавливаем. Сделаем это после запуска потоков. Т.к. при работе
   // в 1-кратном режиме тоже может произойти разрыв соединения
    update();

}
//+++++++++++++++++++++++++++++++++++++
void MainWindow::Device_NOT_connected_Slot()
{
    FTDI_Connected = FALSE;
    camac_tab->KK_connected = FALSE;
    //Проверяем, запущен ли таймер, если нет то запускаем, повторно не запускаем, будет ошибка
    if (!timer->isActive()) { timer->setInterval(500); timer->start();}//Проверяем наличие подключения. Отслеживаем подключение девайса к ПК.
    update();

}
//+++++++++++++++++++++++++++++++++++++
void MainWindow::On_Timer()
{
  //if (!camac_tab->KK_connected) ftdi_answer_parser(KAN->ftdi_init());
;
}
//+++++++++++++++++++++++++++
void MainWindow::On_Transfer_Flashing()
{

    camac_tab->transfer_flashing = !camac_tab->transfer_flashing;

    if (camac_tab->transfer_flashing)
    {
        camac_tab->palette.setColor(QPalette::Light, Qt::green);
        camac_tab->setPalette(camac_tab->palette);
    }
else
    {

        camac_tab->palette.setColor(QPalette::Light, Qt::gray);
        camac_tab->setPalette(camac_tab->palette);
    }
}
//++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++ N*F(24)A(0) +++++++++++++
//Прогоняем N*F(24)A(0) по всем станциям, кто в ответ пришлет "X", тот понимает этот запрос.


//+++++++++++++++++++
void MainWindow::Camac_N_Connected_Slot(int N_num) // Слот обработки Сигнала, что станция N на связи.
{
 widgetList.at(N_num-1)->setStyleSheet(QStringLiteral("background-color: rgb(0, 83, 40) ;"));
 X = TRUE;
 update();

 X_flag->N_X = TRUE;


 camac_tab->palette.setColor(QPalette::Light, Qt::green);
 camac_tab->setPalette(camac_tab->palette);

 X_flag->palette.setColor(QPalette::Light, Qt::red);
 X_flag->setPalette(X_flag->palette);

// X_flag->setStyleSheet(QStringLiteral("background-color: rgb(255, 244, 79); "));
 X_flag->update();
 //background-color

}
//+++++++++++++++++++

void MainWindow::Camac_N_NOT_Connected_Slot(int N_num)
{
//    X = FALSE;
//    update();
 X_flag->N_X = FALSE;

 str.sprintf("Camac Station %u is not connected, there is no X-flag", N_num);
 KAN->KK_Write_To_Log(0x1111, str);

 X_flag->palette.setColor(QPalette::Light, Qt::black);
 X_flag->setPalette(X_flag->palette);
 X_flag->update();
}
//+++++++++
void MainWindow::Camac_N_Q_Flag_Slot()
{
    Q_flag->palette.setColor(QPalette::Light, Qt::red);
    Q_flag->setPalette(Q_flag->palette);
    Q_flag->update();
}

void MainWindow::Camac_N_Q_NOT_Flag_Slot()
{
    Q_flag->palette.setColor(QPalette::Light, Qt::black);
    Q_flag->setPalette(Q_flag->palette);
    Q_flag->update();
}


//Запускаем только поток А на считывание из FTDI
void MainWindow::on_Start_Aquisition_pushButton_clicked()
{

    // Проверить, открыт ли файл набора и если нет, то открыть.

    Write_To_Log(0x2222, "Запущен набор файла в потоке \n");
    chan_A->finthread = FALSE;


    // Добавляем команду N[0]*A[0]*F[27]
  // Camac_SPEC_Command(CAMAC_F27, 0x0000);

    //Camac_Run_Always();  // Для тестов

    //Обновляем буффер в класее FTDI
   emit push_data_buffer(byOutputBuffer); // Копируем данные массива byOutputBuffer в массив внутри класса FTDI_D2XX


    //ОТправляем СПЕЦ-команду
    ftdi_answer_parser( 0x0400 + KAN->Write_B_Single(byOutputBuffer, TRANSFER_SIZE) ); // First send command


    Sleep(1);
    thread_A->start(QThread::HighPriority); //Считываем в ПК из Железа по каналу А

    ui->Status_label->setText("ИДЕТ НАБОР ДАННЫХ !");
    ui->Status_label->setTextFormat(Qt::RichText);
    timer_flash->start(250); // таймер мигающий индикатором связи


 //   emit Write_TO_File_Signal();

}
//+++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++
// ADP16 ONLINE data
void MainWindow::on_Show_Selected_Spectral_pushButton_clicked()
{ // По нажатию кнопки "ПОКАЗАТЬ" отображаем спектр для выбранной станции  и выбранного номера канала этой станции

//Предполагается, что набор был запущен ранее, а сейчас только выводим данные из набранного (файла) буфера !!!.
// Т.к. не работает fpga то сгенерим файл здесь. Т.е. запустим таймер, который будет записывать в файл.
// А другой таймер будет считывать из файла.
 N_SPECTRAL = ui->N_Spectral_spinBox_Single->value();
 N_SPECTRAL_CHANNEL = ui->N_Spectral_Channel_spinBox_Single->value();

 N_SPECTRAL_ADP16 = ui->N_Spectral_ADP16_spinBox->value();
 N_SPECTRAL_ADP16_CHANNEL = ui->N_Spectral_Channel_ADP16_spinBox->value();


 //KAN->sfile.setFile(KAN->mfile);

// KAN->mfile.flush();

 timer_2->setInterval(700); timer_2->start(); //Запустили таймер считывания из (файла) буфера и отрисовки спектра

 // Обработчик для таймера_2 - это FTDI_D2XX::Get_Spectral_Points_Slot()
/*
 byOutputBuffer[0] = 0x11;
 byOutputBuffer[1] = 0x20;
 byOutputBuffer[2] = 0x22;
 byOutputBuffer[3] = 0x22;
*/
 //ui->Tab_2_Spectral_Data_textBrowser->clear();


}
//++++++++++++++++++++++++++++
//Пока не используем и, возможно, вообще не будем.
void MainWindow::on_Show_Spectral_Selected_From_File_pushButton_clicked()
{

    QFile mfile;
    QString filename, mStr;
   UINT16 fdata, fdata_2;
    //qint64 fdata;
    char mbuf[8];
    QByteArray qbuf;
    QByteArray chan_num;
    mStr = "./Camac_N17_data.bin";
    mfile.setFileName(mStr);

    N_SPECTRAL = ui->N_Spectral_spinBox_Single->value();
    N_SPECTRAL_CHANNEL = ui->N_Spectral_Channel_spinBox_Single->value();

 //str.sprintf("Вывод спектра для станции № %d, канал %d",N_SPECTRAL,  N_SPECTRAL_CHANNEL); ui->Tab_2_Spectral_Data_textBrowser->append(str);

    if (mfile.open(QIODevice::ReadOnly )) //If possible, the device is truncated before it is opened.
    {                                                             //All earlier contents of the device are lost.
        //qDebug () << "File  exist !";

    }

else {
        str.sprintf("File does not exist\n"); //ui->Tab_2_Spectral_Data_textBrowser->append(str);
        return ;
    }

    QDataStream infile (&mfile);
    while (!infile.atEnd())
    {
        infile.readRawData(mbuf, 8);

        // Выборка для другого блока  - НЕ ADP16
        if ((mbuf[0] == '\xf0') && (mbuf[7] == '\x80') && ((mbuf[1] & '\x1f')== N_SPECTRAL)) //D1 - q + x + N=17

         {
            fdata = mbuf[3];  fdata = (fdata << 8) & 0xff00;
            fdata_2 = mbuf[4]; fdata_2 &= 0x00ff;
            fdata +=fdata_2;
            double di = 0.005;
        //fdata - это номер канала !!!!
            Hyst_Single->Ch_Data +=1;
            Hyst_Single->intervals->append(QwtIntervalSample(Hyst_Single->Ch_Data, fdata, fdata + di));
            Hyst_Single->hystogram->setSamples(*Hyst_Single->intervals);
            Hyst_Single->hystogram->attach(Hyst_Single);
            Hyst_Single->replot();
        }

 //     Выборка для ADP16
        if ((mbuf[0] == '\xf0') && ((mbuf[7] & '\xE0')== 0) && ((mbuf[7] & '\x1f')== N_SPECTRAL)  ) //D1 - q + x + N=17

        {
            fdata = mbuf[5];  fdata = (fdata << 8) & 0xff00;
            fdata_2 = mbuf[6]; fdata_2 &= 0x00ff;
            fdata +=fdata_2;
            double di = 0.5;
        //fdata - это номер канала !!!!
            Hyst_Single->Ch_Data +=1;
            Hyst_Single->intervals->append(QwtIntervalSample(Hyst_Single->Ch_Data, fdata, fdata + di));
            Hyst_Single->hystogram->setSamples(*Hyst_Single->intervals);
            Hyst_Single->hystogram->attach(Hyst_Single);
            Hyst_Single->replot();
        }


        //qbuf = QByteArray(reinterpret_cast<char*>(mbuf), 16);
        //qbuf.resize(8);
        //chan_num = qbuf.mid(5,2);
    }
}
//++++++++++++
void MainWindow::Show_file_size_slot(QString sss, QString sss2)
{

    ui->File_Size_label->setText(sss);
    ui->Current_Offset_label->setText(sss2);
}
//+++++++++++++++++++++++++
void MainWindow::Show_file_offset_slot(QString sss)
{
    ui->Current_Offset_label->setText(sss);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++
/*
void MainWindow::Draw_Spectral_N_Chan_Slot (QByteArray * ibuf) //QByteArray &ibuf
{
// Получаем массив - добавка/приращение данных относительно прошлого срабатывания таймера
//Разбиваем массив на порции по 8 байт.
// каждую порцию анализируем и если там есть для нас данные, то пускаем на отрисовку.

    int buf_size;
    int i;
    QByteArray ba;
    const char *mbuf;
    UINT16 fdata, fdata_2;
    UINT16 fdata_alpha, fdata_alpha_2;//, fdata_fission;
    QString mydata;

    int dot_number; //Количество точек в  Hyst_Single->intervals не может быть больше int

//QByteArray ibuf = ("Зашли в отрисовку !\n");
    buf_size = ibuf->size();

//buf_size = 0;
    i =0;
 //memcpy(ba, ibuf->constData(), ibuf->size());
   // ba = ibuf;
  //  Write_To_Log(0x2000, "Зашли в отрисовку !\n");
//    mydata.sprintf("Точка перед циклом buf_size : %d \n", buf_size);
//    KAN->KK_Write_To_Log(0xCCCC, mydata);
    while (i <= buf_size)
    {
        QByteArray bb = ibuf->mid(i,8);
        mbuf = bb.constData();
 //       mydata.sprintf("Байт данных на отрисовку: "); mydata.append(bb.toHex().toUpper()); mydata.append("\n");
 //       Write_To_Log(0x2000, mydata);

//Выборка для ADP16
        //      разделитель слов  f0         Тип поманды b1               Номер станции 5 бит
             if ((mbuf[0] == '\xf0')  && ((mbuf[7] & '\x1f')== N_SPECTRAL_ADP16)  ) //D1 - q + x + N=17  && ((mbuf[7] & '\xEF')== 0)
             {
            //fdata - это номер канала в спектре !!!!

          //   KAN->KK_Write_To_Log(0xCCCC, "Отрисовка ADP16\n\n");

            //Альфа
            fdata_alpha = mbuf[5]; fdata_alpha = (fdata_alpha << 8) & 0xff00;
            fdata_alpha_2 = mbuf[6]; fdata_alpha_2 &= 0x00ff;
            fdata_alpha += fdata_alpha_2;
            // Осколок
            fdata = mbuf[3];  fdata = (fdata << 8) & 0xff00;
            fdata_2 = mbuf[4]; fdata_2 &= 0x00ff;
            fdata +=fdata_2; //fdata + fdata_2; //fdata +=fdata_2; //
            double di = 0.005;

            ADP16_Alpha->Ch_Data +=1;
            if ( ADP16_Alpha->Ch_Data >= 0xffff) ADP16_Alpha->Ch_Data = 0xffff;


            //У нас высота виджета 400 точек., так, что даже если их 4000 уже можно выводить каждую 10
            if (ADP16_Alpha->Ch_Data >= ADP16_Alpha->Y_MAX_Current)
             {
                ADP16_Alpha->Y_MAX_Current *=2; //if (ADP16_Alpha->Y_MAX_Current >= 0x20000) (ADP16_Alpha->Y_MAX_Current = 0x10000);

                ADP16_Alpha->setAxisScale(Plot::yLeft, 0.0, ADP16_Alpha->Y_MAX_Current);
                //setAxisScale( yLeft, y_min, y_max );

            }

            ADP16_Alpha->intervals->append(QwtIntervalSample(ADP16_Alpha->Ch_Data, fdata_alpha, fdata_alpha +di));

         // ADP16_Alpha->axi
//            ADP16_Alpha->hystogram->setSamples(*ADP16_Alpha->intervals);
//            ADP16_Alpha->hystogram->attach(ADP16_Alpha);
//            ADP16_Alpha->replot();

        //    if ( ADP16_Alpha->Ch_Data >= 0xffff) ADP16_Alpha->Ch_Data = 0xffff;

            ADP16_Fission->Ch_Data +=1;
            if ( ADP16_Fission->Ch_Data >= 0xffff) ADP16_Fission->Ch_Data = 0xffff;

            if (ADP16_Fission->Ch_Data >= ADP16_Fission->Y_MAX_Current)
             {
                ADP16_Fission->Y_MAX_Current *=2; //if (ADP16_Fission->Y_MAX_Current >= 0x20000) ADP16_Fission->Y_MAX_Current =0x10000;

                ADP16_Fission->setAxisScale(Plot::yLeft, 0.0, ADP16_Fission->Y_MAX_Current);
                //setAxisScale( yLeft, y_min, y_max );

            }

            ADP16_Fission->intervals->append(QwtIntervalSample(ADP16_Fission->Ch_Data, fdata, fdata + di));

//            ADP16_Fission->hystogram->setSamples(*ADP16_Fission->intervals);
//            ADP16_Fission->hystogram->attach(ADP16_Fission);
//            ADP16_Fission->replot();

        //     if ( ADP16_Fission->Ch_Data >= 0xffff) ADP16_Fission->Ch_Data = 0xffff;

            }//Выборка для ADP16



// Выборка для другого блока  - НЕ ADP16 Тип команды b0
//             if ((mbuf[0] == '\xf0') && (mbuf[7] == '\x40') && ((mbuf[1] & '\x1f')== N_SPECTRAL)) //D1 - q + x + N=17
 //            {
//            //fdata - это номер канала в спектре !!!!
//            // НЕ ADP26 хранит данные в других байтах - 3,4
//            fdata = mbuf[3];  fdata = (fdata << 8) & 0xff00;
//            fdata_2 = mbuf[4]; fdata_2 &= 0x00ff;
//            fdata +=fdata_2;
//            double di = 0.005;

//            Hyst_Single->Ch_Data +=1;
//            Hyst_Single->intervals->append(QwtIntervalSample(Hyst_Single->Ch_Data, fdata, fdata + di));
//            Hyst_Single->hystogram->setSamples(*Hyst_Single->intervals);
//            Hyst_Single->hystogram->attach(Hyst_Single);
//            Hyst_Single->replot();
//            }//Выборка для НЕ ADP16


//Обработка прерываний от других блоков (пока только чтение по A(0)*F(0))
           if ((mbuf[0] == '\xf0') && (mbuf[7] == '\x80') && ((mbuf[1] & '\x1f')== N_SPECTRAL)) //D1 - q + x + N=17
           {
            //fdata - это номер канала в спектре !!!!
            // НЕ ADP26 хранит данные в других байтах - 3,4

 //           Write_To_Log(0x2000, "Start append Single\n");
            fdata = mbuf[3];  fdata = (fdata << 8) & 0xff00;
            fdata_2 = mbuf[4]; fdata_2 &= 0x00ff;
            fdata +=fdata_2;
            double di = 0.005;

            Hyst_Single->Ch_Data +=1;
            if (Hyst_Single->Ch_Data >= Hyst_Single->Y_MAX_Current)
             {
  //              Write_To_Log(0x2000, "Start double scale !\n");
                Hyst_Single->Y_MAX_Current *=2;

                Hyst_Single->setAxisScale(Plot::yLeft, 0.0, Hyst_Single->Y_MAX_Current);
                //setAxisScale( yLeft, y_min, y_max );

            }

//            int dot_number; //Количество точек в  Hyst_Single->intervals не может быть больше int
            //int max = 2147483647 == 0x7FFF FFFF
            dot_number = Hyst_Single->intervals->size();

      //      if ((dot_number + Hyst_Single->Ch_Data) >= imax  ) Hyst_Single->intervals->clear(); //очищаем, иначе переполнится.
      //      if ((dot_number + Hyst_Single->Ch_Data) >= Interval_Limit  ) Hyst_Single->intervals->clear(); //очищаем, иначе переполнится.

            Hyst_Single->intervals->append(QwtIntervalSample(Hyst_Single->Ch_Data, fdata, fdata + di));
//            Hyst_Single->hystogram->setSamples(*Hyst_Single->intervals);
//            Hyst_Single->hystogram->attach(Hyst_Single);
//            Hyst_Single->replot();

   //         KAN->KK_Write_To_Log(0xCCCC, "finish append Single\n");
            }//Выборка для НЕ ADP16

        i+=8;

    }//while

 //Точки добавили, а теперь реплот
//    KAN->KK_Write_To_Log(0xCCCC, "Start replot Single\n");


//    QPainter * painter;
//    painter->setPen(Qt::blue);
//    QwtScaleMap x_chan, y_chan;

//    double p1, p2;
//    p1 = 1.0; p2 = 50.0;
//    x_chan.setPaintInterval(p1, p2);
//    y_chan.setPaintInterval(p1, p2);



//    Hyst_Single->hystogram->drawLines(painter, x_chan, y_chan, 1, 50);



    //int max = 2147483647 == 0x7FFF FFFF

    dot_number = Hyst_Single->intervals->size();
    str.sprintf("Interval size is %d \n", dot_number);

    Write_To_Log(0x7777, str);

    Hyst_Single->hystogram->setSamples(*Hyst_Single->intervals);
    Hyst_Single->hystogram->attach(Hyst_Single);
    Hyst_Single->replot();
//    KAN->KK_Write_To_Log(0xCCCC, "Finish replot Single\n");



             ADP16_Alpha->hystogram->setSamples(*ADP16_Alpha->intervals);
             ADP16_Alpha->hystogram->attach(ADP16_Alpha);
             ADP16_Alpha->replot();


              ADP16_Fission->hystogram->setSamples(*ADP16_Fission->intervals);
             ADP16_Fission->hystogram->attach(ADP16_Fission);
             ADP16_Fission->replot();










    // Выборка для другого блока  - НЕ ADP16 Тип команды b0
 //   if ((mbuf[0] == '\xf0') && (mbuf[7] == '\x80') && ((mbuf[1] & '\x1f')== N_SPECTRAL)) //D1 - q + x + N=17



}
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++

//ВЫдаем сообщения в зависимости от кода возврата работы функции ftdi_init
/*
 * Основная идея создания такой функции проистекает из того, что ошибка может возникнуть в процессе выполнения нескольких операций внутри функции.
 * Поэтому нужно иметь ответ в виде СТАТУСА(структура, перечисление enum), внутри которого будет обозначено, на каком этапе возникла ошибка.
 * Перенести возврат статуса с последующим анализом/ответом в MainWindow важно для максимальной автономности работы ф-ции,
 * чтобы вдальнейшем впихнуть её в dll
*/
void MainWindow::ftdi_answer_parser(int value)
{
    QString str, str2;
    QDateTime curdate ;

    QTextStream uin(&KAN->LogFile);


    switch (value) {

                                    //KK_version
    case 0xf000: str = "The application \"";  str +=target_name; str += "\""; Write_To_Log(value, str.append(" is started successfully!!!\n"));  break;




    case 0x0000: // Общий ответ от всех функций
                str.sprintf("Функция сработала без ошибок !\n"); //ui->FTDI_List_textBrowser->append(str);
        break;


// ============================================   Блок ответов ф-ции ftdi_init()    =================================================================
 //   case 0x0000:
 //               str.sprintf("Устройство распознано!\n");ui->FTDI_List_textBrowser->append(str);
 //       break;


    case 0x0001: Write_To_Log(value, "ftdi_init()->Error in getting the number of devices\n");
           //   str.sprintf("0x%04X: ftdi_init()->Error in getting the number of devices\n", value); ui->FTDI_List_textBrowser->append(str);
        break;
    case 0x0002:
                str.sprintf("0x%04X: ftdi_init()->There are no FTDI devices installed\n", value); Write_To_Log(value, str);  break;
    case 0x0003:
                str.sprintf("0x%04X: ftdi_init()->Не удалось открыть ПОРТ А !\n", value); Write_To_Log(value, str);  break;

    case 0x0004:
                str.sprintf("0x%04X: ftdi_init()->Канал А в устройстве не подключен\n", value); Write_To_Log(value, str);  break;

    case 0x0005: Write_To_Log(value, "ftdi_init()->Не удалось открыть ПОРТ В !\n");         break;

    case 0x0006: Write_To_Log(value, "ftdi_init()-> Устройство не подключено !!!\n Подключите устройство и перезапустите программу\n"); break; //Канал В в устройстве не подключен. Не могу передать команду

    case 0x0007: Write_To_Log(value, "ftdi_init()->Не удалось собрать детализацию остройств!\n");  break;

    case 0x0AAA:  Write_To_Log(value, "ftdi_init()->Устройство распознано!\n");  break;

// ============================================ END of   Блок ответов ф-ции ftdi_init()    =================================================================

// ============================================   Блок ответов ф-ции Read_A_Single()    =================================================================
    case 0x0008:
                str.sprintf("0x%04X: Read_A_Single()->FT_GetStatus() Error of FT_GetStatus", value); Write_To_Log(value, str);
                Device_NOT_connected_Slot();        break;
    case 0x0009:
                str.sprintf("0x%04X: Read_A_Single()->FT_Read() Error  Не удалось считать данные из FTDI\n", value); Write_To_Log(value, str);
                Device_NOT_connected_Slot();        break;

    case 0x000A: str.sprintf("0x%04X: Read_A_Single()->mfile.isOpen() Error  Файл не открыт !\n", value); Write_To_Log(value, str);  break;

    case 0x000B: str.sprintf("0x%04X : Read_A_Single()->RxBytes=0 -> Rx-буффер FTDI пуст\n", value); Write_To_Log(value, str); break;

// ============================================ END of  Блок ответов ф-ции Read_A_Single()    =================================================================

// ============================================   Блок ответов ф-ции Write_B_Single()    =================================================================

    case 0x000C: str.sprintf("Write_B_Single()->error writing !!!\n"); Write_To_Log(value, str);  break;

// ============================================ END of  Блок ответов ф-ции Write_B_Single()    =================================================================

// ============================================   Блок ответов ф-ции FTDI_D2XX::File_Open    =================================================================
    case 0x000D: str.sprintf("0x%X: File_Open()->mfile.open()->File does not exist !!!\n", value); Write_To_Log(value, str);  break;

    case 0x0013:  str.sprintf("File %s  is opened for writing!!!\n", Spectral_Data_File);  Write_To_Log(value, str);  break;

// ============================================ END of   Блок ответов ф-ции FTDI_D2XX::File_Open    =================================================================

// ============================================   Блок ответов ф-ции KK_Inhibit()    =================================================================
    case 0x000E: Write_To_Log(value, "KK_Inhibit->FT_Write Нет связи с КК !!!\n"); Device_NOT_connected_Slot();   break;

    case 0x000F: Write_To_Log(value, "KK_Inhibit()->Ответ Верный !!!\n"); Device_Connected_Slot();    break;

    case 0x0010: str.sprintf("0x%04X: KK_Inhibit->memcmp()->НЕВЕРНО ! \n", value); Write_To_Log(value, str); break;

// ============================================   Блок ответов ф-ции FTDI_D2XX::Log_File_Open    =================================================================
    case 0x0011:
                str = curdate.toString("yyyy-MM-dd__hh:mm:ss:z"); str.append(" > ");
                str2.sprintf("0x%04X: Log_File_Open()->LogFile.open()->File does not exist !!!\n", value);
                str.append(str2);
                KAN->LogFile.write(str.toUtf8().constData() ); //KAN->LogFile.write("\n");

      break; //Log_File_Open

    case 0x0012: str.sprintf("File %s  is opened for writing!!!\n", Log_File_Name); Write_To_Log(value, str);  break;

// ============================================ END of   Блок ответов ф-ции FTDI_D2XX::Log_File_Open    =================================================================

//=============================================  Блок ответов ф-ции Command_File_Execute  =================

    case 0x0014: str.sprintf("Не могу открыть файл "); str.append(Camac_Commands_File); str.append("\n"); Write_To_Log(value, str);  break;

//============================================= END of Блок ответов ф-ции Command_File_Execute  =================

//===============  Блок ответов ф-ции Read_A_Single() при выполнении KAN->KK_Inhibit(pData, 64)
                    case 0x1008: str.sprintf("0x%04X: KK_Inhibit()->Read_A_Single()->FT_GetStatus() Error of FT_GetStatus", value); Write_To_Log(value, str);
                                 Device_NOT_connected_Slot(); break;

                    case 0x1009: str.sprintf("0x%04X: KK_Inhibit()->Read_A_Single()->FT_Read() Error  Не удалось считать данные из FTDI\n", value); Write_To_Log(value, str);
                                 Device_NOT_connected_Slot(); break;

                    case 0x100A: str.sprintf("0x%04X: KK_Inhibit()->Read_A_Singl()e->mfile.isOpen() Error  Файл не открыт !\n", value); Write_To_Log(value, str);  break;

                    case 0x100B: str.sprintf("0x%04X : KK_Inhibit()->Read_A_Single()->RxBytes=0 -> Rx-буффер FTDI пуст\n", value);Write_To_Log(value, str);  break;
// ===============  END OF  Блок ответов ф-ции Read_A_Single() при выполнении KAN->KK_Inhibit(pData, 64)


    case 0x02ff: str.sprintf("0x%04X: Устройство не подключено !!! Команды не выполнены\n", value); Write_To_Log(value, str);  break;



// =================== Блок ответов ф-ции Read_A_Single() при выполнении Command_File_Execute (QString fname) =================================================================

    case 0x0300: // Общий ответ от всех функций
        //Write_To_Log(value, "Command_File_Execute()->Read_A_Single() performed correctly\n");
        Write_To_Log(value, "Обработчик команд из файла выполнен\n");
        ;
        break;

    case 0x0301: str.sprintf("0x%04X: Завершено выполнение команд из файла !\n", value); Write_To_Log(value, str);  break;

    case 0x0302: str.sprintf("0x%04X: В командном файле ошибка, ни одна команда не выполнена !\n", value); Write_To_Log(value, str);  break;

    case 0x0308:
                str.sprintf("0x%04X: Command_File_Execute()->Read_A_Single()->FT_GetStatus() Error of FT_GetStatus", value);
                Device_NOT_connected_Slot();
                Write_To_Log(value, str);

        break;
    case 0x0309: str.sprintf("0x%04X: Command_File_Execute()->Read_A_Single()->FT_Read() Error  Не удалось считать данные из FTDI\n", value);
                 Device_NOT_connected_Slot(); Write_To_Log(value, str); break;

    case 0x030A: str.sprintf("0x%04X: Command_File_Execute()->Read_A_Single()->mfile.isOpen() Error  Файл не открыт !\n", value); Write_To_Log(value, str); break;

    case 0x030B: str.sprintf("0x%04X : Command_File_Execute()->Read_A_Single()->RxBytes=0 -> Rx-буффер FTDI пуст\n", value); Write_To_Log(value, str); break;

// ============= END of  Блок ответов ф-ции Read_A_Single() при выполнении Command_File_Execute (QString fname)    =================================================================
// ======   Блок ответов ф-ции Write_B_Single() ===================================================================================================

    case 0x030C: str.sprintf("Command_File_Execute()->Write_B_Single()->error writing !!!\n"); Write_To_Log(value, str); break;

// ======   Блок ответов ф-ции Write_B_Single() при выполнении on_Stop_ALL_pushButton_clicked()  =================================================================

   case 0x0400: str.sprintf("Spec_Command send successfully !!!\n"); Write_To_Log(value, str); break; // on_Stop_ALL_pushButton_clicked()->Write_B_Single()-> Ошибок нет, ничего не пишем

   case 0x040C: str.sprintf("on_Stop_ALL_pushButton_clicked()->Write_B_Single()->error writing !!!\n"); Write_To_Log(value, str);  break;


    default:    str.sprintf("Неизвестный код возврата от ftdi_init()\n");  Write_To_Log(value, str);  break;
    }


}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Выполняем последовательно команды КАМАК, записанные в файле ./camac_commands.txt
void MainWindow::on_Load_commands_pushButton_clicked()
   {
 //   int value;
 //   value= 0x02ff;
 //   str="Зашли в обработчик нажатия кнопки\n";Write_To_Log(value, str);
    ftdi_answer_parser(Command_File_Execute(Camac_Commands_File));
   // value= Command_File_Execute(Camac_Commands_File);
 //  str="Вышли из обработчик нажатия кнопки\n";Write_To_Log(value, str);
                                                       }
//++++++++++++++++++++++++++++++++
// Парсим все строки командного файла.
// Если находим ошибку в записи команды - выходим с ошибкой,пишем в лог, ни одну команду не выполняем.
// Если ошибок в командах нет, возвращаемся в начало файла и выполняем последовательно все команды, по одной в строке.
int  MainWindow::Command_File_Execute (QString fname)
{

 //   BOOL ALL_COMMANDS_CORRECT = false;
  //  QDateTime curdate ;
    QString str1, str2;

  //  QTextStream uin;

    int ftdi_error_code = 0;
    int value = 0x02ff; //Значение ошибки
    QFile f(fname);
    int n = 0;
    //char b_Data[MIN_Rx_BUFFER_SIZE]; //Сюда запишем принятые от ftdi данные

   // str1="Зашли в Command_File_Execute\n";Write_To_Log(0XAAAA, str1);

    // Для начала проверяем подключение.

    if (!camac_tab->KK_connected) {//   "Устройство не подключено !!! Команды не выполнены\n";
        return 0x02ff;
    }

    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
    n = QTextStream(&f).readAll().split('\n').count();
    str2.sprintf("Количество строк в файле %s : %d \n", Camac_Commands_File, n);
    Write_To_Log(value, str2);

    f.seek(0); //Важно ссссука, ведь ранее мы весь файл уже считали  n = QTextStream(&f).readAll().split('\n').count();
   // QTextStream uin(&KAN->LogFile);
    value = 0x0300;
    n=1;
    while (!f.atEnd())
    {

       str1 = f.readLine(); // Считываем строку.
       str1 = str1.simplified();
       ftdi_error_code =  parse_command_line(str1); // Парсим строку, присваиваем переменным  N, A, F, Данные


       switch (ftdi_error_code) {
       case 0:
          //Строка верная, ничего не делаем
            break;
       case 1:
          //Строка комментарий, приравниваем к правильной строке, ничего не делаем
           ftdi_error_code += value; // 0x0301
           break;
       case 2:
          //Строка имеет неправильный формат, сообщаем номер неправильной строки в лог
           str1.sprintf("Строка %d в файле %s имеет неправильный формат \n", n, Camac_Commands_File); Write_To_Log(value, str1);
           return 0x0302;
           break;

       default:
           break;
       } // switch (ftdi_error_code)




         n++;
    } //while (!f.atEnd()) парсинг строк
//
//    ALL_COMMANDS_CORRECT = TRUE;
// Теперь снова проходим весь файл сначала и выполняем команды.
// Комментарии игнорируем

    f.seek(0); //Встаем в начало файла
    value = 0x0300;
    n=0;
    while (!f.atEnd())
    {
        str1 = f.readLine(); // Считываем строку.
        str1 = str1.simplified();
        ftdi_error_code =  parse_command_line(str1); // Парсим строку, присваиваем переменным  N, A, F, Данные
        n++; // Увеличили счетчик выполненных команд

        if (!ftdi_error_code){ //Это команда, не комментарий

            ftdi_error_code =  KAN->Write_B_Single(byOutputBuffer, TRANSFER_SIZE); // Отправляем команду по результатам парсинга
            ftdi_answer_parser(ftdi_error_code + value); //+ 0x0300
            //Пишем в лог, что команда КАМАК отправлена в КК, выполнена и получен ответ на эту команду
                 if (!ftdi_error_code){ //Если возвращает '0' то для if это false, а для ftdi_answer_parser - это отсутствие ошибок

                     str2 = "NOTICE: Command "; str2.append(str1); str2.append(" is sent successfully!!!\n");
                     Write_To_Log(value, str2);
                 }
                 // А если команда вернет ошибку, то это будет записано в лог. Это уже сделано в Write_B_Single()

        }
     //   n++;
    } // while (!f.atEnd()) N2

    f.close();
// Прошли через весь файл, выполнили команды.
   str1.sprintf("Выполнено %d команд в файле %s \n", n, Camac_Commands_File); Write_To_Log(value, str1);
   ftdi_error_code =  0x0300;
 } //if (f.open(QIODevice::ReadOnly | QIODevice::Text))

    else {ftdi_error_code = 0x0014;} // Не могу открыть файл команд

return ftdi_error_code;
}
//++++++++++++++++++++++++++++++++++=
// Парсим строку из файла команд, выдаем NAF
int MainWindow::parse_command_line(QString comstr)
{
// Тут должна быть проверка строки на соответствие шаблону   QRegExp rx("^N\[[0-9][0-9]\]A\[[0-9][0-9]\]F\[[0-9][0-9]\]");
// Если строка неправильная, то сразу QMessageBox     !!!
    int ftdi_error_code = 0;
  //  QRegExp cmt ("^\s*#"); // Строка комментария, начинается с # или с пробела (1 или несколько) и '#'
    QRegExp cmt ("^$|^\n$|^\s*#[^\n]*(?=\n|$)");
    QRegExp comd("N\\[[0-9][0-9]\\]A\\[[0-9][0-9]\\]F\\[[0-9][0-9]\\]|N\\[[0-9][0-9]\\]A\\[[0-9][0-9]\\]F\\[[0-9][0-9]\\][A-Fa-f0-9][A-Fa-f0-9][A-Fa-f0-9][A-Fa-f0-9]");
    QRegExp rx ("([0-9][0-9])"); //
    QRegExp dx ("([A-Fa-f0-9][A-Fa-f0-9][A-Fa-f0-9][A-Fa-f0-9])");
    QStringList list, d_list;

    QString str;
    int pos = 0;

    if (cmt.indexIn(comstr) == 0) return 1;//Специальный код завершения 1. Это строка комментария, не парсим, выходим.
    if (comd.indexIn(comstr) == -1) return 2; // Неправильный формат, не совпадает с шаблоном


    while ((pos = rx.indexIn(comstr, pos)) != -1) {
        list << rx.cap(1);
        pos += rx.matchedLength();
    }

    N_CAMAC =  list.at(0).toInt();
    A_CAMAC =  list.at(1).toInt();
    F_CAMAC =  list.at(2).toInt();

//+++++++++++++++++++ Data

    if (dx.indexIn(comstr) != -1) {

     pos = 0;
    while ((pos = dx.indexIn(comstr, pos)) != -1) {
        d_list << dx.cap(1);
        pos += dx.matchedLength();
    }

    bool ok;
    ok = false;

     str=d_list.at(0);
     QByteArray mystr = QByteArray::fromHex(str.toLocal8Bit().constData());
  //  qDebug()<< "Data as text = " <<d_list.at(0);

  //   n =  mystr.toHex().toInt(&ok, 16);
     ToDev_Bits = mystr.toHex().toInt(&ok, 16);
   // qDebug()<< "Data as Int(), digital = " << n;
   }//
//+++++++++++++++++ END of Data

    CAMAC_make_command(); // Получили буфер на отправку в ПК

    //ToDev_Bits;
    return ftdi_error_code;

}
//++++++++++++++++++++++++++++++++++++++
//Пишет в лог-файл номер ошибки value и сообщение message
void MainWindow::Write_To_Log (int value, QString log_nessage)
{
    QDateTime curdate ;
    QTextStream uin(&KAN->LogFile);

    QString str, str2;
    curdate = QDateTime::currentDateTime();

    str = curdate.toString("yyyy-MM-dd__hh:mm:ss:z").toLocal8Bit(); str.append(" > ");
    str2 = QString("0x%1: ").arg(value, 4, 16, QChar('0'));

    uin << str << str2 << log_nessage;


}


//+++++++++++++++++++++++++++++++++++++++++++
// Отрисовка спектра одиночного во вкладке SINGLE
void MainWindow::on_Show_Selected_Spectral_Single_pushButton_clicked()
{
    // По нажатию кнопки "ПОКАЗАТЬ" отображаем спектр для выбранной станции  и выбранного номера канала этой станции

   //Предполагается, что набор был запущен ранее, а сейчас только выводим данные из набранного (файла) буфера !!!.
   // Т.к. не работает fpga то сгенерим файл здесь. Т.е. запустим таймер, который будет записывать в файл.
   // А другой таймер будет считывать из файла.
    N_SPECTRAL = ui->N_Spectral_spinBox_Single->value();
    N_SPECTRAL_CHANNEL = ui->N_Spectral_Channel_spinBox_Single->value();

    //Передаем значение N_SPECTRAL в класс Plot

 //   Write_To_Log(0, "Передаю номер станции \n");
    emit pass_N_SPECTRAL_signal (&N_SPECTRAL, COMMAND_b2);



    //KAN->sfile.setFile(KAN->mfile);

   // KAN->mfile.flush();
    timer_2->setInterval(1000); timer_2->start(); //Запустили таймер считывания из (файла) буфера и отрисовки спектра
    // Обработчик для таймера_2 - это FTDI_D2XX::Get_Spectral_Points_Slot()
   /*
    byOutputBuffer[0] = 0x11;
    byOutputBuffer[1] = 0x20;
    byOutputBuffer[2] = 0x22;
    byOutputBuffer[3] = 0x22;
   */

}

//+++++++++++++++++++++++++++++++=
void MainWindow::Camac_SPEC_Command(UINT8 F_number, UINT16 F_DATA)
{
  UINT8  naf;
  UINT8 b1, b2, b3, b4; // номер байта в посылке

    //Формируем спецкоманду

  naf = F_number;
  b1 = 0xC0;
  b1 |=  ((naf<<1) & 0x20);  //F16 на 6-м бите, а не на 5, как в арифметике.

  b2 = 0x00;
  b2 |=  ((naf<<4) & 0xf0); //F8,F4,F2,F1 на 8-5 битах

  b3 = (F_DATA >> 8) & 0x00ff;
  b4 = F_DATA & 0x00ff;

  byOutputBuffer[0]= b1;
  byOutputBuffer[1]= b2;
  byOutputBuffer[2]= b3;
  byOutputBuffer[3]= b4;

  //Обновляем буффер в класее FTDI
//  emit push_data_buffer(byOutputBuffer); // Копируем данные массива byOutputBuffer в массив внутри класса FTDI_D2XX

/*
  QString str;
  QByteArray mystr = QByteArray(reinterpret_cast<char*>(byOutputBuffer), TRANSFER_SIZE*2); // 8 символов для 4 байт
  mystr.resize(TRANSFER_SIZE);
  str.sprintf("СПЕЦ-КОМАНДА : ");
  str.append(mystr.toHex().toUpper());
  str.append("\n");
  Write_To_Log(0x7777, str);
*/

}
//++++++++++++++++++++++++++++++++++
// Формируем команду по которой будет запуск потока А на считывание.
void MainWindow::Camac_Run_Always (void)
{
    byOutputBuffer[0] = 0x11;
    byOutputBuffer[1] = 0x20;
    byOutputBuffer[2] = 0x22;
    byOutputBuffer[3] = 0x22;

}



void MainWindow::on_N_Spectral_spinBox_Single_valueChanged(int arg1)
{
    // Изменили номер станции, для которой хотим "Показать Real Time"
    // Отсюда переходим на общую ф-цию смены окна отрисовки.
    // Передаем значения N "N_Spectral_Channel_spinBox_Single" - те, которые в данный момент установлены.
    N_SPECTRAL = arg1; //ui->N_Spectral_spinBox_Single->value();
    N_SPECTRAL_CHANNEL = ui->N_Spectral_Channel_spinBox_Single->value();

    str.sprintf("Изменился номер СТАНЦИИ для отрисовки спектра. Текущий номер : %d \n", N_SPECTRAL); Write_To_Log(0x7777, str);

 //   str.sprintf("Передаваемый аргумент имеет значение : %d \n", arg1); Write_To_Log(0x7777, str);

    Hyst_Single->replot();

    // Ага, а буфер остался со старыми точками ???!!!
}

void MainWindow::on_N_Spectral_Channel_spinBox_Single_valueChanged(int arg1)
{
    // Изменили номер канала в станции, для которой хотим "Показать Real Time"
    // Отсюда переходим на общую ф-цию смены окна отрисовки.
    // Передаем значения N "N_Spectral_Channel_spinBox_Single" - те, которые в данный момент установлены.
//    N_SPECTRAL = arg1; //ui->N_Spectral_spinBox_Single->value();
    N_SPECTRAL_CHANNEL = arg1; //ui->N_Spectral_Channel_spinBox_Single->value();

    str.sprintf("Изменился номер КАНАЛА для отрисовки спектра. Текущий номер : %d \n", arg1); Write_To_Log(0x7777, str);
}
