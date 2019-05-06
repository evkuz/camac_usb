/*

CAMAC функционал


*/
#include "ftdi_d2xx.h"


// Проверяем полученную команду на соответствие заданному формату.
// Выделяем NAF и данные.
void FTDI_D2XX::Camac_N_Data_Parser(int Num_St)
{

    char bt_01;
    int cam_data;
    UINT8 N_cur, A_cur, F_cur;
    UINT8 FISS_lsb, FISS_msb, ALPHA_lsb, ALPHA_msb;
    UINT16 fdata, fdata_2;


    if  (SpectralBuffer[0] == '\xf0') {

       if (SpectralBuffer[7] == '\x40') {//== b0 type
           ;
       }//(SpectralBuffer[7] == '\x40') == b0 type

       if (SpectralBuffer[7] == '\x80') { //other blocks
           N_cur = SpectralBuffer[7];

       }//(SpectralBuffer[7] == '\x80') //other blocks

       if ('\x1f' >= SpectralBuffer[7] >= '\x01') {// ADP16 b1 type
           N_cur = SpectralBuffer[7];
           FISS_lsb = SpectralBuffer[4];
           FISS_msb = SpectralBuffer[3];

           ALPHA_lsb = SpectralBuffer[6];
           ALPHA_msb = SpectralBuffer[5];

           // Данные распарсили, вызываем отрисовку
           //if (N_cur == Num_St) emit Draw_Spectral_N_Chan_Signal(qbuf);
       }
    }//if  (SpectralBuffer[0] == '\xf0')

}
