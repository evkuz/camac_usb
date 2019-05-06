#ifndef TTX_DEVICE
#define TTX_DEVICE

/*
 *  KK_Tab_09
 *
 *  https://download.qt.io/archive/qt/5.4/5.4.0/
 *  https://download.qt.io/archive/qt/5.4/5.4.0/qt-opensource-windows-x86-mingw491_opengl-5.4.0.exe
 *
 *  https://www1.qt.io/download-open-source/
 *
 *
 *  D:\FPGA\Projects\Camac_USB\EK_ALT_USB_Modes_05
 *
 *  23.05.2017
 *
 * f (ft) = 3MHz
 * T (ft) = 300ns
 *
 * RXFn для 4 байт (команда) занимает 2.4мкс
 *
 * Интервал между RXFn-пачками на частоте 5кГц составляет 200мкс. что и эквивалентно частоте 5КГц.
 *
 * При отправке данных в ПК сначала 8 байт записываются в буфер ft2232h, и по сигналу SIWU
 * весь буфер отсылается в ПК.
 * Интервал между фронтом первого RXFn и SIWU равен 8 мкс.
 * По окончании SIWU выдается сигнал clr_data, разрешающий принимать новую порцию данных.
 * С учетом сигнала clr_data полный цикл займет 10мкс.
 *
 *
 * 14:08 start
 * 15:00 stop
 *
 * время сбора данных 52 min. = 3120 sec
 * Получено 27285855 БАЙТ за 3120 sec. = 8745 bytes/sec = 69960 bits/sec
 * А должно быть не менее 16Мбит/сек Нужно в 240 раз быстрее :)
 *
 * 1. Передавать сразу по 8-16-32 байта уже увеличение.
 * 2. "Мертвое" время сделать не 100 мкс. как сейчас, а в разы меньше.


  Про буфер FTDI.

  Размер буфера 4096 байт ! И его нельзя уменьшить.
  Время ожидания заполнения буфера у FTDI = 2мс.
  В итоге мы имеем ситуацию, когда не можем столько времени ждать заполнения буфера,

  !!!!!!!!!!!ПОЭТОМУ !!!!!!  используем SIWU.


  ================= СКОРОСТЬ НАБОРА ФАЙЛА ============

  скорость накопления файла нормальная :
  1Мб за 4.5 мин.
  4Мб за 17 минут
  8 388 608 байт  - за 35 минут.
  8 388 6хх байт  - за 3мин. 36с. !!! (queuedConnection) частота набора 5КГц.

  Если ставить частоту обмена 20 кГц, то файл 8 Мб набирается за 1 минуту.
                           10 кГц - за 2 минуты. Сходится.
  В ЛЯР 8Мб за 5минут. Там 371 точка в сек. тут 512 точек в секунду.



*/


/* Для тестирования. Полученные данные выводим в окно.
  ftStatus = FT_Read(ftHandle_A, byInputBuffer,RECEIVE_SIZE,&dwNumBytesRead);
  if (ftStatus == FT_OK)
    {
  QString str;
  QByteArray mystr = QByteArray(reinterpret_cast<char*>(byInputBuffer), 16);
  mystr.resize(8);
  str.sprintf("Данные массива FTDI : ");
  str.append(mystr.toHex().toUpper());
  str.append("\n");
  emit Show_data(str);
    }
else
      { str.sprintf("Нет связи с КК \n"); emit Show_data(str); emit Device_NOT_connected_Signal(); return FALSE;    }
*/



#endif // TTX_DEVICE

