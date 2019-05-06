#ifndef STAT
#define STAT

/*
 *  07/12/2017
 * Файл статистики сбоев
 * Будем писать программу, которая продвигается по файлу набора и проверяет правильность следования байтов.
 * т.е. 1-й байт из 8 всегда должен быть 0xF0, а 8-й байт всегда 0x80
 *

##################################
        if (ftdi_error_code == 0) // Строка в файле правильная -  не комментарий и не содержит ошибок.
        {                         // Двигаемся дальше по командному файлу, больше ничего не делаем.
            ftdi_error_code =  KAN->Write_B_Single(byOutputBuffer, TRANSFER_SIZE); // Отправляем команду по результатам парсинга
            ftdi_answer_parser(ftdi_error_code + value); //+ 0x0300
     //  curdate = QDateTime::currentDateTime();
       //Пишем в лог, что команда КАМАК отправлена в КК
            if (!ftdi_error_code) //Если возвращает '0' то для if это false, а для ftdi_answer_parser - это отсутствие ошибок
            {
     //  uin << curdate.toString("yyyy-MM-dd__hh:mm:ss:z") << " > " << "NOTICE: Command " << str1 << " is sent successfully!!!\n";
       //    str1.chop(1);
                str2 = "NOTICE: Command "; str2.append(str1); str2.append(" is sent successfully!!!\n");
                Write_To_Log(value, str2);

            }
      //Т.к. режим "1-Разовый" то вводим паузу. Иначе в файл данные не пишутся.
        Sleep(1); // это 1мс. !!!!!!!!!!!, а не 1с.
       //Пишем в лог ответ от КК на команду, посланную ранее ???
       // Или ограничися сообщением ftdi_answer_parser ???
       // Ограниичимся, т.к. ранее в логе есть сообщение о посланной команде.
        ftdi_error_code =  KAN->Read_A_Single(b_Data, MIN_Rx_BUFFER_SIZE );
        ftdi_answer_parser( ftdi_error_code  + value ); //Это если ошибка, а данные-то как посмотреть.

        KAN->Camac_Data_Parser();

       QByteArray  mystr = QByteArray((char*)KAN->byInputBuffer, MIN_Rx_BUFFER_SIZE);//b_Data
       mystr.resize(RECEIVE_SIZE);
       str.sprintf("Ответ на команду из файла : ");
       str.append(mystr.toHex().toUpper());
       str.append("\n");
       Write_To_Log(value, str);
    } //if (ftdi_error_code == 0)
        else {if (ftdi_error_code != 1)
               {str1.sprintf("Строка %d в файле %s имеет неправильный формат \n", n, Camac_Commands_File); Write_To_Log(value, str1);
             // Выходим. Т.е. не выполняем команды дальше
            }

             }
        if (ftdi_error_code == 1) {ftdi_error_code += value;} // 0x0301 // Получена строка с комментарием, но это не ошибка
        n++;
    }

    f.close();

 }
    else {ftdi_error_code = 0x0014;} // Не могу открыть файл
    ######################################################


*/


#endif // STAT

