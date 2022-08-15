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
	bool isDir(wstring pathP) const;

	wstring getExtension(wstring file) const;
	wstring getParent(wstring pathP) const;
	wstring getFilename(wstring pathP) const;

	uintmax_t getFileSize(wstring pathP) const;
	uintmax_t getDirSize(wstring pathP) const;
	uintmax_t getVolumeUsed(wstring pathP) const;

	wstring getVolumeLabel(wstring pathP) const;
	wstring getFSName(wstring pathP) const;
	wstring getUserName() const;
	wstring getCompName() const;

	void executeFile(wstring pathP) const;

	bool createFile(wstring nameP, wstring pathP) const;
	bool createDir(wstring nameP, wstring pathP) const;
	bool copyFile(wstring from, wstring to) const;
	bool renameFile(wstring old, wstring newName) const;
	bool deleteFile(wstring pathP) const;
	bool moveFile(wstring from, wstring to) const;

	vector<wstring> findFiles(wstring mask, wstring target) const;

	vector<wstring> getAllFiles(wstring pathP) const;
	vector<wstring> getDirs(wstring pathP) const;
	vector<wstring> getPartitions() const;
};
