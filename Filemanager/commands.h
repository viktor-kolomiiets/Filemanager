#pragma once

class Command
{
protected:
	vector<Path*>* fileList;

public:
	Command() : fileList{ nullptr } {}
	void setList(vector<Path*>* fileListP) { this->fileList = fileListP; }
	virtual void execute() = 0;
	~Command() { /*if (fileList) delete fileList;*/fileList = nullptr; }
};

class Open : public Command
{
public:
	Open() = default;

	void execute() override
	{
		size_t no = 0;
		for (;;)
		{
			wcout << L"Select object: ";
			wcin >> no;

			if (wcin.fail())
			{
				wcin.clear();
				wcin.ignore(32767, L'\n');
				continue;
			}
			wcin.ignore(32767, L'\n');
			if (no >= 0 && no < fileList->size())
				break;
		}

		if (fileList->at(no)->isFile())
		{
			fileList->at(no)->open();
			return;
		}

		Path* f = new Directory{ fileList->at(no)->getPath() };//fileList->at(no);
		fileList->clear();
		/*if (fileList)
		{
			delete fileList;
			fileList = nullptr;
		}*/
		fileList = f->open();
	}
};