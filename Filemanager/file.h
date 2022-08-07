#pragma once

class Path
{
protected:
	path fPath;
	
public:
	explicit Path(wstring fPathP) : fPath{ fPathP } {}
	explicit Path() : Path{ L"" } {}

	friend wostream& operator<<(wostream& out, const Path& file)
	{
		return out << file.getName();
	}

	virtual wstring getName() const { return fPath.filename(); }
	virtual wstring getPath() const { return fPath.wstring(); }
	virtual wstring getParent() const { return fPath.parent_path(); }
	virtual wstring getSize() const;
	virtual uintmax_t getSizeByte() const = 0;
	virtual bool isDir() const { return false; }
	virtual bool isFile() const { return false; }
	virtual vector<Path*>* open() = 0;
	virtual void openFile(vector<Path*>& list) = 0;
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
};

//------------------------------PARTITION----------------------------------------------------------

class Partition : public Path
{
public:
	explicit Partition(wstring fPathP) : Path{ fPathP } {}
	explicit Partition() : Partition{ L"" } {}

	wstring getName() const override;
	wstring getParent() const override { return L""; }
	uintmax_t getSizeByte() const override;

	vector<Path*>* open() override;
	void openFile(vector<Path*>& list) override;
};

//------------------------------DIRECTORY----------------------------------------------------------

class Directory : public Path
{
public:
	explicit Directory(wstring fPathP) : Path{ fPathP } {}
	explicit Directory() : Directory{ L"" } {}

	uintmax_t getSizeByte() const override;
	bool isDir() const override { return true; }

	vector<Path*>* open() override;
	void openFile(vector<Path*>& list) override;
};

//------------------------------FILE---------------------------------------------------------------

class File : public Path
{
public:
	explicit File(wstring fPathP) : Path{ fPathP } {}
	explicit File() : File{ L"" } {}

	uintmax_t getSizeByte() const override { return file_size(this->fPath); }
	bool isFile() const override { return true; }

	vector<Path*>* open() override;
	void openFile(vector<Path*>& list) override;
};

//------------------------------ROOT---------------------------------------------------------------

inline vector<Path*>* Root::open()
{
	vector<Path*>* buffer = new vector<Path*>;

	int n;
	// функция возвращает битовую маску
	DWORD drives = GetLogicalDrives();

	// прогоняем по битам
	for (int x = 0; x < 26; x++)
	{
		// определяем значение текущего бита
		n = ((drives >> x) & 1);

		// если единица - диск с номером x есть
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
	// функция возвращает битовую маску
	DWORD drives = GetLogicalDrives();

	// прогоняем по битам
	for (int x = 0; x < 26; x++)
	{
		// определяем значение текущего бита
		n = ((drives >> x) & 1);

		// если единица - диск с номером x есть
		if (n)
		{
			wstring partitionPath;

			partitionPath += (wchar_t)(65 + x);
			partitionPath += L":\\";
			list.push_back(new Partition{ partitionPath });
		}
	}
}

//------------------------------PARTITION----------------------------------------------------------

wstring Partition::getName() const
{
	Filesystem fs;
	return fs.getVolumeLabel(fPath.wstring());
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
	ULARGE_INTEGER freeAv{ 0 }, total{ 0 };
	bool f = GetDiskFreeSpaceEx(this->getPath().c_str(), &freeAv, &total, NULL);
	return static_cast<uintmax_t>(total.QuadPart - freeAv.QuadPart);
}

//------------------------------DIRECTORY----------------------------------------------------------

uintmax_t Directory::getSizeByte() const
{
	uintmax_t fsizeb = 0ull;

	try
	{
		for (recursive_directory_iterator next(this->fPath), end; next != end; ++next)
		{
			try
			{
				if (!next->is_directory())
					fsizeb += next->file_size();
			}
			catch (const filesystem_error&)
			{
				fsizeb += 0ull;
				continue;
			}
		}
	}
	catch (const exception&)
	{
		return fsizeb;
	}

	return fsizeb;
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
