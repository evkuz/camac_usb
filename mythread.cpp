
#include "mainwindow.h"
//#include "mythread.h"
#include <iostream>

#include <boost/chrono/include.hpp>

#include <boost/chrono/chrono_io.hpp>
#include <boost/chrono/system_clocks.hpp>
#include <boost/chrono/io/timezone.hpp>
#include <iomanip>


using namespace std;
using namespace boost::chrono;


//MyThread::MyThread(QObject *parent) : QObject(parent)
MyThread::MyThread()
{
 finthread = false;
 pause_thread = false;
}

MyThread::~MyThread()
{
// delete chan_A;
// delete chan_B;
}
//+++++++++++++
void MyThread::A_SLOT() // ОБработчик для thread_A прием данных в ПК от железа по каналу А  QThread::usleep(150);
{

    while ((!this->finthread) ) // !!!event-loop!!! && !this->once
    {
        QThread::usleep(1000);
 /*       boost::chrono::steady_clock::time_point start= boost::chrono::steady_clock::now();
        boost::chrono::steady_clock::duration delay= boost::chrono::microseconds(300);
        while (boost::chrono::steady_clock::now() - start <= delay) {;}
*/

  //Вызываем функцию или связываем с функциями marsohod.cpp через сигнал/слот
    // Если надо поставить на паузу, то не выдаем этот сигнал !!!
   if (!this->pause_thread) emit Process_A(); // Запись данных из железа  в ПК по каналу А
  // else {;} //Пауза, пустая операция
    }
    emit finished();


//emit finished();

}
//+++++++++++=

void MyThread::B_SLOT ()       // ОБработчик для thread_2
{
    int interval = this->Duration_mks;
    while ((!this->finthread) ) //& !this->once
    {
      //  QThread::usleep(5);

        boost::chrono::steady_clock::time_point start= boost::chrono::steady_clock::now();
        boost::chrono::steady_clock::duration delay= boost::chrono::microseconds(interval);//50 //this->Duration_mks interval
        while (boost::chrono::steady_clock::now() - start <= delay) {;}


  //Вызываем функцию или связываем с функциями marsohod.cpp через сигнал/слот
   if (!this->pause_thread)    emit Process_B(); // Запись данных из ПК в железо по каналу B
  // else {;} //Пауза, пустая операция

    }//while
      emit finished();
}
//+++++++++++++++++++++
void MyThread::Thread_Pause_Slot()
{
   this->pause_thread = TRUE;

}
//+++++++++++++++++++++++++++++
void MyThread::Thread_Pause_OFF_Slot()
{

  this->pause_thread = FALSE;
}
//++++++++++++++++++++++++++++++
// Получаем адрес буфера с данными. копируем эти данные в локальный буфер.
// Далее работаем с данными внутри своего класса.
// Создаем 2 буфера N1 N2
// Если буфер накопления N1 достиг заданного размера [0x100000] пишем его в файл на диск.
// При этом продолжаем набор уже в другой буфер N2, потом снова меняем буфер для записи на N1
// Сигнал на запись файла будем давать отсюда
void MyThread::F_SLOT()
{
    while ((!this->finthread) ) //& !this->once
    {
        ;
    }//while
      emit finished();

}
//+++++++++++++++++++++


/*
Каркас для кода внутри потока
void MyThread::F_SLOT()
{
    while ((!this->finthread) ) //& !this->once
    {

    }//while
      emit finished();

}



*/


/*
void MyThread::Thread_Write_To_File_SLOT(QByteArray *thr_buf)
{

    //  QDataStream inFile(&mfile);
   //   inFile.writeRawData(byInputBuffer, numBytes);

  while (!this->finthread)
    {
      if (wrfile)
      {
     //   QDataStream inFile(&thr_file);
      //  inFile.writeRawData(thr_buf, thr_buf->size());
          ;
      }

  }
  emit finished();
}
//+++++++++++++
void MyThread::Thread_Open_Bin_File(QString thr_name)
{

    thr_file.setFileName(thr_name);

    if (!thr_file.open(QIODevice::ReadWrite | QIODevice::Truncate )) { ; } //error
    else {;} // NO eror

}
*/
