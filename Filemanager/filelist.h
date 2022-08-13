#pragma once

#include <iostream>
#include "file.h"

class FileList
{
private:
	vector<Path*>* files;
	Path* current;
	size_t page;
	const size_t PAGE_SIZE{ 20ull };

public:
	explicit FileList() :
		files{ nullptr },
		current{ nullptr },
		page{ 1ull }
	{ /*openRoot();*/ }

	FileList& operator=(vector<Path*>*&& f);
	Path* operator[](size_t index) const { return files->at(index); }
	Path*& operator[](size_t index) { return files->at(index); }

	void openRoot();
	void openPath(wstring);
	void openItem(size_t no);
	void openParent();

	void print() const;
	void nextPage();
	void prevPage();

	size_t getSize() const;
	wstring getCurrentPathStr() const;

	void clear();

	~FileList();

private:
	size_t getMaxPage() const;
	void clearCurrent();
	void setCurrent(wstring pathP);
};
