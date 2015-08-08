Полная документация на http://pyhrol.ru/

Pyhrol ЭТО:
  * мост между языками C++ и Python
  * мост с односторонним движением, из C++ в Python
  * libpyhrol.so, pyhrol.h, тесты, примеры и сайт pyhrol.ru
  * вещь, заточенная исключительно под Python 2.x
  * честное ПО с открытытм исходным текстом

Pyhrol это НЕ:
  * волшебная кнопка, требует как минимум декларировать объекты и
    немножко кодить
  * вещь, поддерживающая Python 3.x

Сборка:
  tar -x -f pyhrol-${VERSION}.tar.gz
  cd pyhrol-${VERSION}
  cmake -D CMAKE_BUILD_TYPE=Release .
  make

Установка:
  make install

Запуск примеров:
  make test
    или
  ctest
    или
  ctest -R <номер примера>

Примеры (директория examples/):
  Функции:
    0010 Простая функция
    0020 Функция с аргументами
    0030 Функция с возвращаемым значением
    0040 Аргументы по умолчанию
    0050 Именованные аргументы
    0060 Перегрузка аргументов
    0070 "Перегрузка" возвращаемых значений
    0080 Объектные типы
    0090 Объекты заданного типа
    0095 Строковые типы
    0100 Произвольные типы
    0110 Обработка ошибок
    0120 Справка
  Классы:
    0400 Определение
    0410 Создание/разрушение
    0420 Методы
    0430 Статические методы
    0440 Поля
    0450 Атрибуты
    0455 Атрибуты 2
    0460 Указатели
    0470 Синглетон
    0480 "Умные" указатели
    0490 Арифметические операции (Number Protocol)
    0495 Арифметические операции над произвольными типами
    0500 Последовательности (Sequence Protocol)
    0505 Операции над последовательностями с произвольными типами
    0510 Ассоциативные массивы (Mapping Protocol)
    0520 Итераторы (Iterator Protocol)
    0540 Наследование
    0550 Аргументы объектного типа
    0560 "Внешний" конструктор
  Прочее:
    0800 Модуль pyhrol
    0810 Трассировка
    0820 Форматирование справки
    0830 Версия с минимальным функционалом
    0840 Перегрузка по имени аргумента
    0850 Сокрытие методов
  Типичные ошибки:
    0900 Неверное наследование
    0910 Конфликты в таблице символов
    0920 Неверное использование макросов
    0925 Неверное использование макросов в классах
    0926 Неверное использование макросов в классах (обход)
    0930 Неуникальность указателей
    0940 Недопустимые имена и символы
    0950 std::bad_cast
    0960 Закрытый конструктор копирования
    0970 Вызов конструктора TypeBase по умолчанию
