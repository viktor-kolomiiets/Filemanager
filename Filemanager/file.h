#pragma once

class Path
{
protected:
	wstring fPath;

public:
	explicit Path(wstring fPathP) : fPath{ fPathP } {}
	explicit Path() : Path{ L"" } {}

	friend wostream& operator<<(wostream& out, const Path& pathP)
	{
		return pathP.print(out);
	}

	virtual wstring getName() const { return Filesystem{}.getFilename(fPath); }
	virtual wstring getPath() const { return fPath; }
	virtual wstring getParent() const { return Filesystem{}.getParent(fPath); }
	virtual wstring getSizeStr() const;
	virtual uintmax_t getSizeByte() const = 0;
	virtual bool isDir() const { return false; }
	virtual bool isFile() const { return false; }

protected:
	virtual wostream& print(wostream& out) const;
};

inline wstring Path::getSizeStr() const
{
	const short int SIZE = 5;
	wstring vals[SIZE]{ L" bytes", L" KiB", L" MiB", L" GiB", L" TiB" };
	uintmax_t fsize = getSizeByte();

	short int i = 0;
	for (; i < SIZE; i++)
	{
		if (fsize < 1024ull)
			break;
		fsize /= 1024ull;
	}

	return wstring(to_wstring(fsize) + vals[i]);
}

inline wostream& Path::print(wostream& out) const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | DEFAULT_CLR));
	return out;
}

//------------------------------ROOT---------------------------------------------------------------

class Root : public Path
{
public:
	Root() : Path{ L"" } {}

	wstring getName() const override { return Filesystem{}.getCompName(); }
	wstring getPath() const override;
	wstring getParent() const override { return L""; }
	wstring getSizeStr() const override { return L""; }
	uintmax_t getSizeByte() const override { return 0ull; }

private:
	wostream& print(wostream& out) const override;
};

//------------------------------PARTITION----------------------------------------------------------

class Partition : public Path
{
public:
	explicit Partition(wstring fPathP) : Path{ fPathP } {}
	explicit Partition() : Partition{ L"" } {}

	wstring getName() const override;
	uintmax_t getSizeByte() const override;

private:
	wostream& print(wostream& out) const override;
};

//------------------------------DIRECTORY----------------------------------------------------------

class Directory : public Path
{
public:
	explicit Directory(wstring fPathP) : Path{ fPathP } {}
	explicit Directory() : Directory{ L"" } {}

	Directory& operator=(const Directory& dir);
	Directory& operator=(const Partition& part);
	Directory& operator=(const Path& p);

	uintmax_t getSizeByte() const override;
	bool isDir() const override { return true; }

private:
	wostream& print(wostream& out) const override;
};

//------------------------------FILE---------------------------------------------------------------

class File : public Path
{
public:
	explicit File(wstring fPathP) : Path{ fPathP } {}
	explicit File() : File{ L"" } {}

	uintmax_t getSizeByte() const override { return Filesystem{}.getFileSize(fPath); }
	bool isFile() const override { return true; }

private:
	wostream& print(wostream& out) const override;
};

//------------------------------ROOT---------------------------------------------------------------

inline wstring Root::getPath() const
{
	wstring pc_name = this->getName();
	wstring pc_user = Filesystem{}.getUserName();
	return wstring{ pc_name + L'\\' + pc_user };
}

inline wostream& Root::print(wostream& out) const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | DEFAULT_CLR));
	return out;
}

//------------------------------PARTITION----------------------------------------------------------

wstring Partition::getName() const
{
	Filesystem fs;
	wstring partChar(this->getParent());
	return wstring{ fs.getVolumeLabel(fPath) + L" (" + partChar + L')' };
}

inline uintmax_t Partition::getSizeByte() const
{
	return Filesystem{}.getVolumeUsed(fPath);
}

inline wostream& Partition::print(wostream& out) const
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

inline Directory& Directory::operator=(const Directory& dir)
{
	this->fPath = dir.fPath;
	return *this;
}

inline Directory& Directory::operator=(const Partition& part)
{
	this->fPath = part.getPath();
	return *this;
}

inline Directory& Directory::operator=(const Path& p)
{
	this->fPath = p.getPath();
	return *this;
}

uintmax_t Directory::getSizeByte() const
{
	return Filesystem{}.getDirSize(fPath);
}

inline wostream& Directory::print(wostream& out) const
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

inline wostream& File::print(wostream& out) const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | FILE_CLR));
	out << this->getSizeStr();
	out.width(4);
	out << L"\t" << this->getName();
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | DEFAULT_CLR));
	return out;
}
