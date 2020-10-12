#include <iostream>
#include <Windows.h>
PVOID Address;

LONG WINAPI VectoredHandler(
    struct _EXCEPTION_POINTERS* ExceptionInfo
)
{
    UNREFERENCED_PARAMETER(ExceptionInfo);
    DWORD oldProtect=NULL;
    printf("Thread ID %d \n", GetCurrentThreadId());
    printf("ExceptionCode %x \n", ExceptionInfo->ExceptionRecord->ExceptionCode);
    printf("ExceptionAddress %x \n", ExceptionInfo->ExceptionRecord->ExceptionAddress);
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == STATUS_SINGLE_STEP)
    {
        VirtualProtect(Address, 1024, PAGE_NOACCESS, &oldProtect);
    }
    if (ExceptionInfo->ExceptionRecord->ExceptionCode==EXCEPTION_ACCESS_VIOLATION)
    {
        printf("ExceptionAccessAddress %x \n", ExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
        if (ExceptionInfo->ExceptionRecord->ExceptionInformation[1]==(DWORD)Address)
        {
            ExceptionInfo->ContextRecord->EFlags |= 0x100;
            VirtualProtect(Address, 1024, PAGE_READWRITE, &oldProtect);

        }
    }
   
    return EXCEPTION_CONTINUE_EXECUTION;
}

int main()
{
    DWORD oldProtect = NULL;
	AddVectoredExceptionHandler(1, VectoredHandler);
    Sleep(1000);
    printf("Main Thread ID %d \n", GetCurrentThreadId());
    
    Address=VirtualAlloc(NULL,1024, MEM_COMMIT, PAGE_READWRITE);
    printf("Address %x \n", Address);
   
    VirtualProtect(Address, 1024, PAGE_NOACCESS ,&oldProtect);
    *(DWORD*)Address = 123456;
    system("pause");
}
