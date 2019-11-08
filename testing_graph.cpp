#include "ftdi_d2xx.h"
#include <QFile>

int FTDI_D2XX::Data_File_Open(QString fname)
{
    int ftdi_error_code = 0;
    tstfile.setFileName(fname);

    if (!tstfile.open(QIODevice::ReadOnly )) { ftdi_error_code = 0x0015; } // Ошибка
    else ftdi_error_code = 0x0016; // Нормально открылся

 //   EK_BUF(BUF_PTR, QIODevice::OpenModeFlag::ReadWrite);

return ftdi_error_code; // 0 means it's OK !


}
//++++++++++++++++++++++++++++++==
void FTDI_D2XX::File_read_Current(char *buf_data)
{

    uchar num;

    if (!tstfile.atEnd())
    {

    if (!tstfile.seek(file_pos)) {str.sprintf("Seek operation ERROR in string 23, file_pos value is  %d", file_pos); KK_Write_To_Log(0x1234, str);}
    tstfile.read(buf_data, 8);
    //num = reinterpret_cast<unsigned char&>(buf_data);
/*
    QByteArray mystr = QByteArray(reinterpret_cast<char*>(buf_data), 8);
    QString logstr;
    logstr.sprintf("Из тестового файла считаны данные : "); logstr.append(mystr.toHex().toUpper()); logstr.append("\n");  KK_Write_To_Log(0x1234, logstr);
*/
    file_pos += 8;
    }
    else
    {
        file_pos = 0;
         if (!tstfile.seek(file_pos)) {str.sprintf("Seek operation ERROR in string 23, file_pos value is  %d", file_pos); KK_Write_To_Log(0x1234, str);}
        tstfile.read(buf_data, 8);


    }


}
