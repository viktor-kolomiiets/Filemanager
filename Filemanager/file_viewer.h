#pragma once

#include "filelist.h"
#include "input.h"

#define OPEN_BTN L'o'
#define BACK_BTN L'q'
#define NEW_FILE_BTN L'a'
#define NEW_FOLDER_BTN L'n'
#define DELETE_BTN L'x'
#define RENAME_BTN L'r'
#define COPY_BTN L'c'
#define MOVE_BTN L'm'
#define INFO_BTN L'i'
#define FIND_BTN L'f'
#define NEXT_BTN L'\t'
#define PREV_BTN L' '
#define PARTITION_BTN L'p'

class FileViewer
{
private:
	FileList fl;
	Input ui;

public:
	FileViewer() { fl.openRoot(); }

	void menu();

	~FileViewer() { fl.clear(); }

private:
	void draw() const;

	size_t selectItem() const;
};
