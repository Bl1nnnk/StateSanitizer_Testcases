#include <Windows.h>
#include <iostream>

int main()
{

    HANDLE hFile = CreateFileW(L"example.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to open file. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    // ��ȡ�ļ���Ϣ
    FILE_BASIC_INFO fileInfo;
    if (!GetFileInformationByHandleEx(hFile, FileBasicInfo, &fileInfo, sizeof(fileInfo)))
    {
        std::cerr << "Failed to get file information. Error code: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    // �޸��ļ���Ϣ
    fileInfo.CreationTime.QuadPart = 0;
    fileInfo.LastAccessTime.QuadPart = 0;
    fileInfo.LastWriteTime.QuadPart = 0;
    fileInfo.ChangeTime.QuadPart = 0;

    // �����ļ���Ϣ
    if (!SetFileInformationByHandle(hFile, FileBasicInfo, &fileInfo, sizeof(fileInfo)))
    {
        std::cerr << "Failed to set file information. Error code: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    std::cout << "File information modified successfully." << std::endl;

    CloseHandle(hFile);
    return 0;
}