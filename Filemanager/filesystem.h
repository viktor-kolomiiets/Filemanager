#pragma once

class Filesystem
{
public:
	bool isExist(wstring) const;
	bool isDir(wstring) const;

	wstring getExtension(wstring) const;
	wstring getParent(wstring) const;
	wstring getFilename(wstring) const;

	wstring getVolumeLabel(wstring) const;
	wstring getFSName(wstring) const;
	wstring getUserName() const;
	wstring getCompName() const;

	bool createFile(wstring, wstring) const;
	bool createDir(wstring, wstring) const;
	bool copyFile(wstring, wstring) const;
	bool renameFile(wstring, wstring) const;
	bool deleteFile(wstring) const;
};

inline bool Filesystem::isExist(wstring pathP) const
{
	return exists(path{ pathP });
}

inline bool Filesystem::isDir(wstring pathP) const
{
	error_code ec;
	return is_directory(path{ pathP }, ec);
}

inline wstring Filesystem::getExtension(wstring file) const
{
	wstring buffer{ L"" };

	size_t dot = file.find_last_of(L'.');
	if (dot != wstring::npos)
		for (size_t i{ dot }; i < file.size(); i++)
			buffer += file[i];
	
	return buffer;
}

inline wstring Filesystem::getParent(wstring pathP) const
{
	wstring buffer{ L"" };

	size_t delim = pathP.find_last_of(LR"(\/)");
	if (delim != wstring::npos)
		for (size_t i{ 0 }; i < delim; i++)
			buffer += pathP[i];

	return buffer;
}

inline wstring Filesystem::getFilename(wstring pathP) const
{
	wstring buffer{ L"" };

	size_t delim = pathP.find_last_of(LR"(\/)");
	if (delim != wstring::npos)
		for (size_t i{ delim + 1u }; i < pathP.size(); i++)
			buffer += pathP[i];

	return buffer;
}

inline wstring Filesystem::getVolumeLabel(wstring pathP) const
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

inline wstring Filesystem::getFSName(wstring pathP) const
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

inline wstring Filesystem::getUserName() const
{
	wchar_t u_name[MAX_PATH + 1]{};
	u_name[MAX_PATH] = L'\0';
	DWORD len{ 0 };

	GetUserName(NULL, &len);
	bool gun = GetUserName(u_name, &len);

	return wstring{ u_name };
}

inline wstring Filesystem::getCompName() const
{
	wchar_t pc_name[MAX_PATH + 1]{};
	pc_name[MAX_PATH] = L'\0';
	DWORD len{ 0 };

	GetComputerName(NULL, &len);
	bool gun = GetComputerName(pc_name, &len);

	return wstring{ pc_name };
}

inline bool Filesystem::createFile(wstring nameP, wstring pathP) const
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
	catch (const wstring& msg)
	{
		_fcloseall();
		wcout << msg << L"\n";
		return false;
	}
	catch (const std::exception&)
	{
		_fcloseall();
		wcout << L"Can\'t create a " << fpath << L"\n";
		return false;
	}

	return false;
}

inline bool Filesystem::createDir(wstring nameP, wstring pathP) const
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

inline bool Filesystem::copyFile(wstring from, wstring to) const
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

inline bool Filesystem::renameFile(wstring old, wstring newName) const
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

inline bool Filesystem::deleteFile(wstring pathP) const
{
	try
	{
		if (this->isExist(pathP))
		{
			uintmax_t rmd = remove_all(path{ pathP });
			wcout << L"Successfully deleted " << rmd << L" object(s)\n";
			return true;
		}
	}
	catch (const filesystem_error&)
	{
		return false;
	}

	return false;
}