#include <Windows.h>
#include <iostream>

#define STATUS_SUCCESS                   ((NTSTATUS)0x00000000L) 

#include "agent.h"

using namespace std;

/*
包含：
Windows api
ReadFile()，WriteFile()，WriteFileGather()，GetFileSize()，GetFileAttributes()，SetFileAttributes()，GetFileTime()，SetFileTime()，
GetFileInformationByHandle()，SetFileInformationByHandle()，GetFinalPathNameByHandle，MoveFileEx()，CopyFile()，DeleteFile()，CancelIoEx()，
FlushFileBuffers()，LockFileEx()，UnlockFileEx()，GetFileAttributesExA()

ntdll.dll
ZwSetEaFile()，NtQueryEaFile()

ReadFile(): 从文件中读取数据。
WriteFile(): 向文件中写入数据。
WriteFileGather()：可以一次性写入多个数据块。
GetFileSize(): 获取文件的大小。
GetFileAttributes(): 获取文件的属性信息，例如文件大小、创建时间、修改时间等。
SetFileAttributes(): 设置文件的属性信息。
GetFileTime()：获取文件的创建时间、最后访问时间和最后修改时间。
SetFileTime()：设置文件的创建时间、最后访问时间和最后修改时间。
GetFileInformationByHandle()：用于获取已经打开的文件的信息，例如文件大小、创建时间、修改时间、访问时间等。
SetFileInformationByHandle()：用于设置已经打开的文件的信息，例如文件的最后修改时间、访问时间等。
GetFinalPathNameByHandle()：用于获取一个文件的最终路径名。
MoveFileEx()：移动一个文件。
CopyFile()：复制一个文件到另一个位置。
CancelIoEx()：取消与指定文件句柄相关联的异步I/O操作。
FlushFileBuffers()：刷新文件缓冲区。
LockFileEx()：对文件进行锁定操作
UnlockFileEx()：对文件进行解锁操作
GetFileAttributesExA()：获取文件的属性信息。
NtQueryEaFile：检索文件的扩展属性（EA）。
ZwSetEaFile()：设置指定文件的扩展属性。

DeleteFile()：删除一个文件。
*/

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID Pointer;
    };
    ULONG_PTR Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;


typedef NTSTATUS(NTAPI* PFN_ZwSetEaFile)(
    HANDLE FileHandle,
    PIO_STATUS_BLOCK IoStatusBlock,
    PVOID Buffer,
    ULONG Length
    );

typedef NTSTATUS(NTAPI* PFN_NTQUERYEAFILE)(
    HANDLE          FileHandle,
    PIO_STATUS_BLOCK IoStatusBlock,
    PVOID           Buffer,
    ULONG           Length,
    BOOLEAN         ReturnSingleEntry,
    PVOID           EaList,
    ULONG           EaListLength,
    PULONG          EaIndex,
    BOOLEAN         RestartScan
    );

int main() {

    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);

    // 加载dll
    HMODULE hNtDll = LoadLibrary(L"ntdll.dll");
    if (!hNtDll) {
        printf("Failed to load ntdll.dll!\n");
        return 1;
    }

    string fileName = "test.txt";
    char buffer[1024];

    // 打开文件，读取内容
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    HANDLE hFile = CreateFile(L"test.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        cout << "Failed to open file." << endl;
        return 1;
    }

    OVERLAPPED overlapped = { 0 };
    overlapped.Offset = 0;
    overlapped.OffsetHigh = 0;
    overlapped.hEvent = NULL;

    // 锁定文件
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    BOOL result = LockFileEx(hFile, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
    if (!result) {
        cout << "Failed to lock file.\n";
        CloseHandle(hFile);
        return 1;
    }

    // 获取文件的最终路径名
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    WCHAR finalPath[MAX_PATH];
    DWORD pathLength = GetFinalPathNameByHandle(hFile, finalPath, MAX_PATH, FILE_NAME_NORMALIZED);
    if (pathLength == 0)
    {
        wcerr << "无法获取文件的最终路径名，错误码为：" << GetLastError() << endl;
        CloseHandle(hFile);
        return 1;
    }

    // 打印最终路径名
    wcout << "文件的最终路径名为：" << finalPath << endl;

    // 获取文件的大小
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    DWORD dwFileSize = GetFileSize(hFile, NULL);
    if (dwFileSize == INVALID_FILE_SIZE) {
        cout << "Failed to get file size." << endl;
        CloseHandle(hFile);
        return 1;
    }

    // 获取文件的创建时间、最后访问时间和最后修改时间
    FILETIME createTime, accessTime, writeTime;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!GetFileTime(hFile, &createTime, &accessTime, &writeTime))
    {
        cerr << "Failed to get file time" << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    // 获得文件属性
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    DWORD dwAttributes = GetFileAttributes(L"test.txt");
    if (dwAttributes == INVALID_FILE_ATTRIBUTES) {
        cout << "Failed to get file attributes." << endl;
        CloseHandle(hFile);
        return 1;
    }

    WIN32_FILE_ATTRIBUTE_DATA fileAttributes = { 0 };
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    result = GetFileAttributesExA("test.txt", GetFileExInfoStandard, &fileAttributes);
    if (!result) {
        std::cout << "Failed to get file attributes.\n";
        return 1;
    }

    std::cout << "File size: " << fileAttributes.nFileSizeLow << " bytes\n";
    std::cout << "Creation time: " << fileAttributes.ftCreationTime.dwLowDateTime << "\n";
    std::cout << "Last access time: " << fileAttributes.ftLastAccessTime.dwLowDateTime << "\n";
    std::cout << "Last write time: " << fileAttributes.ftLastWriteTime.dwLowDateTime << "\n";
    std::cout << "File attributes: " << fileAttributes.dwFileAttributes << "\n";

    // 获取文件的信息
    BY_HANDLE_FILE_INFORMATION fileInfo;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!GetFileInformationByHandle(hFile, &fileInfo))
    {
        cerr << "Failed to get file information" << endl;
        CloseHandle(hFile);
        return 1;
    }

    cout << "File attributes: " << fileInfo.dwFileAttributes << endl;
    cout << "File size: " << fileInfo.nFileSizeLow << endl;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    char* pBuffer = new char[dwFileSize];
    DWORD dwBytesRead = 0;
    BOOL bResult = ReadFile(hFile, pBuffer, dwFileSize, &dwBytesRead, NULL);
    if (!bResult || dwBytesRead != dwFileSize) {
        cout << "Failed to read file." << endl;
        delete[] pBuffer;
        CloseHandle(hFile);
        return 1;
    }

    // 打印文件内容
    for (DWORD i = 0; i < dwBytesRead; i++) {
        cout << pBuffer[i];
    }

    // 向文件里写入数据
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    bResult = WriteFile(hFile, pBuffer, dwBytesRead, &dwBytesRead, NULL);
    if (!bResult || dwBytesRead != dwFileSize) {
        cout << "Failed to write file." << endl;
        CloseHandle(hFile);
        return 1;
    }

    const char* data = "Hello, world!";
    DWORD numWritten = 0;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!WriteFile(hFile, data, strlen(data), &numWritten, NULL)) {
        std::cout << "Failed to write file.\n";
        CloseHandle(hFile);
        return 1;
    }
    
    // 刷新文件缓冲区
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!FlushFileBuffers(hFile)) {
        std::cout << "Failed to flush file buffers.\n";
        CloseHandle(hFile);
        return 1;
    }


    // 写入多个数据块
    char buffer1[1024] = { 0 };
    char buffer2[1024] = { 0 };
    char buffer3[1024] = { 0 };
    DWORD bytesWritten = 0;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    result = WriteFileGather(hFile, (FILE_SEGMENT_ELEMENT*)buffer1, sizeof(buffer1), NULL, NULL);

    if (!result)
    {
        wcerr << "无法写入数据块1，错误码为：" << GetLastError() << endl;
        CloseHandle(hFile);
        return 1;
    }
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    result = WriteFileGather(hFile, (FILE_SEGMENT_ELEMENT*)buffer2, sizeof(buffer2), NULL, NULL);
    if (!result)
    {
        wcerr << "无法写入数据块2，错误码为：" << GetLastError() << endl;
        CloseHandle(hFile);
        return 1;
    }
    result = WriteFileGather(hFile, (FILE_SEGMENT_ELEMENT*)buffer3, sizeof(buffer3), NULL, (LPOVERLAPPED) &bytesWritten);
    if (!result)
    {
        wcerr << "无法写入数据块3，错误码为：" << GetLastError() << endl;
        CloseHandle(hFile);
        return 1;
    }

    // 打印写入的字节数
    wcout << "已成功写入 " << bytesWritten << " 字节数据" << endl;

    // 获取ZwSetEaFile函数地址
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    PFN_ZwSetEaFile pfnZwSetEaFile = reinterpret_cast<PFN_ZwSetEaFile>(
        GetProcAddress(hNtDll, "ZwSetEaFile"));
    if (!pfnZwSetEaFile) {
        cout << "GetProcAddress failed: " << GetLastError() << endl;
        FreeLibrary(hNtDll);
        return 1;
    }

    // 启动异步I/O操作
    overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!ReadFile(hFile, buffer, sizeof(buffer), NULL, &overlapped)) {
        DWORD error = GetLastError();
        if (error != ERROR_IO_PENDING) {
            cout << "Failed to start I/O operation" << endl;
            CloseHandle(hFile);
            CloseHandle(overlapped.hEvent);
            return 1;
        }
    }

    // 取消异步I/O操作
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!CancelIoEx(hFile, &overlapped)) {
        DWORD error = GetLastError();
        if (error != ERROR_NOT_FOUND) {
            std::cout << "Failed to cancel I/O operation" << std::endl;
            CloseHandle(hFile);
            CloseHandle(overlapped.hEvent);
            return 1;
        }
    }

    // 等待异步I/O操作完成
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (WaitForSingleObject(overlapped.hEvent, INFINITE) != WAIT_OBJECT_0) {
        std::cout << "Failed to wait for I/O operation" << std::endl;
        CloseHandle(hFile);
        CloseHandle(overlapped.hEvent);
        return 1;
    }

    // 获取异步I/O操作的结果状态
    DWORD bytesRead = 0;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!GetOverlappedResult(hFile, &overlapped, &bytesRead, FALSE)) {
        DWORD error = GetLastError();
        cout << "Failed to get I/O operation result: " << error << endl;
    }

    // 设置EA数据
    char eaData[] = "EA Data Test";
    IO_STATUS_BLOCK ioStatusBlock;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    NTSTATUS status = pfnZwSetEaFile(hFile, &ioStatusBlock, eaData, sizeof(eaData));
    if (status != STATUS_SUCCESS) {
        cout << "ZwSetEaFile failed: " << status << endl;
        CloseHandle(hFile);
        FreeLibrary(hNtDll);
        return 1;
    }

    // 修改文件属性
    dwAttributes |= FILE_ATTRIBUTE_HIDDEN;
    bResult = SetFileAttributes(L"test.txt", dwAttributes);
    if (!bResult) {
        cout << "Failed to set file attributes." << endl;
        CloseHandle(hFile);
        return 1;
    }

    // 将最后修改时间设置为当前时间
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    SYSTEMTIME sysTime;
    GetSystemTime(&sysTime);
    FILETIME newWriteTime;
    SystemTimeToFileTime(&sysTime, &newWriteTime);
    if (!SetFileTime(hFile, NULL, NULL, &newWriteTime))
    {
        cerr << "Failed to set file time" << endl;
        CloseHandle(hFile);
        return 1;
    }

    // 设置文件的信息
    FILE_BASIC_INFO basicInfo;
    basicInfo.CreationTime.QuadPart = fileInfo.ftCreationTime.dwLowDateTime + 1000000000LL; // 增加 1 秒
    basicInfo.LastAccessTime.QuadPart = fileInfo.ftLastAccessTime.dwLowDateTime + 2000000000LL; // 增加 2 秒
    basicInfo.LastWriteTime.QuadPart = fileInfo.ftLastWriteTime.dwLowDateTime + 3000000000LL; // 增加 3 秒
    basicInfo.ChangeTime.QuadPart = fileInfo.ftLastWriteTime.dwLowDateTime + 4000000000LL; // 增加 4 秒
    basicInfo.FileAttributes = fileInfo.dwFileAttributes;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!SetFileInformationByHandle(hFile, FileBasicInfo, &basicInfo, sizeof(basicInfo)))
    {
        cerr << "Failed to set file information" << endl;
        CloseHandle(hFile);
        return 1;
    }

    cout << "File information has been set" << endl;

    // 获取ZwSetEaFile函数地址
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    PFN_NTQUERYEAFILE NtQueryEaFile = (PFN_NTQUERYEAFILE)GetProcAddress(hNtDll, "NtQueryEaFile");
    if (NtQueryEaFile == NULL)
    {
        printf("Failed to get address of NtQueryEaFile\n");
        FreeLibrary(hNtDll);
        return 1;
    }

    BYTE buffer4[1024];
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = NtQueryEaFile(hFile, &ioStatusBlock, buffer4, sizeof(buffer4), FALSE, NULL, 0, NULL, FALSE);
    if (status == STATUS_SUCCESS)
    {
        // 处理EA信息
    }
    else
    {
        printf("NtQueryEaFile failed with status 0x%X\n", status);
    }

    CloseHandle(hFile);

    // 原始文件路径
    LPCWSTR sourceFile = L"test.txt";

    // 目标目录路径1
    LPCWSTR targetDir1 = L"C:\\";
    // 目标目录路径1
    LPCWSTR targetDir2 = L"D:\\";

    // 复制文件到目标目录
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (CopyFile(sourceFile, targetDir1, FALSE))
    {
        cout << "文件已成功复制到目标目录" << endl;
    }
    else
    {
        cerr << "复制文件时出错，错误码为：" << GetLastError() << endl;
    }

    // 移动文件到新目录
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (MoveFileEx(sourceFile, targetDir2, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED))
    {
        cout << "文件已成功移动到新目录" << endl;
    }
    else
    {
        cerr << "移动文件时出错，错误码为：" << GetLastError() << endl;
    }

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!DeleteFile(L"test.txt"))
    {
        cerr << "Failed to delete file" << endl;
        return 1;
    }
    cout << "File has been deleted" << endl;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    result = UnlockFileEx(hFile, 0, MAXDWORD, MAXDWORD, &overlapped);
    if (!result) {
        cout << "Failed to unlock file.\n";
        CloseHandle(hFile);
        return 1;
    }


    FreeLibrary(hNtDll);

    return 0;
}
