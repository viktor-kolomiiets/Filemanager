#pragma once

#include <iostream>
#include "file.h"

class FileList
{
private:
	vector<Path*>* files;
	vector<Path*> history;
	wstring current;
	size_t page;
	const size_t PAGE_SIZE{ 20ull };

public:
	explicit FileList() :
		files{ nullptr },
		current{ L"" },
		page{ 1ull }
	{ }

	FileList& operator=(vector<Path*>*&& f);
	FileList& operator=(vector<Path*>*& f);
	Path* operator[](size_t index) const { return files->at(index); }
	Path*& operator[](size_t index) { return files->at(index); }

	void openRoot();
	void openItem(size_t no);
	void openParent();
	void updateList();

	void addLastDir(wstring pathP);

	void print() const;
	void nextPage();
	void prevPage();

	size_t getSize() const { return this->files->size(); }
	wstring getCurrentPathStr() const { return current; }

	void clear();

	~FileList() { this->clear(); }

private:
	size_t getMaxPage() const;
};
