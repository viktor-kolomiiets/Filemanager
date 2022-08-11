#pragma once

class FileInfo
{
	wstring fname;
	wstring fpath;
	wstring fsize;

public:
	FileInfo() = default;

	void setInfoPath(wstring);
	void setInfoPart(wstring);
	void setInfoDir(wstring);
	void setInfoFile(wstring);

	void show() const;
};

inline void FileInfo::setInfoPath(wstring p)
{
	fname = Filesystem{}.getCompName();
	fpath = fname + L'\\' + Filesystem{}.getUserName();
	fsize = L"";
}

inline void FileInfo::setInfoPart(wstring p)
{
	fname = Filesystem{}.getVolumeLabel(p);
	fpath = p;
	fsize = L"";
}

inline void FileInfo::setInfoDir(wstring p)
{
	fname = Filesystem{}.getVolumeLabel(p);
	fpath = p;
	fsize = L"";
}

inline void FileInfo::setInfoFile(wstring p)
{
	fname = Filesystem{}.getFilename(p);
	fpath = p;
	fsize = L"";
}

inline void FileInfo::show() const
{
	
}

