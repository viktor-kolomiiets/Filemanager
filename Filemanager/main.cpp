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
#define BACK_BTN L'q'
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
constexpr auto DEFAULT_CLR = 15;

#include "filesystem.h"
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
	Filesystem fs;
	FileList fl(L"D:\\Documents");

	/*Input ui;
	FileList fl(L"D:\\Documents");

	while (true)
	{
		system("cls");
		fl.print();
		
		wchar_t btn = ui.getKey();
		if (btn == BACK_BTN)
			break;

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
	}*/

	/*Filesystem fs;
	wcout << fs.getVolumeLabel(L"C:\\") << L"\n";
	wcout << fs.getFSName(L"C:\\") << L"\n";
	wcout << Filesystem{}.getVolumeLabel(L"D:\\") << L"\n";
	wcout << fs.getCompName() << L"\\" << fs.getUserName() << L"\n";*/

	/*wstring str(97, (wchar_t)45);
	wcout << str << L"\n";*/

	/*FileList fl;
	Path* f = new File{ L"D:\\Images\\Trash\\Id3ELZModOA.jpg" };
	Path* ff = new Directory{ L"D:\\Images" };*/

	/*wcout << Partition{ L"C:\\" } << L"\n";
	wcout << Partition{ L"D:\\" } << L"\n";
	wcout << Partition{ L"E:\\" } << L"\n";*/

	vector<wstring> fls = *fs.getAllFiles(L"D:\\Images");
	vector<wstring> ds = fs.getDirs(L"D:\\Images");
	vector<wstring> prs = fs.getPartitions();

	for (size_t i{ 0 }; i < fls.size(); i++)
		wcout << fls.at(i) << L"\n";

	for (size_t i{ 0 }; i < ds.size(); i++)
		wcout << ds.at(i) << L"\n";

	for (size_t i{ 0 }; i < prs.size(); i++)
		wcout << prs.at(i) << L"\n";

	return 0;
}
