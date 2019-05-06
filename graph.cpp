#include "graph.h"

//++++++++++++++++++++++++++++++++++++++++++++++
void Draw_Spectral_N_Chan (QByteArray *qbuf, UINT8 N_number, UINT8 Chan_number)
{
//Выборка данных из массива qbuf для заданной станции - N_number и канала в этой станции - Chan_number,
//добавление этих данных к гистограмме - указатель или номер в списке
// Пока гистограмма одна - можно и без указателя, а вот если будем добавлять

}
//+++++++++++
/*
void Plot::ek_drawLines(void)
{
    QPainter * painter;
  //  painter.drawLine(10, 20);
    painter->setPen(Qt::blue);
    QwtScaleMap x_chan, y_chan;

    double p1, p2;
    p1 = 1.0; p2 = 50.0;
    x_chan.setPaintInterval(p1, p2);
    y_chan.setPaintInterval(p1, p2);

   hystogram->drawLines(painter, x_chan, y_chan, 1, 50);

}
*/
//+++++++++++++++++++++++++++++++++++++++

// Сюда попадаем из слота для timer_2, где эмиттируем сигнал "Draw_Spectral_N_Chan_Signal", указатель на * ibuf оттуда же.
// Еще нужно передавать указатель на виджет, который перерисовываем.
// Либо все 3 графика сразу ???
void Plot::Draw_Spectral_N_Chan_Slot (QByteArray * ibuf) //QByteArray &ibuf
{
// Получаем массив - добавка/приращение данных относительно прошлого срабатывания таймера
// Разбиваем массив на порции по 8 байт.
// каждую порцию анализируем и если там есть для нас данные, то пускаем на отрисовку.

    int buf_size;
    int i;
    QByteArray ba;
    const char *mbuf;
    UINT16 fdata, fdata_2;
    UINT16 fdata_alpha, fdata_alpha_2;//, fdata_fission;
    QString mydata;

    UINT8 msb, lsb;
    char mbuf_7, st_number_index_mbuf;
    UINT8 st_number_value_mbuf;

  //  int dot_number; //Количество точек в  Hyst_Single->intervals не может быть больше int


   // mydata.sprintf("REPLOT command type is %d \n", command_type);
  //  emit Show_data_signal(mydata);

  // Откуда тут знание о "command_type" ?
  // А из слота
    switch (command_type) //См "Формат данных4.docx"
    {
      case 1 :
        msb = 3;
        lsb = 4;
        mbuf_7 = 0x40;           // Тип команды. это 1. b0
        st_number_index_mbuf = 1;
        st_number_value_mbuf = 0x1f;
      case 2 :
        msb = 3;
        lsb = 4;
        break;
      case 3 :   //b0
        msb = 3;
        lsb = 4;
        mbuf_7 = '\x80';           // Тип команды. это 3.
        st_number_index_mbuf = 1;
        st_number_value_mbuf = '\x1f';
         break;
     case 4 :
        msb = 3;
        lsb = 4;
        break;

    }



    buf_size = ibuf->size();

 //   mydata.sprintf("Bufer size is  %d \n", buf_size);
 //   emit Show_data_signal(mydata);

    i =0;
    while (i <= buf_size)
    {
        QByteArray bb = ibuf->mid(i,8);
        mbuf = bb.constData();
 //       mydata.sprintf("Байт данных на отрисовку: "); mydata.append(bb.toHex().toUpper()); mydata.append("\n");
 //       Write_To_Log(0x2000, mydata);


//Обработка прерываний от других блоков (пока только чтение по A(0)*F(0))
           if ((mbuf[0] == '\xf0') && (mbuf[7] == mbuf_7) && ((mbuf[st_number_index_mbuf] & st_number_value_mbuf)== N_number)) //D1 - q + x + N=17
           {
            //fdata - это номер канала в спектре !!!!
            // НЕ ADP16 хранит данные в других байтах - 3,4
            fdata = mbuf[msb];  fdata = (fdata << 8) & 0xff00;
            fdata_2 = mbuf[lsb]; fdata_2 &= 0x00ff;
            fdata +=fdata_2;
            double di = 0.005;

            this->Ch_Data +=1;  //this->  Hyst_Single->
            if (this->Ch_Data >= this->Y_MAX_Current)
             {
                this->Y_MAX_Current *=2;
                this->setAxisScale(Plot::yLeft, 0.0, this->Y_MAX_Current);
            }

            this->intervals->append(QwtIntervalSample(this->Ch_Data, fdata, fdata + di));
            }//Выборка для НЕ ADP16

        i+=8;

    }//while

 //Точки добавили, а теперь реплот
//    KAN->KK_Write_To_Log(0xCCCC, "Start replot Single\n");

    //int max = 2147483647 == 0x7FFF FFFF

    mydata.sprintf("Number of points : %.0f \n", this->Ch_Data);
    emit Show_data_signal(mydata);

    this->hystogram->setSamples(*this->intervals);
    this->hystogram->attach(this);
    this->replot();

}
//++++++++++++++++++++++++++++++++++++=
//+++++++++++
// Это слот сигнала, запускаемого из слота обработки timer_2(timeout)
//Принимаем значение станции + номер типа команды для выборки данных на отрисовку спектра
void Plot::pass_N_SPECTRAL_slot (UINT8 * st_number, UINT8 comm_type)
{
    QString str;
    N_number = *st_number;
    command_type = comm_type;
  //  N_number = 100;
    str.sprintf("N_number value %d, command type is %d \n", N_number, command_type);
    emit Show_data_signal(str);

}
//+++++++++++++++++++++++++++++++++++++++

