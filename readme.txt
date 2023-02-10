Task - multiply an elliptic curve point by a scalar, using webasm, C++(или Rust).
Проект собрали и описали - Астровская и Комкова. 3к8гр1п

Умножение точки эллиптической кривой на скаляр.
Проект состоит из:
1) проект на с++, обеспечивающий умножение точки на скаляр при передаваемых ему параметрах - Ellyptic_Curve_mult.
2) javascript файл со скриптом, обрабатывающим нажатия кнопок, вставку данных в форму и вызов функции из формы - custom_script.js.
3)EC.html - страница задания, на которой выводится форма и кнопки, а также загружает зависимости модуля wasm.
4)EC.js - js файл автоматически генерируется при компиляции с++ в wasm, содержит инициализацию модуля от wasm.
5)EC.wasm - wasm файл автоматически генерируется при компиляции с++ в wasm .
6)serv.js - сервер на node.js, для запуска страницы html и избежания блокировок запросов cors.

Процесс создания проекта:
Был написан код на с++. Использовался этот ресурс- https://eax.me/elliptic-curves-crypto/
В коде была определена функция, которая будет скомпилирована в WASM , это функция mult. Она принимает и возвращает необходимые параметры. Wasm по умолчанию компилирует функцию с названием main, для того чтобы он скомпилировал созданную, функция опеределяется специальным параметром:
#define EXTERNC extern "C"
EXTERNC char* mult(char* a, char* b, char* p, char* pointX, char* pointY, char* multk)

Далее происходит создание custom_script и EC.html, которые принимают и передают нужные для взаимодейтсвия с функцией параметры.

Для компиляции WASM использовалась утилита emscripten. Ресурс по ее установке - https://www.wasm.builders/jennifer/using-c-in-a-web-app-with-webassembly-17ad

Команда в emscripten, которая была использована(в папке проекта):
> emcc Ellyptic_Curve_mult/bignum.cpp Ellyptic_Curve_mult/Ellyptic_Curve_mult.cpp -O3 -s WASM=1 -s EXPORTED_FUNCTIONS="['_mult']" -s EXPORTED_RUNTIME_METHODS="['ccall','cwrap']" -o EC.js -s

(указаны два пути к cpp файлам, которые необходимо скомпилировать, -s WASM=1 — Указывает, что мы хотим получить wasm модуль, Exported_functions указывает какую функцию мы компилируем, методы необходимы для использования в js файле, -o EC.js -s означает что мы хотим получить wasm и js файлы в результате компиляции) 

После компиляции необходимо запустить сервер с помощью( в папке проекта):
> node serv.js

И перейти по указанному адресу в коммандной строке(http://127.0.0.1:3000/).

Стандартные параметры эллиптической кривой и точки со скаляром:
 char a[] = "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC";
 char b[] = "5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b";
 char p[] = "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF";
 char pointX[] = "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC";
 char pointY[] = "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC";
 char multk[] = "70B99F5ADCFDBEF81058967018BA95736AA2B60060EC25FFFFFFFFFFFFFFFFFF";

Удобный онлайн сервис с визуализацией, который делает то же самое, лучше(в десятичной системе,и возможно ему будет плохо от очень больших чисел) -
https://andrea.corbellini.name/ecc/interactive/modk-mul.html

Перевод из десятичной в hex- 
https://www.rapidtables.com/convert/number/hex-to-decimal.html
