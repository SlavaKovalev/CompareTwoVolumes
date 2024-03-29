#include "pch.h"

#include <list>
#include <string>
#include <thread>

#include <stdio.h>

#include <Windows.h>
#include <tchar.h>

bool EnumerateFiles(const std::wstring& root, std::list<std::wstring>* files)
{
    std::wstring search_path = root + _T("/*");
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (wcscmp(fd.cFileName, _T(".")) != 0 && wcscmp(fd.cFileName, _T("..")) != 0)
                {
                    std::wstring entPath = root + _T("\\") + fd.cFileName;
                    if (!EnumerateFiles(entPath, files))
                        return false;
                }
            }
            else
            {
                files->push_back(root + _T("\\") + fd.cFileName);
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
    return true;
}

int wmain(int argc, wchar_t** argv)
{
    if (argc < 3)
    {
        printf("wrong argument\n");
        return 1;
    }
    std::list<std::wstring> files1;
    std::list<std::wstring> files2;
    std::thread worker(EnumerateFiles, argv[1], &files1);
    EnumerateFiles(argv[2], &files2);
    if (worker.joinable())
    {
        worker.join();
    }
    return 0;
}
