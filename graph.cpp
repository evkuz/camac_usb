#include "graph.h"


//++++++++++++++++++++++++++++++++++++++++++++++
/*
void Draw_Spectral_N_Chan (QByteArray *qbuf, UINT8 N_number, UINT8 Chan_number)
{
//Выборка данных из массива qbuf для заданной станции - N_number и канала в этой станции - Chan_number,
//добавление этих данных к гистограмме - указатель или номер в списке
// Пока гистограмма одна - можно и без указателя, а вот если будем добавлять

}
*/
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

Plot::Plot()
{

    N_number=0;
    N_curr_number=0;
    Y_MAX_Current=0;
}

//+++++++++++++++++++++++++++++++++++++++

// Сюда попадаем из слота для timer_2, где эмиттируем сигнал "Draw_Spectral_N_Chan_Signal", указатель на * ibuf оттуда же.
// Еще нужно передавать указатель на виджет, который перерисовываем.
// Либо все 3 графика сразу ???
// 05.11.2019 Пришла пора вводить проверку на номер станции.
// Если номер станции поменяли, то нужно очищать текущее окно и начинать отрисовку заново
// В таком случае проще очищать ibuf ?
// Значит вводим новую переменную - N_curr_number текущий номер станции - это тот номер, для которого УЖЕ нарисован спектр.
// И если этот текущий номер не совпадает с тем, который в переменной N_number, значит номер станции поменялся
// и это N_number.


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
 //   UINT16 fdata_alpha, fdata_alpha_2;//, fdata_fission;
    QString mydata;

    // Т.к. в разных типах блоков данные находятся в разных байтах,
    // то номера этих байт будем хранить в переменных
    UINT8 msb, lsb; // Старший байт, младший байт
    char mbuf_7;    // Последний байт
    UINT8 st_number_value_mbuf, st_number_index_mbuf;
    UINT8 st_number;

  //  int dot_number; //Количество точек в  Hyst_Single->intervals не может быть больше int


   // mydata.sprintf("REPLOT command type is %d \n", command_type);
  //  emit Show_data_signal(mydata);

  // Откуда тут знание о "command_type" ?
  // А из слота
//     mbuf_7 = '\xf0'; // Инициализируем

/*
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
      case 3 :   //b2
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

*/

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
// Ответ на команду b0
// if ((mbuf[0] == '\xf0')

//+++++++++++++++ Определяем тип блока и данные для отрисовки +++++++++++++++
           if ((mbuf[0] == '\xf0') && (mbuf[7] == type_B0) )
           {
             // Команда b0 задаем значения msb, lsb, определяем номер станции
               msb = 3;
               lsb = 4;
               st_number = mbuf[1] & 0x1f;
               //mydata.sprintf("Номер станции: %d", st_number); mydata.append("\n");
               //emit Show_data_signal(mydata);

           }

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//Ответ на команду типа b0:
   //        if ((mbuf[0] == '\xf0') && (mbuf[7] == mbuf_7) && ((mbuf[st_number_index_mbuf] & st_number_value_mbuf)== N_number)) //D1 - q + x + N=17
           if (st_number == N_number)
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
// Принимаем значение станции + номер типа команды для выборки данных на отрисовку спектра
// Сохраняем полученное значение номера станции в "public" переменную для класса plot
void Plot::pass_N_SPECTRAL_slot (UINT8 * st_number, UINT8 comm_type)
{
    QString str;
    N_number = *st_number;
    command_type = comm_type;
    // type 1 = b0, 2 = b1, 3 = b2

  //  N_number = 100;
    //str.sprintf("N_number value %d, command type is %d \n", N_number, command_type);
    str.sprintf("Делаем выборку для станции %d, command type is %d \n", N_number, command_type);

    emit Show_data_signal(str);

}
//+++++++++++++++++++++++++++++++++++++++
// Слот очистки буферов с точками гистограммы
// Вызывается сигналом очистки буферов
void Plot::clear_intervals_buffer_slot()
{
    QString str;
    int cap = this->intervals->size();
    this->intervals->clear();
    this->Ch_Data = 0;
    str.sprintf("Intervals before clear is %d, and after clearing is %d", cap, this->intervals->size() );

    emit Show_data_signal(str);
}
//++++++++++++++++++++++++++++++++
// Слот отрисовки шкалы Y графика в исходном значении, т.е. как при создании шкалы
void Plot::make_original_scale_slot()
{
    this->Y_MAX_Current = 4096;
    this->setAxisScale(Plot::yLeft, 0.0, this->Y_MAX_Current);
    this->Ch_Data = 0.0;//1000000.0;// 0x0000;
    this->Ch_Data_2 = 0.0;//0x0000;

    this->replot();

}
