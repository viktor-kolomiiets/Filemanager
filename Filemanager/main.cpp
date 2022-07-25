/*Первое задание. Реализовать простейший файловый менеджер с использованием ООП (классы,
наследование и так далее). Файловый менеджер должен иметь такие возможности:
 - показывать содержимое дисков;
 - создавать папки/файлы;
 - удалять папки/файлы;
 - переименовывать папки/файлы;
 - копировать/переносить папки/файлы;
 - вычислять размер папки/файла;
 - производить поиск по маске (с поиском по подпапкам) и так далее*/

#include <iostream>
#include <vector>
#include <fcntl.h>
#include <io.h>
#include <string>

#include <filesystem>

using namespace std;
using namespace std::filesystem;

#include "file.h"

int wmain()
{
	int results;
	//Задает режим преобразования файлов, чтобы включить режим Юникода
	//так, как Windows использует utf-16 для имен файлов, директорий
	results = _setmode(_fileno(stdout), _O_U16TEXT);
	results = _setmode(_fileno(stdin), _O_U16TEXT);
	results = _setmode(_fileno(stderr), _O_U16TEXT);

	return 0;
}
