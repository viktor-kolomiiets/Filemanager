#pragma once

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
	/*if (this->history->size() > 0)
	{
		current = *this->history->back();
		this->history->pop_back();
	}
	
	this->openDir();*/
	current = Directory{ current.getParent() };
	fl.openDir(current);
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
	//fl.open(selectItem());
	
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
			fl.openRoot();
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
	//wcout << fl.getCurrentPathStr() << L"\n";
	wcout << current.getPath() << L"\n";
	wcout << dlmtr << L"\n";
	
	fl.print();

	wcout << dlmtr << L"\n";
	wcout << L"o. open | a. create file   | x. delete | c. copy | i. info | p. change partition\n";
	wcout << L"q. back | n. create folder | r. rename | m. move | f. find | 0. Exit\n";
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
