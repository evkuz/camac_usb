#include "ftdi_d2xx.h"
#include <iostream>
#include <cstring>


//+++++++++++++++++++++ EK +++++++++++++++++++++++++++++++++++
FT_DEVICE_LIST_INFO_NODE *devInfo;
/*
const char Morph_ID[] = "FT112244EKA"; //Этот девайс будем выбирать среди всех доступных
const char Mini_ID_Sem[] = "FT112244SemA";

const char Mini_ID_A[] = "Camac_021216A"; //FT112244EKA
const char Mini_ID_B[] = "Camac_021216B"; //FT112244EKB
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//FTDI_D2XX::FTDI_D2XX(QObject *parent) : QObject(parent)
FTDI_D2XX::FTDI_D2XX()
{


//memset(byInputBuffer, 0, USB_Buffer_Size); //sizeof(byInputBuffer)
  // Инициализируем буфер данными
  memset(byInputBuffer, 0xEE, accum_buffer_SIZE); //sizeof(byInputBuffer)
  file_pos = 0;
  CURRENT_BUFFER_INDEX_1 = TRUE;
  BUF_PTR = &BUF_01; //new QByteArray;//


  for (int i=0; i<=3200; i+=8)
  {
      t_points[i]   = 0xf0;  //1
      t_points[i+1] = 0x11;  //2
      t_points[i+2] = 0x00;  //3
      t_points[i+3] = 0x07;  //4
      t_points[i+4] = 0xD0;  //5
      t_points[i+5] = 0x00;  //6
      t_points[i+6] = 0x00;  //7
      t_points[i+7] = 0x80;  //8

  }

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
FTDI_D2XX::~FTDI_D2XX()
{
    mfile.close();
    LogFile.close();

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int FTDI_D2XX::ftdi_init(char *looking_for_device_A, char *looking_for_device_B)
{
    QString str;
    int ft_error_code = FTDI_OK; // If it's OK

    str.sprintf("0x%04X: NOTICE: Запущена функция FTDI_D2XX::ftdi_init(void)\n", 0xF666); emit Show_data(str);

    // Does an FTDI device exist?
    ftStatus = FT_CreateDeviceInfoList(&dwNumDevs);
    // Get the number of FTDI devices ONLY number !
    if (ftStatus != FT_OK) // Did the command execute OK?
    {
      //   printf("Error in getting the number of devices\n");
         ft_error_code = 0x0001; return ft_error_code; // Exit with error
    }

    if (dwNumDevs < 1) // Exit if we don't see any
    {
        printf("There are no FTDI devices installed\n");
        ft_error_code = 0x0002; return ft_error_code; // Exist with error
    }


  //  printf("%lu FTDI devices found - the count includes individual ports on a single chip\n", dwNumDevs);// %d
    ftHandle=NULL;

    FT_HANDLE ftHandleTemp;
    DWORD Flags;
    DWORD ID;
    DWORD Type;
    DWORD LocId;

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ FT_GetDeviceInfoDetail +++++++++++++++++++++++++++++++++
    // Эта функция дает читаемые строки у SerialNumber и Description

    for (unsigned int i = 0; i < dwNumDevs; i++)
    {
     // Перебираем в цикле девайсы и по каждому из них вызываем детали
        ftStatus = FT_GetDeviceInfoDetail(i, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandleTemp);

        if (ftStatus == FT_OK)
        {
 /*         printf("Dev %d:\n",i);//printf(str);  str.s
            printf(" Flags=0x%p\n",(void *)Flags);//printf(str);
            printf(" Type=0x%lu\n",Type);//printf(str);
            printf(" ID=0x0%lu\n",ID);//printf(str);
            printf(" LocId=0x%lu\n", LocId);//printf(str);
            printf(" SerialNumber=%s\n",SerialNumber);//printf(str);
            printf(" Description=%s\n",Description);//printf(str);
            printf(" ftHandle=0x%p\n",(void *)ftHandleTemp);//printf(str);
   */
            //+++++++++++++++++++ Сразу открываем порты А и В +++++++++++++++
            ft2232H = qstrcmp(SerialNumber, looking_for_device_A ); //Morph_ID Mini_ID_A
            if (ft2232H == 0) {
                DevNumber_A = i;
                ftHandle_A = &ftHandleTemp;
               // ftStatus = FT_GetDeviceInfoDetail(DevNumber_A, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandle_A);
                ftStatus = FT_Open(DevNumber_A, &ftHandle_A);
                if (ftStatus !=0) {ft_error_code = 0x0003; return ft_error_code;} //Порт А не смогли открыть, хотя устройство подключено
                ft_error_code = FTDI_OK; // Все хорошо                             //и SerialNumber совпадает

                ftStatus = FT_SetUSBParameters(ftHandle_A,USB_Buffer_Size, 0); // USB_Buffer_Size
                if (ftStatus != FT_OK) KK_Write_To_Log(0x1000, "Cann't SET USBParameters ! \n");

                ftStatus = FT_ResetDevice(ftHandle_A);
                if (ftStatus != FT_OK) KK_Write_To_Log(0x1000, "Cann't Reset device A ! \n");

            }
            else  // Девайс A не подключен, зажигаем красную лампочку !
                  // А если подключено стороннее устройство на базе FTDI, например, USB-Blaster ???
            {
              ft_error_code = 0x0004; // Это не канал А, да, НО это может быть канал В
             //emit Device_NOT_connected_Signal();
            }

            // Проверяем канал В
            ft2232H = qstrcmp(SerialNumber, looking_for_device_B); //Morph_ID Mini_ID_B
            if (ft2232H == 0) {
                DevNumber_B = i;
                ftHandle_B = &ftHandleTemp;
                 ftStatus = FT_Open(DevNumber_B, &ftHandle_B);
                if (ftStatus != 0) {ft_error_code = 0x0005; return ft_error_code;} //Порт В не отктрыт
                ft_error_code = FTDI_OK; // Все хорошо, ошибок нет, даже если это не канал А :)
                                        // Т.е. тут переписываем ранее возникшие ошибки

                ftStatus = FT_ResetDevice(ftHandle_B);
                if (ftStatus != FT_OK) KK_Write_To_Log(0x1000, "Cann't Reset device B ! \n");


            }
            else  // Подключено устройство FTDI, но это не Девайс "B", зажигаем красную лампочку !
                  // А если подключено стороннее устройство на базе FTDI, например, USB-Blaster ???
            {
             if (ft_error_code == 0x0004) ft_error_code = 0x0006;// Это и не А и не Б - вообще стороннее устройство

            }

            //++++++++++++++++++++++++++++++++++++++++++++++
        }//if
        else
        {
          ft_error_code = 0x0007;   //FT_GetDeviceInfoDetail error
        }

    } //for

Purge_USB_Buffers();
return ft_error_code;

}
//++++++++++++++++++++++++++++++++++++++++++++++
void FTDI_D2XX::PutBack_ftHandle_B() //это слот
{

       ftStatus = FT_GetDeviceInfoDetail(DevNumber_B, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandle_B);
       printf(" 1-st FTHANDLE B %d : 0x%p\n", DevNumber_B, (void *)ftHandle_B);

}
//++++++++++++++++++++++++++++++++++++++++++++++
void FTDI_D2XX::PutBack_ftHandle_A() //это слот
{

       ftStatus = FT_GetDeviceInfoDetail(DevNumber_A, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandle_A);
       printf(" 1-st FTHANDLE A %d : 0x%p\n", DevNumber_A, (void *)ftHandle_A);

}
//++++++++++++++++++++++++++++++++++++++++++++++
 void FTDI_D2XX::Close_ftdi_chan_B()
 {
     FT_Close(ftHandle_B);
 }

 //++++++++++++++++++++++++++++++++++++++++++++++
 void FTDI_D2XX::Close_ftdi_chan_A()
 {
     FT_Close(ftHandle_A);
 }

//++++++++++++++++++++++++++++++++++++++++++++++


 int FTDI_D2XX::Write_B_Single(char *OutBuffer, int numbytes)
 {
     int ft_error_code = 0x0000;
     DWORD RxxBytes;
  //   char byBuffer[4] = {0x55,0x31,0xbc,0xaa};
  //   LPVOID lpData;
 //    lpData = (LPVOID)byBuffer;

     //   lpData = (LPVOID)OutBuffer;
     ftStatus = FT_Purge(ftHandle_B, FT_PURGE_TX);
     ftStatus = FT_Purge(ftHandle_B, FT_PURGE_RX);

     ftStatus = FT_Purge(ftHandle_A,FT_PURGE_RX);

     ftStatus = FT_GetQueueStatus(ftHandle_A,&RxxBytes);


     //Считав данные из буфера, мы его тем самым очистили.
     ftStatus = FT_Read(ftHandle_A, byInputBuffer,RxxBytes,&dwNumBytesRead);

 if (ftStatus != FT_OK)
 {
     str.sprintf("Read_A->Не удалось очистить Rx буфер FTDI\n"); KK_Write_To_Log(0xeeee, str);//  emit Show_data(str);
     return 0xeee;
 }
 if (RxxBytes != dwNumBytesRead) {str.sprintf("В буфере было %lu RxxBytes байт, а считано %lu байт\n",RxxBytes, dwNumBytesRead);}

// А теперь еще и занулим byInputBuffer
// memset(byInputBuffer, 0, sizeof(byInputBuffer));


     ftStatus = FT_Write(ftHandle_B, OutBuffer, numbytes, &dwNumBytesSent); //byOutputBuffer  TRANSFER_SIZE

   if (ftStatus != FT_OK)  { ft_error_code = 0x000C; ; } // ui->Status_label->setText("error writing !!!");

return ft_error_code;
 }

 //++++++++++++++++++++++++++++++++++++++++++++++
void FTDI_D2XX::Write_B_Thread()
{
    ftStatus = Write_B(OutputBuffer, TRANSFER_SIZE);
   //int answer =  Write_B(OutputBuffer, TRANSFER_SIZE); //А вот тут уже можно записывать в лог !!!
  // if (answer != 0) {str.sprintf("Write_B_Thread->error writing !!!\n"); emit Show_data(str);}
}
//+++++++++++++
int FTDI_D2XX::Write_B(char *OutBuffer, int numbytes )
{
    int ft_error_code = 0x0000;
  //  char mybuf[4] = {'\x11', '\x20', '\x22', '\x22'};
    ftStatus = FT_Write(ftHandle_B, OutBuffer, numbytes, &dwNumBytesSent); //byOutputBuffer OutBuffer numbytes mybuf

   if (ftStatus != FT_OK)  { emit Device_NOT_connected_Signal(); ft_error_code = 0x0001; ; } // ui->Status_label->setText("error writing !!!");

return ft_error_code;


}
//===================================================
// Слот обработки сигнала Process_A() от потока chan_A
void FTDI_D2XX::Read_AAA(void)
{

   ftStatus = FT_GetQueueStatus(ftHandle_A,&RxBytes); //RxBytes_2
   if (RxBytes > 0)
    {
   //    str.sprintf("Read_AAA()->FT_GetQueueStatus()->Got %d bytes instead of 8 bytes\n", RxBytes );
  //     KK_Write_To_Log(0xF555, str);
       Read_A(RxBytes);

     }
   else {

      // str.sprintf("0x%04X: Read_AAA()->FT_GetStatus()->Данных От FTDI нет\n", 0xF555); //emit Show_data(str);
     //  KK_Write_To_Log(0xF555, "Read_AAA()->FT_GetQueueStatus()->Буфер FTDI пуст. Данных нет\n" );
       KK_Write_To_Log(0xF555, "Read_AAA()->FT_GetQueueStatus()->В буфере нет данных для считывания, RxBytes == 0, пропускаем\n" );
   }


}
//===================================================
void FTDI_D2XX::Read_A(uint numBytes)
{
    ftStatus = FT_Read(ftHandle_A, byInputBuffer,numBytes,&dwNumBytesRead);

if (ftStatus != FT_OK)
{
    str.sprintf("Read_A->Не удалось считать данные из FTDI\n"); KK_Write_To_Log(0xeeee, str);//  emit Show_data(str);
    return ;
}

if (numBytes != dwNumBytesRead) {str.sprintf("Было доступно %du байт, а считано %lu байт\n",numBytes, dwNumBytesRead);}


//Считали, [ сразу очищаем ] Значит буфер очищен

// Буфер byInputBuffer получили, теперь его копируем-добавляем в отрисовку
//memcpy(SpectralBuffer, byInputBuffer, RECEIVE_SIZE);

//qbuf.append(byInputBuffer, RECEIVE_SIZE); // Копируем полученные данные в буфер накопления спектров

//##########################################################################################################

mutex.lock();
BUF_PTR->append(byInputBuffer, dwNumBytesRead); // А это уже добавили в буфер данные.
mutex.unlock();

//##########################################################################################################

//И вот теперь пишем в файл.

//QDataStream
// EK_BUF(BUF_PTR, QIODevice::OpenModeFlag::Append); //QIODevice::OpenModeFlag::ReadWrite

// EK_BUF.writeRawData(byInputBuffer, numBytes);

//   BUF_01.append(byInputBuffer, RECEIVE_SIZE);
//   str.sprintf("%d Bytes written !\n", BUF_01.size());
//   KK_Write_To_Log(0xeeee, str);

   // if  (BUF_01.size() >= 0x10000) {KK_Write_To_Log(0xeeee, "65535 Bytes written !"); BUF_01.clear();}
//if (BUF_PTR->size() >= 0x100000) {KK_Write_To_Log(0xeeee, "65536 Bytes written !\n"); BUF_PTR->clear();}

//KK_Write_To_Log(0xF666, "Read_A()->BUF_PTR->append()-> PASSED \n" );

if (mfile.isOpen())
{
  //  QDataStream inFile(&mfile);
  //  inFile.writeRawData(byInputBuffer, numBytes);

    // Это пока лишь запись в буфер. На диск запись будет после mfile.close();
    // Вот здесь надо писать в область памяти, а потом на диск.
    mutex.lock();
    dwNumBytesRead = mfile.write(byInputBuffer, numBytes); //databuf.constData(), qstrlen(databuf) dwNumBytesRead byInputBuffer mystr
    mutex.unlock();
    if (dwNumBytesRead != numBytes) {KK_Write_To_Log(0xeeee, "Error on Write to file!\n");}
    quint64 fsize = mfile.size();

     if (fsize >= data_file_max_size) //‭0x100000 = 1048576‬  0x40 0000 = ‭4194304‬  0x10 000 = 65536 (1min) 0x800000 = 8 388 608 байт
    {                                                                              //          0xC00000= 12 582 912
     // 02.11.2017

        // Теперь тут надо как-то поменять адрес qbuf с текущего на другой.
        // Записать из текущего данные в файл, и при этом продолжать накапливать уже в другой буфер.
        // Передать текущий буфер через указатель в другой поток, где файл пишется на диск...
        // BUF_PTR = &BUF_02; это нулевой индекс массива BUF_02, поэтому последующие append-ы будут сначала массива.

        f_old_size = 0;
        mutex.lock();
        // Очень важно очистить уже заполненный буфер, т.к. иначе, при следующем обращении к этому буферу
        // Будет запись по адресу В КОНЦЕ ЭТОГО БУФЕРА

        if (CURRENT_BUFFER_INDEX_1) {BUF_PTR = &BUF_02; CURRENT_BUFFER_INDEX_1 = FALSE; BUF_01.clear();} // ЭТо так поменяли буфер для записи

        else { BUF_PTR = &BUF_01; CURRENT_BUFFER_INDEX_1 = TRUE; BUF_02.clear();}
mutex.unlock();

       fname = mfile.fileName();
       mfile.close(); //if (!ftStatus) { str.sprintf("Неудалось СЛИТЬ данные\n"); emit Show_data(str);}
       KK_Write_To_Log(0xeeee, "File is written on disk \n");
       str = fname.right(7); // Camac_Thread_data_000.bin
       str = str.left(3);
      int     index = str.toInt();  // Получили индекс файла
              index++; //Увеличили индекс для следующего файла
      QString newindex = QString::number(index);
       if (index <10) newindex.prepend("00");
       if (index > 9 && index < 100 ) newindex.prepend("0");
      // fname.replace(20,3,newindex); //qDebug() << fname; засада для // Camac_data_000.bin
         fname.replace(13,3,newindex); //qDebug() << fname; Правильный вариант для // Camac_data_000.bin

     //  QString str_2; str_2 = str.append("xxx");
    //   mfile.setFileName(fname);
     File_Open(fname);
     Sleep(50); //100ms, т.к. nsec_timer.elapsed(); выдавал 100мс.
      //  NANOSEC = nsec_timer.elapsed();
       //NANOSEC = nsec_timer.nsecsElapsed();
    //   QString mstr;
    //   mstr.sprintf("Write to file elapsed for  %llu  msecs", NANOSEC); mstr.append("\n");
    //   emit Show_data(mstr);
    
    }//if (fsize >= 0x80000)



} //if (mfile.isOpen())
else {str.sprintf("Не открыт файл \n"); emit Show_data(str);} //ui->FTDI_List_textBrowser->append





// Вот тут всегда будет тормоз при поточном режиме.
// Следует с этим разобраться

/*
 * В поточном режиме это вообще блокирует запись в файл.
 * Наверное, потому что отнимает время, а за это время поток "улетает"
 * Т.е. возникает ситуация, когда отрабатывается запись в файл, а уже пришли новые данные...
 * надо отладить. т.е. данные из буфера пишутся в файл, а вновь приходящие - в другой буфер...
if (!mfile.flush()) //;
{
    str.sprintf("Writing Fault\n");
    emit Show_data(str);
    //printf("Writing Fault");
}
*/
/*
 * Если ЗАКОММЕНТИРОВАТЬ очистку буфера, то данные, считываемые из буфера, меняться не будут !!!!
 * Поэтому очистку буфера чтения после считывания делаем обязательно !!!!
*/


//Очистка стоит выше, сразу после чтения !!!

//ftStatus = FT_Purge(ftHandle_A, FT_PURGE_RX | FT_PURGE_TX);
//memset(byInputBuffer, 0, USB_Buffer_Size); //sizeof(byInputBuffer)



}
//===================================================
int FTDI_D2XX::Read_A_Single ( char *buf_data, int buf_size )
{
    int ftdi_error_code = 0;
    ftStatus = FT_GetStatus(ftHandle_A,&RxBytes,&TxBytes,&EventDWord);
    if ((ftStatus != FT_OK)) {ftdi_error_code = 0x0008; return ftdi_error_code;}
    if (RxBytes > 0)
    {  // Отсекаем лишние байты, иногда это бывает

     str.sprintf("Read_A_Single()->Считано %lu байт в буфер \n", RxBytes);
     KK_Write_To_Log(0x1111, str);                // Чтобы не вычислять каждый раз sizeof(byInputBuffer)
     if (RxBytes > accum_buffer_SIZE ) {RxBytes = accum_buffer_SIZE;}  //byInputBuffer [EK_BUFFER_SIZE]; sizeof(byInputBuffer)
    //+++++++++++++++++++++
     ftStatus = FT_Read(ftHandle_A, byInputBuffer,RxBytes,&dwNumBytesRead); //RxBytes
         if (ftStatus == FT_OK)  { memcpy(buf_data, byInputBuffer, buf_size);}

         else { ftdi_error_code = 0x0009; return ftdi_error_code; }

 // И вот теперь пишем в файл.
// Так работает только если закрывать файл в деструкторе класса.
    if (mfile.isOpen())
     {
         int written_bytes;
         QDataStream out(&mfile);
         written_bytes = out.writeRawData(byInputBuffer, RxBytes); //RxBytes

         str.sprintf("Read_A_Single()->Записано %d байт в файл, Всего считано %lu \n", written_bytes, dwNumBytesRead); emit Show_data(str);

     }
    else { ftdi_error_code = 0x000A; return ftdi_error_code; }
}//if (RxBytes > 0)

  else { ftdi_error_code = 0x000B; return ftdi_error_code; }

    ftStatus = FT_Purge(ftHandle_A, FT_PURGE_RX);

    return ftdi_error_code; // 0 means it's OK !
}
//===================================================
//Открываем файл для записи ДВОИЧНЫХ ДАННЫХ
int FTDI_D2XX::File_Open(QString fname)
{
    int ftdi_error_code = 0;
    mfile.setFileName(fname);

    if (!mfile.open(QIODevice::ReadWrite | QIODevice::Truncate )) { ftdi_error_code = 0x000D; } // Ошибка
    else ftdi_error_code = 0x0013; // Нормально открылся

 //   EK_BUF(BUF_PTR, QIODevice::OpenModeFlag::ReadWrite);

return ftdi_error_code; // 0 means it's OK !
}
//==================================================
// Открываем лог-файл, текстовый режим
int FTDI_D2XX::Log_File_Open(QString lname)
{
    int ftdi_error_code = 0;
    LogFile.setFileName(lname);

    if (!LogFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text )) { ftdi_error_code = 0x0011; } //??? Какой код будет
    else  ftdi_error_code = 0x0012; //Log_File_Open correctly

return ftdi_error_code; // 0 means it's OK !

}
//+++++++++++++++++++++++++++++++++++++
void FTDI_D2XX::Purge_USB_Buffers(void)
{
       ftStatus = FT_Purge(ftHandle_A, FT_PURGE_RX);
       ftStatus = FT_Purge(ftHandle_A, FT_PURGE_TX);

       ftStatus = FT_Purge(ftHandle_B, FT_PURGE_RX);
       ftStatus = FT_Purge(ftHandle_B, FT_PURGE_TX);

}
//+++++++++++++=
void FTDI_D2XX::Purge_Data_Buffer(void)
{
    BUF_01.clear();
    BUF_02.clear();

}
//++++++++++++++++++++++++
int FTDI_D2XX::Get_RxBytes(void)
{
  int num;
    FT_GetStatus(ftHandle_A,&RxBytes,&TxBytes,&EventDWord);
    num = RxBytes;
    if (RxBytes > 0)
    {
      return num;
    }
 else {
        printf("Ошибка чтения буфера");
        return 0;
    }
}
//+++++++++++++++++++=
void FTDI_D2XX::Get_Buffer_Slot(char * OutBuffer)
{
    memcpy(OutputBuffer, OutBuffer, TRANSFER_SIZE);
}
//+++++++++++++++++++++

void FTDI_D2XX::Write_TO_File_Slot()
{
 // char byBuffer[8] = {0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E};
 char byBuffer[8] = {'\xF0', '\xD1', '\x00', '\x07', '\xD0', '\x00', '\x00', '\x80'};
 //memcpy (byBuffer, byInputBuffer, 8);

 bool f_size = false;
 quint64 fsize;

 while (!f_size)
 {
     mfile.write(byBuffer, 8);

     fsize = mfile.size();
     if (fsize >= 0x800000)
     {
         mfile.close(); //if (!ftStatus) { str.sprintf("Неудалось СЛИТЬ данные\n"); emit Show_data(str);}
         KK_Write_To_Log(0xeeee, "File is written on disk \n");
         f_size = true;

     }

 }



 // str.sprintf("WRITE Записано 8 байт \n"); emit Show_data(str);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Посылаем команду Inhibit в КК
// Ответ от КК:   F0 00 00 00 00 00 00 01.
// Ответ от ftdi записывается в массив buf_data !!!
int FTDI_D2XX::KK_Inhibit( char *buf_data, int buf_size )
{
  int ftdi_answer = 0; // 0 means it's OK !
  char CC_Inhibit[TRANSFER_SIZE] = {0x00, 0x00,0x00,0x00};
//  char KK_data[MIN_Rx_BUFFER_SIZE];

  ftStatus = FT_Purge(ftHandle_B, FT_PURGE_TX);
  ftStatus = FT_Purge(ftHandle_B, FT_PURGE_RX);

  ftStatus = FT_Purge(ftHandle_A, FT_PURGE_TX);
  ftStatus = FT_Purge(ftHandle_A, FT_PURGE_RX);

  Sleep(10);
  memset(byInputBuffer, 0, sizeof(byInputBuffer));

  // Отправляем команду 'Inhibit'
  ftStatus = FT_Write(ftHandle_B, CC_Inhibit, TRANSFER_SIZE, &dwNumBytesSent); //byOutputBuffer

  if (ftStatus != FT_OK)  { ftdi_answer = 0x000E; return ftdi_answer;  } // str.sprintf("Нет связи с КК \n"); emit Show_data(str); emit Device_NOT_connected_Signal();
  Sleep(10); // ПЕСЕЦ КАКАЯ ВАЖНАЯ ОПЦИЯ !!! Без этой задержки ответ приходит неверный.

//+++++++++ Рабочий код
 // ftStatus = FT_Purge(ftHandle_A, FT_PURGE_TX); // Это тоже задержка :) но с ней не работает
 // ftStatus = FT_Purge(ftHandle_A, FT_PURGE_RX);


//QString str;
//bool answer;            // Вернет "0", если все ОК

// Чтобы отличать коды ошибок от самой ф-ции и от этой же ф-ции, вызванной внутри другой ф-ции
// Прибавляю к коду 0x0010
  //                                 ВАЖНО !!! buf_data, А НЕ byInputBuffer
ftdi_answer =  Read_A_Single(buf_data, buf_size);
//Если Read_A_Single() вернула ошибку, то код ответа ф-ции KK_Inhibit() корректируем на заданное значение, чтобы
// отличить от других ситуаций.

if (ftdi_answer !=0) {ftdi_answer += KK_Inhibit_code_sign;   KK_Write_To_Log(ftdi_answer, "Read_A_Single Answer ");    return ftdi_answer;}

// Данные считали, массив buf_data, byInputBuffer  получили.
// Дальше работаем с buf_data,  byInputBuffer НЕ ТРОГАЕМ !!!

// memcpy(buf_data, KK_data, buf_size);

  QByteArray mystr = QByteArray(reinterpret_cast<char*>(buf_data), 16); //ДА, byInputBuffer НЕ ТРОГАЕМ !!!
  mystr.resize(RECEIVE_SIZE);
  char Inhibit [RECEIVE_SIZE] = {'\xf0', 0x00, 0x00, 0x00, 0x00,0x00, 0x00,0x01};

  ft2232H = memcmp(Inhibit, mystr, RECEIVE_SIZE); //Вот этот код возвращаем выше, анализируем и далее по списку.
  if (ft2232H == 0)
      {
      ftdi_answer = 0x000F;
   }

  else
  {
      str.sprintf("Нет связи с КК \n");
      str.sprintf("НЕВЕРНО ! Данные массива FTDI : ");
      str.append(mystr.toHex().toUpper());
      str.append("\n");

      ftdi_answer = 0x0010;

     // emit Show_data(str); emit Device_NOT_connected_Signal();
      KK_Write_To_Log(ftdi_answer, str);

  }


return ftdi_answer; // 0 means it's OK !

}

//++++++++++++++++++++++++++++++++++++++++++++++++
void FTDI_D2XX::Camac_Data_Parser(void)
{
int fefe;
int X_CAMAC;
int Q_CAMAC;
int NN_camac;

fefe = 0x00;
                                                             // 0x40 - никогда не сработает !!! т.к. это int, а надо char
    if ((byInputBuffer[0] == '\xf0') && ((byInputBuffer[7] == '\x40') || (byInputBuffer[7] == '\x80')) ) //Т.е. это ответ на известную команду  && (byInputBuffer[7] == '\x40')
    {

    X_CAMAC = (byInputBuffer[1] >>6 ) & 0x01;
    Q_CAMAC = (byInputBuffer[1] >>7 ) & 0x01;
    NN_camac  = byInputBuffer[1];  //N_CAMAC
    fefe = (NN_camac & 0x1f);
    if (X_CAMAC == 0x01) {emit Camac_N_Connected_Signal(fefe); str.sprintf ("Получен 'X' от блока в станции N= %d \n",fefe) ;emit Show_data(str);}
    if (Q_CAMAC == 0x01) {emit Camac_N_Q_Flag_Signal();  str.sprintf ("Получен 'Q' от блока в станции N= %d \n",fefe) ;emit Show_data(str);}

    }
else { emit Camac_N_NOT_Connected_Signal(fefe); str.sprintf ("Нет ответа 'X' от блока в станции N= %d \n",fefe) ;
       emit Camac_N_Q_NOT_Flag_Signal(); emit Show_data(str);}
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Это обработчик сигнала от timer_2
// ЗА время ожидания таймера_2 должно прибыть уже несколько СОТЕН посылок
// Их все надо в буфер записывать.
// Также надо различать новое и старое значение размера qbuf, чтобы каждый раз не перерисовывать.
// В этой ф-ции отбираем новые точки из текущего буфера набора и копируем эти точки в другой буфер - increment_qbuf
// Точки отрисовки только ДОБАВЛЯЕМ. Т.е. берем только новые точки, полученные за вермя ожидания таймера.
// Т.е. это новая порция данных, полученная от FTDI, которую вычленяем и передаем через массив increment_qbuf на отрисовку
void FTDI_D2XX::Get_Spectral_Points_Slot()
{
//Считываем буфер с текущими данными набора, берем "разницу" данных с предыдущими данными набора.
// ОТправляем эту разницу на отрисовку спектра.
   QString mstr;
   QByteArray increment_qbuf;
//   quint64 buf_size;

  // mstr.sprintf("Вывод спектра для станции № %d, канал %d \n",N_SPECTRAL,  N_SPECTRAL_CHANNEL);
//   mstr.sprintf("НАчало Вывод спектра для станции \n");
//   KK_Write_To_Log(0x2222, mstr);


   mutex.lock();
   if (CURRENT_BUFFER_INDEX_1) f_size = BUF_01.size(); //BUF_PTR = BUF_02; // Это так поменяли буфер для записи
   else {f_size = BUF_02.size(); }


 // f_size = qbuf.size(); //Вот тут может возникнуть отрицательное число !!!
  if (f_size < f_old_size) {f_offset = f_size;  KK_Write_To_Log(0x9999, "Negative OFFSET ! \n");}
  else f_offset = f_size - f_old_size ;
  f_old_size = f_size;             // буфер только что сменился и у него еще размер МЕНЬШЕ, ЧЕМ f_old_size

//  if (f_offset > 0x1000) f_offset = 0x1000;  //4096 Больше быть не может, т.к. размер буфера такой.
  // Но если отрисовку включить не сразу, то может накопиться большое количество данных.


  if (CURRENT_BUFFER_INDEX_1) increment_qbuf = BUF_01.right(f_offset); //BUF_PTR = BUF_02; // ЭТо так поменяли буфер для записи
  else increment_qbuf = BUF_02.right(f_offset);
mutex.unlock();
//mstr.sprintf("Конец Вывод спектра для станции \n"); KK_Write_To_Log(0x2222, mstr);


  //increment_qbuf = qbuf.right(f_offset);

 // QByteArray bb = increment_qbuf.left(8);





//  buf_size = myqbuff.read(cur_banch,10);
//  myqbuff.close();
//   buf_size = increment_qbuf.size();

//Теперь это просто ф-ция, не слот.
//Мы сейчас в классе FTDI_D2XX:: а надо передать массив в класс Plot::

emit Draw_Spectral_N_Chan_Signal(&increment_qbuf); //&increment_qbuf  &t_points



}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++
void FTDI_D2XX::KK_Write_To_Log (int value, QString log_message)
{
    QDateTime curdate ;
    QTextStream uin(&this->LogFile);

    QString str, str2;
    curdate = QDateTime::currentDateTime();

    str = curdate.toString("yyyy-MM-dd__hh:mm:ss:z").toLocal8Bit(); str.append(" > ");
    str2 = QString("0x%1: ").arg(value, 4, 16, QChar('0'));

    uin << str << str2 << log_message;


}
//==================================================
void FTDI_D2XX::make_io_file_slot()
{
    QString str;

    fname = mfile.fileName();
        str = fname.right(7);
        str = str.left(3);
       int     index = str.toInt();  // Получили индекс файла
               index++; //Увеличили индекс для следующего файла
       QString newindex = QString::number(index);
        if (index <10) newindex.prepend("00");
        if (index > 9 && index < 100 ) newindex.prepend("0");
        fname.replace(13,3,newindex); //qDebug() << fname;


      //  QString str_2; str_2 = str.append("xxx");
     //   mfile.setFileName(fname);
      File_Open(fname);

      // Делаем запись в лог об открытии файла
       str.sprintf("Открыт файл набора данных ");
       str.append(fname); str.append("\n");
       KK_Write_To_Log (0x4444, str);

}
//===================================================
// Очищаем буфферы с точками отрисовки
void FTDI_D2XX::clear_data_buffers_slot()
{
    Purge_Data_Buffer();
}
