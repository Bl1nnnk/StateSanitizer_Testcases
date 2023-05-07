#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "agent.h"

/* 1-10
AreFileApisANSI()->GetACP()：用于判断当前系统是否在 ANSI 模式下打开文件。
AreShortNamesEnabled()->GetVolumeInformation()：检查是否启用了短名称。
CreateDirectoryA()：创建一个新目录。
CreateDirectoryW()：创建一个新目录，。"W"表示宽字符版本的函数，它接受Unicode字符串作为输入。
CreateFile2()：创建一个新文件。
CreateFileA()：创建一个新文件。
CreateFileW()：创建一个新文件。
DefineDosDeviceW()->DefineDosDevice()：为指定的设备名称分配一个驱动器号。
DeleteFileA()：删除指定文件。
DeleteFileW()：删除指定文件。
*/

/* 11-20
DeleteVolumeMountPointW()：接受一个宽字符串作为参数，表示要删除的挂载点路径。
FindClose()：关闭文件查找句柄。
FindCloseChangeNotification()：关闭通知句柄。
FindFirstChangeNotificationA()：获取更改通知句柄。
FindFirstChangeNotificationW()：获取更改通知句柄。
FindFirstFileA()：查找符合条件的第一个文件。
FindFirstFileExA()：用于获取下一个匹配的文件，直到所有匹配的文件都被找到。
FindFirstFileExW()：函数用于打开查找句柄并获得第一个匹配的文件。
FindFirstFileNameW()：函数来查找第一个与该模式匹配的文件。
FindFirstFileW()：在指定路径下搜索所有文件，并将每个找到的文件名输出到标准输出流中。
*/

/* 21-30
FindFirstStreamW()：用于查找指定文件的数据流。
FindFirstVolumeW()：用于查找计算机上的卷名称。
FindNextChangeNotification()：用于等待一个由FindFirstChangeNotification函数创建的监视器的通知。
FindNextFileA()：用来查找下一个文件或目录。
FindNextFileNameW()：可以用于查找与FindFirstFileNameW函数返回的搜索句柄匹配的下一个文件名。
FindNextFileW()：用于查找与FindFirstFileW函数返回的搜索句柄匹配的下一个文件。
FindNextStreamW()：用于查找与FindFirstStreamW函数返回的搜索句柄匹配的下一个数据流。
FindNextVolumeW()：检索下一个卷名，直到没有更多的卷名可用。
FindVolumeClose()：关闭搜索句柄。
GetCompressedFileSizeA()：用于获取指定文件的压缩大小。
*/

/* 31-40
GetCompressedFileSizeW()：用于获取指定文件的压缩大小。
GetDiskFreeSpaceA()：获取磁盘的空闲空间信息，包括每簇的扇区数、每扇区的字节数、可用簇数和总簇数。
GetDiskFreeSpaceExA()：获取磁盘的空闲空间信息，包括可用空间、总空间和总空闲空间。
GetDiskFreeSpaceExW()：获取磁盘的空闲空间信息，包括可用空间、总空间和总空闲空间。
GetDiskFreeSpaceW()：获取磁盘的空闲空间信息，包括每簇的扇区数、每扇区的字节数、可用簇数和总簇数。
GetDriveTypeA()：获取指定驱动器的类型。
GetDriveTypeW()：获取指定驱动器的类型。
GetFileAttributesA()：获取指定文件或目录的属性。
GetFileAttributesExA()：获取指定文件或目录的文件属性信息，如文件大小、创建时间、最后访问时间、最后修改时间等。
GetFileAttributesW()：获取指定文件或目录的文件属性信息，如文件大小、创建时间、最后访问时间、最后修改时间等。
*/

/* 41-50
GetFileInformationByHandle()：获取指定文件的文件信息，如文件大小、分配大小、文件链接数、是否待删除、是否为目录等。
GetFileSize()：获取指定文件的文件大小，以字节为单位。
GetFileSizeEx()：获取指定文件的文件大小，以字节为单位。
GetFileType()：获取指定文件句柄的文件类型。
GetFinalPathNameByHandleA()：用于获取指定文件句柄的最终路径名。
GetFinalPathNameByHandleW()：用于获取指定文件句柄的最终路径名。
GetFullPathNameA()：获取完整路径名。
GetFullPathNameW()：获取完整路径名。
GetLogicalDrives()：获取系统上所有可用的逻辑驱动器的位掩码。
GetLogicalDriveStringsW()：获取系统上所有可用的逻辑驱动器的名称，并打印出哪些驱动器是存在的。
*/

int A()
{
    //tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);

    // 获取当前系统的 ANSI 代码页
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    UINT codePage = GetACP();
    std::cout << "ANSI code page: " << codePage << std::endl;

    // 检查是否启用了短名称
    const char* path = "C:\\"; // 需要检查的路径
    char volName[MAX_PATH + 1];
    DWORD volSerialNum;
    DWORD maxFileNameLen;
    DWORD fileSystemFlags;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    BOOL success = GetVolumeInformation((LPCWSTR)path, (LPWSTR)volName, MAX_PATH, &volSerialNum, &maxFileNameLen, &fileSystemFlags, NULL, 0);

    if (success && (fileSystemFlags & FILE_CASE_PRESERVED_NAMES) && (fileSystemFlags & FILE_CASE_SENSITIVE_SEARCH) && !(fileSystemFlags & FILE_UNICODE_ON_DISK))
    {
        printf("Short names are enabled.\n");
    }
    else
    {
        printf("Short names are not enabled.\n");
    }

    // 函数创建一个名为"MyFolder1"的新目录
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    success = CreateDirectoryA("C:\\MyFolder1", NULL);
    if (!success)
    {
        std::cerr << "Error creating directory: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "Directory created successfully." << std::endl;

    // 函数创建一个名为"MyFolder2"的新目录
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    success = CreateDirectoryW(L"C:\\MyFolder2", NULL);
    if (!success)
    {
        std::cerr << "Error creating directory: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "Directory created successfully." << std::endl;

    HANDLE hFile;

    // 创建一个名为"example1.txt"的新文件
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    hFile = CreateFile2(L"example1.txt", GENERIC_WRITE, 0, CREATE_ALWAYS, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Error creating file: " << GetLastError() << std::endl;
        return 1;
    }
    CloseHandle(hFile);

    // 创建一个名为"example2.txt"的新文件
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    hFile = CreateFileA("example2.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Error creating file: " << GetLastError() << std::endl;
        return 1;
    }

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    DWORD fileSize = GetFileSize(hFile, NULL);

    LARGE_INTEGER filesize;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (GetFileSizeEx(hFile, &filesize))
    {
        std::cout << "File size of " << "example2.txt" << ": " << filesize.QuadPart << " bytes" << std::endl;
    }
    else
    {
        std::cerr << "Failed to get file size" << std::endl;
    }

    FILE_STANDARD_INFO fileinfo;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    success = GetFileInformationByHandleEx(hFile, FileStandardInfo, &fileinfo, sizeof(fileinfo));

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    DWORD fileType = GetFileType(hFile);
    if (fileType == FILE_TYPE_DISK)
    {
        std::cout << "File type of " << "example2.txt" << ": Disk file" << std::endl;
    }
    else if (fileType == FILE_TYPE_CHAR)
    {
        std::cout << "File type of " << "example2.txt" << ": Character file" << std::endl;
    }
    else if (fileType == FILE_TYPE_PIPE)
    {
        std::cout << "File type of " << "example2.txt" << ": Pipe file" << std::endl;
    }
    else
    {
        std::cout << "File type of " << "example2.txt" << ": Unknown" << std::endl;
    }

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    DWORD pathLen = GetFinalPathNameByHandleA(hFile, (LPSTR)path, MAX_PATH, VOLUME_NAME_NT);
    if (pathLen == 0)
    {
        std::cerr << "Failed to get file path" << std::endl;
    }
    else
    {
        std::cout << "Final path of " << "example2.txt" << ": " << (LPSTR)path << std::endl;
    }

    CloseHandle(hFile);

    // 创建一个名为"example3.txt"的新文件
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    hFile = CreateFileW(L"example3.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) // 检查文件是否成功打开
    {
        std::cout << "File open failed." << std::endl;
        return 1;
    }

    path[MAX_PATH];
    pathLen = GetFinalPathNameByHandleW(hFile, (LPWSTR)path, MAX_PATH, VOLUME_NAME_NT);
    if (pathLen == 0)
    {
        std::cerr << "Failed to get file path" << std::endl;
    }
    else
    {
        std::wcout << L"Final path of " << L"example3.txt" << L": " << path << std::endl;
    }

    CloseHandle(hFile); // 关闭文件句柄

    LPCWSTR deviceName = L"MyDevice"; // 设备名称
    LPCWSTR devicePath = L"\\Device\\MyDevice"; // 设备路径
    WCHAR driveLetter = L'X'; // 驱动器号

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!DefineDosDeviceW(DDD_RAW_TARGET_PATH, deviceName, devicePath))
    {
        std::cout << "DefineDosDeviceW failed. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    WCHAR targetPath[MAX_PATH];
    wsprintfW(targetPath, L"%c:\\", driveLetter);

    // 为指定的设备名称分配一个驱动器号
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!DefineDosDeviceW(DDD_RAW_TARGET_PATH, &driveLetter, targetPath))
    {
        std::cout << "DefineDosDeviceW failed. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    // 删除设备别名
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!DefineDosDeviceW(DDD_REMOVE_DEFINITION, deviceName, NULL))
    {
        std::cout << "RemoveDosDeviceW failed. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    // 调用 DeleteFileA 函数来删除该文件
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    success = DeleteFileA("example1.txt");
    if (success) {
        printf("文件删除成功！\n");
    }
    else {
        printf("文件删除失败！错误码：%d\n", GetLastError());
    }

    // 调用 DeleteFileW 函数来删除该文件
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    success = DeleteFileW(L"example2.txt");
    if (success) {
        wprintf(L"文件删除成功！\n");
    }
    else {
        wprintf(L"文件删除失败！错误码：%d\n", GetLastError());
    }

    // 调用 DeleteVolumeMountPointW 函数
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    success = DeleteVolumeMountPointW(L"C:\\MyMountPoint");
    if (success) {
        wprintf(L"挂载点删除成功！\n");
    }
    else {
        wprintf(L"挂载点删除失败！错误码：%d\n", GetLastError());
    }

    // 查找符合条件的第一个文件
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    HANDLE searchHandle = FindFirstFileA("*.txt", NULL); 
    if (searchHandle == INVALID_HANDLE_VALUE) {
        printf("查找文件失败！错误码：%d\n", GetLastError());
        return 1;
    }

    WIN32_FIND_DATAA data;
    if (searchHandle != INVALID_HANDLE_VALUE) // 检查句柄是否有效
    {
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        do
        {
            std::cout << data.cFileName << std::endl; // 输出文件名
        } while (FindNextFileA(searchHandle, &data)); // 查找下一个文件

        FindClose(searchHandle); // 关闭句柄
    }
    else
    {
        std::cerr << "Error: " << GetLastError() << std::endl; // 输出错误信息
    }

    WIN32_FIND_DATAA findData;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (searchHandle == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: FindFirstFileExA failed with error code " << GetLastError() << std::endl;
        return 1;
    }
    do {
        std::cout << findData.cFileName << std::endl;
    } while (FindNextFileA(searchHandle, &findData));

    // 关闭文件查找句柄
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    success = FindClose(searchHandle); 
    if (success) {
        printf("文件查找句柄已成功关闭！\n");
    }
    else {
        printf("文件查找句柄关闭失败！错误码：%d\n", GetLastError());
    }

    WIN32_FIND_DATAW FindData;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    HANDLE hFind = FindFirstFileExW(
        L"C:\\*.txt",                   // 文件名匹配模式
        FindExInfoStandard,             // 返回的数据类型
        &FindData,                      // 存储文件信息的结构体
        FindExSearchNameMatch,          // 文件名匹配类型
        NULL,                           // 用于搜索通配符的额外字符串
        FIND_FIRST_EX_LARGE_FETCH       // 可选的搜索标记
    );
    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: FindFirstFileExW failed with error code " << GetLastError() << std::endl;
        return 1;
    }
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    do {
        std::wcout << FindData.cFileName << std::endl;
    } while (FindNextFileW(hFind, &FindData));
    FindClose(hFind);

    const wchar_t* directory = L"D:\\";
    const wchar_t* pattern = L"*.txt";

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    hFind = FindFirstFileNameW(directory, 0, nullptr, (PWSTR)pattern);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to find matching file." << std::endl;
        return 1;
    }

    WCHAR buffer[MAX_PATH];
    DWORD dwResult = 0;
    if (hFind != INVALID_HANDLE_VALUE) // 检查句柄是否有效
    {
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        do
        {
            std::wcout << buffer << std::endl; // 输出文件名
        } while (FindNextFileNameW(hFind, &dwResult, buffer)); // 查找下一个文件名

        FindClose(hFind); // 关闭句柄
    }
    else
    {
        std::cerr << "Error: " << GetLastError() << std::endl; // 输出错误信息
    }

    FindClose(hFind);


    WIN32_FIND_DATAW fileData;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    hFind = FindFirstFileW(L"D:\\*", &fileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        do {
            // 输出找到的文件名
            std::wcout << fileData.cFileName << std::endl;
        } while (FindNextFileW(hFind, &fileData) != 0);
        FindClose(hFind);
    }
    else {
        std::cout << "无法找到指定路径：" << (LPSTR)path << std::endl;
    }

    // 获取更改通知句柄
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    HANDLE notificationHandle = FindFirstChangeNotification(
        L"D:\\",    // 目录路径
        FALSE,                    // 不监视子目录
        FILE_NOTIFY_CHANGE_LAST_WRITE   // 当文件被写入或删除时通知
    );

    // 关闭通知句柄。
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!FindCloseChangeNotification(notificationHandle))
    {
        std::cerr << "Error: FindCloseChangeNotification failed with error code " << GetLastError() << std::endl;
        return 1;
    }

    // 获取更改通知句柄
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    notificationHandle = FindFirstChangeNotificationA(
        "D:\\",            // 目录路径
        FALSE,                    // 不监视子目录
        FILE_NOTIFY_CHANGE_LAST_WRITE   // 当文件被写入或删除时通知
    );

    if (notificationHandle == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Error: FindFirstChangeNotification failed with error code " << GetLastError() << std::endl;
        return 1;
    }

    // 关闭通知句柄
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!FindCloseChangeNotification(notificationHandle))
    {
        std::cerr << "Error: FindCloseChangeNotification failed with error code " << GetLastError() << std::endl;
        return 1;
    }

    // 获取更改通知句柄
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    notificationHandle = FindFirstChangeNotificationW(
        L"D:\\",            // 目录路径
        FALSE,                    // 不监视子目录
        FILE_NOTIFY_CHANGE_LAST_WRITE   // 当文件被写入或删除时通知
    );

    // 关闭通知句柄
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (!FindCloseChangeNotification(notificationHandle))
    {
        std::cerr << "Error: FindCloseChangeNotification failed with error code " << GetLastError() << std::endl;
        return 1;
    }

    LPCWSTR filename = L"example.txt:stream1"; // 指定带有流名称的文件路径
    WIN32_FIND_STREAM_DATA streamData;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    HANDLE hStream = FindFirstStreamW(filename, FindStreamInfoStandard, &streamData, 0); // 调用FindFirstStreamW函数

    DWORD dwStreamCount = 0;
    if (hStream != INVALID_HANDLE_VALUE) // 检查句柄是否有效
    {
        std::wcout << "Stream name: " << streamData.cStreamName << std::endl; // 输出流的名称
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        do
        {
            std::wcout << "Data Stream #" << ++dwStreamCount << ": " << buffer << std::endl; // 输出数据流名称
        } while (FindNextStreamW(hFind, buffer)); // 查找下一个数据流
        FindClose(hStream); // 关闭查找句柄
    }
    else
    {
        std::cerr << "Error: " << GetLastError() << std::endl; // 输出错误信息
    }

    WCHAR volumeName[MAX_PATH];

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    HANDLE hFindVolume = FindFirstVolumeW(volumeName, ARRAYSIZE(volumeName)); // 调用FindFirstVolumeW函数

    if (hFindVolume != INVALID_HANDLE_VALUE) // 检查句柄是否有效
    {
        std::wcout << "Volume name: " << volumeName << std::endl; // 输出卷名称

        while (true) {
            std::wcout << volumeName << std::endl;

            tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
            if (!FindNextVolumeW(hFindVolume, volumeName, ARRAYSIZE(volumeName))) {
                if (GetLastError() == ERROR_NO_MORE_FILES) {
                    break;
                }
                else {
                    std::cout << "FindNextVolumeW failed." << std::endl;
                    FindVolumeClose(hFindVolume);
                    return 1;
                }
            }
        }

        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        FindVolumeClose(hFindVolume); // 关闭查找句柄
    }
    else
    {
        std::cerr << "Error: " << GetLastError() << std::endl; // 输出错误信息
    }

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    HANDLE hChange = FindFirstChangeNotification(L"D:\\", FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE); // 创建一个监视器

    if (hChange != INVALID_HANDLE_VALUE) // 检查句柄是否有效
    {
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        DWORD waitStatus = WaitForSingleObject(hChange, INFINITE); // 等待通知
        if (waitStatus == WAIT_OBJECT_0) // 检查等待状态是否成功
        {
            std::cout << "File changes detected!" << std::endl;
            tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
            FindNextChangeNotification(hChange); // 等待下一个通知
        }
        else
        {
            std::cerr << "Error: " << GetLastError() << std::endl; // 输出错误信息
        }
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        FindCloseChangeNotification(hChange); // 关闭监视器句柄
    }
    else
    {
        std::cerr << "Error: " << GetLastError() << std::endl; // 输出错误信息
    }

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    fileSize = GetCompressedFileSizeA("example.txt", NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        std::cout << "GetCompressedFileSizeA failed." << std::endl;
        return 1;
    }

    std::cout << "The compressed file size is: " << fileSize << " bytes." << std::endl;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    fileSize = GetCompressedFileSizeW(L"example.txt", NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        std::cout << "GetCompressedFileSizeW failed." << std::endl;
        return 1;
    }

    std::cout << "The compressed file size is: " << fileSize << " bytes." << std::endl;

    char drive[] = "C:\\";
    DWORD sectorsPerCluster;
    DWORD bytesPerSector;
    DWORD numberOfFreeClusters;
    DWORD totalNumberOfClusters;
    BOOL result;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    result = GetDiskFreeSpaceA(
        drive,
        &sectorsPerCluster,
        &bytesPerSector,
        &numberOfFreeClusters,
        &totalNumberOfClusters
    );

    if (result) {
        printf("Sectors per cluster: %d\n", sectorsPerCluster);
        printf("Bytes per sector: %d\n", bytesPerSector);
        printf("Number of free clusters: %d\n", numberOfFreeClusters);
        printf("Total number of clusters: %d\n", totalNumberOfClusters);
    }
    else {
        printf("Error getting disk free space.\n");
    }

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    result = GetDiskFreeSpaceW(
        (LPCWSTR)drive,
        &sectorsPerCluster,
        &bytesPerSector,
        &numberOfFreeClusters,
        &totalNumberOfClusters
    );

    if (result) {
        wprintf(L"Sectors per cluster: %d\n", sectorsPerCluster);
        wprintf(L"Bytes per sector: %d\n", bytesPerSector);
        wprintf(L"Number of free clusters: %d\n", numberOfFreeClusters);
        wprintf(L"Total number of clusters: %d\n", totalNumberOfClusters);
    }
    else {
        wprintf(L"Error getting disk free space.\n");
    }

    ULARGE_INTEGER freeBytesAvailable;
    ULARGE_INTEGER totalNumberOfBytes;
    ULARGE_INTEGER totalNumberOfFreeBytes;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    result = GetDiskFreeSpaceExA(
        drive,
        &freeBytesAvailable,
        &totalNumberOfBytes,
        &totalNumberOfFreeBytes
    );

    if (result) {
        printf("Free bytes available on drive: %llu\n", freeBytesAvailable.QuadPart);
        printf("Total number of bytes on drive: %llu\n", totalNumberOfBytes.QuadPart);
        printf("Total number of free bytes on drive: %llu\n", totalNumberOfFreeBytes.QuadPart);
    }
    else {
        printf("Error getting disk free space.\n");
    }

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    result = GetDiskFreeSpaceExW(
        (LPCWSTR)drive,
        &freeBytesAvailable,
        &totalNumberOfBytes,
        &totalNumberOfFreeBytes
    );

    if (result) {
        wprintf(L"Free bytes available on drive: %llu\n", freeBytesAvailable.QuadPart);
        wprintf(L"Total number of bytes on drive: %llu\n", totalNumberOfBytes.QuadPart);
        wprintf(L"Total number of free bytes on drive: %llu\n", totalNumberOfFreeBytes.QuadPart);
    }
    else {
        wprintf(L"Error getting disk free space.\n");
    }

    UINT driveType;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    driveType = GetDriveTypeA("C:\\");

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    driveType = GetDriveTypeW(L"C:\\");

    DWORD attributes;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    attributes = GetFileAttributesA("example.txt");

    if (attributes == INVALID_FILE_ATTRIBUTES) {
        printf("Failed to get file attributes.\n");
        return 1;
    }

    if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
        printf("%s is a directory.\n", "example.txt");
    }
    else {
        printf("%s is a file.\n", "example.txt");
    }

    if (attributes & FILE_ATTRIBUTE_HIDDEN) {
        printf("%s is hidden.\n", "example.txt");
    }

    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    success = GetFileAttributesExA("example.txt", GetFileExInfoStandard, &fileInfo);

    if (success)
    {
        std::cout << "File attributes for " << "example.txt" << ":" << std::endl;
        std::cout << "Size: " << fileInfo.nFileSizeLow << std::endl;
        std::cout << "Creation time: " << fileInfo.ftCreationTime.dwLowDateTime << std::endl;
        std::cout << "Last access time: " << fileInfo.ftLastAccessTime.dwLowDateTime << std::endl;
        std::cout << "Last write time: " << fileInfo.ftLastWriteTime.dwLowDateTime << std::endl;
    }
    else
    {
        std::cerr << "Failed to get file attributes for " << "example.txt" << std::endl;
    }

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    DWORD FileInfo = GetFileAttributesW(L"example.txt");

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    result = GetFullPathNameA("example.txt", MAX_PATH, (LPSTR)buffer, nullptr);

    if (result != 0) {
        std::cout << "Full path of " << filename << ": " << buffer << std::endl;
    }
    else {
        std::cerr << "Failed to get full path of " << filename << std::endl;
    }

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    result = GetFullPathNameW(L"example.txt", MAX_PATH, buffer, nullptr);
    if (result == 0) {
        std::cout << "Failed to retrieve full path." << std::endl;
        return 1;
    }
    std::wcout << "Full path: " << buffer << std::endl;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    DWORD drives = GetLogicalDrives();
    if (drives == 0) {
        std::cout << "Failed to retrieve logical drives." << std::endl;
        return 1;
    }
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    result = GetLogicalDriveStringsW(MAX_PATH, buffer);
    if (result == 0) {
        std::cout << "Failed to retrieve logical drive strings." << std::endl;
        return 1;
    }

    return 0;
}


void CALLBACK readCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
    if (dwErrorCode == 0) {
        cout << "Read " << dwNumberOfBytesTransfered << " bytes: " << (char*)lpOverlapped->hEvent << endl;
    }
    else {
        cerr << "Error: " << dwErrorCode << endl;
    }
}

int B()
{
    //tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    {

        // Define the file path
        const char* filePathA = "C:\\Windows\\System32\\notepad.exe";
        const wchar_t* filePathW = L"C:\\Windows\\System32\\notepad.exe";

        // Define the buffer and buffer size
        char fullPathA[MAX_PATH];
        wchar_t fullPathW[MAX_PATH];

        // Retrieve the full path using GetFullPathNameA
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        DWORD resultA = GetFullPathNameA(filePathA, MAX_PATH, fullPathA, NULL);

        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        // Retrieve the full path using GetFullPathNameW
        DWORD resultW = GetFullPathNameW(filePathW, MAX_PATH, fullPathW, NULL);

        // Print the results
        cout << "GetFullPathNameA result: " << resultA << endl;
        cout << "Full path (ANSI): " << fullPathA << endl;

        wcout << L"GetFullPathNameW result: " << resultW << endl;
        wcout << L"Full path (Unicode): " << fullPathW << endl;
    }

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    DWORD drives = GetLogicalDrives();
    // Print information about each logical drive
    for (char driveLetter = 'A'; driveLetter <= 'Z'; ++driveLetter) {
        if (drives & (1 << (driveLetter - 'A'))) {
            std::wcout << "Drive " << driveLetter << " is available.\n";
        }
    }
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    // Get a list of all available logical drives as a null-terminated string
    WCHAR driveStrings[MAX_PATH];
    DWORD numChars = GetLogicalDriveStringsW(MAX_PATH, driveStrings);

    // Print information about each logical drive in the list
    WCHAR* drive = driveStrings;
    while (*drive != '\0') {
        std::wcout << "Drive " << drive[0] << " is available.\n";
        drive += wcslen(drive) + 1;
    }

    // The path to be converted
    const std::wstring longPath = L"C:\\Windows\\explorer.exe";

    // Get the short path name using GetShortPathNameW
    WCHAR shortPath[MAX_PATH];
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    DWORD shortPathLength = GetShortPathNameW(longPath.c_str(), shortPath, MAX_PATH);

    if (shortPathLength == 0)
    {
        std::cout << "Error getting short path name: " << GetLastError() << std::endl;
        return 1;
    }

    std::wcout << L"Short path: " << shortPath << std::endl;

    // Get the long path name using GetLongPathNameW
    WCHAR longPathBuffer[MAX_PATH];
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    DWORD longPathLength = GetLongPathNameW(shortPath, longPathBuffer, MAX_PATH);

    if (longPathLength == 0)
    {
        std::cout << "Error getting long path name: " << GetLastError() << std::endl;
        return 1;
    }

    std::wcout << L"Long path: " << longPathBuffer << std::endl;

    // Convert the long path to ASCII using GetLongPathNameA
    char asciiBuffer[MAX_PATH];
    WideCharToMultiByte(CP_ACP, 0, longPathBuffer, -1, asciiBuffer, MAX_PATH, NULL, NULL);

    std::cout << "ASCII path: " << asciiBuffer << std::endl;

    // Define a buffer to store the temporary path
    char tempPathA[MAX_PATH];
    wchar_t tempPathW[MAX_PATH];

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    // Use GetTempPathA to get the temporary path as ANSI string
    DWORD resultA2 = GetTempPathA(MAX_PATH, tempPathA);
    if (resultA2 == 0) {
        std::cerr << "GetTempPathA failed with error code: " << GetLastError() << std::endl;
        return 1;
    }
    std::cout << "Temporary path (ANSI): " << tempPathA << std::endl;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    // Use GetTempPathW to get the temporary path as Unicode string
    DWORD resultW2 = GetTempPathW(MAX_PATH, tempPathW);
    if (resultW2 == 0) {
        std::cerr << "GetTempPathW failed with error code: " << GetLastError() << std::endl;
        return 1;
    }
    std::wcout << L"Temporary path (Unicode): " << tempPathW << std::endl;


    const wchar_t* volumePath = L"\\\\.\\C:";

    // Use GetVolumeInformationA to retrieve information about the volume in ANSI format
    char volumeNameBuffer[MAX_PATH + 1];
    DWORD volumeSerialNumber;
    DWORD maximumComponentLength;
    DWORD fileSystemFlags;
    char fileSystemNameBuffer[MAX_PATH + 1];
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    GetVolumeInformationA("C:\\", volumeNameBuffer, MAX_PATH + 1, &volumeSerialNumber, &maximumComponentLength, &fileSystemFlags, fileSystemNameBuffer, MAX_PATH + 1);


    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    // Use GetVolumeInformationByHandleW to retrieve information about the volume by its handle in Unicode format
    HANDLE volumeHandle = CreateFileW(volumePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (volumeHandle == INVALID_HANDLE_VALUE)
    {
        cout << "CreateFileW failed." << endl;
        return 1;
    }

    wchar_t volumeNameBufferW[MAX_PATH + 1];
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    GetVolumeInformationByHandleW(volumeHandle, volumeNameBufferW, MAX_PATH + 1, &volumeSerialNumber, &maximumComponentLength, &fileSystemFlags, NULL, 0);

    // Use GetVolumeInformationW to retrieve information about the volume in Unicode format
    wchar_t fileSystemNameBufferW[MAX_PATH + 1];
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    GetVolumeInformationW(volumePath, volumeNameBufferW, MAX_PATH + 1, &volumeSerialNumber, &maximumComponentLength, &fileSystemFlags, fileSystemNameBufferW, MAX_PATH + 1);

    // Display the retrieved information
    cout << "Volume Name (ANSI): " << volumeNameBuffer << endl;
    wcout << L"Volume Name (Unicode): " << volumeNameBufferW << endl;
    cout << "Volume Serial Number: " << volumeSerialNumber << endl;
    cout << "Maximum Component Length: " << maximumComponentLength << endl;
    cout << "File System Flags: " << fileSystemFlags << endl;
    wcout << L"File System Name: " << fileSystemNameBufferW << endl;

    {
        WCHAR buffer[32768];
        DWORD bufferLength = sizeof(buffer);

        // Call the QueryDosDeviceW function to retrieve the device names
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        if (!QueryDosDeviceW(nullptr, buffer, bufferLength))
        {
            std::cerr << "QueryDosDeviceW failed with error code " << GetLastError() << std::endl;
            return 1;
        }

        // Loop through the list of device names and print them to the console
        std::wstring deviceNames(buffer);
        std::size_t pos = 0;
        while (pos < deviceNames.length())
        {
            std::wstring deviceName = deviceNames.substr(pos);
            std::size_t nullPos = deviceName.find(L'\0');
            if (nullPos != std::wstring::npos)
            {
                deviceName.resize(nullPos);
                pos += nullPos + 1;
            }
            else
            {
                pos = deviceNames.length();
            }

            std::wcout << deviceName << std::endl;
        }
    }


    {

        // Get the name of the current volume mount point
        wchar_t mountPoint[MAX_PATH];
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        GetCurrentDirectoryW(MAX_PATH, mountPoint);

        // Get the name of the volume associated with the mount point
        wchar_t volumeName[MAX_PATH];
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        GetVolumeNameForVolumeMountPointW(mountPoint, volumeName, MAX_PATH);

        // Get a list of paths associated with the volume
        std::vector<std::wstring> volumePaths;
        DWORD bufferSize = 0;
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        GetVolumePathNamesForVolumeNameW(volumeName, nullptr, 0, &bufferSize);

        wchar_t* buffer = new wchar_t[bufferSize];
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        GetVolumePathNamesForVolumeNameW(volumeName, buffer, bufferSize, &bufferSize);

        wchar_t* currentPath = buffer;
        while (*currentPath != '\0')
        {
            volumePaths.push_back(currentPath);
            currentPath += wcslen(currentPath) + 1;
        }

        delete[] buffer;

        // Print out the information we retrieved
        std::wcout << "Mount point: " << mountPoint << std::endl;
        std::wcout << "Volume name: " << volumeName << std::endl;
        std::wcout << "Volume paths: ";
        for (const auto& path : volumePaths)
        {
            std::wcout << path << "; ";
        }
        std::wcout << std::endl;

    }

    const char* filenameA = "example.txt";
    const wchar_t* filenameW = L"example.txt";


    // Get the current file attributes
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    DWORD attributesA = GetFileAttributesA(filenameA);
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    DWORD attributesW = GetFileAttributesW(filenameW);

    // Print the current attributes
    std::cout << "Current attributes (A): " << attributesA << std::endl;
    std::wcout << L"Current attributes (W): " << attributesW << std::endl;

    // Change the file attributes
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (SetFileAttributesA(filenameA, FILE_ATTRIBUTE_HIDDEN) &&
        SetFileAttributesW(filenameW, FILE_ATTRIBUTE_HIDDEN))
    {
        std::cout << "File attributes updated successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to update file attributes." << std::endl;
    }

    // Get the updated file attributes
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    attributesA = GetFileAttributesA(filenameA);
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    attributesW = GetFileAttributesW(filenameW);

    // Print the updated attributes
    std::cout << "Updated attributes (A): " << attributesA << std::endl;
    std::wcout << L"Updated attributes (W): " << attributesW << std::endl;

    WCHAR lpPathBuffer[MAX_PATH];
    WCHAR lpPrefixString[] = L"TEMP_";
    UINT uUnique = 0;

    // Get the path to the temporary directory
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    DWORD dwRetVal = GetTempPathW(MAX_PATH, lpPathBuffer);
    if (dwRetVal > MAX_PATH || dwRetVal == 0) {
        cout << "Error: Unable to get temporary path.\n";
        return 1;
    }
    wcout << "Temporary path: " << lpPathBuffer << endl;

    // Generate a temporary file name
    WCHAR lpTempFileName[MAX_PATH];
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    dwRetVal = GetTempFileNameW(lpPathBuffer, lpPrefixString, uUnique, lpTempFileName);
    if (dwRetVal == 0) {
        cout << "Error: Unable to generate temporary file name.\n";
        return 1;
    }
    wcout << "Temporary file name: " << lpTempFileName << endl;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    HANDLE hFile = CreateFileA("example.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        cout << "Error opening file!" << endl;
        return 1;
    }

    time_t currentTime = time(nullptr);

    // Convert the time to a FILETIME structure
    FILETIME ft;
    ULARGE_INTEGER ull;
    ull.QuadPart = currentTime * 10000000ULL + 116444736000000000ULL;
    ft.dwLowDateTime = ull.LowPart;
    ft.dwHighDateTime = ull.HighPart;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    BOOL bResult = SetFileTime(hFile, &ft, &ft, &ft);
    if (bResult == FALSE) {
        cout << "Error: Unable to set file times.\n";
        CloseHandle(hFile);
        return 1;
    }

    OVERLAPPED ov = { 0 };
    ov.Offset = 0;
    ov.OffsetHigh = 0;
    ov.hEvent = NULL;

    char data[] = "This is an example text.";
    DWORD bytesWritten = 0;
    DWORD bytesToWrite = sizeof(data);
    DWORD bytesWrittenEx = 0;
    DWORD bytesToWriteEx = sizeof(data);

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    BOOL bResult1 = WriteFile(hFile, data, bytesToWrite, &bytesWritten, NULL);
    if (!bResult1) {
        cout << "Error writing to file!" << endl;
        CloseHandle(hFile);
        return 1;
    }

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    BOOL bResult2 = WriteFileEx(hFile, data, bytesToWriteEx, &ov, NULL);
    if (!bResult2) {
        if (GetLastError() != ERROR_IO_PENDING) {
            cout << "Error writing to file using WriteFileEx!" << endl;
            CloseHandle(hFile);
            return 1;
        }
    }

    CloseHandle(hFile);

    {
        // Open file handle
        HANDLE hFile = CreateFile(L"example.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            std::cout << "Failed to open file" << GetLastError() << std::endl;
            return 1;
        }

        // Set file size to 1MB
        LARGE_INTEGER fileSize;
        fileSize.QuadPart = 1024 * 1024;
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        if (!SetFileInformationByHandle(hFile, FileEndOfFileInfo, &fileSize, sizeof(fileSize))) {
            std::cout << "Failed to set file size" << std::endl;
            CloseHandle(hFile);
            return 1;
        }

        // Set file creation time to current time
        FILETIME creationTime, lpLastAccessTime, lpLastWriteTime;
        SYSTEMTIME st;
        GetSystemTime(&st);
        SystemTimeToFileTime(&st, &creationTime);
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        SetFileTime(hFile, &creationTime, &lpLastAccessTime, &lpLastWriteTime);

        // Close file handle
        CloseHandle(hFile);
    }

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    hFile = CreateFileA("example.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        cout << "Error opening file!" << endl;
        return 1;
    }

    DWORD bytesRead = 0;
    char buffer[256];
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    if (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL))
    {
        cout << "Read " << bytesRead << " bytes using ReadFile function: " << buffer << endl;
    }
    else
    {
        cout << "Error: Unable to read file using ReadFile function!" << endl;
    }

    CloseHandle(hFile);

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    hFile = CreateFileA("example.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        cout << "Error opening file!" << endl;
        return 1;
    }

    OVERLAPPED overlapped = { 0 };
    overlapped.hEvent = buffer;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    BOOL result = ReadFileEx(hFile, buffer, sizeof(buffer), &overlapped, readCompletion);
    if (!result) {
        cerr << "Error reading file" << endl;
        return 1;
    }

    CloseHandle(hFile);

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    hFile = CreateFileA("example.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        cout << "Error opening file!" << endl;
        return 1;
    }

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    DWORD dwFileSize = GetFileSize(hFile, NULL);
    DWORD dwBytesToLock = dwFileSize / 2;
    DWORD dwBytesToUnlock = dwFileSize - dwBytesToLock;
    DWORD dwLockOffset = 0;
    DWORD dwUnlockOffset = dwBytesToLock;
    LPOVERLAPPED lpOverlapped = NULL;

    // Lock the first half of the file
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    BOOL lockSuccess = LockFile(hFile, dwLockOffset, 0, dwBytesToLock, 0);
    if (!lockSuccess) {
        cout << "Error: Unable to lock file.\n";
        CloseHandle(hFile);
        return 1;
    }
    cout << "Successfully locked the first " << dwBytesToLock << " bytes of the file.\n";

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    // Unlock the first half of the file using UnlockFile
    BOOL unlockSuccess = UnlockFile(hFile, dwLockOffset, 0, dwBytesToLock, 0);
    if (!unlockSuccess) {
        cout << "Error: Unable to unlock file.\n";
        CloseHandle(hFile);
        return 1;
    }
    cout << "Successfully unlocked the first " << dwBytesToLock << " bytes of the file using UnlockFile.\n";

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    // Lock the second half of the file using LockFileEx
    OVERLAPPED overlapped1 = { 0 };
    overlapped1.Offset = dwUnlockOffset;
    overlapped1.OffsetHigh = 0;
    BOOL lockExSuccess = LockFileEx(hFile, LOCKFILE_EXCLUSIVE_LOCK, 0, dwBytesToUnlock, 0, &overlapped1);
    if (!lockExSuccess) {
        cout << "Error: Unable to lock file.\n";
        CloseHandle(hFile);
        return 1;
    }
    cout << "Successfully locked the last " << dwBytesToUnlock << " bytes of the file using LockFileEx.\n";

    // Unlock the second half of the file using UnlockFileEx
    OVERLAPPED overlapped2 = { 0 };
    overlapped2.Offset = dwUnlockOffset;
    overlapped2.OffsetHigh = 0;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    BOOL unlockExSuccess = UnlockFileEx(hFile, 0, dwBytesToUnlock, 0, &overlapped2);
    if (!unlockExSuccess) {
        cout << "Error: Unable to unlock file.\n";
        CloseHandle(hFile);
        return 1;
    }
    cout << "Successfully unlocked the last " << dwBytesToUnlock << " bytes of the file using UnlockFileEx.\n";


    ULONGLONG length = 4096;
    UCHAR offset = 0;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    DWORD errorCode = SetFileIoOverlappedRange(hFile, &offset, length);
    if (errorCode != ERROR_SUCCESS) {
        cout << "Error: Unable to reserve file range for overlapped I/O.\n";
        CloseHandle(hFile);
        return 1;
    }

    cout << "File range reserved for overlapped I/O successfully.\n";

    {
        HANDLE hFile = CreateFileA("test", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            printf("Failed to create or open file.\n");
            return 1;
        }

        // Move the file pointer to the beginning of the file
        DWORD dwOffset = 0;
        DWORD dwMoveMethod = FILE_BEGIN;
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        DWORD dwNewFilePointer = SetFilePointer(hFile, dwOffset, NULL, dwMoveMethod);

        if (dwNewFilePointer == INVALID_SET_FILE_POINTER)
        {
            printf("Failed to move file pointer.\n");
            CloseHandle(hFile);
            return 1;
        }

        // Write some data to the file
        char buffer[] = "This is a test";
        DWORD dwBytesWritten;
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        BOOL bSuccess = WriteFile(hFile, buffer, sizeof(buffer), &dwBytesWritten, NULL);

        if (!bSuccess)
        {
            printf("Failed to write data to file.\n");
            CloseHandle(hFile);
            return 1;
        }

        // Move the file pointer to the end of the file
        LARGE_INTEGER liDistanceToMove;
        liDistanceToMove.QuadPart = 0;
        LARGE_INTEGER liNewFilePointer;
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        bSuccess = SetFilePointerEx(hFile, liDistanceToMove, &liNewFilePointer, FILE_END);

        if (!bSuccess)
        {
            printf("Failed to move file pointer.\n");
            CloseHandle(hFile);
            return 1;
        }

        // Set the end of the file to the current position of the file pointer
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        bSuccess = SetEndOfFile(hFile);

        if (!bSuccess)
        {
            printf("Failed to set end of file.\n");
            CloseHandle(hFile);
            return 1;
        }

        CloseHandle(hFile);
    }



    {
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        HANDLE hFile = CreateFileA("test.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        FILE_BASIC_INFO fileBasicInfo;
        memset(&fileBasicInfo, 0, sizeof(fileBasicInfo));
        // Set the creation time to a specific value
        FILETIME creationTime;
        // Set the last access time to the current time
        FILETIME lastAccessTime;
        GetSystemTimeAsFileTime(&lastAccessTime);
        // Set the file attributes to read-only
        fileBasicInfo.FileAttributes = FILE_ATTRIBUTE_READONLY;
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        SetFileInformationByHandle(hFile, FileBasicInfo, &fileBasicInfo, sizeof(fileBasicInfo));


        FILE_STANDARD_INFO fileStandardInfo;
        memset(&fileStandardInfo, 0, sizeof(fileStandardInfo));
        // Set the file size to a specific value
        LARGE_INTEGER fileSize;
        fileSize.QuadPart = 1024;
        fileStandardInfo.EndOfFile = fileSize;
        // Set the number of links to the file to 2
        fileStandardInfo.NumberOfLinks = 2;
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        SetFileInformationByHandle(hFile, FileStandardInfo, &fileStandardInfo, sizeof(fileStandardInfo));


        FILE_NAME_INFO fileNameInfo;
        memset(&fileNameInfo, 0, sizeof(fileNameInfo));
        // Set the new name of the file or directory
        const char* newFileName = "newFileName";
        fileNameInfo.FileNameLength = sizeof(newFileName);
        memcpy(fileNameInfo.FileName, newFileName, fileNameInfo.FileNameLength);
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        SetFileInformationByHandle(hFile, FileNameInfo, &fileNameInfo, sizeof(fileNameInfo));

        FILE_RENAME_INFO fileRenameInfo;
        memset(&fileRenameInfo, 0, sizeof(fileRenameInfo));
        // Set the new name of the file or directory
        fileRenameInfo.ReplaceIfExists = TRUE;
        fileRenameInfo.FileNameLength = sizeof(newFileName);
        memcpy(fileRenameInfo.FileName, newFileName, fileRenameInfo.FileNameLength);
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        SetFileInformationByHandle(hFile, FileRenameInfo, &fileRenameInfo, sizeof(fileRenameInfo));

        FILE_DISPOSITION_INFO fileDispositionInfo;
        memset(&fileDispositionInfo, 0, sizeof(fileDispositionInfo));
        // Set the delete pending flag to TRUE
        fileDispositionInfo.DeleteFile = TRUE;
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        SetFileInformationByHandle(hFile, FileDispositionInfo, &fileDispositionInfo, sizeof(fileDispositionInfo));


        FILE_ALLOCATION_INFO fileAllocationInfo;
        memset(&fileAllocationInfo, 0, sizeof(fileAllocationInfo));
        // Set the allocation size to 4096 bytes
        LARGE_INTEGER allocationSize;
        allocationSize.QuadPart = 4096;
        fileAllocationInfo.AllocationSize = allocationSize;
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        SetFileInformationByHandle(hFile, FileAllocationInfo, &fileAllocationInfo, sizeof(fileAllocationInfo));


        FILE_END_OF_FILE_INFO fileEndOfFileInfo;
        memset(&fileEndOfFileInfo, 0, sizeof(fileEndOfFileInfo));
        // Set the end-of-file position to 8192 bytes
        LARGE_INTEGER endOfFile;
        endOfFile.QuadPart = 8192;
        fileEndOfFileInfo.EndOfFile = endOfFile;
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        SetFileInformationByHandle(hFile, FileEndOfFileInfo, &fileEndOfFileInfo, sizeof(fileEndOfFileInfo));

        FILE_STREAM_INFO fileStreamInfo;
        memset(&fileStreamInfo, 0, sizeof(fileStreamInfo));
        // Set the name of the file stream to "AlternateStream"
        fileStreamInfo.StreamNameLength = sizeof("AlternateStream");
        memcpy(fileStreamInfo.StreamName, L"A", 1);

        // Set the flags for the file stream
        fileStreamInfo.StreamAllocationSize.QuadPart = 0;
        fileStreamInfo.StreamSize.QuadPart = 0;
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        SetFileInformationByHandle(hFile, FileStreamInfo, &fileStreamInfo, sizeof(fileStreamInfo));


        FILE_COMPRESSION_INFO fileCompressionInfo;
        memset(&fileCompressionInfo, 0, sizeof(fileCompressionInfo));
        // Enable compression for the file or directory
        fileCompressionInfo.CompressionFormat = COMPRESSION_FORMAT_DEFAULT;
        fileCompressionInfo.CompressionUnitShift = 16;
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        SetFileInformationByHandle(hFile, FileCompressionInfo, &fileCompressionInfo, sizeof(fileCompressionInfo));


        FILE_ATTRIBUTE_TAG_INFO fileAttributeTagInfo;
        memset(&fileAttributeTagInfo, 0, sizeof(fileAttributeTagInfo));
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        SetFileInformationByHandle(hFile, FileAttributeTagInfo, &fileAttributeTagInfo, sizeof(fileAttributeTagInfo));


        FILE_ID_BOTH_DIR_INFO fileIdBothDirInfo;
        memset(&fileIdBothDirInfo, 0, sizeof(fileIdBothDirInfo));
        // Retrieve information about the file or directory
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        GetFileInformationByHandleEx(hFile, FileIdBothDirectoryInfo, &fileIdBothDirInfo, sizeof(fileIdBothDirInfo));


        FILE_ID_EXTD_DIR_INFO fileIdExtdDirInfo;
        memset(&fileIdExtdDirInfo, 0, sizeof(fileIdExtdDirInfo));
        // Retrieve extended information about the file or directory
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        GetFileInformationByHandleEx(hFile, FileIdExtdDirectoryInfo, &fileIdExtdDirInfo, sizeof(fileIdExtdDirInfo));


        FILE_ID_INFO fileIdInfo;
        memset(&fileIdInfo, 0, sizeof(fileIdInfo));
        // Retrieve information about the file or directory
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        GetFileInformationByHandleEx(hFile, FileIdInfo, &fileIdInfo, sizeof(fileIdInfo));

        {
            FILE_NAME_INFO fileNameInfo;
            memset(&fileNameInfo, 0, sizeof(fileNameInfo));
            // Retrieve the name of the file or directory
            tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
            GetFileInformationByHandleEx(hFile, FileNameInfo, &fileNameInfo, sizeof(fileNameInfo));
        }


        FILE_ALIGNMENT_INFO fileAlignmentInfo;
        memset(&fileAlignmentInfo, 0, sizeof(fileAlignmentInfo));
        // Retrieve the alignment requirement for the file
        tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        GetFileInformationByHandleEx(hFile, FileAlignmentInfo, &fileAlignmentInfo, sizeof(fileAlignmentInfo));

        {
            FILE_ATTRIBUTE_TAG_INFO fileAttributeTagInfo;
            memset(&fileAttributeTagInfo, 0, sizeof(fileAttributeTagInfo));
            // Retrieve the attribute tag for the file or directory
            tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
            GetFileInformationByHandleEx(hFile, FileAttributeTagInfo, &fileAttributeTagInfo, sizeof(fileAttributeTagInfo));
        }

        CloseHandle(hFile);
    }


    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    SetFileApisToANSI();
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    SetFileApisToOEM();
    LPCWSTR lpPathNameW = L"C:\\TestDirectory";

    LPCSTR lpPathName = "C:\\TestDirectory";

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    RemoveDirectoryW(lpPathNameW);
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    RemoveDirectoryA(lpPathName);

    CloseHandle(hFile);
    return 0;
}

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    A();
    B();
}