#include "file_viewer.h"

void FileViewer::menu()
{
	for (;;)
	{
		this->draw();

		wchar_t btn = ui.getKey();
		bool root = fl.isRoot();

		switch (btn)
		{
		case EXIT_BTN:
			return;
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
			if (!root)
				newFileOption();
			break;
		case NEW_FOLDER_BTN:					//create new folder
			if (!root)
				newFolderOption();
			break;
		case DELETE_BTN:						//delete file/folder
			if (!root)
				deleteFileOption();
			break;
		case RENAME_BTN:						//rename file/folder
			if (!root)
				renameFileOption();
			break;
		case COPY_BTN:							//copy file/folder
			if (!root)
				copyFileOption();
			break;
		case MOVE_BTN:							//move to another directory file/folder
			if (!root)
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
	wstring dlmtr(105, L'-');

	wcout << dlmtr << L"\n ";
	wcout << fl.getCurrentPathStr() << L"\n";
	wcout << dlmtr << L"\n";

	fl.print();

	wcout << dlmtr << L"\n";
	wcout << L" o. open | a. create file   | x. delete | c. copy to | i. info | p. change partition\n";
	wcout << L" q. back | n. create folder | r. rename | m. move to | f. find | 0. Exit\n";
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
		wcout << L"Renamed " << ren << L" to " << n << L"\nPress any key...";
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

void FileViewer::findFileOption()
{
	wstring target = fl.getCurrentPathStr();
	wcout << L"Search for: ";
	wstring mask = ui.getLine();

	vector<wstring> search = Filesystem{}.findFiles(mask, target);
	vector<Path*>* sfiles = new vector<Path*>;

	for (size_t i{ 0ull }; i < search.size(); i++)
		sfiles->push_back(new File{ search.at(i) });

	fl = sfiles;
	fl.addLastDir(target);
}

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

void Viewer::newFolderOption()
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

void Viewer::infoFileOption() const
{
	fl[selectItem()]->displayInfo();
	wcout << L"Press any key...";
	ui.getKey();
}

//------------------------------OPEN_DIALOG--------------------------------------------------------

void OpenDialog::menu()
{
	for (;;)
	{
		this->draw();
		wchar_t btn = ui.getKey();

		switch (btn)
		{
		case EXIT_BTN:
			return;
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
		case SELECT_BTN:
			selection = fl[selectItem()]->getPath();
			if (!Filesystem{}.isDir(selection))
			{
				wcout << L"Can\'t select file. Press any key...\n";
				ui.getKey();
				continue;
			}
			return;
		case PARTITION_BTN:						//select partitions to open
			fl.openRoot();
			break;
		case NEW_FOLDER_BTN:					//create new folder
			if (!fl.isRoot())
				newFolderOption();
			break;
		case INFO_BTN:							//display information about foder
			infoFileOption();
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
	wstring dlmtr(105, L'-');

	wcout << dlmtr << L"\n ";
	wcout << fl.getCurrentPathStr() << L"\n";
	wcout << L"-----------------------------Select destination directory [s]------------------------------------\n";

	fl.print();

	wcout << dlmtr << L"\n";
	wcout << L" o. open | s. select folder | i. info | p. change partition\n";
	wcout << L" q. back | n. create folder |         | 0. Cancel\n";
	wcout << dlmtr << L"\n";
}
