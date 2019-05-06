#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>


#include <QFile>
//#include <QString>

class MyThread : public QObject
{
    Q_OBJECT
public:
  //  explicit MyThread(QObject *parent = 0);
     MyThread();
    ~MyThread();
    bool finthread;   //Флаг работы самого потока. Поток закрываем по нажатию кнопок в окне пользователя
    bool once;        //Флаг 1-кратной посылки/считывания данных в USB
    int Duration_mks; // длительность паузы в мкс = интервал между циклами работы потока.
    bool pause_thread; // True - значит ставим на паузу

    bool wrfile; //True - значит пишем файл на диск

 //   void Thread_Write_To_File_SLOT(QByteArray *thr_buf); // Открыть файл для накопления данных
 //   void Thread_Open_Bin_File(QString thr_name);

    QFile thr_file;

public slots:
        void A_SLOT();  /* Канал А, Прием данных в ПК из железа  */
        void B_SLOT();  /* Канал В, отправка данных из ПК в железо */
        void F_SLOT();  // Поток работы с файлом
        void Thread_Pause_Slot();
        void Thread_Pause_OFF_Slot(); // Снимаем с паузы поток.

  //      void Thread_Write_To_File (QByteArray *);


signals:
        void Process_A();
        void Process_B();
        void Process_F();
        void finished();   /* Сигнал о завершении работы Потока... того, в котором он сигнал вызывается ? */
        void error(QString err);


};




#endif // MYTHREAD_H
