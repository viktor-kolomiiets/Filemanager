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
	virtual wstring getSize() const;
	virtual uintmax_t getSizeByte() const = 0;
	virtual bool isDir() const { return false; }
	virtual bool isFile() const { return false; }
	virtual vector<Path*>* open() = 0;
	virtual void openFile(vector<Path*>& list) = 0;

protected:
	virtual wostream& print(wostream& out) const;
};

inline wstring Path::getSize() const
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

	wstring getName() const override { return L""; }
	wstring getPath() const override { return L""; }
	wstring getParent() const override { return L""; }
	wstring getSize() const override { return L""; }
	uintmax_t getSizeByte() const override { return 0ull; }

	vector<Path*>* open() override;
	void openFile(vector<Path*>& list) override;

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

	vector<Path*>* open() override;
	void openFile(vector<Path*>& list) override;

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

	vector<Path*>* open() override;
	void openFile(vector<Path*>& list) override;

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

	vector<Path*>* open() override;
	void openFile(vector<Path*>& list) override;

private:
	wostream& print(wostream& out) const override;
};

//------------------------------ROOT---------------------------------------------------------------

inline vector<Path*>* Root::open()
{
	vector<Path*>* buffer = new vector<Path*>;

	int n;
	// ������� ���������� ������� �����
	DWORD drives = GetLogicalDrives();

	// ��������� �� �����
	for (int x = 0; x < 26; x++)
	{
		// ���������� �������� �������� ����
		n = ((drives >> x) & 1);

		// ���� ������� - ���� � ������� x ����
		if (n)
		{
			wstring partitionPath{ L"" };

			partitionPath += (wchar_t)(65 + x);
			partitionPath += L":\\";
			buffer->push_back(new Partition{ partitionPath });
		}
	}

	return buffer;
}

inline void Root::openFile(vector<Path*>& list)
{
	list.clear();

	int n;
	// ������� ���������� ������� �����
	DWORD drives = GetLogicalDrives();

	// ��������� �� �����
	for (int x = 0; x < 26; x++)
	{
		// ���������� �������� �������� ����
		n = ((drives >> x) & 1);

		// ���� ������� - ���� � ������� x ����
		if (n)
		{
			wstring partitionPath;

			partitionPath += (wchar_t)(65 + x);
			partitionPath += L":\\";
			list.push_back(new Partition{ partitionPath });
		}
	}
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
	return fs.getVolumeLabel(fPath);
}

inline vector<Path*>* Partition::open()
{
	vector<Path*> buffer;
	buffer.push_back(new Root);

	return &buffer;
}

inline void Partition::openFile(vector<Path*>& list)
{

}

inline uintmax_t Partition::getSizeByte() const
{
	return Filesystem{}.getVolumeUsed(fPath);
}

inline wostream& Partition::print(wostream& out) const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | DIRECTORY_CLR));
	out << this->getSize();
	out.width(4);
	out << L"\t[" << this->getName() << L" (" << this->getParent() << L")]";
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

inline vector<Path*>* Directory::open()
{
	vector<Path*>* buffer = new vector<Path*>;
	buffer->push_back(new Directory{ this->fPath });

	for (directory_iterator next(fPath, directory_options::skip_permission_denied), end; next != end; ++next)
	{
		try
		{
			path file = next->path();
			if (is_directory(file))
				buffer->push_back(new Directory{ file.wstring() });
			else
				buffer->push_back(new File{ file.wstring() });
		}
		catch (filesystem_error&)
		{
			continue;
		}
	}

	return buffer;
}

inline void Directory::openFile(vector<Path*>& list)
{

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

vector<Path*>* File::open()
{
	ShellExecute(NULL, NULL, this->getPath().c_str(), NULL, NULL, SW_RESTORE);
	return nullptr;
}

inline void File::openFile(vector<Path*>& list)
{
	//ShellExecute(NULL, NULL, this->getPath().c_str(), NULL, NULL, SW_RESTORE);
	Filesystem{}.openFile(this->getPath());
}

inline wostream& File::print(wostream& out) const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | FILE_CLR));
	out << this->getSize();
	out.width(4);
	out << L"\t" << this->getName();
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | DEFAULT_CLR));
	return out;
}
