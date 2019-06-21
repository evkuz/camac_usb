#ifndef COMMAND_LIST
#define COMMAND_LIST
/*
# Строки, начинающиеся с '#' являются комментариями
#
# Формат команды имеет вид N[xx]A[xx]F[xx]hex-data
# В скобках [] указывается десятичное число, всегда 2-значное, т.е. числа меньше 10 пишутся с ведущим нулем, в виде "07"
# hex-data - это данные в hex-формате вида 'af38', всегда указывается 16 бит
# Пустые строки допускаются
# В данном примере строки 10 и 14 записаны неправильно, об этом будет сообщать лог.
# Если программа видит строку неправильного формата, то НИ ОДНА строка из файла не преобразуется в команду, НИКАКИЕ команды из файла не посылаются в KK_USB.

N[7]A[09]F[17] тут ошибка
N[18]A[13]F[20]
N[07]A[09]F[17]8dE6
N[15]A[10]F[24]
N[16]A[11]F18] тут ошибка
N[17]A[12]F[04]
N[18]A[13]F[20]37A8


*/
#endif // COMMAND_LIST

