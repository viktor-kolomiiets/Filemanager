#include "file_viewer.h"

Window::Window()
{
	current = Directory{ Root{}.getPath() };
	fl.openRoot();
}

void Window::menu()
{
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

Window::~Window()
{

}

void Window::goLastDir()
{
	current = Directory{ current.getParent() };
	fl.openPath(current.getPath());
}

void Window::openDir()
{
	current = *fl[selectItem()];		//polnoye imya fayla toge prinimayet
	fl.openPath(current.getPath());
}

void Window::draw() const
{
	wstring dlmtr(97, L'-');

	wcout << dlmtr << L"\n";
	wcout << current.getPath() << L"\n";
	wcout << dlmtr << L"\n";

	fl.print();

	wcout << dlmtr << L"\n";
	wcout << L"o. open | a. create file   | x. delete | c. copy | i. info | p. change partition\n";
	wcout << L"q. back | n. create folder | r. rename | m. move | f. find | 0. Exit\n";
	wcout << dlmtr << L"\n";
}

size_t Window::selectItem() const
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
