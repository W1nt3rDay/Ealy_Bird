#include <Windows.h>

void EarlyBird(const char* TargetPath,const wchar_t* dllPath)
{
	STARTUPINFOA si = {};   //Ҫ����Ľ�����Ϣ
	PROCESS_INFORMATION pi = {};
	CreateProcessA(TargetPath, 0, 0, 0, FALSE, CREATE_SUSPENDED, 0, 0, &si, &pi);//����һ������
	HANDLE hProcess = pi.hProcess;
	HANDLE hThread = pi.hThread;
	size_t bufferSize = (wcslen(dllPath) + 1) * sizeof(wchar_t); // +1 Ϊ��ֹ��

	auto p = VirtualAllocEx(hProcess, 0, 1 << 12, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	WriteProcessMemory(hProcess, p, dllPath, bufferSize, 0);

	QueueUserAPC((PAPCFUNC)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW"), hThread, (ULONG_PTR)p);
	ResumeThread(hThread);
	CloseHandle(hProcess);
	CloseHandle(hThread);
}

int main()
{
	EarlyBird("E:\\asm64\\�ڴ�����\\patcher\\patcher.exe",L"D:\\Desk\\mydll.dll"); //Ŀ��exe·��   Ҫע���dll·��
}  