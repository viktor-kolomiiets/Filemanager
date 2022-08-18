#pragma once

#include "filelist.h"
#include "input.h"

#define OPEN_BTN L'o'
#define BACK_BTN L'q'
#define SELECT_BTN L's'
#define EXIT_BTN L'0'

#define NEXT_BTN L'\t'
#define PREV_BTN L' '

#define NEW_FILE_BTN L'a'
#define NEW_FOLDER_BTN L'n'
#define DELETE_BTN L'x'
#define RENAME_BTN L'r'
#define COPY_BTN L'c'
#define MOVE_BTN L'm'
#define INFO_BTN L'i'
#define FIND_BTN L'f'
#define PARTITION_BTN L'p'

class Viewer
{
protected:
	FileList fl;
	Input ui;

public:
	Viewer() = default;
	virtual void menu() = 0;
	virtual ~Viewer() {}

protected:
	virtual void draw() const = 0;
	size_t selectItem() const;
	void newFolderOption();
	void infoFileOption() const;
};

class OpenDialog : public Viewer
{
private:
	wstring selection;

public:
	OpenDialog() { fl.openRoot(); }
	void menu() override;
	wstring selectDirectory();
	~OpenDialog() { fl.clear(); }

private:
	void draw() const override;
};

class FileViewer : public Viewer
{
public:
	FileViewer() { fl.openRoot(); }
	void menu() override;
	~FileViewer() { fl.clear(); }

private:
	void draw() const override;

	void newFileOption();
	void deleteFileOption();
	void renameFileOption();
	void copyFileOption();
	void moveFileOption();
	void findFileOption();
};
