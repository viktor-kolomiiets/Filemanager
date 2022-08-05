/*������ �������. ����������� ���������� �������� �������� � �������������� ��� (������,
������������ � ��� �����). �������� �������� ������ ����� ����� �����������:
 - ���������� ���������� ������;
 - ��������� �����/�����;
 - ������� �����/�����;
 - ��������������� �����/�����;
 - ����������/���������� �����/�����;
 - ��������� ������ �����/�����;
 - ����������� ����� �� ����� (� ������� �� ���������) � ��� �����*/

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

#include "filesystem.h"
#include "file.h"
#include "commands.h"
#include "app.h"

int wmain()
{
	int results;
	//������ ����� �������������� ������, ����� �������� ����� �������
	//���, ��� Windows ���������� utf-16 ��� ���� ������, ����������
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

	/*Input ui;
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
	}*/
	/*wchar_t vol_name[MAX_PATH + 1];
	wchar_t fs_name[MAX_PATH + 1];
	vol_name[MAX_PATH] = L'\0';
	fs_name[MAX_PATH] = L'\0';

	bool gvi = GetVolumeInformation(L"D:\\",
		vol_name, MAX_PATH,
		NULL, NULL, NULL,
		fs_name, MAX_PATH);

	wcout << vol_name << L"\n" << fs_name << L"\n";
	wcout << gvi << L"\n";

	path p{ L"D:\\" };
	wcout << p.has_root_directory() << L" has root\n";
	wcout << p.root_directory() << L" root dir\n";
	wcout << p.root_name() << L" root name\n";
	wcout << p.root_path() << L" root path\n";
	wcout << p.stem() << L" stem\n";*/

	Filesystem fs;
	wcout << fs.getVolumeLabel(L"D:\\") << L"\n";
	wcout << fs.getFSName(L"D:\\") << L"\n";

	/*Path* f = new File{ L"D:\\Images\\img.png" };
	Path* ff = new Directory{ L"D:\\Images" };
	wcout << *f << L"\n";*/

	/*wcout << L"get key " << ui.getKey() << L"\n";
	wcout << L"get line " << ui.getLine() << L"\n";
	wcout << L"is key " << ui.isKey(NEXT_BTN) << L"\n";
	wcout << L"get num " << ui.getNum() << L"\n";*/

	return 0;
}
