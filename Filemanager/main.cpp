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

#include "file.h"

int wmain()
{
	int results;
	//������ ����� �������������� ������, ����� �������� ����� �������
	//���, ��� Windows ���������� utf-16 ��� ���� ������, ����������
	results = _setmode(_fileno(stdout), _O_U16TEXT);
	results = _setmode(_fileno(stdin), _O_U16TEXT);
	results = _setmode(_fileno(stderr), _O_U16TEXT);

	Root root;
	vector<Path*>* list = root.open();
	size_t s = list->size();
	for (size_t i = 0; i < s; i++)
	{
		wcout << list->at(i)->getName() << L"\n";
	}

	return 0;
}
