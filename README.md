# Qt_search_engine
Educational work 
(Учебный проект).
Простой и удобный поисковик с графическим интерфейсом Qt.

## Технологии    
- C++
-  Qt
-  JSON
-  Google Test
-  многопоточность. 

## Назначение:
Производит поиск точного соответствия набору слов в __Поисковом
 запросе__, в файлах формата _txt_, расположенных в поисковой 
директории приложения.

## Описание
Соответствующая директория размещена в корневой папке проекта.
Для каждого целевого файла, приложение рассчитывает его 
релевантность __Поисковому запросу__, и выводит список файлов
в порядке убывания релевантности. 

__Пример расчета релевантности :__<br/><br/>
_(псевдо-код)_<br/>
_MAX_ = максимальное число слов соответствующх запросу, 
содержащихся в файле, из всех файлов в __Поисковой директории__.<br/>
_CUR_ = число слов, соответствующх запросу, 
в каждом отдельном файле.<br/>
_rank_ - релевантность файла запросу<br/>

_rank = MAX/CUR_;

__Пример поиска:__<br/><br/>
_(псевдо-код)_<br/>
_file_1 = {"the table"};<br/>
file_2 = {"the window};<br/>
file_3 = {"the the table"};_<br/>

_request_1 = "the";<br/>
request_2 = "the table";_<br/>

(код поисковика - упрощенный __JSON__, без некоторых элементов синтаксиса)<br/><br/>
_request001 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;// номер запроса<br/>
result: true &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; // есть файлы соответствующие запросу на 100%<br/>
docid: 3, rank: 1 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; // файл №3 - 2 слова (MAX = 2; rank = 2/2)<br/>
docid: 2, rank: 0.5 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;  &nbsp; // файл №2 - 1 слово (rank = 1/MAX = 0,5)<br/>
docid: 1, rank: 0.5 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; // файл №1 - 1 слово (rank = 1/MAX = 0,5)_<br/>

_request002<br/>
result: true<br/>
docid: 3, rank: 1<br/>
docid: 1, rank: 0.66_<br/>

## Интерфейс
![](https://github.com/ShevlokovM/Qt_search_engine/blob/main/%D0%BF%D0%BE%D0%B8%D1%81%D0%BA%D0%BE%D0%B2%D0%B8%D0%BA.jpg)

1. Кнопка __GET CONFIG__ - позволяет посмотреть текущую конфигуацию приложения
2. Кнопка __SET CONFIG__ - позволяет установить новую конфигурацию приложения
3. Кнопка __SET REQUEST__ - очищает окно для нового поискового запроса 
4. Кнопка __SEND REQUEST__ - сохраняет поисковый запрос
5. Кнопка __ANSWER__ - выводит результатов всех ранее сохраненных поисковых запросов в 
рабочее окно
6. Окно состояния - показывет текущее состояние проекта, выводит сообщения об ошибках
7. Рабочее окно - окно ввода и вывода. 
Используется для запросов, конфигурации, результатов поиска.

## Описание работы приложения

### Начало использования

Перед началом запуска, разместите целевые файлы поиска в папке ___resources___, в корне проекта.
При первом запуске приложения, в окне состояния появится предложение установить конфигурацию приложения.
Шаблон конфигурации будет выведен в рабочее окно. Необходимо заполнить его, 
соблюдая формат, и нажать кнопку __SET CONFIG__.

### Пример конфигурации
_name: MyProject_ &nbsp; &nbsp; &nbsp; &nbsp; // название проекта. <br/>
_version: 0.1_ &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; // версия<br/>
_max_responses: 5_ &nbsp; &nbsp; &nbsp;  // максимальное кол-во релевантных файлов в результатах поиска<br/>
_files:_ &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; // список файлов, по которым происходит поиск<br/>
_file002.txt_ &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; // названия файлов<br/>
_file003.txt<br/>
file004.txt<br/>
file005.txt<br/>
file006.txt_

Не страшно, если вы сделали ошибку. В это случае в Окне состояния появится сообщение, 
какую именно, а в Рабочее окно снова будет выведен шаблон. 
Нужно заполнить его еще раз без ошибок, и опять нажать кнопку __SET CONFIG__.


### Изменение конфигурации

- Если вы хотите увидеть текущую конфигурацию, нажмите кнопку __GET CONFIG__, 
и она будет выведена в Рабочее окно.
- Если вы хотите изменить текущую конфигурацию, можете отредактировать
конфигурацию, выведенную на экран, соблюдая формат, и нажать __SET CONFIG__.

### Поисковый запрос, или группа запросов

- Чтобы создать поисковый запрос, нажмите __SET REQUEST__, Рабочее окно будет очищено.
Введите в него запрос. Например:

   _The table golden finger_

   По готовности запроса, нажмите __SEND REQUEST__.

- Если хотите одновременно получить результаты по нескольким __Поисковым запросам__, 
еще раз нажмите __SET REQUEST__, введите, нажмите __SEND REQUEST__.
   Повторяйте столько раз, сколько нужно.

### Получение результатов поиска

- Нажмите __ANSWER__. Пример вывода результатов был выше.
- Если хотите ввести новый __Поисковый запрос__, 
нажмите __SET REQUEST__, и далее по инструкции выше.


## Работа с файлами JSON.
Если вам привычней работать с файлами __JSON__, есть и такая возможность.
В корне проекта лежат файлы __config.json, requests.json, answers.json__,
в которых вы можете работать с информацией, соответствующей их названиям.
Единственное что, для получения ответов все равно придется использовать кнопку
__ANSWER__ графического интерфейса.

__ВНИМАНИЕ!__ при первом запуске программы этих файлов не будет
 в корневой папке, и их нужно создать. При этом необходимо,
 чтобы названия и форматы файлов соответствовали.

Рекомендую для первого запуска все-таки воспользоваться графическим интерфейсом, 
он сделает файлы без ошибок, и дальше ими можно будет безопасно пользоваться.

Если Вы все таки сделали ошибку, приложение не будет работать.
Рекомендую в этом случае удалить файл __config.json__, 
и установить файл через графический интерфейс.

# Приятного использования!
 
