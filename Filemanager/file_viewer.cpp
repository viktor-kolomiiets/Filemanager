#include "file_viewer.h"

void FileViewer::menu()
{
	for (;;)
	{
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
	system("cls");
	wstring dlmtr(97, L'-');

	wcout << dlmtr << L"\n";
	wcout << fl.getCurrentPathStr() << L"\n";
	wcout << dlmtr << L"\n";

	fl.print();

	wcout << dlmtr << L"\n";
	wcout << L"o. open | a. create file   | x. delete | c. copy to | i. info | p. change partition\n";
	wcout << L"q. back | n. create folder | r. rename | m. move to | f. find | 0. Exit\n";
	wcout << dlmtr << L"\n";
}

void FileViewer::newFileOption()
{
	wstring dest = fl.getCurrentPathStr();
	wstring file;
	for (;;)
	{
		wcout << L"Enter name: ";
		file = ui.getLine();
		if (ui.isValidName(file))
			break;
		else
			wcout << L"Name can\'t contain \\/:*?\'\"<>| symbols\n";
	}

	bool r = Filesystem{}.createFile(file, dest);
	fl.updateList();
	draw();

	if (r)
	{
		wcout << L"Created file: " << file << L"\nPress any key...";
		ui.getKey();
	}
}

void FileViewer::newFolderOption()
{
	wstring dest = fl.getCurrentPathStr();
	wstring folder;
	for (;;)
	{
		wcout << L"Enter name: ";
		folder = ui.getLine();
		if (ui.isValidName(folder))
			break;
		else
			wcout << L"Name can\'t contain \\/:*?\'\"<>| symbols\n";
	}

	bool r = Filesystem{}.createDir(folder, dest);
	fl.updateList();
	draw();

	if (r)
	{
		wcout << L"Created folder: [" << folder << L"]\nPress any key...";
		ui.getKey();
	}
}

void FileViewer::deleteFileOption()
{
	wcout << L"Select File/Folder: ";
	wstring del = fl[selectItem()]->getPath();
	bool r = Filesystem{}.deleteFile(del);
	fl.updateList();
	draw();

	if (r)
	{
		wcout << L"Deleted " << del << L"\nPress any key...";
		ui.getKey();
	}
}

void FileViewer::renameFileOption()
{
	wcout << L"Select File/Folder: ";
	wstring ren = fl[selectItem()]->getPath();
	wstring n;
	for (;;)
	{
		wcout << L"Enter new name: ";
		n = ui.getLine();
		if (ui.isValidName(n))
			break;
		else
			wcout << L"Name can\'t contain \\/:*?\'\"<>| symbols\n";
	}
	
	bool r = Filesystem{}.renameFile(ren, n);
	fl.updateList();
	draw();

	if (r)
	{
		wcout << L"Renamed " << ren << L"\nPress any key...";
		ui.getKey();
	}
}

void FileViewer::copyFileOption()
{
	wcout << L"Select File/Folder to copy: ";
	wstring from = fl[selectItem()]->getPath();

	OpenDialog od;
	wstring to = od.selectDirectory();
	
	bool r = Filesystem{}.copyFile(from, to);
	fl.updateList();
	draw();

	if (r)
	{
		wcout << L"Copied " << from << L" to " << to << L"\nPress any key...";
		ui.getKey();
	}
}

void FileViewer::moveFileOption()
{
	wcout << L"Select File/Folder to move: ";
	wstring from = fl[selectItem()]->getPath();

	OpenDialog od;
	wstring to = od.selectDirectory();
	
	bool r = Filesystem{}.moveFile(from, to);
	fl.updateList();
	draw();

	if (r)
	{
		wcout << L"Moved " << from << L" to " << to << L"\nPress any key...";
		ui.getKey();
	}
}

void FileViewer::infoFileOption() const
{
}

void FileViewer::findFileOption() const
{
}

//size_t FileViewer::selectItem() const
//{
//	size_t s;
//	size_t maxS = fl.getSize() - 1;
//	for (;;)
//	{
//		wcout << L"Enter No [0 - " << maxS << L"]: ";
//		s = ui.getNum();
//
//		if (ui.isValidRange(s, 0ull, maxS))
//			break;
//	}
//
//	return s;
//}

//------------------------------VIEWER-------------------------------------------------------------

size_t Viewer::selectItem() const
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

//------------------------------OPEN_DIALOG--------------------------------------------------------

void OpenDialog::menu()
{
	for (;;)
	{
		this->draw();

		wchar_t btn = ui.getKey();
		if (btn == L'0')
			break;
		if (btn == SELECT_BTN)
		{
			selection = fl[selectItem()]->getPath();
			if (!Filesystem{}.isDir(selection))
			{
				wcout << L"Can\'t select file. Press any key...\n";
				ui.getKey();
				continue;
			}
			break;
		}

		switch (btn)
		{
		case OPEN_BTN:							//open file or folder
			open();
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
		case NEW_FOLDER_BTN:					//create new folder
			//newFolderOption();
			break;
		case INFO_BTN:							//display information about foder
			//infoFileOption();
			break;
		default:
			break;
		}
	}
}

wstring OpenDialog::selectDirectory()
{
	this->menu();
	return selection;
}

void OpenDialog::draw() const
{
	system("cls");
	wstring dlmtr(97, L'-');

	wcout << dlmtr << L"\n";
	wcout << fl.getCurrentPathStr() << L"\n";
	wcout << L"-----------------------------Select destination directory [s]------------------------------------\n";

	fl.print();

	wcout << dlmtr << L"\n";
	wcout << L"o. open | s. select folder | i. info | p. change partition\n";
	wcout << L"q. back | n. create folder |         | 0. Cancel\n";
	wcout << dlmtr << L"\n";
}

void OpenDialog::open()
{
	fl.openItem(selectItem());
}
