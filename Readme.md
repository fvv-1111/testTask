
# Описание программы-клиент.

При создании клиентской программы применена стандартная архитектура Model-View-Controller.

View's классы(каталог ClientApp/MVS/Views) служат для отображение элементов GUI на экране. Они содержат методы для обновления элементов GUI новыми данными, а также они генерируют Qt-signals при изменении пользователем интерактивных элементов GUI - start/stop measure check-box и combobox для задания значения range для каналов.

Классы данных (каталог ClientApp/MVS/DataModel)- основной класс MVC::Model::Data содержит массив объектов данных для каждого канала. Для упрощения доступа к ннему он реализован как singleton. При изменении полей данных генерируются Qt::signals. 

Класс контроллера (каталог ClientApp/MVS/ViewController)- отслеживает signals генерируемые как view-классами, так и классами данных. В своих методах-обработчиков сигналов от view генерирует команды для передачи на сервер, а в обработчиках сигналов от данных с помощью методов view-классов обновляет содержимое контролов на экране.

## Классы команд программы-клиента.

Для классов-команд клиентской программы есть базовый интерфейс ICommand. Он предоставляет 2 метода:


    virtual std::string get_server_command_string() const = 0;
    virtual void process_responce(const std::string&) = 0;


Первый служит для генерации текста команды при отправке его на сервер, второй для обработки полученного от сервера responce.

Для создания команд предусмотрен класс factory - CommandFactory. Его статические методы создания команд возвращают: `std::unique_ptr<ClientCommands::ICommand>`

Взаимодействие клиента с сервером осуществляется в отдельном потоке и реализовано в классе ServerExecutor. Этот класс содержит потоко-безопасную очередь `Utils::QueueWithWaiting<std::unique_ptr<ClientCommands::ICommand>>`.
Новая команда создается классом-контроллером при обработке сигнала от view в main-thread приложения и помещается в данную очередь методом `ServerExecutor::execute`.
В специальном потоке ServerExecutor::m_thread команда считывается с очереди, с помощью метода `ICommand::get_server_command_string()` с нее берется текст и отправляется на сервер, после чего ожидается ответ с сервера. При получения ответа с сервера дальнейшая обработка команды продолжается в main-thread. Для этого создается event-объект ResponceEvent и вызывается функция:
`QApplication::postEvent(ClientApp::singleton(), pevent);`

> Примечание: ResponceEvent содержит `std::shared_ptr<ICommand>` вместо std::unique_ptr, т.к. у QEvent есть метод clone().

Далее в основном потоке в методе: `bool ClientApp::event(QEvent*e)override;`
командой из responce извлекаются данные и(или) код ошибки, команда передает эти новые данные классам модели данных, которые при этом генерируют сигналы. Сигналы "перехватываются" контроллером и он обновляет элементы GUI на экране.

## Обработка ошибок клиентом.

Ошибки делятся на 2 вида:
1) возникают при операциях передачи данных на/с сервера. Классом ServerExecutor при создании объекта ResponceEvent код ошибок помешается в специальное поле `std::error_code m_ec`. Затем в основном потоке в методе ClientApp::event() это поле анализируется и в случае не пустого значения генерируется Qt::signal, который перехватывается контроллером и он показывает соответствующий диалог с текстом ошибки. Эти ошибки я посчитал не восстанавливаемыми и после закрытия диалога клиентская программа закрываеется.
2) ошибки, которые сервер возвращает в responce. Они реализованы как std::error_code с новой категорией и специализированным методом std::make_error_code. Код ошибки помещается в данные(класс Model::ChannelData), при этом генерируется Qt::signal, обрабатывая который контроллер помещает текст ошибки(std::error_code::message()) в контрол(он находится на экране правее контрола "channel status").

Немного поясню об ошибках при задании range для канала. Во-первых, я посчитал, что изменять range после старта цикла измерений нельзя, поэтому сервер будет возвращать соответсвующий код ошибки. Во-вторых, каждый канал является общим разделяемым объектом для всех клиентов и наверное необходимо как-то менеджировать задание range для него. Я реализовал простейший вариант - первый клиент может задать range, после этого остальные(до завершения работы первого клиента) могут только считывать range (иначе они получают ошибку). Наверное, по хорошему тут надо бы продумать какой-то более "продвинутый" алгоритм (может приоритеты у клиентов или какой-то механизм их оповещения при изменении range кем-либо из них и т.д.). Но это требует дополнительного обсуждения.

## Описание реализации цикла считывания состояние и значения из каналов.

После отработки команды "start measure" запоминает номер канала в классе GetResultsProcess и по таймеру запускается в его коде создается команда "get_result" со всеми зарегистрированными в нем номерами каналов. Далее она как и все осталные команды она помещается в очередь команд ServerExecutor и стандартно обрабатывается. 

# Описание программы-сервер

Сервер реализован как многопоточное приложение. При старте приложеия в основном потоке стандартными функциями (socket, bind, listen) создается сокет, затем управление передается в цикл, в котором функцией select опрашивается созданный сокет на обнаружение новых соединений. Когда новое соединение будет найдено, то ее идентификатор, полученный функцией accept(), передается в класс Connections с помощью его метода: `void add_connection(SocketApp::SocketHandle&& h);`

Connections работает в другом потоке (поле класса Connections `std::thread m_listen_thread;`). Его основные данные:
1) `std::vector<std::shared_ptr<Connection>> m_connections;` массив обнаруженных соединений;
2) `std::vector<std::thread> m_reading_threads;` массив вспомогательных потоков, в которых происходит считывание/запись данный с соедениний;
3) `Utils::QueueWithWaiting<std::weak_ptr<Connection>> m_reading_data_queue;` очередь соединений для считывания данных
4) `fd_set m_fdset;` множество Ids соединений для "прослушивания" (копия его передается как параметр в функцию select)

Основная цель этого класса - это прослушивание соединений функцией select(), чтобы найти те из них, в который есть данные для чтения. 
При обнаружении такого соединения его Id удаляется из Connections::m_fdset и сам объект Connection помещается в очередь m_reading_data_queue. Взаимодействие потока вызвавшего ее метод push() с потоком-читателем данных и предоставление данных последнему инкапсулировано внутри класса очереди. Это взаимодействие реализуется с помощью std::condition_variable и mutex, которые являются внутренними объектами класса очереди. В качастве поток-читателей используются потоки из массива m_reading_threads.
Все это предоставляет возможность того, что чтение, обработка команды и запись responce выполняется в отдельном потоке в методе класса Connection::process_command() одновременно с выполнением других операций(обнаружение новых соединений, обнаружение данных для чтение, а также чтение и исполнение команд других соединений) и не тормозит их. 

После завершения процесcа обработки команды вызывается метод `void Connections::restore_listen_connection(int id);`, в котором восстанавливается Id соединения в Connections::m_fdset и соединение снова будет "прослушиваться" на готовность к чтению данных.
Поскольку `restore_listen_connection(int id)` вызывается в другом потоке, Connections::m_fdset защищен mutex.

## Описание процесса обработки команд.

В процессе обработки команд используется функциональность регулярных выражений (std::regex, std::regex_match, std::sregex_iterator). 
Базовым классом для классов-обработчиков команд является шаблонный класс 
`template<class Command, class RegexFactory> class BaseCommand;`

Параметры шаблона - 
 1) Command - класс обработчик команды(который наследуется от BaseCommand - здесь применяется паттерн CRTP);
 2) RegexFactory - класс для построения регулярных выражений используемый в BaseCommand.

Требования к `<Command>` классам-обработчикам команд - 
1) должен быть конструктор по-умолчанию (используется для построения объекта в Utils::CommandProcessor::Processor);
2) необходимо реализовать метод `void execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, Connection& conn_obj);`
Извлекает данные параметров команды из (std::smatch& match_obj) и выполняет необходимы для команду действия.

Пример - команда `set_range channel1:range0 channel2:range1 channel3:range2 channel4:range3`
Будет 4 вызова `execute_impl(...)` - первый - с `std::smatch& match_obj` по подстроке `channel1:range0`, из match_obj извлекается номер канала - 0 и диапазон - `ChannelRange::Range0`, значения диапазона устанавливается в ChannelsSet класс(он singleton); второй вызой для `channel2:range1` и т.д.


Классы для `<RegexFactory>` определены в файле Uitls/CommandProcessor/RegexFactory.hpp в `namespace Command`. Они должны предоставлять 2 статических метода:
1) `static const std::string& match_regex_string()`
2) `static const std::string& item_regex_string()`

Оба возвращают строку для построения регулярных выражений std::regex.
Также класс предоставляет методы, необходимые для извлечения данных параметров команды (например: из подстроки "channel1:range0" номера канала 1 и значения диапазона "enum ChannelRange::Range0") из std::smatch.

BaseCommand предоставляет 2 основный метода:
  - `bool is_mycommand(const std::string& com) const;`
  - `template<typename T> std::string execute(const std::string &str, T& obj);`

Первый использует std::regex построенный из строки `<RegexFactory>::match_regex_string()` и служит для поиска объекта-обработчика который соответсвует данной команде(std::string& com).

Второй строит std::sregex_iterator по строке `<RegexFactory>::item_regex_string()`, перебирает его в цикле и вызывает используя паттерн CRTP метод `<Command>::execute_impl` в следующем коде:

  `static_cast<Command*>(this)->execute_impl(*it, responce_stream, obj);`

Возвращает строку для responce.


Также BaseCommand предоставляет для класса-наследника "protected" методы для построения responce-строки.

Аналогично "серверным" обработчикам команд класс BaseCommand и его метод BaseCommand::execute используется также в классах-командах в клиентском приложении для обработки responce. Классы `<RegexFactory>` для клиентских классов команд помещаеются в Uitls/CommandProcessor/RegexFactory.hpp в `namespace Responce`.


Все классы-обработчики команд регистрируются как параметры шаблонного класса в ServerCommandProcessor/CommandProcessor.hpp:

`using Processor = Utils::CommandProcessor::Processor<GetResultCommand,
                                                     GetStatusCommand,
                                                     SetRangeCommand,
                                                     GetRangeCommand,
                                                     StartMeasureCommand,
                                                     StopMeasureCommand>;`

Класс Utils::CommandProcessor::Processor определен в Utils/CommandProcessor/CommandProcessor.hpp:
`template <class...Args> class Processor`

Внутри себя этот класс строит массив `std::variant<Args...>`("автоматически" - с помощью функциональности шаблонов с++).
В классе определен метод 
    template<typename T> std::string execute(const std::string& str, T& conn_obj);
В нем находится соответствующий объект-обработчик команды(для каждого объекта из внутреннего массива вызывается метод is_mycommand, берется тот объект, для которого этот метод вернет true) и для него вызывается метод execute(). 
Если объект обработчик не найден - возвращается responce с кодом ошибки "команда не найдена".



## Регистрация новой команды на сервере

1) в файле RegexFactory.hpp создается класс, который предоставляет 2 метода 
    - `static const std::string& match_regex_string()` - для построения std::regex, который используется в BaseCommand::is_mycommand
    - `static const std::string& item_regex_string()` - для построения std::regex, который используется в BaseCommand::execute()

    и дополнительные методы для получения данных параметров команды по объекту std::smatch(зависят от параметров команды)

2) создается новый класс-обработчик, в качестве базового класса исполльзуется BaseCommand<сам новый класс, новый класс из RegexFactory.hpp>, реализуется конструктора по-умолчанию и метод `void execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, Connection& conn_obj);`

3) новый класс-обработчик добавляется как параметр к шаблону в ServerCommandProcessor/CommandProcessor.hpp:
`namespace CommandProcessor
{

using Processor = Utils::CommandProcessor::Processor<GetResultCommand,
                                                     GetStatusCommand,
                                                     SetRangeCommand,
                                                     GetRangeCommand,
                                                     StartMeasureCommand,
                                                     StopMeasureCommand>;
}`

> Примечание: использование regex довольно затратно, поэтому на мой взгляд предпочтительнее использовать для команд и responce binary format: первый байт(или слово) - длина команды, затем байт код команды и далее - параметры.
Если есть требование использовать именно строки(не двоичные коды) - тогда можно было бы кодировать байты, например, в 16-ричной системе (квази-binary format).

# Сборка проекта.

1) Необходимо отредактировать в файле CMakePresets.json путь до компилятора:

```
            "name": "Debug",
            "displayName": "GCC 11.5.0 x86_64-apple-darwin22",
            "description": "Using compilers: C = /usr/local/bin/gcc-11, CXX = /usr/local/bin/g++-11",
            "binaryDir": "${sourceDir}/build/${presetName}",
            "cacheVariables": {
                "CMAKE_INSTALL_PREFIX": "${sourceDir}/install/${presetName}",
здесь  -->      "CMAKE_C_COMPILER": "/usr/local/bin/gcc-11",
и здесь -->     "CMAKE_CXX_COMPILER": "/usr/local/bin/g++-11",
                "CMAKE_BUILD_TYPE": "Debug"
            }

```

2) запустить команды
```
cmake --preset Debug
cmake --build --preset Debug --target install
```

В каталоге install/Debug/ появятся 2 исполняемых файла - Server и Client. Запускается вначале Server, потом Client(или несколько клиентов). Если запустить клиента перед сервером, он не сможет установить соединение с сокетом, появится диалог об ошибке и программа завершится.

Файл для сокета создается автоматически в tmp каталоге (путь к нему получаю с помощью std::filesystem::temp_directory_path()).
