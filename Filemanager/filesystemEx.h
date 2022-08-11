#pragma once

class FilesystemEx : public Filesystem
{
public:
	vector<Path*> findPartitions() const;
	vector<Path*> findDirs(Path*) const;
	vector<Path*>* findAllFiles(Path*) const;
};

inline vector<Path*> FilesystemEx::findPartitions() const
{
	vector<Path*> buffer;

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
			buffer.push_back(new Partition{ partitionPath });
		}
	}

	return buffer;
}

inline vector<Path*> FilesystemEx::findDirs(Path* pathP) const
{
	vector<Path*> buffer;
	for (directory_iterator next(pathP->getPath(), directory_options::skip_permission_denied), end; next != end; ++next)
		try
		{
			wstring p = next->path();
			if (this->isDir(p))
				buffer.push_back(new Directory{ p });
		}
		catch (const filesystem_error&)
		{
			continue;
		}

	return buffer;
}

inline vector<Path*>* FilesystemEx::findAllFiles(Path* pathP) const
{
	vector<Path*>* buffer = new vector<Path*>;
	for (directory_iterator next(pathP->getPath(), directory_options::skip_permission_denied), end; next != end; ++next)
	{
		try
		{
			wstring p = next->path();
			if (this->isDir(p))
				buffer->push_back(new Directory{ p });
			else
				buffer->push_back(new File{ p });
		}
		catch (const filesystem_error&)
		{
			continue;
		}
	}

	return buffer;
}
