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
#include <fcntl.h>
#include <io.h>

//#include "filesystem.h"
//#include "file.h"
//#include "filelist.h"
//#include "input.h"
#include "file_viewer.h"

int wmain()
{
	int results;
	//Задает режим преобразования файлов, чтобы включить режим Юникода
	//так, как Windows использует utf-16 для имен файлов, директорий
	results = _setmode(_fileno(stdout), _O_U16TEXT);
	results = _setmode(_fileno(stdin), _O_U16TEXT);
	results = _setmode(_fileno(stderr), _O_U16TEXT);

	//o - open
	//q - back
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

	//Window win;
	//win.menu();

	//filesystem.h
	wcout << Filesystem{}.getCompName() << L"\n";
	wcout << Filesystem{}.getUserName() << L"\n";
	wcout << Filesystem{}.getFSName(L"D:\\") << L"\n";

	//file.h
	wcout << File{ L"D:\\Documents\\p7INiq7Bhng.jpg" }.getPath() << L"\n";
	//File{ L"D:\\Documents\\p7INiq7Bhng.jpg" }.execute();

	//filelist.h
	FileList fl;
	fl.openRoot();
	fl.print();
	fl.openPath(L"D:\\Documents");
	fl.print();
	fl.nextPage();
	fl.print();

	//input.h
	wcout << Input{}.isValidName(L"name") << L"\n";
	wcout << Input{}.isValidRange(3ull, 0ull, 5ull) << L"\n";

	//file_viewer.h
	Window w;
	w.menu();


	return 0;
}
