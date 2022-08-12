#pragma once

#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <vector>

using namespace std;
using namespace std::filesystem;

class Filesystem
{
public:
	bool isExist(wstring pathP) const { return exists(path{ pathP }); }
	bool isDir(wstring) const;

	wstring getExtension(wstring) const;
	wstring getParent(wstring) const;
	wstring getFilename(wstring) const;

	uintmax_t getFileSize(wstring) const;
	uintmax_t getDirSize(wstring) const;
	uintmax_t getVolumeUsed(wstring) const;

	wstring getVolumeLabel(wstring) const;
	wstring getFSName(wstring) const;
	wstring getUserName() const;
	wstring getCompName() const;

	void executeFile(wstring) const;

	bool createFile(wstring, wstring) const;
	bool createDir(wstring, wstring) const;
	bool copyFile(wstring, wstring) const;
	bool renameFile(wstring, wstring) const;
	bool deleteFile(wstring) const;
	bool moveFile(wstring, wstring) const;

	vector<wstring> findFiles(wstring, wstring) const;

	vector<wstring> getAllFiles(wstring) const;
	vector<wstring> getDirs(wstring) const;
	vector<wstring> getPartitions() const;
};
