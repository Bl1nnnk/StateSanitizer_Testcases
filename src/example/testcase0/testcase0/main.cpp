#include <Windows.h>
#include <iostream>

#include "../../../agent.h"

int main()
{
    ss_hypercall(SS_HC_SUBMIT_CR3, 0);

    HANDLE hFile = CreateFile(L"example.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to open file. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    ss_hypercall(SS_HC_BEGIN_TEST, 0);

    // 获取文件信息
    FILE_BASIC_INFO fileInfo;
    if (!GetFileInformationByHandleEx(hFile, FileBasicInfo, &fileInfo, sizeof(fileInfo)))
    {
        std::cerr << "Failed to get file information. Error code: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    // 修改文件信息
    fileInfo.CreationTime.QuadPart = 0;
    fileInfo.LastAccessTime.QuadPart = 0;
    fileInfo.LastWriteTime.QuadPart = 0;
    fileInfo.ChangeTime.QuadPart = 0;

    // 设置文件信息
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