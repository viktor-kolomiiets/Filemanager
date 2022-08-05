#pragma once

class Filesystem
{
public:
	bool isExist(wstring) const;

	wstring getVolumeLabel(wstring) const;
	wstring getFSName(wstring) const;

	bool createFile(wstring, wstring) const;
	bool createDirectory(wstring, wstring) const;
};

inline bool Filesystem::isExist(wstring pathP) const
{
	return exists(path{ pathP });
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

inline bool Filesystem::createDirectory(wstring nameP, wstring pathP) const
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
