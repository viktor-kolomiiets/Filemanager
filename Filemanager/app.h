#pragma once

//------------------------------INPUT--------------------------------------------------------------

class Input
{
public:
	Input() = default;

	bool isKey(wchar_t) const;
	wchar_t getKey() const;
	wstring getLine() const;
	size_t getNum() const;

	bool isValidRange(size_t, size_t, size_t) const;
	bool isValidName(wstring) const;
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

inline bool Input::isValidRange(size_t val, size_t min, size_t max) const
{
	if (val < min || val > max)
		return false;

	return true;
}

inline bool Input::isValidName(wstring str) const
{
	size_t pos = str.find_first_of(L"\\/:*?\'\"<>|");
	if (pos != wstring::npos)
		return true;

	return false;
}

//------------------------------FILE_LIST----------------------------------------------------------

class FileList
{
private:
	vector<Path*>* files;
	size_t page;
	const size_t PAGE_SIZE{ 20u };

public:
	explicit FileList() : files{ nullptr }, page{ 1ull } { openRoot(); }
	explicit FileList(wstring pathP) : files{ nullptr }, page{ 1ull } { openDir(Directory { pathP }); }

	Path* operator[](size_t index) const { return files->at(index); }
	Path*& operator[](size_t index) { return files->at(index); }

	void openRoot();
	void openDir(Directory);

	void printAll() const;
	void print() const;
	void nextPage();
	void prevPage();

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

//------------------------------WINDOW-------------------------------------------------------------

class Window
{
private:
	Directory current;
	vector<Directory*>* history{ nullptr };
	FileList fl;
	Input ui;

public:
	Window();

	void setCurrentDir(size_t);

	void menu();

	~Window();

private:
	void goLastDir();
	void openDir();
	void draw() const;

	size_t selectItem() const;
};

void Window::goLastDir()
{
	if (this->history->size() > 0)
	{
		current = *this->history->back();
		this->history->pop_back();
	}
	
	this->openDir();
}

inline Window::Window()
{
	history = new vector<Directory*>;
}

inline void Window::setCurrentDir(size_t no)
{
	if (fl[no]->isFile())
	{
		Filesystem{}.executeFile(fl[no]->getPath());
		return;
	}
	
	current = *fl[no];
	history->push_back(new Directory{ fl[no]->getParent() });
}

void Window::openDir()
{
	fl.openDir(current);
}

inline void Window::menu()
{
	//fl.openDir(Directory{ L"D:\\Documents" });
	for (;;)
	{
		system("cls");
		this->draw();

		wchar_t btn = ui.getKey();
		if (btn == L'0')
			break;

		switch (btn)
		{
		case OPEN_BTN:							//open file or folder
			this->setCurrentDir(selectItem());
			this->openDir();
			break;
		case BACK_BTN:							//back to previous folder
			this->goLastDir();
			break;
		case NEXT_BTN:							//display next page
			fl.nextPage();
			break;
		case PREV_BTN:							//display previous page
			fl.prevPage();
			break;
		case PARTITION_BTN:						//select partitions to open
			break;
		case NEW_FILE_BTN:						//create new file
			break;
		case NEW_FOLDER_BTN:					//create new folder
			break;
		case DELETE_BTN:						//delete file/folder
			break;
		case RENAME_BTN:						//rename file/folder
			break;
		case COPY_BTN:							//copy file/folder
			break;
		case MOVE_BTN:							//move to another directory file/folder
			break;
		case INFO_BTN:							//display information about foder
			break;
		case FIND_BTN:							//find file(s)
			break;
		default:
			break;
		}
	}
}

inline void Window::draw() const
{
	wstring dlmtr(97, L'-');

	wcout << dlmtr << L"\n";
	wcout << current.getPath() << L"\n";
	wcout << dlmtr << L"\n";

	fl.print();

	wcout << dlmtr << L"\n";
	wcout << "\n";
	wcout << dlmtr << L"\n";
}

inline size_t Window::selectItem() const
{
	size_t s;
	size_t maxS = fl.getSize() - 1;
	for (;;)
	{
		wcout << L"Enter No [0 - " << maxS << L"]: ";
		s = ui.getNum();
		
		if (ui.isValidRange(s, 0u, maxS))
			break;
	}

	return s;
}

Window::~Window()
{
	if (history)
	{
		delete history;
		history = nullptr;
	}
}
