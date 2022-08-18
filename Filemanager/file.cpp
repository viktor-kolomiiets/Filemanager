#include "file.h"

//------------------------------PATH---------------------------------------------------------------

wstring Path::getSizeStr() const
{
	const short int SIZE = 5;
	wstring vals[SIZE]{ L" bytes", L" KiB", L" MiB", L" GiB", L" TiB" };
	uintmax_t fsize = this->getSizeByte();

	short int i = 0;
	for (; i < SIZE; i++)
	{
		if (fsize < 1024ull)
			break;
		fsize /= 1024ull;
	}

	return wstring(to_wstring(fsize) + vals[i]);
}

wostream& Path::print(wostream& out) const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | DEFAULT_CLR));
	return out;
}

//------------------------------ROOT---------------------------------------------------------------

wstring Root::getPath() const
{
	wstring pc_name = this->getName();
	wstring pc_user = Filesystem{}.getUserName();
	return wstring{ pc_name + L'\\' + pc_user };
}

vector<Path*>* Root::open() const
{
	vector<Path*>* buffer = new vector<Path*>;
	vector<wstring> p = Filesystem{}.getPartitions();
	for (size_t i{ 0ull }; i < p.size(); i++)
		buffer->push_back(new Partition(p.at(i)));

	return buffer;
}

wostream& Root::print(wostream& out) const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | DEFAULT_CLR));
	return out;
}

//------------------------------PARTITION----------------------------------------------------------

wstring Partition::getName() const
{
	Filesystem fs;
	return wstring{ fs.getVolumeLabel(fPath) };
}

void Partition::displayInfo() const
{
	wcout << L"Partition Label: " << getName() << L"\n";
	wcout << L"Partition Root : " << getPath() << L"\n";
	wcout << L"Total Used     : " << getSizeStr() << L"\n";
	wcout << L"File System    : " << Filesystem{}.getFSName(fPath) << L"\n";
}

vector<Path*>* Partition::open() const
{
	vector<Path*>* buffer = new vector<Path*>;
	vector<wstring> fls = Filesystem{}.getAllFiles(fPath);
	for (size_t i{ 0ull }; i < fls.size(); i++)
	{
		wstring f = fls.at(i);
		if (Filesystem{}.isDir(f))
			buffer->push_back(new Directory{ f });
		else
			buffer->push_back(new File{ f });
	}

	return buffer;
}

wostream& Partition::print(wostream& out) const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	wstring partChar(fPath.begin(), fPath.begin() + 2u);

	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | DIRECTORY_CLR));
	out << this->getSizeStr();
	out.width(4);
	out << L"\t[" << this->getName() + L" (" + partChar + L')' << L"]";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | DEFAULT_CLR));

	return out;
}

//------------------------------DIRECTORY----------------------------------------------------------

Directory& Directory::operator=(const Directory& dir)
{
	this->fPath = dir.fPath;
	return *this;
}

Directory& Directory::operator=(const Partition& part)
{
	this->fPath = part.getPath();
	return *this;
}

Directory& Directory::operator=(const Path& p)
{
	this->fPath = p.getPath();
	return *this;
}

void Directory::displayInfo() const
{
	wcout << L"Directory Name: " << getName() << L"\n";
	wcout << L"Full Path     : " << getPath() << L"\n";
	wcout << L"Directory Size: " << getSizeStr() << L"\n";
}

vector<Path*>* Directory::open() const
{
	vector<Path*>* buffer = new vector<Path*>;
	vector<wstring> fls = Filesystem{}.getAllFiles(fPath);
	for (size_t i{ 0ull }; i < fls.size(); i++)
	{
		wstring f = fls.at(i);
		if (Filesystem{}.isDir(f))
			buffer->push_back(new Directory{ f });
		else
			buffer->push_back(new File{ f });
	}

	return buffer;
}

wostream& Directory::print(wostream& out) const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | DIRECTORY_CLR));
	out << L"-------";
	out.width(4);
	out << L"\t[" << this->getName() << L"]";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | DEFAULT_CLR));
	return out;
}

//------------------------------FILE---------------------------------------------------------------

void File::displayInfo() const
{
	wcout << L"File Name: " << getName() << L"\n";
	wcout << L"Full Path: " << getPath() << L"\n";
	wcout << L"File Size: " << getSizeStr() << L"\n";
}

wostream& File::print(wostream& out) const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | FILE_CLR));
	out << this->getSizeStr();
	out.width(4);
	out << L"\t" << this->getName();
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | DEFAULT_CLR));
	return out;
}
