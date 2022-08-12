#include "filelist.h"

void FileList::openRoot()
{
	this->clear();

	files = new vector<Path*>;
	vector<wstring> parts = Filesystem{}.getPartitions();
	for (size_t i{ 0 }; i < parts.size(); i++)
		files->push_back(new Partition{ parts.at(i) });

	//currentPath = new Root;
}

void FileList::openPath(wstring pathP)
{
	if (!Filesystem{}.isDir(pathP))
	{
		Filesystem{}.executeFile(pathP);
		return;
	}

	this->clear();

	files = new vector<Path*>;
	vector<wstring> allfiles = Filesystem{}.getAllFiles(pathP);
	for (size_t i{ 0 }; i < allfiles.size(); i++)
	{
		wstring f = allfiles.at(i);
		if (Filesystem{}.isDir(f))
			files->push_back(new Directory{ f });
		else
			files->push_back(new File{ f });
	}
}

void FileList::print() const
{
	if (files)
	{
		size_t lsize = files->size();
		size_t min = (page - 1ull) * PAGE_SIZE;

		for (size_t i{ 0 }; i < PAGE_SIZE; i++)
		{
			size_t it = i + min;
			if (it >= lsize)
			{
				wcout << L"\n";
				continue;
			}

			wcout << L" " << it << L"\t" << *files->at(it);
			wcout << L"\n";
		}
	}

	wcout << L"\n\t\t\t(space) << ";
	wcout << page << L"/" << getMaxPage();
	wcout << L" >> (tab)\n";
}

void FileList::nextPage()
{
	if (page < getMaxPage())
		page++;
}

void FileList::prevPage()
{
	if (page > 1)
		page--;
}

size_t FileList::getSize() const
{
	return this->files->size();
}

void FileList::clear()
{
	page = 1ull;
	if (files)
	{
		delete files;
		files = nullptr;
	}
}

FileList::~FileList()
{
	this->clear();
}

size_t FileList::getMaxPage() const
{
	size_t maxPage{ 1ull };

	if (files)
	{
		size_t s = this->files->size();
		size_t fullSize = s + (PAGE_SIZE - (s % PAGE_SIZE));
		maxPage = fullSize / PAGE_SIZE;
	}

	return maxPage;
}
