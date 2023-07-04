#include <Windows.h>
#include <iostream>
#include <AccCtrl.h>
#include <Aclapi.h>
#include <sddl.h>
#include"agent.h"

#define UNICODE 

#pragma comment(lib, "Advapi32.lib")

int main()
{
    ss_hypercall(SS_HC_SUBMIT_CR3, 0);
    ss_hypercall(SS_HC_BEGIN_TEST, 0);
    
    PSID pSid = NULL; 

    if (!ConvertStringSidToSid(TEXT("S-1-5-10"), &pSid)) {
        printf("Failed to convert SID string.\n");
        return 1;
    }
    

    HANDLE hFile = CreateFileW(L"temp", GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to open the file." << std::endl;
        return 1;
    }

  
    
    {
       
        DWORD dwRes;
        PACL pDacl = NULL;
        PSECURITY_DESCRIPTOR pSD = NULL;
        PSID pSidOwner = NULL;
        BOOL bOwnerDefaulted = FALSE;

        // Get the object's security descriptor.
        dwRes = GetSecurityInfo(
            hFile,
            SE_FILE_OBJECT,
            DACL_SECURITY_INFORMATION,
            &pSidOwner,
            NULL,
            &pDacl,
            NULL,
            &pSD);

        if (dwRes != ERROR_SUCCESS)
        {
            printf("Could not get security info (error %d)\n", dwRes);
            CloseHandle(hFile);
            return 1;
        }

        DWORD dwGrantedAccess;
        // Check access.
        BOOL bAccessGranted;
        dwRes = AccessCheck(
            pSD,
            GetCurrentProcessToken(),
            GENERIC_READ,
            NULL,
            NULL,
            NULL,
            &dwGrantedAccess,
            &bAccessGranted);

        if (dwRes != ERROR_SUCCESS)
        {
            printf("Could not check access (error %d)\n", dwRes);
            LocalFree(pSD);
            CloseHandle(hFile);
            return 1;
        }

        if (bAccessGranted)
        {
            printf("Access granted\n");
        }
        else
        {
            printf("Access denied\n");
        }

        LocalFree(pSD);

    }
    
    

    {
        DWORD dwRes;
        PACL pDacl = NULL;
        PSECURITY_DESCRIPTOR pSD = NULL;
        PSID pSidOwner = NULL;
        BOOL bOwnerDefaulted = FALSE;

        // Get the object's security descriptor.
        dwRes = GetSecurityInfo(
            hFile,
            SE_FILE_OBJECT,
            DACL_SECURITY_INFORMATION,
            &pSidOwner,
            NULL,
            &pDacl,
            NULL,
            &pSD);

        if (dwRes != ERROR_SUCCESS)
        {
            printf("Could not get security info (error %d)\n", dwRes);
            CloseHandle(hFile);
            return 1;
        }

        DWORD dwGrantedAccess;
        // Check access.
        BOOL bAccessGranted;
        dwRes = AccessCheckByType(
            pSD,
            pSid,
            GetCurrentProcessToken(),
            GENERIC_READ,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            &dwGrantedAccess,
            &bAccessGranted);

        if (dwRes != ERROR_SUCCESS)
        {
            printf("Could not check access (error %d)\n", dwRes);
            LocalFree(pSD);
            CloseHandle(hFile);
            return 1;
        }

        if (bAccessGranted)
        {
            printf("Access granted\n");
        }
        else
        {
            printf("Access denied\n");
        }

        LocalFree(pSD);
    }

   
    {
        DWORD dwRes;
        PACL pDacl = NULL;
        PSECURITY_DESCRIPTOR pSD = NULL;
        PSID pSidOwner = NULL;
        BOOL bOwnerDefaulted = FALSE;

        // Get the object's security descriptor.
        dwRes = GetSecurityInfo(
            hFile,
            SE_FILE_OBJECT,
            DACL_SECURITY_INFORMATION,
            &pSidOwner,
            NULL,
            &pDacl,
            NULL,
            &pSD);

        if (dwRes != ERROR_SUCCESS)
        {
            printf("Could not get security info (error %d)\n", dwRes);
            CloseHandle(hFile);
            return 1;
        }

        DWORD dwGrantedAccess;
        // Check access.
        DWORD bAccessGranted;
        dwRes = AccessCheckByTypeResultList(
            pSD,
            pSid,
            GetCurrentProcessToken(),
            GENERIC_READ,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            &dwGrantedAccess,
            &bAccessGranted);

        if (dwRes != ERROR_SUCCESS)
        {
            printf("Could not check access (error %d)\n", dwRes);
            LocalFree(pSD);
            CloseHandle(hFile);
            return 1;
        }

        if (bAccessGranted)
        {
            printf("Access granted\n");
        }
        else
        {
            printf("Access denied\n");
        }

        LocalFree(pSD);

    }

    
    {
        HANDLE hToken;
        DWORD dwSize;
        PTOKEN_GROUPS pGroups;

        // Open the access token for the current process.
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS_P, &hToken))
        {
            printf("OpenProcessToken failed (error %d)\n", GetLastError());
            return 1;
        }

        // Get the current group information.
        if (!GetTokenInformation(hToken, TokenGroups, NULL, 0, &dwSize) && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
        {
            printf("GetTokenInformation failed (error %d)\n", GetLastError());
            CloseHandle(hToken);
            return 1;
        }

        pGroups = (PTOKEN_GROUPS)LocalAlloc(LPTR, dwSize);

        if (pGroups == NULL)
        {
            printf("LocalAlloc failed (error %d)\n", GetLastError());
            CloseHandle(hToken);
            return 1;
        }

        if (!GetTokenInformation(hToken, TokenGroups, pGroups, dwSize, &dwSize))
        {
            printf("GetTokenInformation failed (error %d)\n", GetLastError());
            LocalFree(pGroups);
            CloseHandle(hToken);
            return 1;
        }


        // Modify the token groups as needed (e.g., add or remove groups)

        // Adjust the token groups
        if (!AdjustTokenGroups(hToken, FALSE, pGroups, 0, nullptr, nullptr))
        {
            printf("AdjustTokenGroups fail\n");
            LocalFree(pGroups);
            CloseHandle(hToken);
            return 0;
        }

        // Clean up resources
        LocalFree(pGroups);
        CloseHandle(hToken);
    }


    
    {
        HANDLE hToken;
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS_P, &hToken)) 
        {
            return 0;
        }

        // Obtain the current token privileges
        DWORD dwSize;
        GetTokenInformation(hToken, TokenPrivileges, nullptr, 0, &dwSize);
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) 
        {
            CloseHandle(hToken);
            return 0;
        }

        PTOKEN_PRIVILEGES pTokenPrivileges = (PTOKEN_PRIVILEGES)LocalAlloc(LPTR, dwSize);
        if (!pTokenPrivileges) 
        {
            CloseHandle(hToken);
            return 0;
        }

        if (!GetTokenInformation(hToken, TokenPrivileges, pTokenPrivileges, dwSize, &dwSize)) 
        {
            LocalFree(pTokenPrivileges);
            CloseHandle(hToken);
            return 0;
        }

        // Adjust the token privileges
        if (!AdjustTokenPrivileges(hToken, FALSE, pTokenPrivileges, 0, nullptr, nullptr)) 
        {
            LocalFree(pTokenPrivileges);
            CloseHandle(hToken);
            return 0;
        }

        // Clean up resources
        LocalFree(pTokenPrivileges);
        CloseHandle(hToken);

    }

    
    {
        PSID pSid = nullptr;
        SID_IDENTIFIER_AUTHORITY authority = SECURITY_WORLD_SID_AUTHORITY;
        DWORD dwSubAuthority = SECURITY_WORLD_RID;

        if (!AllocateAndInitializeSid(
            &authority,
            1,
            dwSubAuthority,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            &pSid
        )) 
        {
            return 0;
        }

        // Convert the SID to a string representation
        LPTSTR pSidString = nullptr;
        if (!ConvertSidToStringSid(pSid, &pSidString)) {
            FreeSid(pSid);

        }

        // Print the SID string
        wprintf(L"SID: %s\n", pSidString);

        // Clean up resources
        LocalFree(pSidString);
        FreeSid(pSid);

    }

    
    {
        LUID luid;

        if (!AllocateLocallyUniqueId(&luid)) {
            return 0;
        }

        // Print the LUID values
        printf("LUID: %u-%u\n", luid.HighPart, luid.LowPart);

    }

    
    {
        DWORD grantedAccess = 0;
        DWORD desiredAccess = FILE_READ_DATA | FILE_WRITE_DATA; // Specify the desired access rights
        BOOL result = AreAllAccessesGranted(desiredAccess, grantedAccess);

        if (result) {
            printf("All access rights are granted.\n");
        }
        else {
            printf("Not all access rights are granted.\n");
        }
    }

    {
        DWORD grantedAccess = 0;
        DWORD desiredAccess = FILE_READ_DATA | FILE_WRITE_DATA; // Specify the desired access rights
        BOOL result = AreAnyAccessesGranted(desiredAccess, grantedAccess);

        if (result) {
            printf("At least one access right is granted.\n");
        }
        else {
            printf("No access rights are granted.\n");
        }

    }


    
    {
        BOOL hasCapability = FALSE;
        SID capabilitySid; // Specify the capability SID

        BOOL result = CheckTokenCapability(NULL, &capabilitySid, &hasCapability);

        if (result) {
            printf("Token has the specified capability.\n");
        }
        else {
            printf("Token does not have the specified capability.\n");
        }

    }


    
    {
        
        BOOL result;
        if (!CheckTokenMembership(NULL, pSid, &result)) {
            printf("CheckTokenMembership failed.\n");
            return 0;
        }

        if (result) {
            printf("Token belongs to the specified SID.\n");
        }
        else {
            printf("Token does not belong to the specified SID.\n");
        }

    }

    
    {
        
        DWORD dwFlags = CTMF_INCLUDE_APPCONTAINER;

        BOOL result;
        if (!CheckTokenMembershipEx(NULL, pSid, dwFlags, &result)) {
            printf("CheckTokenMembershipEx failed.\n");
            return 0;
        }

        if (result) {
            printf("Token belongs to the specified SID.\n");
        }
        else {
            printf("Token does not belong to the specified SID.\n");
        }

    }

    
    {
        DWORD dwRes;
        PACL pDacl = NULL;
        PSECURITY_DESCRIPTOR pSD = NULL;

        // Create a new security descriptor.
        dwRes = CreatePrivateObjectSecurity(
            NULL,
            NULL,
            &pSD,
            FALSE,
            NULL,
            NULL);

        if (dwRes != ERROR_SUCCESS)
        {
            printf("Could not create security descriptor (error %d)\n", dwRes);
            return 1;
        }
    }

    
    {
        DWORD dwRes;
        PACL pDacl = NULL;
        PSECURITY_DESCRIPTOR pSD = NULL;

        // Create a new security descriptor.
        dwRes = CreatePrivateObjectSecurityEx(
            NULL,
            NULL,
            &pSD,
            NULL,
            FALSE,
            SEF_DACL_AUTO_INHERIT | SEF_SACL_AUTO_INHERIT,
            NULL,
            NULL);

        if (dwRes != ERROR_SUCCESS)
        {
            printf("Could not create security descriptor (error %d)\n", dwRes);
            return 1;
        }
    }

    
    {
        DWORD dwRes;
        PACL pDacl = NULL;
        PSECURITY_DESCRIPTOR pSD = NULL;

        // Create a new security descriptor.
        dwRes = CreatePrivateObjectSecurityWithMultipleInheritance(
            NULL,
            NULL,
            &pSD,
            NULL,
            0,
            FALSE,
            SEF_DACL_AUTO_INHERIT | SEF_SACL_AUTO_INHERIT,
            NULL,
            NULL);

        if (dwRes != ERROR_SUCCESS)
        {
            printf("Could not create security descriptor (error %d)\n", dwRes);
            return 0;
        }
    }


    
    {
        DWORD dwRes;
        HANDLE hToken = NULL;
        HANDLE hRestrictedToken = NULL;

        // Open the current process's token.
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
        {
            printf("Could not open process token (error %d)\n", GetLastError());
            return 1;
        }

        // Create a new restricted token.
        SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
        PSID pIntegritySid = NULL;

        if (!AllocateAndInitializeSid(
            &NtAuthority,
            1,
            SECURITY_MANDATORY_LOW_RID,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            &pIntegritySid))
        {
            printf("Could not allocate integrity SID (error %d)\n", GetLastError());
            CloseHandle(hToken);
            return 1;
        }


        dwRes = CreateRestrictedToken(
            hToken,
            DISABLE_MAX_PRIVILEGE | SANDBOX_INERT,
            1,
            NULL,
            0,
            NULL,
            0,
            NULL,
            &hRestrictedToken);

        if (dwRes != ERROR_SUCCESS)
        {
            printf("Could not create restricted token (error %d)\n", dwRes);
            CloseHandle(hToken);
            FreeSid(pIntegritySid);
            return 1;
        }

        // Use the restricted token.

        CloseHandle(hRestrictedToken);
        CloseHandle(hToken);
        FreeSid(pIntegritySid);
    }

    
    {

        PSID pSid = NULL;
        DWORD n =0;
        CreateWellKnownSid(
            WinBuiltinAdministratorsSid,
            NULL,
            &pSid,
            &n
        );

        char* buf = new char[n];

        if (!CreateWellKnownSid(
            WinBuiltinAdministratorsSid,
            NULL,
            buf,
            &n
        ))
        {
            printf("Could not create SID (error %d)\n", GetLastError());
            return 1;
        }

        delete[] buf;
    }

    
    {
        DWORD dwRes;
        HANDLE hToken = NULL;
        HANDLE hNewToken = NULL;

        // Open the current process's token.
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
        {
            printf("Could not open process token (error %d)\n", GetLastError());
            return 1;
        }

        // Duplicate the token.
        if (!DuplicateToken(hToken, SecurityImpersonation, &hNewToken))
        {
            printf("Could not duplicate token (error %d)\n", GetLastError());
            CloseHandle(hToken);
            return 1;
        }

        // Use the new token.

        CloseHandle(hNewToken);
        CloseHandle(hToken);

    }

    
    {
        BOOL equalResult;

        equalResult = EqualDomainSid(pSid, pSid, &equalResult);

        if (equalResult) {
            // The SIDs belong to the same domain
            printf("The SIDs belong to the same domain.\n");
        }
        else {
            // The SIDs do not belong to the same domain
            printf("The SIDs do not belong to the same domain.\n");
        }

    }

    
   
    {
       
        ULONG Flags = 0x01;
        ULONG SigningLevel;
        UCHAR Thumbprint;
        ULONG ThumbprintSize = sizeof(Thumbprint);
        ULONG ThumbprintAlgorithm;

        auto dwRes = GetCachedSigningLevel(
            hFile,
            &Flags,
            &SigningLevel,
            &Thumbprint,
            &ThumbprintSize,
            NULL);

        if (dwRes != ERROR_SUCCESS)
        {
            printf("Could not get cached signing level (error %d)\n", dwRes);
            return 1;
        }


    }

    
    {
        LPCWSTR filePath = L"temp"; // Path to the file to retrieve the security descriptor for
        PSECURITY_DESCRIPTOR pSD = nullptr; // Pointer to the security descriptor

        // Retrieve the size of the security descriptor
        DWORD dwSize = 0;
        if (!GetFileSecurityW(filePath, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
            DACL_SECURITY_INFORMATION, nullptr, 0, &dwSize) && GetLastError() == ERROR_INSUFFICIENT_BUFFER) 
        {

            // Allocate memory for the security descriptor
            pSD = static_cast<PSECURITY_DESCRIPTOR>(LocalAlloc(LPTR, dwSize));
            if (pSD != nullptr) {
                // Retrieve the security descriptor
                if (GetFileSecurityW(filePath, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
                    DACL_SECURITY_INFORMATION, pSD, dwSize, &dwSize)) {

                    // Convert the security descriptor to a string format
                    LPWSTR pSDString = nullptr;
                    if (ConvertSecurityDescriptorToStringSecurityDescriptorW(pSD, SDDL_REVISION_1, OWNER_SECURITY_INFORMATION |
                        GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, &pSDString, nullptr)) {
                        std::wcout << "Security Descriptor: " << pSDString << std::endl;

                        // Free the memory allocated for the string
                        LocalFree(pSDString);
                    }
                    else {
                        std::cout << "Failed to convert security descriptor to string." << std::endl;
                    }
                }
                else {
                    std::cout << "Failed to retrieve the security descriptor. Error code: " << GetLastError() << std::endl;
                }

                // Free the memory allocated for the security descriptor
                LocalFree(pSD);
            }
            else {
                std::cout << "Failed to allocate memory for the security descriptor." << std::endl;
            }
        }
        else {
            std::cout << "Failed to retrieve the size of the security descriptor. Error code: " << GetLastError() << std::endl;
        }

    }

    
    {
        PSID pSid = nullptr;
        SID_IDENTIFIER_AUTHORITY authority = SECURITY_WORLD_SID_AUTHORITY;
        DWORD dwSubAuthority[] = { SECURITY_WORLD_RID };

        if (!::AllocateAndInitializeSid(&authority, 1, dwSubAuthority[0], 0, 0, 0, 0, 0, 0, 0, &pSid)) {
            std::cout << "Failed to allocate and initialize the SID. Error code: " << ::GetLastError() << std::endl;
        }

        DWORD dwLength = ::GetLengthSid(pSid);
        if (dwLength > 0) {
            std::cout << "SID length: " << dwLength << " bytes" << std::endl;
        }
        else {
            std::cout << "Failed to retrieve the length of the SID. Error code: " << ::GetLastError() << std::endl;
        }

        ::FreeSid(pSid);
    }

 

    
    {
        HANDLE hToken;

        if (OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
        {
            if (ImpersonateSelf(SecurityImpersonation))
            {
                // Impersonation successful, perform actions as the current user

                // Example: Get the username of the impersonated user
                wchar_t buffer[256];
                DWORD size = sizeof(buffer) / sizeof(buffer[0]);
                if (GetUserNameW(buffer, &size))
                {
                    std::wcout << L"Impersonated user: " << buffer << std::endl;
                }
                else
                {
                    std::cout << "Failed to get the username." << std::endl;
                }

                // Stop impersonating
                if (!RevertToSelf())
                {
                    std::cout << "Failed to revert to self." << std::endl;
                }
            }
            else
            {
                std::cout << "Impersonation failed." << std::endl;
            }

            CloseHandle(hToken);
        }
        else
        {
            std::cout << "OpenProcessToken failed." << std::endl;
        }

    }

    
    {
        HANDLE hToken;

        // Replace "PROCESS_ID" with the desired process ID or use GetCurrentProcess() for the current process
        DWORD dwProcessId = GetCurrentProcessId();

        if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
        {
            // Token opened successfully

            if (IsTokenRestricted(hToken))
            {
                std::cout << "Token is restricted." << std::endl;
            }
            else
            {
                std::cout << "Token is not restricted." << std::endl;
            }

            CloseHandle(hToken);
        }
        else
        {
            std::cout << "Failed to open token." << std::endl;
        }

    }

    
    {
        // Replace "PRIVILEGE_NAME" with the name of the privilege to check
        LPCWSTR PrivilegeName = L"SeSecurityPrivilege";

        // Replace "PRIVILEGE_ENABLED" with the desired privilege enabled state
        BOOL bPrivilegeEnabled = 1;

        HANDLE hToken;
        if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
        {
            PRIVILEGE_SET PrivilegeSet;
            PrivilegeSet.PrivilegeCount = 1;
            PrivilegeSet.Control = PRIVILEGE_SET_ALL_NECESSARY;
            PrivilegeSet.Privilege[0].Attributes = bPrivilegeEnabled ? SE_PRIVILEGE_ENABLED : 0;

            LUID luid;
            if (LookupPrivilegeValueW(nullptr, PrivilegeName, &luid))
            {
                PrivilegeSet.Privilege[0].Luid = luid;

                BOOL bResult;
                if (PrivilegeCheck(hToken, &PrivilegeSet, &bResult) && bResult)
                {
                    std::cout << "Privilege '" << PrivilegeName << "' is enabled." << std::endl;
                }
                else
                {
                    std::cout << "Privilege '" << PrivilegeName << "' is not enabled." << std::endl;
                }
            }
            else
            {
                std::cout << "Failed to lookup privilege value." << std::endl;
            }

            CloseHandle(hToken);
        }
        else
        {
            std::cout << "Failed to open process token." << std::endl;
        }

    }

    
    {
        LPCWSTR filePath = L"temp"; // Path to the file to retrieve the security descriptor for
        PSECURITY_DESCRIPTOR pSD = nullptr; // Pointer to the security descriptor

        // Retrieve the size of the security descriptor
        DWORD dwSize = 0;
        if (!GetFileSecurityW(filePath, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
            DACL_SECURITY_INFORMATION, nullptr, 0, &dwSize) && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {

            // Allocate memory for the security descriptor
            pSD = static_cast<PSECURITY_DESCRIPTOR>(LocalAlloc(LPTR, dwSize));
            if (pSD != nullptr) {
                // Retrieve the security descriptor
                if (GetFileSecurityW(filePath, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
                    DACL_SECURITY_INFORMATION, pSD, dwSize, &dwSize)) {

                    DWORD dwResult = SetFileSecurityW(filePath, DACL_SECURITY_INFORMATION, pSD);
                    if (dwResult != 0)
                    {
                        std::cout << "File security descriptor set successfully." << std::endl;
                    }
                    else
                    {
                        DWORD dwError = GetLastError();
                        std::cout << "Failed to set file security descriptor. Error code: " << dwError << std::endl;
                    }
                }
                else {
                    std::cout << "Failed to retrieve the security descriptor. Error code: " << GetLastError() << std::endl;
                }

                // Free the memory allocated for the security descriptor
                LocalFree(pSD);
            }
            else {
                std::cout << "Failed to allocate memory for the security descriptor." << std::endl;
            }
        }
        else {
            std::cout << "Failed to retrieve the size of the security descriptor. Error code: " << GetLastError() << std::endl;
        }

       
    }

    
    {

        HANDLE hToken;
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS_P, &hToken))
        {
            return 0;
        }

        // Obtain the current token privileges
        DWORD dwSize;
        GetTokenInformation(hToken, TokenOwner, nullptr, 0, &dwSize);
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
        {
            CloseHandle(hToken);
            return 0;
        }

        PTOKEN_OWNER  pTokenPrivileges = (PTOKEN_OWNER)LocalAlloc(LPTR, dwSize);
        if (!pTokenPrivileges)
        {
            CloseHandle(hToken);
            return 0;
        }

        if (!GetTokenInformation(hToken, TokenOwner, pTokenPrivileges, dwSize, &dwSize))
        {
            LocalFree(pTokenPrivileges);
            CloseHandle(hToken);
            return 0;
        }

        // Set the modified token information
        if (!SetTokenInformation(hToken, TokenOwner, pTokenPrivileges, dwSize))
        {
            printf("%d\n", GetLastError());
            return 0;
        }

    }

    
    {
        HANDLE anonymousToken = nullptr;

        if (ImpersonateAnonymousToken(anonymousToken) == FALSE)
        {
            // Failed to impersonate anonymous token
        }

        // Perform actions as the impersonated anonymous user

        if (RevertToSelf() == FALSE)
        {
            // Failed to revert to the original security context
        }

    }

    printf("success\n");
    return 0;
}
