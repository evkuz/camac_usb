#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <stdlib.h>


#define OneSector 64*1024
#define SectorNum 2000

FT_HANDLE ftHandle; // Handle of the FTDI device
FT_STATUS ftStatus; // Result of each D2XX call
FT_HANDLE ftHandle_A;
FT_HANDLE ftHandle_B;

DWORD dwNumDevs; // The number of devices
DWORD dwNumBytesToRead = 0; // Number of bytes available to read in the driver's input buffer
DWORD dwNumBytesRead;
DWORD dwNumBytesSent;
DWORD dwNumBytesToSend;
//unsigned char byOutputBuffer[1024]; // Buffer to hold MPSSE commands and data to be sent to the FT2232H

DWORD EventDWord;
DWORD TxBytes;
DWORD RxBytes;
DWORD RxBytes_2;
DWORD BytesReceived;


int ft2232H ; // High speed device (FTx232H) found. Default - full speed, i.e. FT2232D
DWORD dwClockDivisor = 0;
DWORD dwCount;
char SerialNumber[16+1];
char Description[64+1];

//char byOutputBuffer[64];
//char byInputBuffer[64]; // Buffer to hold data readed from the FT2232H

//char RxBuffer[OneSector];

DWORD Flags;
DWORD ID;
DWORD Type;
DWORD LocId;
UCHAR Mask = 0xff;
UCHAR Mode;
UCHAR LatencyTimer = 2; //16ms default. The receive buffer timeout that is used to flush remaining data from the receive buffer

QString str;

int DevNumber; //Номер НАШЕГО девайса

int DevNumber_A; //Порт А, Номер НАШЕГО девайса
int DevNumber_B; //Порт B, Номер НАШЕГО девайса


//int DevNumber; //Номер НАШЕГО девайса

//+++++++++++++++++++++ EK +++++++++++++++++++++++++++++++++++
FT_DEVICE_LIST_INFO_NODE *devInfo;

const char Morph_ID[] = "FT112244EKA"; //Этот девайс будем выбирать среди всех доступных
const char Mini_ID_Sem[] = "FT112244SemA";

const char Mini_ID_A[] = "Camac_021216A"; //FT112244EKA
const char Mini_ID_B[] = "Camac_021216B"; //FT112244EKB

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
    - Создаем список(количество) FTDI-устройств - dwNumDevs
      Если таковых нет - сообщаем "Связи нет Create"
    - Перебираем устройства по порядку, получаем FTHANDLE, SerialNumber для портов А и В
    - Опрашиваем конкретно порт А т.к. уже известен его номер.
      Если порта нет в списке девайсов - сообщаем "нет связи A"
      Если есть, то проверяем подключен и открыт ли он ?
      Если порт подключен и открыт - сообщаем "подключено", заканчиваем
      если нет - открываем, проверяем результат.
      Если успешно, сообщаем "подключено", выставляем флаги

      В итоге, по завершеии этой ф-ции порт А всегда открыт, либо сообщение, что не могу открыть порт.
*/
void MainWindow::ftdi_init(void)
{
    //FT_DEVICE ftDevice;
   // DWORD deviceID;
   // QString str;

    ftStatus = FT_CreateDeviceInfoList(&dwNumDevs);

    // Get the number of FTDI devices
    if (ftStatus != FT_OK) // Did the command execute OK?
    {
        FTDI_Connected = FALSE; update();ui->Status_label->setText("Связи нет Create");
       // return ; // Exit and should check connection periodically
                   //return не работает !!! Нельзя его использовать
    }

    if (dwNumDevs < 1) // Exit if we don't see any
    {
        FTDI_Connected = FALSE; update();ui->Status_label->setText("Связи нет dwNumDevs");
      //  return ; // Exist with error
                   //return не работает !!! Нельзя его использовать
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ FT_GetDeviceInfoList +++++++++++++++++++++++++++++++++
    FT_HANDLE ftHandleTemp;
    DWORD Flags;
    DWORD ID;
    DWORD Type;
    DWORD LocId;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ FT_GetDeviceInfoDetail +++++++++++++++++++++++++++++++++
    // Эта функция дает читаемые строки у SerialNumber и Description
    for (unsigned int i = 0; i < dwNumDevs; i++)
    {
     // Перебираем в цикле и по каждому устройству вызываем детали
     // Нам нужен только указатель ftHandleTemp и строка с номером SerialNumber,  остальные данные тут не нужны.
        ftStatus = FT_GetDeviceInfoDetail(i, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandleTemp);

        if (ftStatus == FT_OK)
        {
            ft2232H = qstrcmp(SerialNumber, Mini_ID_A); //
            if (ft2232H == 0) {
                DevNumber_A = i; FTDI_Connected = TRUE; update(); ui->Status_label->setText("Подключено");

            }

            ft2232H = qstrcmp(SerialNumber, Mini_ID_B); //
            if (ft2232H == 0) {
                DevNumber_B = i; FTDI_Connected = TRUE; update(); //ui->Status_label->setText("Подключено");

            }
            //++++++++++++++++++++++++++++++++++++++++++++++

        }//if

        else { str.sprintf("FT_GetDeviceInfoDetail Failed with error %d\n", ftStatus);  ui->FTDI_List_textBrowser->append(str);  }
    } //for

    ftStatus = FT_GetDeviceInfoDetail(DevNumber_B, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandle_B);
    ftStatus = FT_Open(DevNumber_B, &ftHandle_B); ftStatus |= FT_SetUSBParameters(ftHandle_B,64,64);    ftStatus |= FT_SetLatencyTimer(ftHandle_B, LatencyTimer);
    if(ftStatus != FT_OK)
    {
        str.sprintf("Open Chan B Failed with error %d\n", ftStatus);
        ui->Thread_Status_label->setText(str);
        FT_Close(ftHandle_B);
    }
    else { QString str; str.sprintf(" FTHANDLE B %d : 0x%x\n", DevNumber_B, ftHandle_B);ui->Message_label->setText(str);    }


    ftStatus = FT_GetDeviceInfoDetail(DevNumber_A, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandle_A);
    ftStatus = FT_Open(DevNumber_A, &ftHandle_A);
    //ftStatus |= FT_SetUSBParameters(ftHandle_A,64,64); //0x00040,0x00040
    ftStatus |= FT_SetLatencyTimer(ftHandle_A, LatencyTimer);
    ftStatus |= FT_SetUSBParameters(ftHandle_A,0x00040,0x00040);

    if(ftStatus != FT_OK)
    {
        str.sprintf("Open Chan B Failed with error %d\n", ftStatus);
        ui->Thread_Status_label->setText(str);
        FT_Close(ftHandle_B);
    }
    else { QString str; str.sprintf(" FTHANDLE A %d : 0x%x\n", DevNumber_B, ftHandle_B);ui->Message_label->setText(str);    }


    //str.sprintf(" Camac Device has number %d\n",DevNumber);ui->FTDI_List_textBrowser->append(str);

    //Тут опрашиваем конкретно порты А и В, т.к. уже известны их номера.
/*    ftStatus = FT_GetDeviceInfoDetail(DevNumber_A, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandle_A);
    if (ftStatus == FT_OK)
    {
      if (FTDI_Connected & A_Open ) ui->Status_label->setText("подключено"); //USB подключено, порты открыты
      else {
            ftStatus = FT_Open(DevNumber_A, &ftHandle_A);
            if (ftStatus == FT_OK)
             {
                FTDI_Connected = TRUE; update(); A_Open = TRUE; ui->Status_label->setText("подключено");
                ftStatus |= FT_SetUSBParameters(ftHandle_A,0x00040,0x00040);
             }

             else {FTDI_Connected = FALSE; update(); A_Open = FALSE; ui->Status_label->setText("не могу открыть порт"); }
      }
    }
    else {FTDI_Connected = FALSE; update(); A_Open = FALSE; FT_Close(&ftHandle_A); ui->Status_label->setText("нет связи A"); }//(ftStatus == FT_OK)
*/
//++++++++++++++++++++++++
/*
    ftStatus = FT_GetDeviceInfoDetail(DevNumber_B, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandle_B);
    if (ftStatus == FT_OK)
    {
      if (FTDI_Connected & B_Open ) ui->Status_label->setText("подключено"); //USB подключено, порты открыты
      else {
            ftStatus = FT_Open(DevNumber_B, &ftHandle_B);
            if (ftStatus == FT_OK)
             {
                FTDI_Connected = TRUE; update(); B_Open = TRUE; ui->Status_label->setText("подключено");
             }
             //else {FTDI_Connected = FALSE; update(); B_Open = FALSE; ui->Status_label->setText("нет связи"); }
      }
    }

    else {FTDI_Connected = FALSE; update(); B_Open = FALSE; FT_Close(&ftHandle_B);ui->Status_label->setText("нет связи B"); }//(ftStatus == FT_OK)

*/
//    filename = "../Camac_Thread_data.bin";
//    mfile.setFileName(filename);


/*
    if (!mfile.open(QIODevice::ReadWrite | QIODevice::Truncate ))
    {

       // ui->File_Open_label->setText("File does not exist");
        str.sprintf("File does not exist");ui->FTDI_List_textBrowser->append(str);

        return;
    }

    else
    {
        // ui->File_Open_label->setText("File is opened");
         str.sprintf("File is opened");ui->FTDI_List_textBrowser->append(str);
    }

*/

}

//++++++++++++++++++++++++++++++++++++++++++++++++

//Запускаем поток на отправку из ПК в Железо. Канал В
/*
void MainWindow::on_SEND_pushButton_clicked()
{
  ftStatus = FT_GetDeviceInfoDetail(DevNumber_B, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandle_B);
  ftStatus = FT_Open(DevNumber_B, &ftHandle_B);
 // str.sprintf(" FTHANDLE B %d : 0x%x\n", DevNumber_B, ftHandle_B);ui->FTDI_List_textBrowser->append(str);

  chan_B->finthread = FALSE;
  thread_B->start(QThread::HighPriority); // TimeCriticalPriority HighestPriority Запускаем поток, и у нас генерируется сигнал started()====> Срабатывает слот B_SLOT(),
                                            // в котором и происходит уже отправка данных USB. !
}
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++


//Слот обработки сигнала Process_B - запись данных в канал B
//void MainWindow::Write_B()
//void MainWindow::on_SEND_pushButton_clicked()
void MainWindow::on_SEND_TO_USB_pushButton_clicked()
{
    timer->stop();
    CAMAC_make_command(); //Заполняем буфер на отправку данных
/*
    ftStatus = FT_GetDeviceInfoDetail(DevNumber_B, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandle_B);
    ftStatus = FT_Open(DevNumber_B, &ftHandle_B); ftStatus |= FT_SetUSBParameters(ftHandle_B,0x00040,0x00040);
    if(ftStatus != FT_OK)
    {
        str.sprintf("Open Chan B Failed with error %d\n", ftStatus);
        ui->Thread_Status_label->setText(str);
        FT_Close(ftHandle_B);
    }
    else { QString str; str.sprintf(" FTHANDLE B %d : 0x%x\n", DevNumber_B, ftHandle_B);ui->Message_label->setText(str);    }
*/
    //Т.к. частота меняется, то в поток необходимо передавать параметр - частоту.
    int index, duration;

   index = ui->Frequency_comboBox->currentIndex();
   char  strval[10];
   itoa(index, strval, 10);
   ui->Index_Value_label->setText(strval);
    switch (index)
    {
      case 0 :
        duration = 0;
   //     chan_B->once = TRUE;
      case 1 :
        duration = 200; //5kHZ
  //      chan_B->once = FALSE;
        break;
      case 2 :
        duration = 100; //10kHZ
  //      chan_B->once = FALSE;
         break;
     case 3 :
//        chan_B->once = FALSE;
        duration = 50;  //20kHZ
        break;
    case 4 :
       duration = 500;  //2kHZ
       break;

    case 5 :
       duration = 3000; //333HZ
       break;

    }

    chan_B->Duration_mks = duration; //ui->Frequency_comboBox->currentIndex();

    // If "No cycle"  so make  1 cycle
    if  (index == 0) {

/*        if (!A_Open)
        {
            ftStatus = FT_GetDeviceInfoDetail(DevNumber_A, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandle_A);
            ftStatus = FT_Open(DevNumber_A, &ftHandle_A);  ftStatus |= FT_SetUSBParameters(ftHandle_A,0x00040,0x00040);
            A_Open = TRUE;
        }
  */
     Purge_USB_Buffers();

        Write_B(); // First send command
        FT_GetStatus(ftHandle_A,&RxBytes,&TxBytes,&EventDWord);
        FT_GetQueueStatus(ftHandle_A,&RxBytes_2);

        UINT32 rdata;
        char udata[50];
    //    QByteArray mystr;
        if (RxBytes >= 0)
        {
            rdata = RxBytes;
            itoa(rdata, udata, 10);
            str.sprintf("В буфере чтения байт : %d\n",rdata); ui->FTDI_List_textBrowser->append(str);
           // str.append("В буфере чтения байт : ");
          //  str.append(udata);
            //ui->FTDI_List_textBrowser->setText(udata);
        }

        if (RxBytes_2 >= 0)
        {
            rdata = RxBytes_2;
            itoa(rdata, udata, 10);
            str.sprintf("В буфере чтения-Queue байт : %d\n",rdata); ui->FTDI_List_textBrowser->append(str);
        }


        Read_A(RxBytes); // Then Read data from FTDI. X = 1; Q = 1;
 //       Debug_Output();
        // 350мкс уходит на открытие порта
        update();
    }
    else { // Если циклически, то запускаем поток.

        emit Get_ftHandle_B();   //Get_ftHandle_B();
        chan_B->finthread = FALSE;
        thread_B->start(QThread::HighPriority); //Пишем из ПК в Железо по каналу B

      // TimeCriticalPriority HighestPriority HighPriority Запускаем поток, и у нас генерируется
                                                      //сигнал started()====> Срабатывает слот B_SLOT(),

     // chan_A->finthread = FALSE;
    //  thread_A->start(QThread::HighPriority); //Считываем в ПК из Железа по каналу А

    }


//    chan_B->finthread = FALSE;
//    thread_B->start(QThread::HighPriority); // TimeCriticalPriority HighestPriority Запускаем поток, и у нас генерируется сигнал started()====> Срабатывает слот B_SLOT(),

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::Write_B()
{

    // Теперь NAF_Bits и ToDev_Bits объединяем в 1-о 32 битное слово

    ftStatus = FT_Write(ftHandle_B, byOutputBuffer,TRANSFER_SIZE, &dwNumBytesSent); //byOutputBuffer
    if (ftStatus == FT_OK) {  FTDI_Connected = TRUE; }
    else
    {
 //    ui->Status_label->setText("error Writing !!!");
       QString str;
  //       str.sprintf("Open Failed with error %d\n", ftStatus);ui->FTDI_List_textBrowser->append(str);

  //       str.sprintf(" FTHANDLE B %d : 0x%x\n", DevNumber_B, ftHandle_B);ui->FTDI_List_textBrowser->append(str);

     //    ui->Thread_Status_label->setText(str);

//str.sprintf(" Camac Device has number %d\n",DevNumber);ui->FTDI_List_textBrowser->append(str);


     FTDI_Connected = FALSE;
     B_Open = FALSE;
 //    FT_Close(ftHandle_B);
     // И поток тоже останавливаем.
      chan_B->finthread = TRUE;

    }

 //   update();
 //   ftStatus = FT_Purge(ftHandle_A, FT_PURGE_RX);
 //   ftStatus = FT_Purge(ftHandle_A, FT_PURGE_TX);

  //  ftStatus = FT_Purge(ftHandle_B, FT_PURGE_RX);
  //  Read_AAA();
  //     ftStatus = FT_Purge(ftHandle_A, FT_PURGE_RX);
  //     ftStatus = FT_Purge(ftHandle_A, FT_PURGE_TX);

//    ftStatus = FT_Purge(ftHandle_B, FT_PURGE_TX);

    FT_GetStatus(ftHandle_A,&RxBytes,&TxBytes,&EventDWord);
    if (RxBytes > 0)
    {
    // if (RxBytes > sizeof(byInputBuffer)) {RxBytes = sizeof(byInputBuffer);}  //byInputBuffer [EK_BUFFER_SIZE];
     Read_A(RxBytes);
  //   str.sprintf("В буфере чтения есть данные\n"); ui->FTDI_List_textBrowser->append(str);
    }

ftStatus = FT_Purge(ftHandle_B, FT_PURGE_TX);
ftStatus = FT_Purge(ftHandle_A, FT_PURGE_RX);
/**/

}
//===================================================
// Теперь это СЛОТ
void MainWindow::Read_AAA()
{

    FT_GetStatus(ftHandle_A,&RxBytes,&TxBytes,&EventDWord);
    if (RxBytes > 0)
    {
     if (RxBytes > sizeof(byInputBuffer)) {RxBytes = sizeof(byInputBuffer);}  //byInputBuffer [EK_BUFFER_SIZE];
     Read_A(RxBytes);
     str.sprintf("В буфере чтения есть данные\n"); ui->FTDI_List_textBrowser->append(str);
    }

}
//===================================================
//Теперь считывам данные и пишем в файл
// 3-й аргумент - это сколько ожидаем, 4-й аргумент - сколько на самом деле пришло
void MainWindow::Read_A(int numBytes)
{
ftStatus = FT_Read(ftHandle_A, byInputBuffer,numBytes,&dwNumBytesRead); //RECEIVE_SIZE 64 EK_BUFFER_SIZE
if (ftStatus == FT_OK) {  FTDI_Connected = TRUE; }
else
{
   // timer->stop();
 // При работе с потоками каждый вывод строки занимает время, поэтому в production Это не ставим
    //QString str;
 //   str.sprintf("Read Failed with error %d\n", ftStatus);ui->FTDI_List_textBrowser->append(str);
    FTDI_Connected = FALSE;//Флаг выставили
    A_Open = FALSE;
    chan_A->finthread = TRUE;
    //if (!chan_A->finthread)
    //При работе с потоками повторное закрытие вызовет ошибку, поэтому в production Это не ставим
//        FT_Close(ftHandle_A);
    // И поток тоже останавливаем.

   //  chan_A->A_finished();
//     Purge_IO_Buffers();
 //    update();
     return;
}
//Пока уберем перерисовку
//update(); //Перерисовываем

// И вот теперь пишем в файл.

if (mfile.isOpen())
{
  //  mfile.write(byInputBuffer, numBytes); //EK_BUFFER_SIZE RECEIVE_SIZE databuf.constData(), qstrlen(databuf) dwNumBytesRead byInputBuffer mystr
    QDataStream out(&mfile);   // we will serialize the data into the file
//    out << *byInputBuffer;   // serialize data
      out.writeRawData(byInputBuffer, numBytes);
      cycle_counnter++;
      if (cycle_counnter == 8192) //1024
      {
        mfile.flush();
        cycle_counnter =0;
      }

  //  mfile.flush();
}
else {ui->Thread_Status_label->setText("Нет данных для записи\n");}

/**/
ftStatus = FT_Purge(ftHandle_A, FT_PURGE_RX);
//tStatus = FT_Purge(ftHandle_A, FT_PURGE_TX);
}
//++++++++++++ Проверяем состояние подключения FTDI-устройств
void MainWindow::onTimer()
{
    ftdi_init();


}
//===================================================
void MainWindow::RW_Cycle()
{

  Write_B();
  if (!A_Open)
  {
      ftStatus = FT_GetDeviceInfoDetail(DevNumber_A, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandle_A);
      ftStatus = FT_Open(DevNumber_A, &ftHandle_A);
      A_Open = TRUE;
      if(ftStatus != FT_OK)
      {
          str.sprintf("Open A Failed with error %d\n", ftStatus);
          ui->FTDI_List_textBrowser->append(str);
          FT_Close(ftHandle_A);
      }
      ftStatus |= FT_SetLatencyTimer(ftHandle, LatencyTimer);
      ftStatus |= FT_SetUSBParameters(ftHandle_A,0x00040,0x00040); //Transfer sizes must be set to a multiple
                                                                   //of 64 bytes between 64 bytes and 64k bytes.

  }

  Read_A(RxBytes);

}//===================================================


//++++++++++++++++++++++++++



//+++++++++++++++++++


//==================================================
/*
FT_HANDLE MainWindow::ftdi_set_mode(int DevNumber_ff)

{
    ftStatus = FT_GetDeviceInfoDetail(DevNumber_ff, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandle);
    ftStatus = FT_Open(DevNumber_ff, &ftHandle);
    if(ftStatus != FT_OK)
    {
        str.sprintf("Open Failed with error %d\n", ftStatus);
        FT_Close(ftHandle);
    }
    Mode = 0x00; //reset mode
    ftStatus |= FT_SetBitMode(ftHandle, Mask, Mode); // Mask
    //mleep(10);
    //Sleep(1000); //Желательно сделать паузу
    Mode = 0x40; //Sync FIFO mode
    ftStatus |= FT_SetBitMode(ftHandle, Mask, Mode);

    if (ftStatus != FT_OK)
    {
       str.sprintf("Error in initializing1 %d\n", ftStatus);ui->FTDI_List_textBrowser->append(str);
        FT_Close(ftHandle);
        //return 1; // Exit with error
    }

    ftStatus |= FT_SetLatencyTimer(ftHandle, LatencyTimer);
    ftStatus |= FT_SetUSBParameters(ftHandle,0x10000,0x10000);
    ftStatus |= FT_SetFlowControl(ftHandle,FT_FLOW_RTS_CTS,0,0);//0x10,0x13
    if (ftStatus == FT_OK)
    {
    str.sprintf(" Включен режим Synchronous FIFO");ui->FTDI_List_textBrowser->append(str);
    //access data from here
    str.sprintf(" FTHANDLE %d : 0x%x\n", DevNumber_ff, ftHandle);ui->FTDI_List_textBrowser->append(str);

    }
    else
    {
    // FT_SetBitMode FAILED!
     str.sprintf(" Не могу включить режим Synchronous FIFO");ui->FTDI_List_textBrowser->append(str);
     FT_Close(ftHandle);
    // return 1;
    }

  //  ftStatus = FT_Purge(ftHandle, FT_PURGE_RX);
    ftStatus = FT_Purge(ftHandle, FT_PURGE_TX | FT_PURGE_RX);

    if (ftStatus == FT_OK)
    {
    str.sprintf(" Буферы очищены");ui->FTDI_List_textBrowser->append(str);

    }
    else
    {
     str.sprintf(" Не могу очистить буферы");ui->FTDI_List_textBrowser->append(str);
     FT_Close(ftHandle);

    }

// Set read timeout of 1sec, write timeout of 0.5sec
    ftStatus = FT_SetTimeouts(ftHandle, 1000, 500);
    if (ftStatus == FT_OK)  { ;// FT_SetTimeouts OK
    }
    else
        { // FT_SetTimeouts failed
           str.sprintf(" Не могу задать таймауты");ui->FTDI_List_textBrowser->append(str);
           FT_Close(ftHandle);
        }


     ftStatus |= FT_GetQueueStatus(ftHandle, &dwNumBytesToRead);
     if ((ftStatus == FT_OK) && (dwNumBytesToRead > 0))
     {

         str.sprintf(" Есть данные в очереди");ui->FTDI_List_textBrowser->append(str);


     }
     else str.sprintf(" Нет данных в очереди");ui->FTDI_List_textBrowser->append(str);

     return ftHandle;
}
*/
//===================================================

