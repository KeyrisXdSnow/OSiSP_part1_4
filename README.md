# OSiSP_part1_4
Главный процесс создаёт 3 дочерних, в 1 дочерний передается с помощью сигнала строка принятая из консоли,
он считает кол-во байт в строке и возвращает его в роди-тельский, далее родительский отсылает кол-во байт во
2 дочерний который вычитает из этого числа номер вашего варианта и создаёт тако
е же кол-во дочерних процессов. Главный ждёт 10 сек и посылает сигнал SIGTERM всем созданным 2 процессом.
А 3 дочерний должен игнорировать сигнал SIGTERM.
