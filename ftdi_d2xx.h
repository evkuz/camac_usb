/*
Класс FTDI_D2XX будет описывать FTDI-устройство на основе чипа FT2232H (ft_eeprom_2232h)
Выдает список ftdi устройств и их параметры - Имя, указатель, описание.
Отвечает за прием/передачу данных от ПК в ftdi-устройство, либо из него.
Выполняет запись в файл ПК принятых данных. Так можно выполнять эту операцию в другом потоке, не основном.
Т.к. если это выполнять в основном потоке, то будет тормозить GUI.
Предполагается, что анализ и обработку принятых данных будет выполнять объект другого класса.

*/

#ifndef FTDI_D2XX_H
#define FTDI_D2XX_H

#include <QObject>

#include "ftd2xx.h"
#include <QFile>
#include <QDataStream>
#include <QFileInfo>
#include <QBuffer>
#include <QElapsedTimer>
#include <QDateTime>
#include <QTextStream>
#include <QMutex>
//#include <QMutableByteArrayListIterator>
/*
namespace ftdi {
class FTDI_D2XX;
}

namespace Ui {
class MainWindow;
}

*/
class FTDI_D2XX : public QObject
{
    Q_OBJECT
public:
  //  explicit FTDI_D2XX(QObject *parent = 0);
     FTDI_D2XX();
    ~FTDI_D2XX();

#define TRANSFER_SIZE 4 //Команда ОТ ПК для FTDI из 4 байт
#define RECEIVE_SIZE  8 // Ответ из ftdi в ПК 8 байт

#define MIN_Rx_BUFFER_SIZE 8 // То же, что и RECEIVE_SIZE, но для других частей кода в целях улучшения работы.

#define KK_Inhibit_code_sign 0x1000

#define accum_buffer_SIZE  0x400000 // это 4 194 304 байт !
#define USB_Buffer_Size   65536 //10000  0xFFC0
                         // on USB2.0, we have found that transfer sizes greater than  64K fail catastrophically.
#define data_file_max_size 0xC00000 //0xC00000= 12 582 912


/*
Note that, currently, FT_SetUSBParameters supports transfer size changes for the IN endpoint
only, and the remainder of this note is restricted to consideration of read requests.
*/

#define FT_efficient_size 63448
/*
Maximum throughput is achieved using a current USB transfer size of 64K bytes, and application
requests that are multiples of 63448 bytes.
*/

#define FTDI_OK 0x0AAA // Ответ "все хорошо"  от ftdi_init()


     QFile       mfile;
     QFile       LogFile;
     QString     filename;
     int         file_pos;  //Позиция в файле
     QFileInfo   sfile;     // Будем смотреть размер файла и вычислять позицию.
     QByteArray  qbuf;      // Просто буфер для общения с другими классами.
                            // Пока передаем данные из файла набора для отрисовки гистограмм
     QFile       tstfile;   // Файл с тестовыми данными для отрисовки гистограмм. должна быть доступна из Read_A()

     quint64 f_offset =0;   // Смещение в файле-буфере
     quint64 f_size=0;      // Текущий размер файла-буфера
     quint64 f_old_size=0;  // Предыдущий размер файла-буфера

     quint64 NANOSEC; // Количество наносекунд за которые файл-буфер накопления пишется на диск.
     QElapsedTimer nsec_timer;


FT_HANDLE ftHandle; // Handle of the FTDI device
FT_STATUS ftStatus; // Result of each D2XX call
FT_HANDLE ftHandle_A;
FT_HANDLE ftHandle_B;

DWORD dwNumDevs; // The number of devices
DWORD dwNumBytesToRead = 0; // Number of bytes available to read in the driver's input buffer
DWORD dwNumBytesRead;
DWORD dwNumBytesSent;
DWORD dwNumBytesToSend;

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
char OutputBuffer[TRANSFER_SIZE];
char byInputBuffer[accum_buffer_SIZE]; //4096  0x40000// USB_Buffer_Size Buffer to hold data readed from the FT2232H MIN_Rx_BUFFER_SIZE
char SpectralBuffer[MIN_Rx_BUFFER_SIZE]; // Buffer to hold data coopied from byInputBuffer for online graph

QByteArray BUF_01;// [0x100000]; //0x400000 = 4 194 304 ;// [accum_buffer_SIZE]
QByteArray BUF_02;// [0x100000]; [accum_buffer_SIZE]
QByteArray * BUF_PTR;// = BUF_01;

const char TEST_DATA_B2[RECEIVE_SIZE] = {0xf0, 0x11, 0x00, 0x07, 0xD0, 0x00, 0x00, 0x80}; // Тестовые данные для отрисовки спектров. Имитация hardware
const char TEST_DATA_B0[RECEIVE_SIZE] = {0xf0, 0x11, 0x00, 0x07, 0xD0, 0x00, 0x00, 0x40}; // Тестовые данные для отрисовки спектров. Имитация hardware

QDataStream EK_BUF;
BOOL CURRENT_BUFFER_INDEX_1;


DWORD Flags;
DWORD ID;
DWORD Type;
DWORD LocId;
UCHAR Mask = 0xff;
UCHAR Mode;
UCHAR LatencyTimer = 8; //16ms default

QString str, fname;

int DevNumber; //Номер НАШЕГО девайса

int DevNumber_A; //Порт А, Номер НАШЕГО девайса
int DevNumber_B; //Порт B, Номер НАШЕГО девайса



//    const char Morph_ID[] = "FT112244EKA"; //Этот девайс будем выбирать среди всех доступных
//    const char Mini_ID_Sem[] = "FT112244SemA";




int ftdi_init(char * looking_for_device_A, char * looking_for_device_B);        //Выдает список подключенных ftdi устройств, открывает порты А В в нужном устройстве
void Read_A(uint numBytes);   // Считывает данные из порта А в поточном режиме
int Read_A_Single (char *buf_data, int buf_size);   // Считывает данные из порта А в 1-КРАТНОМ режиме
int File_Open(QString fname); //Открыть файл на запись данных, полученных по USB
int Log_File_Open(QString lname);

void Purge_USB_Buffers(void); // Очистка FTDI буфферов
void Purge_Data_Buffer(void); // Очистка буфферов с накопленными данными.

int Write_B_Single(char *OutBuffer, int numbytes); // Запись данных из ПК в порт 'В' 1-кратный режим
int Write_B(char *OutBuffer, int numbytes); //запись данных в канал 'B' в поточном режиме
int  Get_RxBytes(void);
int KK_Inhibit(char *buf_data, int buf_size ); // Команда "Inhibit" для КАМАК

void File_read_Current(char *buf_data); //Считываем текущие данные из файла для отрисовки спектра в online

//+++++++++++++++++++++ CAMAC ++++++++++++++
void Camac_Data_Parser(void); // Парсинг пришедших от ftdi данных, т.е. будет вытаскивать NAF и данные станции из пришедшей посылки.
void Camac_N_Data_Parser(int Num_St); // То же что и выше, но с номером станции в агруементе
//QDataStream inFile;
void KK_Write_To_Log (int value, QString log_nessage);

int Data_File_Open(QString fname); //Открыть файл для чтения тестовых данных, и передачи на отрисовку спектра

QMutex mutex;

//QByteArray t_points; //временный массив точек гистограммы для отладки быстродействия.
//++++++++++++++++++++ END OF CAMAC ++++++++++++

signals:
        void Show_data(QString sss);          // Сигнал для отображения данных в GUI
        void Device_connected_Signal();       // Сигнал для индикации подключения к ПК ftdi-устройства
        void Device_NOT_connected_Signal();   // Сигнал для индикации ОТключения ОТ ПК ftdi-устройства
        //+++++++++++ CAMAC concerned signals
        void Camac_N_Connected_Signal(int N_num); // Сигнал, что станция N на связи. Есть X.
        void Camac_N_NOT_Connected_Signal(int N_num); // Сигнал, что станция N !!! НЕ !!! на связи. НЕТ X
        void Camac_N_Q_Flag_Signal(); // Сигнал, что  есть ответ Q от станции
        void Camac_N_Q_NOT_Flag_Signal(); // Сигнал, что  НЕТ ответа Q от станции

        void Show_file_size_signal(QString sss, QString sss2);  // Сигнал-передача данных для текстового окна
        void Show_file_offset_signal (QString sss);             // Сигнал-передача данных для текстового окна

        // Сигнал-передача массива qbuf на выборку данных для заданной станции N и канала Chan_number и отрисовку гистограммы  по этим данным
        void Draw_Spectral_N_Chan_Signal (QByteArray *); //, UINT8 N_number, UINT8 Chan_number QByteArray &ibuf
        void Thread_Pause_Signal();      //Сигнал "ставим паузу" для потока
        void Thread_Pause_OFF_Signal();  // Сигнал "снимаем с паузы" поток

public slots:


    void Write_B_Thread(); //запись данных в канал B в поточном режиме
    void Read_AAA(void); // Считывание данных канал А

    void PutBack_ftHandle_B(); // Получить из GUI ftHandle_B
    void Close_ftdi_chan_B();  // Закрыть порт В

    void PutBack_ftHandle_A(); // Получить из GUI ftHandle_A
    void Close_ftdi_chan_A();  // Закрыть порт  А
    void Get_Buffer_Slot(char * );// Скопировать из GUI буфер данных для передачи из ПК в ftdi -  OutBuffer
    void Write_TO_File_Slot();    // По нажатию кнопки в GUI записать в файл заранее заданные данные. В тестовых целях.
    void Get_Spectral_Points_Slot(); // Делаем выборку данных из файла для обновления спектра.

    void make_io_file_slot(); // Слот обработки сигнала void make_io_file_signal();

    void clear_data_buffers_slot();// Слот очистки буфферов с точками отрисовки

};

#endif // FTDI_D2XX_H
