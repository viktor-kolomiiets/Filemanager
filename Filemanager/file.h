#pragma once

class Path
{
protected:
	path fPath;
	
public:
	explicit Path(wstring fPathP) : fPath{ fPathP } {}
	explicit Path() : Path{ L"" } {}

	virtual wstring getName() const { return fPath.filename(); }
	virtual wstring getPath() const { return fPath.wstring(); }
	virtual wstring getParent() const { return fPath.parent_path(); }
	virtual wstring getSize() const;
	virtual uintmax_t getSizeByte() const = 0;
	virtual bool isDir() const = 0;
	virtual bool isFile() const = 0;
	virtual vector<Path*>* open() = 0;
};

inline wstring Path::getSize() const
{
	wstring vals[]{ L" bytes", L" KiB", L" MiB", L" GiB" };
	uintmax_t fsize = getSizeByte();

	int i = 0;
	for (; i < 4; i++)
	{
		if (fsize < 1024ull)
			break;
		fsize /= 1024ull;
	}

	return wstring(to_wstring(fsize) + vals[i]);
}

class Root : public Path
{
public:

};

class Partition : public Path
{
private:
	wstring label;

public:
	explicit Partition(wstring fPathP, wstring labelP) : Path{ fPathP }, label{ labelP } {}
	explicit Partition(wstring fPathP) : Partition{ fPathP, L"Local Disk" } {}
	explicit Partition() : Partition{ L"", L"Local Disk"} {}

	wstring getName() const override;
	//wstring getPath() const override { return L""; }
	wstring getParent() const override { return L""; }
	wstring getSize() const override { return L""; }
	uintmax_t getSizeByte() const override { return 0ull; }
	bool isDir() const override { return false; }
	bool isFile() const override { return false; }
};

wstring Partition::getName() const
{
	return L"Local Disk";
}

class Directory : public Path
{
public:
	explicit Directory(wstring fPathP) : Path{ fPathP } {}
	explicit Directory() : Directory{ L"" } {}

	uintmax_t getSizeByte() const override;
	bool isDir() const override { return true; }
	bool isFile() const override { return false; }
};

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

class File : public Path
{
public:
	explicit File(wstring fPathP) : Path{ fPathP } {}
	explicit File() : File{ L"" } {}
	
	uintmax_t getSizeByte() const override { return file_size(this->fPath); }
	bool isDir() const override { return false; }
	bool isFile() const override { return true; }
};
