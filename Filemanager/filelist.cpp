#include "filelist.h"

FileList& FileList::operator=(vector<Path*>*&& f)
{
	if (files == f)
		return *this;

	this->clear();
	files = f;
	f = nullptr;

	return *this;
}

FileList& FileList::operator=(vector<Path*>*& f)
{
	if (files == f)
		return *this;

	this->clear();
	files = f;
	f = nullptr;

	return *this;
}

void FileList::openRoot()
{
	*this = Root{}.open();
	history.push_back(new Root);
	setCurrent(Root{}.getPath());
}

void FileList::openItem(size_t no)
{
	if (files->at(no)->isFile())
	{
		files->at(no)->execute();
		return;
	}

	history.push_back(files->at(no));
	wstring c = files->at(no)->getPath();
	setCurrent(c);
	*this = this->files->at(no)->open();
}

void FileList::openParent()
{
	if (history.size() > 1ull)
	{
		history.pop_back();
		setCurrent(history.back()->getPath());
		*this = history.back()->open();
	}
}

void FileList::updateList()
{
	*this = history.back()->open();
}

void FileList::addLastDir(wstring pathP)
{
	history.push_back(new Directory{ pathP });
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

void FileList::clear()
{
	page = 1ull;
	if (files)
	{
		delete files;
		files = nullptr;
	}
}

void FileList::clearCurrent()
{
	if (current)
	{
		delete current;
		current = nullptr;
	}
}

void FileList::setCurrent(wstring pathP)
{
	this->clearCurrent();
	current = new Directory{ pathP };
}

FileList::~FileList()
{
	this->clear();
	this->clearCurrent();
}

size_t FileList::getMaxPage() const
{
	size_t maxPage{ 1ull };

	if (files)
	{
		size_t s = this->files->size() - 1ull;
		size_t fullSize = s + (PAGE_SIZE - (s % PAGE_SIZE));
		maxPage = fullSize / PAGE_SIZE;
	}

	return maxPage;
}
