#include "file_viewer.h"

void FileViewer::menu()
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
			fl.openItem(selectItem());
			break;
		case BACK_BTN:							//back to previous folder
			fl.openParent();
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
			newFileOption();
			break;
		case NEW_FOLDER_BTN:					//create new folder
			newFolderOption();
			break;
		case DELETE_BTN:						//delete file/folder
			deleteFileOption();
			break;
		case RENAME_BTN:						//rename file/folder
			renameFileOption();
			break;
		case COPY_BTN:							//copy file/folder
			copyFileOption();
			break;
		case MOVE_BTN:							//move to another directory file/folder
			moveFileOption();
			break;
		case INFO_BTN:							//display information about foder
			infoFileOption();
			break;
		case FIND_BTN:							//find file(s)
			findFileOption();
			break;
		default:
			break;
		}
	}
}

void FileViewer::draw() const
{
	wstring dlmtr(97, L'-');

	wcout << dlmtr << L"\n";
	wcout << fl.getCurrentPathStr() << L"\n";
	wcout << dlmtr << L"\n";

	fl.print();

	wcout << dlmtr << L"\n";
	wcout << L"o. open | a. create file   | x. delete | c. copy | i. info | p. change partition\n";
	wcout << L"q. back | n. create folder | r. rename | m. move | f. find | 0. Exit\n";
	wcout << dlmtr << L"\n";
}

void FileViewer::newFileOption() const
{
}

void FileViewer::newFolderOption() const
{
}

void FileViewer::deleteFileOption() const
{
}

void FileViewer::renameFileOption() const
{
}

void FileViewer::copyFileOption() const
{
}

void FileViewer::moveFileOption() const
{
}

void FileViewer::infoFileOption() const
{
}

void FileViewer::findFileOption() const
{
}

size_t FileViewer::selectItem() const
{
	size_t s;
	size_t maxS = fl.getSize() - 1;
	for (;;)
	{
		wcout << L"Enter No [0 - " << maxS << L"]: ";
		s = ui.getNum();

		if (ui.isValidRange(s, 0ull, maxS))
			break;
	}

	return s;
}
