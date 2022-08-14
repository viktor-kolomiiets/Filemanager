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
	wstring partChar(fPath.begin(), fPath.begin() + 2u);
	return wstring{ fs.getVolumeLabel(fPath) + L" (" + partChar + L')' };
}

wstring Partition::getParentName() const
{
	return Root{}.getPath();
}

uintmax_t Partition::getSizeByte() const
{
	return Filesystem{}.getVolumeUsed(fPath);
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
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | DIRECTORY_CLR));
	out << this->getSizeStr();
	out.width(4);
	out << L"\t[" << this->getName() << L"]";
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

uintmax_t Directory::getSizeByte() const
{
	return Filesystem{}.getDirSize(fPath);
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

vector<Path*>* File::open() const
{
	return nullptr;
}

void File::execute() const
{
	Filesystem{}.executeFile(this->fPath);
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
