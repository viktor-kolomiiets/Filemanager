#pragma once

#include <string>
#include "filesystem.h"

#define FILE_CLR 14
#define DIRECTORY_CLR 2
constexpr auto DEFAULT_CLR = 15;

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
	virtual wstring getParentName() const { return Filesystem{}.getParent(fPath); }

	virtual wstring getSizeStr() const;
	virtual uintmax_t getSizeByte() const = 0;

	virtual bool isDir() const { return false; }
	virtual bool isFile() const { return false; }

	virtual void execute() const { return; }

	virtual Path* getParent() const = 0;
	virtual vector<Path*>* open() const = 0;

protected:
	virtual wostream& print(wostream& out) const;
};

//------------------------------ROOT---------------------------------------------------------------

class Root : public Path
{
public:
	Root() : Path{ L"" } {}

	wstring getName() const override { return Filesystem{}.getCompName(); }
	wstring getPath() const override;
	wstring getParentName() const override { return L""; }
	wstring getSizeStr() const override { return L""; }
	uintmax_t getSizeByte() const override { return 0ull; }

	Path* getParent() const override { return new Root; }
	vector<Path*>* open() const override;

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
	wstring getParentName() const override { return Root{}.getPath(); }
	uintmax_t getSizeByte() const override { return Filesystem{}.getVolumeUsed(fPath); }
	Path* getParent() const override { return new Root; }
	vector<Path*>* open() const override;

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

	uintmax_t getSizeByte() const override { return Filesystem{}.getDirSize(fPath); }
	bool isDir() const override { return true; }

	Path* getParent() const override { return new Directory{ getParentName() }; }
	vector<Path*>* open() const override;

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

	Path* getParent() const override { return new Directory{ getParentName() }; }
	vector<Path*>* open() const override { return nullptr; }
	void execute() const override { Filesystem{}.executeFile(this->fPath); }

private:
	wostream& print(wostream& out) const override;
};
