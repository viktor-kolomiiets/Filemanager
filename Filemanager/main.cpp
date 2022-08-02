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

#include <Windows.h>
#include <filesystem>

using namespace std;
using namespace std::filesystem;

#define OPEN_BTN L'o'
#define NEW_FILE_BTN L'a'
#define NEW_FOLDER_BTN L'n'
#define DELETE_BTN L'x'
#define RENAME_BTN L'r'
#define COPY_BTN L'c'
#define MOVE_BTN L'm'
#define INFO_BTN L'i'
#define FIND_BTN L'f'
#define NEXT_BTN L'\t'
#define PREV_BTN L' '
#define PARTITION_BTN L'p'

#define FILE_CLR 14
#define DIRECTORY_CLR 2
#define DEFAULT_CLR 15

#include "file.h"
#include "commands.h"
#include "app.h"

int wmain()
{
	int results;
	//Задает режим преобразования файлов, чтобы включить режим Юникода
	//так, как Windows использует utf-16 для имен файлов, директорий
	results = _setmode(_fileno(stdout), _O_U16TEXT);
	results = _setmode(_fileno(stdin), _O_U16TEXT);
	results = _setmode(_fileno(stderr), _O_U16TEXT);

	//o - open
	//a - create file
	//n - create folder
	//x - delete
	//r - rename
	//c - copy
	//m - move
	//i - info
	//f - find
	//tab - next page
	//space - prev page
	//p - change partition 

	Input ui;
	FileList fl(L"D:\\Documents");

	while (true)
	{
		system("cls");
		fl.print();
		
		wchar_t btn = ui.getKey();

		switch (btn)
		{
		case NEXT_BTN:
			fl.nextPage();
			break;
		case PREV_BTN:
			fl.prevPage();
			break;
		default:
			break;
		}
	}

	/*Path* f = new File{ L"D:\\Images\\img.png" };
	Path* ff = new Directory{ L"D:\\Images" };
	wcout << *f << L"\n";*/

	/*wcout << L"get key " << ui.getKey() << L"\n";
	wcout << L"get line " << ui.getLine() << L"\n";
	wcout << L"is key " << ui.isKey(NEXT_BTN) << L"\n";
	wcout << L"get num " << ui.getNum() << L"\n";*/

	return 0;
}
