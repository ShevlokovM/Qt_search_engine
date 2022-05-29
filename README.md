# Qt_search_engine
Educational work.
Учебный проект Qt_Search_Engine

Простой и удобный поисковик с графическим интерфейсом Qt.

Технологии:
C++, Qt, JSON, многопоточность. 

Назначение:
Производит поиск точного соответствия набору слов в поисковом
 запросе, в файлах формата txt, расположенных в поисковой 
директории приложения.

Описание: 
Соответствующая директория размещена в корневой папке проекта.
Для каждого целевого файла, приложение рассчитывает его 
релевантность Поисковому запросу, и выводит список файлов
в порядке убывания релевантности. 

Пример расчета релевантности (псевдо-код):

MAX = максимальное число слов соответствующх запросу, 
содержащихся в файле, из всех файлов в поисковой директории.

CUR = число слов, соответствующх запросу, 
в каждом отдельном файле.

rank - релевантность файла запросу

rank = MAX/CUR;

Пример поиска:
(псевдо-код)
file_1 = {"the table"};
file_2 = {"the window};
file_3 = {"the the table"};

request_1 = "the";
request_2 = "the table";

(код поисковика - упрощенный JSON, без некоторых элементов синтаксиса)
request001
result: true         // есть файлы соответствующие запросу на 100%
docid: 3, rank: 1    // файл №3 - 2 слова (MAX = 2)
docid: 2, rank: 0.5  // файл №2 - 1 слово (1/MAX = 0,5)
docid: 1, rank: 0.5  // файл №1 - 1 слово (1/MAX = 0,5)

request002
result: true
docid: 3, rank: 1
docid: 1, rank: 0.66

Интерфейс:
1. Кнопка "GET CONFIG" - позволяет посмотреть текущую конфигуацию приложения
2. Кнопка "SET CONFIG" - позволяет установить новую конфигурацию приложения
3. Кнопка "SET REQUEST" - очищает окно для нового поискового запроса 
4. Кнопка "SEND REQUEST" - сохраняет поисковый запрос
5. Кнопка "ANSWER" - выводит результатов всех ранее сохраненных поисковых запросов в 
рабочее окно
6. Окно состояния - показывет текущее состояние проекта, выводит сообщения об ошибках
7. Рабочее окно - окно ввода и вывода. 
Используется для запросов, конфигурации, результатов поиска.

Описание работы приложения.

Начало использования:

Перед началом запуска, разместите целевые файлы поиска в папке resources, в корне проекта.
При первом запуске приложения, в окне состояния появится предложение установить конфигурацию приложения.
Шаблон конфигурации будет выведен в рабочее окно. Необходимо заполнить его, 
соблюдая формат, и нажать кнопку "SET CONFIG".

Пример конфигурации:
name: MyProject              - название проекта. 
version: 0.1                 - версия
max_responses: 5             - максимальное кол-во релевантных файлов в результатах поиска
files:                       - список файлов, по которым происходит поиск
file002.txt                  - названия файлов
file003.txt
file004.txt
file005.txt
file006.txt

Не страшно, если вы сделали ошибку. В это случае в Окне состояния появится сообщение, 
какую именно, а в Рабочее окно снова будет выведен шаблон. 
Нужно заполнить его еще раз без ошибок, и опять нажать кнопку "SET CONFIG".


Изменение конфигурации:

Если вы хотите увидеть текущую конфигурацию, нажмите кнопку "GET CONFIG", 
и она будет выведена в Рабочее окно.

Если вы хотите изменить текущую конфигурацию, можете отредактировать
конфигурацию, выведенную на экран, соблюдая формат, и нажать "SET CONFIG".

Поисковый запрос, или группа запросов:

Чтобы создать поисковый запрос, нажмите "SET REQUEST", Рабочее окно будет очищено.
Введите в него запрос. Например:

The table golden finger

По готовности запроса, нажмите "SEND REQUEST".

Если хотите одновременно получить результаты по нескольким запросам, 
еще раз нажмите "SET REQUEST", введите, нажмите "SEND REQUEST".
Повторяйте столько раз, сколько нужно.

Получение результатов поиска:
Нажмите "ANSWER". Пример вывода результатов был выше.

Если хотите ввести новый поисковый запрос, 
нажмите "SET REQUEST", и далее по инструкции выше.


Работа с файлами JSON.
Если вам привычней работать с файлами JSON, есть и така возможность.
В корне проекта лежат файлы config.json, requests.json, answers.json,
в которых вы можете работать с информацией, соответствующей их названиям.
Единственное что, для получения ответов все равно придется использовать кнопку
"ANSWER" графического интерфейса.

ВНИМАНИЕ! при первом запуске программы этих файлов не будет
 в корневой папке, и их нужно создать. При это необходимо,
 чтобы названия и форматы файлов соответствовали.

Рекомендую для первого запуска все-таки воспользоваться графическим интерфейсом, 
он сделает файлы без ошибок, и дальше ими можно будет безопасно пользоваться.

Если Вы все таки сделали ошибку, приложение не будет работать.
Рекомендую в этом сулчае удалить файл config.json, 
и установить файл через графический интерфейс.

Приятного использования!
 
