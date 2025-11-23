#include <iostream>
#include <windows.h>
int main(){
    HANDLE res = CreateFileW(L"\\\\.\\Symple2", GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (res == INVALID_HANDLE_VALUE) {
        std::cout << GetLastError();
        return 1;
    }
    CloseHandle(res);
}

