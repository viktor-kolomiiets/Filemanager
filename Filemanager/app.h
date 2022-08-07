#pragma once

class Window
{
private:
	Directory dir;
	vector<Path*>* files;

public:
	explicit Window() : dir{ L"" }, files{ nullptr } {}

	~Window()
	{
		if (files)
		{
			delete files;
			files = nullptr;
		}
	}
};

//------------------------------INPUT--------------------------------------------------------------

class Input
{
public:
	Input() = default;
	bool isKey(wchar_t) const;
	wchar_t getKey() const;
	wstring getLine() const;
	size_t getNum() const;
};

inline bool Input::isKey(wchar_t k) const
{
	return k == this->getKey();
}

inline wchar_t Input::getKey() const
{
	return towlower(_getwch_nolock());
}

inline wstring Input::getLine() const
{
	wstring buffer{ L"" };
	getline(wcin, buffer, L'\n');
	return buffer;
}

inline size_t Input::getNum() const
{
	size_t buffer{ 0ull };
	wcin >> buffer;
	if (wcin.fail())
	{
		wcin.clear();
	}
	wcin.ignore(32767, L'\n');
	return buffer;
}

//------------------------------FILE_LIST----------------------------------------------------------

class FileList
{
private:
	vector<Path*>* files;
	size_t page;
	const size_t PAGE_SIZE{ 20u };

public:
	explicit FileList() : files{ nullptr }, page{ 1ull } {}
	explicit FileList(wstring pathP) : files{ nullptr }, page{ 1ull } { openPath(new Directory { pathP }); }

	Path* operator[](size_t index) const { return files->at(index); }
	Path*& operator[](size_t index) { return files->at(index); }

	void openPath(Path*);
	/*void open(const Path* p) { return; }
	void open(File*);
	void open(Directory*);*/
	void printAll() const;
	void print() const;
	void nextPage();
	void prevPage();
	void clear();

	~FileList();

private:
	size_t getMaxPage() const;
};

inline void FileList::openPath(Path* fpath)
{
	if (fpath->isFile())
	{
		ShellExecute(NULL, NULL, fpath->getPath().c_str(), NULL, NULL, SW_RESTORE);
		return;
	}

	page = 1u;
	this->clear();

	files = new vector<Path*>;
	files->push_back(new Directory{ fpath->getParent() });

	for (directory_iterator next(fpath->getPath(), directory_options::skip_permission_denied), end; next != end; ++next)
	{
		try
		{
			path file = next->path();
			if (is_directory(file))
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

//inline void FileList::open(File* file)
//{
//	ShellExecute(NULL, NULL, file->getPath().c_str(), NULL, NULL, SW_RESTORE);
//}
//
//inline void FileList::open(Directory* dir)
//{
//	page = 1u;
//	this->clear();
//
//	files = new vector<Path*>;
//	files->push_back(new Directory{ dir->getParent() });
//
//	for (directory_iterator next(dir->getPath(), directory_options::skip_permission_denied), end; next != end; ++next)
//	{
//		try
//		{
//			path file = next->path();
//			if (is_directory(file))
//				files->push_back(new Directory{ file.wstring() });
//			else
//				files->push_back(new File{ file.wstring() });
//		}
//		catch (filesystem_error&)
//		{
//			continue;
//		}
//	}
//}

inline void FileList::printAll() const
{
	size_t s = files->size();
	for (size_t i{ 0 }; i < s; i++)
		wcout << i << L"\t" << files->at(i)->getName() << L"\n";
}

inline void FileList::print() const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
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
		//SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));	//set color
		wstring spacer{ L"-------" };
		int ccode = DIRECTORY_CLR;

		if (files->at(it)->isFile())
		{
			ccode = FILE_CLR;
			spacer = files->at(it)->getSize();
		}

		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | ccode));	//set color
		wcout << L" " << it << L"\t" << spacer;
		wcout.width(4);
		
		wcout << L"\t" << files->at(it)->getName();
		wcout << L"\n";
	}

	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | DEFAULT_CLR));	//set white color
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