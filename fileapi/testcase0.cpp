#include <Windows.h>
#include <iostream>

#define STATUS_SUCCESS                   ((NTSTATUS)0x00000000L) 

#include "agent.h"

using namespace std;

/*
������
Windows api
ReadFile()��WriteFile()��WriteFileGather()��GetFileSize()��GetFileAttributes()��SetFileAttributes()��GetFileTime()��SetFileTime()��
GetFileInformationByHandle()��SetFileInformationByHandle()��GetFinalPathNameByHandle��MoveFileEx()��CopyFile()��DeleteFile()��CancelIoEx()��
FlushFileBuffers()��LockFileEx()��UnlockFileEx()��GetFileAttributesExA()

ntdll.dll
ZwSetEaFile()��NtQueryEaFile()

ReadFile(): ���ļ��ж�ȡ���ݡ�
WriteFile(): ���ļ���д�����ݡ�
WriteFileGather()������һ����д�������ݿ顣
GetFileSize(): ��ȡ�ļ��Ĵ�С��
GetFileAttributes(): ��ȡ�ļ���������Ϣ�������ļ���С������ʱ�䡢�޸�ʱ��ȡ�
SetFileAttributes(): �����ļ���������Ϣ��
GetFileTime()����ȡ�ļ��Ĵ���ʱ�䡢������ʱ�������޸�ʱ�䡣
SetFileTime()�������ļ��Ĵ���ʱ�䡢������ʱ�������޸�ʱ�䡣
GetFileInformationByHandle()�����ڻ�ȡ�Ѿ��򿪵��ļ�����Ϣ�������ļ���С������ʱ�䡢�޸�ʱ�䡢����ʱ��ȡ�
SetFileInformationByHandle()�����������Ѿ��򿪵��ļ�����Ϣ�������ļ�������޸�ʱ�䡢����ʱ��ȡ�
GetFinalPathNameByHandle()�����ڻ�ȡһ���ļ�������·������
MoveFileEx()���ƶ�һ���ļ���
CopyFile()������һ���ļ�����һ��λ�á�
CancelIoEx()��ȡ����ָ���ļ������������첽I/O������
FlushFileBuffers()��ˢ���ļ���������
LockFileEx()�����ļ�������������
UnlockFileEx()�����ļ����н�������
GetFileAttributesExA()����ȡ�ļ���������Ϣ��
NtQueryEaFile�������ļ�����չ���ԣ�EA����
ZwSetEaFile()������ָ���ļ�����չ���ԡ�

DeleteFile()��ɾ��һ���ļ���
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

    // ����dll
    HMODULE hNtDll = LoadLibrary(L"ntdll.dll");
    if (!hNtDll) {
        printf("Failed to load ntdll.dll!\n");
        return 1;
    }

    string fileName = "test.txt";
    char buffer[1024];

    // ���ļ�����ȡ����
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

    // �����ļ�
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    BOOL result = LockFileEx(hFile, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
    if (!result) {
        cout << "Failed to lock file.\n";
        CloseHandle(hFile);
        return 1;
    }

    // ��ȡ�ļ�������·����
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    WCHAR finalPath[MAX_PATH];
    DWORD pathLength = GetFinalPathNameByHandle(hFile, finalPath, MAX_PATH, FILE_NAME_NORMALIZED);
    if (pathLength == 0)
    {
        wcerr << "�޷���ȡ�ļ�������·������������Ϊ��" << GetLastError() << endl;
        CloseHandle(hFile);
        return 1;
    }

    // ��ӡ����·����
    wcout << "�ļ�������·����Ϊ��" << finalPath << endl;

    // ��ȡ�ļ��Ĵ�С
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    DWORD dwFileSize = GetFileSize(hFile, NULL);
    if (dwFileSize == INVALID_FILE_SIZE) {
        cout << "Failed to get file size." << endl;
        CloseHandle(hFile);
        return 1;
    }

    // ��ȡ�ļ��Ĵ���ʱ�䡢������ʱ�������޸�ʱ��
    FILETIME createTime, accessTime, writeTime;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!GetFileTime(hFile, &createTime, &accessTime, &writeTime))
    {
        cerr << "Failed to get file time" << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    // ����ļ�����
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

    // ��ȡ�ļ�����Ϣ
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

    // ��ӡ�ļ�����
    for (DWORD i = 0; i < dwBytesRead; i++) {
        cout << pBuffer[i];
    }

    // ���ļ���д������
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
    
    // ˢ���ļ�������
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!FlushFileBuffers(hFile)) {
        std::cout << "Failed to flush file buffers.\n";
        CloseHandle(hFile);
        return 1;
    }


    // д�������ݿ�
    char buffer1[1024] = { 0 };
    char buffer2[1024] = { 0 };
    char buffer3[1024] = { 0 };
    DWORD bytesWritten = 0;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    result = WriteFileGather(hFile, (FILE_SEGMENT_ELEMENT*)buffer1, sizeof(buffer1), NULL, NULL);

    if (!result)
    {
        wcerr << "�޷�д�����ݿ�1��������Ϊ��" << GetLastError() << endl;
        CloseHandle(hFile);
        return 1;
    }
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    result = WriteFileGather(hFile, (FILE_SEGMENT_ELEMENT*)buffer2, sizeof(buffer2), NULL, NULL);
    if (!result)
    {
        wcerr << "�޷�д�����ݿ�2��������Ϊ��" << GetLastError() << endl;
        CloseHandle(hFile);
        return 1;
    }
    result = WriteFileGather(hFile, (FILE_SEGMENT_ELEMENT*)buffer3, sizeof(buffer3), NULL, (LPOVERLAPPED) &bytesWritten);
    if (!result)
    {
        wcerr << "�޷�д�����ݿ�3��������Ϊ��" << GetLastError() << endl;
        CloseHandle(hFile);
        return 1;
    }

    // ��ӡд����ֽ���
    wcout << "�ѳɹ�д�� " << bytesWritten << " �ֽ�����" << endl;

    // ��ȡZwSetEaFile������ַ
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    PFN_ZwSetEaFile pfnZwSetEaFile = reinterpret_cast<PFN_ZwSetEaFile>(
        GetProcAddress(hNtDll, "ZwSetEaFile"));
    if (!pfnZwSetEaFile) {
        cout << "GetProcAddress failed: " << GetLastError() << endl;
        FreeLibrary(hNtDll);
        return 1;
    }

    // �����첽I/O����
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

    // ȡ���첽I/O����
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

    // �ȴ��첽I/O�������
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (WaitForSingleObject(overlapped.hEvent, INFINITE) != WAIT_OBJECT_0) {
        std::cout << "Failed to wait for I/O operation" << std::endl;
        CloseHandle(hFile);
        CloseHandle(overlapped.hEvent);
        return 1;
    }

    // ��ȡ�첽I/O�����Ľ��״̬
    DWORD bytesRead = 0;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!GetOverlappedResult(hFile, &overlapped, &bytesRead, FALSE)) {
        DWORD error = GetLastError();
        cout << "Failed to get I/O operation result: " << error << endl;
    }

    // ����EA����
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

    // �޸��ļ�����
    dwAttributes |= FILE_ATTRIBUTE_HIDDEN;
    bResult = SetFileAttributes(L"test.txt", dwAttributes);
    if (!bResult) {
        cout << "Failed to set file attributes." << endl;
        CloseHandle(hFile);
        return 1;
    }

    // ������޸�ʱ������Ϊ��ǰʱ��
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

    // �����ļ�����Ϣ
    FILE_BASIC_INFO basicInfo;
    basicInfo.CreationTime.QuadPart = fileInfo.ftCreationTime.dwLowDateTime + 1000000000LL; // ���� 1 ��
    basicInfo.LastAccessTime.QuadPart = fileInfo.ftLastAccessTime.dwLowDateTime + 2000000000LL; // ���� 2 ��
    basicInfo.LastWriteTime.QuadPart = fileInfo.ftLastWriteTime.dwLowDateTime + 3000000000LL; // ���� 3 ��
    basicInfo.ChangeTime.QuadPart = fileInfo.ftLastWriteTime.dwLowDateTime + 4000000000LL; // ���� 4 ��
    basicInfo.FileAttributes = fileInfo.dwFileAttributes;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!SetFileInformationByHandle(hFile, FileBasicInfo, &basicInfo, sizeof(basicInfo)))
    {
        cerr << "Failed to set file information" << endl;
        CloseHandle(hFile);
        return 1;
    }

    cout << "File information has been set" << endl;

    // ��ȡZwSetEaFile������ַ
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
        // ����EA��Ϣ
    }
    else
    {
        printf("NtQueryEaFile failed with status 0x%X\n", status);
    }

    CloseHandle(hFile);

    // ԭʼ�ļ�·��
    LPCWSTR sourceFile = L"test.txt";

    // Ŀ��Ŀ¼·��1
    LPCWSTR targetDir1 = L"C:\\";
    // Ŀ��Ŀ¼·��1
    LPCWSTR targetDir2 = L"D:\\";

    // �����ļ���Ŀ��Ŀ¼
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (CopyFile(sourceFile, targetDir1, FALSE))
    {
        cout << "�ļ��ѳɹ����Ƶ�Ŀ��Ŀ¼" << endl;
    }
    else
    {
        cerr << "�����ļ�ʱ����������Ϊ��" << GetLastError() << endl;
    }

    // �ƶ��ļ�����Ŀ¼
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (MoveFileEx(sourceFile, targetDir2, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED))
    {
        cout << "�ļ��ѳɹ��ƶ�����Ŀ¼" << endl;
    }
    else
    {
        cerr << "�ƶ��ļ�ʱ����������Ϊ��" << GetLastError() << endl;
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
