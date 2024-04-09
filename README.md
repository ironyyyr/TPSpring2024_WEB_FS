# TPSpring2024_WEB_FS

## Task1
Если в числе содержится только один бит со значением 1, записать в выходной
поток OK. Иначе записать FAIL

## Task2
Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел
B[0..m-1]. Для каждого элемента массива B[i] найдите минимальный индекс k
минимального элемента массива A, равного или превосходящего B[i]: A[k] >= B[i].
Если такого элемента нет, выведите n. n, m ≤ 10000. Требования:  Время работы
поиска k для каждого элемента B[i]: O(log(k)). Внимание! В этой задаче для
каждого B[i] сначала нужно определить диапазон для бинарного поиска размером
порядка k с помощью экспоненциального поиска, а потом уже в нем делать бинарный
поиск. Формат входных данных. В первой строчке записаны числа n и m. Во второй и
третьей массивы A и B соответственно.

## Task3
Реализовать очередь с динамическим зацикленным буфером (на основе
динамического массива). Требования: Очередь должна быть реализована в виде
класса.

## Task4
В операционной системе Technux есть планировщик процессов.
Каждый процесс характеризуется: приоритетом P
временем, которое он уже отработал t
временем, которое необходимо для завершения работы процесса T
Планировщик процессов выбирает процесс с минимальным значением P * (t + 1),
выполняет его время P и кладет обратно в очередь процессов. Если выполняется
условие t >= T, то процесс считается завершенным и удаляется из очереди.
Требуется посчитать кол-во переключений процессора.
Формат входных данных:  Сначала вводится кол-во процессов. После этого процессы
в формате P T Формат выходных данных: Кол-во переключений процессора.

## Task5
На числовой прямой окрасили N отрезков. Известны координаты левого и правого
концов каждого отрезка (Li и Ri). Найти сумму длин частей числовой прямой,
окрашенных ровно в один слой

## Task6
Реализуйте стратегию выбора опорного элемента “случайный элемент”. Функцию
Partition реализуйте методом прохода двумя итераторами от конца массива к
началу.

## Task7
Дан массив строк. Количество строк не больше 105. Отсортировать массив методом
поразрядной сортировки MSD по символам. Размер алфавита - 256 символов.
Последний символ строки = ‘\0’.
