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

	return 0;
}
