#ifndef BUFFER_OF_HYST_POINTS
#define BUFFER_OF_HYST_POINTS
/*
 * Механизм отрисовки спектра.
 *
 * При считываниии данных из FTDI получаем массив точек byInputBuffer
 * Далее используем имеющиеся массивы
 *
 * QByteArray BUF_01;// [0x100000]; //0x400000 = 4 194 304 ;// [accum_buffer_SIZE]
 * QByteArray BUF_02;// [0x100000]; [accum_buffer_SIZE]
 * QByteArray * BUF_PTR;// = BUF_01;
 *
 * Каждая новая порция точек из byInputBuffer добавляется в тот буфер, на который в данный момент указывает  BUF_PTR.
 * Это либо BUF_01, либо BUF_02
 * Когда размер файла достигает заданной величины "data_file_max_size" указатель BUF_PTR меняется,
 * и указывает на другой буфер из пары BUF_01/BUF_02
 *
 * Ф-ция FTDI_D2XX::Get_Spectral_Points_Slot() добавляет точки из BUF_01/BUF_02 в локальный массив "increment_qbuf"
 *
 *   if (CURRENT_BUFFER_INDEX_1) increment_qbuf = BUF_01.right(f_offset); //BUF_PTR = BUF_02; // ЭТо так поменяли буфер для записи
     else increment_qbuf = BUF_02.right(f_offset);
 *
 * Далее эмиттируется сигнал на добавление точек в гистограмму :
 * emit Draw_Spectral_N_Chan_Signal(&increment_qbuf); //&increment_qbuf  &t_points
 *
 * Этот сигнал обрабатывается слотом :
 * Plot::Draw_Spectral_N_Chan_Slot (QByteArray * ibuf)
 * где Разбиваем массив на порции по 8 байт.
 * Каждую порцию анализируем и если там есть для нас данные, то пускаем на отрисовку.
 *
 * this->intervals->append(QwtIntervalSample(this->Ch_Data, fdata, fdata + di));
 *
 * Тогда для очистки окна спектра следует сделать следующее :
 * - Очистка буфера BUF_01/BUF_02
 * - Очистка this->intervals
 * -
 *  this->hystogram->setSamples(*this->intervals);
    this->hystogram->attach(this);
    this->replot();

 *

 *



*/
#endif // BUFFER_OF_HYST_POINTS

