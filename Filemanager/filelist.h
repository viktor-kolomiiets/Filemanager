#pragma once

//------------------------------FILE_LIST----------------------------------------------------------

class FileList
{
private:
	vector<Path*>* files;
	Path* currentPath;
	size_t page;
	const size_t PAGE_SIZE{ 20u };

public:
	explicit FileList() :
		files{ nullptr },
		page{ 1ull },
		currentPath{ nullptr }
	{ openRoot(); }

	explicit FileList(wstring pathP) :
		files{ nullptr },
		page{ 1ull }
	{ openDir(Directory{ pathP }); }

	Path* operator[](size_t index) const { return files->at(index); }
	Path*& operator[](size_t index) { return files->at(index); }

	void openRoot();
	void open(size_t);
	void openDir(Directory);

	void printAll() const;
	void print() const;
	void nextPage();
	void prevPage();

	wstring getCurrentPathStr() const;
	size_t getSize() const;

	void clear();
	~FileList();

private:
	size_t getMaxPage() const;
};

inline void FileList::openRoot()
{
	page = 1u;
	this->clear();

	files = new vector<Path*>;
	vector<wstring> parts = Filesystem{}.getPartitions();
	for (size_t i{ 0 }; i < parts.size(); i++)
		files->push_back(new Partition{ parts.at(i) });

	currentPath = new Root;
}

inline void FileList::open(size_t no)
{
	if (files->at(no)->isFile())
	{
		Filesystem{}.executeFile(files->at(no)->getPath());
		return;
	}

	if (currentPath)
	{
		delete currentPath;
		currentPath = nullptr;
	}
	currentPath = new Directory{ files->at(no)->getPath() };
	vector<wstring> fls = Filesystem{}.getAllFiles(files->at(no)->getPath());

	page = 1u;
	this->clear();

	files = new vector<Path*>;
	for (size_t i{ 0 }; i < fls.size(); i++)
	{
		if (Filesystem{}.isDir(fls.at(i)))
			files->push_back(new Directory{ fls.at(i) });
		else
			files->push_back(new File{ fls.at(i) });
	}
}

inline void FileList::openDir(Directory dir)
{
	page = 1u;
	this->clear();

	files = new vector<Path*>;

	for (directory_iterator next(dir.getPath(), directory_options::skip_permission_denied), end; next != end; ++next)
	{
		try
		{
			path file = next->path();
			bool isDir = Filesystem{}.isDir(file.wstring());
			if (isDir)
				files->push_back(new Directory{ file.wstring() });
			else
				files->push_back(new File{ file.wstring() });
		}
		catch (filesystem_error&)
		{
			continue;
		}
	}
}

inline void FileList::printAll() const
{
	size_t s = files->size();
	for (size_t i{ 0 }; i < s; i++)
		wcout << i << L"\t" << files->at(i)->getName() << L"\n";
}

//inline void FileList::print() const
//{
//	//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//	size_t lsize = files->size();
//	size_t min = (page - 1u) * PAGE_SIZE;
//
//	for (size_t i{ 0 }; i < PAGE_SIZE; i++)
//	{
//		size_t it = i + min;
//		if (it >= lsize)
//		{
//			wcout << L"\n";
//			continue;
//		}
//		//SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));	//set color
//		//wstring spacer{ L"-------" };
//		//int ccode = DIRECTORY_CLR;
//
//		//if (files->at(it)->isFile())
//		//{
//		//	ccode = FILE_CLR;
//		//	spacer = files->at(it)->getSize();
//		//}
//
//		//SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | ccode));	//set color
//		//wcout << L" " << it << L"\t" << spacer;
//		//wcout.width(4);
//		//
//		//wcout << L"\t" << files->at(it)->getName();
//
//		wcout << L" " << it << L"\t" << *files->at(it);
//		wcout << L"\n";
//	}
//
//	//SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | DEFAULT_CLR));	//set white color
//}

inline void FileList::print() const
{
	size_t lsize = files->size();
	size_t min = (page - 1u) * PAGE_SIZE;

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

	wcout << L"\n\t\t\t(space) << ";
	wcout << page << L"/" << getMaxPage();
	wcout << L" >> (tab)\n";
}

inline void FileList::nextPage()
{
	if (page < getMaxPage())
		page++;
}

inline void FileList::prevPage()
{
	if (page > 1)
		page--;
}

inline wstring FileList::getCurrentPathStr() const
{
	return currentPath->getPath();
}

inline size_t FileList::getSize() const
{
	return this->files->size();
}

inline size_t FileList::getMaxPage() const
{
	size_t maxPage{ 1u };

	size_t s = this->files->size();
	size_t fullSize = s + (PAGE_SIZE - (s % PAGE_SIZE));
	maxPage = fullSize / PAGE_SIZE;

	return maxPage;
}

inline void FileList::clear()
{
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
