#include "filesystem.h"

bool Filesystem::isDir(wstring pathP) const
{
	error_code ec;
	return is_directory(path{ pathP }, ec);
}

wstring Filesystem::getExtension(wstring file) const
{
	wstring buffer{ L"" };

	size_t dot = file.find_last_of(L'.');
	if (dot != wstring::npos)
		for (size_t i{ dot }; i < file.size(); i++)
			buffer += file[i];

	return buffer;
}

wstring Filesystem::getParent(wstring pathP) const
{
	wstring buffer{ L"" };

	size_t delim = pathP.find_last_of(LR"(\/)");
	if (delim != wstring::npos)
		for (size_t i{ 0 }; i < delim; i++)
			buffer += pathP[i];

	if (buffer.back() == L':')
		buffer += L'\\';

	return buffer;
}

wstring Filesystem::getFilename(wstring pathP) const
{
	wstring buffer{ L"" };

	size_t delim = pathP.find_last_of(LR"(\/)");
	if (delim != wstring::npos)
		for (size_t i{ delim + 1u }; i < pathP.size(); i++)
			buffer += pathP[i];

	return buffer;
}

uintmax_t Filesystem::getFileSize(wstring pathP) const
{
	return file_size(pathP);
}

uintmax_t Filesystem::getDirSize(wstring pathP) const
{
	uintmax_t fsizeb = 0ull;

	try
	{
		for (recursive_directory_iterator next(pathP), end; next != end; ++next)
		{
			try
			{
				if (!next->is_directory())
					fsizeb += next->file_size();
			}
			catch (const filesystem_error&)
			{
				fsizeb += 0ull;
				continue;
			}
		}
	}
	catch (const exception&)
	{
		return fsizeb;
	}

	return fsizeb;
}

uintmax_t Filesystem::getVolumeUsed(wstring pathP) const
{
	ULARGE_INTEGER freeAv{ 0 }, total{ 0 };
	bool f = GetDiskFreeSpaceEx(pathP.c_str(), &freeAv, &total, NULL);
	return static_cast<uintmax_t>(total.QuadPart - freeAv.QuadPart);
}

wstring Filesystem::getVolumeLabel(wstring pathP) const
{
	wchar_t label[MAX_PATH + 1]{};
	label[MAX_PATH] = L'\0';

	bool gvi = GetVolumeInformation(pathP.c_str(),
		label, MAX_PATH,
		NULL, NULL, NULL,
		NULL, 0);

	if (gvi)
		return wstring{ label };
	return L"";
}

wstring Filesystem::getFSName(wstring pathP) const
{
	wchar_t fs_name[MAX_PATH + 1]{};
	fs_name[MAX_PATH] = L'\0';

	bool gvi = GetVolumeInformation(pathP.c_str(),
		NULL, 0,
		NULL, NULL, NULL,
		fs_name, MAX_PATH);

	if (gvi)
		return wstring{ fs_name };
	return L"";
}

wstring Filesystem::getUserName() const
{
	wchar_t u_name[MAX_PATH + 1]{};
	u_name[MAX_PATH] = L'\0';
	DWORD len{ 0 };

	GetUserName(NULL, &len);
	bool gun = GetUserName(u_name, &len);

	return wstring{ u_name };
}

wstring Filesystem::getCompName() const
{
	wchar_t pc_name[MAX_PATH + 1]{};
	pc_name[MAX_PATH] = L'\0';
	DWORD len{ 0 };

	GetComputerName(NULL, &len);
	bool gun = GetComputerName(pc_name, &len);

	return wstring{ pc_name };
}

void Filesystem::executeFile(wstring pathP) const
{
	//CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	ShellExecute(NULL, NULL, pathP.c_str(), NULL, NULL, SW_RESTORE);
}

bool Filesystem::createFile(wstring nameP, wstring pathP) const
{
	wstring fpath{ pathP + L'\\' + nameP + L".txt" };

	FILE* file;
	try
	{
		if (this->isExist(pathP))
		{
			errno_t err = _wfopen_s(&file, fpath.c_str(), L"w");

			if (err)
				throw L"Can\'t create a " + nameP + L" in " + pathP + L" directory";

			if (file)
				err = fclose(file);

			return !err;
		}

	}
	catch (const wstring&/* msg*/)
	{
		_fcloseall();
		//wcout << msg << L"\n";----------------------------------------------------------------------X
		return false;
	}
	catch (const std::exception&)
	{
		_fcloseall();
		//wcout << L"Can\'t create a " << fpath << L"\n";---------------------------------------------X
		return false;
	}

	return false;
}

bool Filesystem::createDir(wstring nameP, wstring pathP) const
{
	wstring fpath{ pathP + L'\\' + nameP };

	//if parent path of new directory is exist
	if (this->isExist(pathP))
	{
		create_directory(path{ fpath });
		return true;
	}

	return false;
}

bool Filesystem::copyFile(wstring from, wstring to) const
{
	try
	{
		if (this->isDir(from))
		{
			//create directory()
			wstring name = this->getFilename(from);
			to += L"\\" + name;
			create_directory(to);
		}
		copy(path{ from }, path{ to }, copy_options::recursive);
	}
	catch (const std::exception&)
	{
		return false;
	}

	return false;
}

bool Filesystem::renameFile(wstring old, wstring newName) const
{
	wstring extn = this->getExtension(old);
	wstring newFile = this->getParent(old) + L"\\" + newName + extn;

	try
	{
		if (this->isExist(old))
		{
			rename(path{ old }, path{ newFile });
			return true;
		}
	}
	catch (const filesystem_error&)
	{
		return false;
	}

	return false;
}

bool Filesystem::deleteFile(wstring pathP) const
{
	try
	{
		if (this->isExist(pathP))
		{
			uintmax_t rmd = remove_all(path{ pathP });
			//wcout << L"Successfully deleted " << rmd << L" object(s)\n";---------------------------------X
			return true;
		}
	}
	catch (const filesystem_error&)
	{
		return false;
	}

	return false;
}

bool Filesystem::moveFile(wstring from, wstring to) const
{
	try
	{
		to += L"\\" + this->getFilename(from);
		rename(path{ from }, path{ to });
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}

	return false;
}

vector<wstring> Filesystem::findFiles(wstring mask, wstring target) const
{
	vector<wstring> folders;
	vector<wstring> results;

	//forming list of subdirs
	folders.push_back(target);
	try
	{
		for (recursive_directory_iterator next(folders.at(0), directory_options::skip_permission_denied), end; next != end; ++next)
		{
			try
			{
				if (this->isDir(next->path().wstring()))
					folders.push_back(next->path().wstring());
			}
			catch (const filesystem_error&)
			{
				continue;
			}
		}
	}
	catch (const std::exception&)
	{
		//wcout << L"Error\n";----------------------------------------------------------------X
	}

	//search by mask in each subdirs
	for (size_t i{ 0 }; i < folders.size(); i++)
	{
		wstring search;
		search = folders.at(i) + L'\\' + mask;

		try
		{
			WIN32_FIND_DATA fileData;
			HANDLE hFind;

			hFind = FindFirstFile(search.c_str(), &fileData);
			if (hFind == INVALID_HANDLE_VALUE)
				continue;
			else
			{
				do
				{
					wstring fname = fileData.cFileName;
					wstring fpath = folders.at(i) + L'\\' + fname;

					if (!this->isDir(fpath))
						results.push_back(fpath);

				} while (FindNextFile(hFind, &fileData));
			}
			if (hFind)
				FindClose(hFind);
		}
		catch (const std::exception&)
		{
			continue;
		}
	}

	return results;
}

vector<wstring> Filesystem::getAllFiles(wstring pathP) const
{
	vector<wstring> buffer;// = new vector<wstring>;
	for (directory_iterator next(pathP, directory_options::skip_permission_denied), end; next != end; ++next)
	{
		try
		{
			buffer.push_back(next->path().wstring());
		}
		catch (const filesystem_error&)
		{
			continue;
		}
	}

	return buffer;
}

vector<wstring> Filesystem::getDirs(wstring pathP) const
{
	vector<wstring> buffer;
	for (directory_iterator next(pathP, directory_options::skip_permission_denied), end; next != end; ++next)
		try
	{
		wstring p = next->path();
		if (this->isDir(p))
			buffer.push_back(p);
	}
	catch (const filesystem_error&)
	{
		continue;
	}

	return buffer;
}

vector<wstring> Filesystem::getPartitions() const
{
	vector<wstring> buffer;

	int n;
	// функция возвращает битовую маску
	DWORD drives = GetLogicalDrives();

	// прогоняем по битам
	for (int x = 0; x < 26; x++)
	{
		// определяем значение текущего бита
		n = ((drives >> x) & 1);

		// если единица - диск с номером x есть
		if (n)
		{
			wstring partitionPath{ L"" };

			partitionPath += (wchar_t)(65 + x);
			partitionPath += L":\\";
			buffer.push_back(partitionPath);
		}
	}

	return buffer;
}
