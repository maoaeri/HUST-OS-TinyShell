
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <windows.h>
#include <time.h>
#include <sys/types.h>
#include <psapi.h>
#include <tlhelp32.h>
#include "Util.h"
#include "Command.h"
#include "Process.h"
using namespace std;

#define MAX_CWD_LENS 128
#define MAX_BUFFER_SIZE 64
#define MAX_TOK_BUFSIZE 64
#define TOKEN_DELIMETERS " \t\r\n\a"

HANDLE hForeProcess;


//Code to handle process

/**
 * handle signal
 **/
void signalhandler(int signum) {

    if (hForeProcess != NULL) {
        TerminateProcess(hForeProcess, 0);
        hForeProcess = NULL;
    }
    return;
    // exit(signum);
}

/**
 * Print running processes in the form of: Name, id, parent process id
 * Command: pc all
 * 
 **/
int getAllProcesses() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32; // Describes an entry from a list of the processes residing in the system address space when a snapshot was taken.

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // Taken all processes
    // handle error
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        cout << "ERROR: CreateToolhelp32Snapshot Fail " << GetLastError() << endl;
        return 0;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);
    // Check the first result
    if (!Process32First(hProcessSnap, &pe32)) {
        cout << "ERROR: Process32First Fail " << GetLastError() << endl;
        return 0;
    }
    printf("%-50s%-20s%-20s\n", "Process Name", "Process ID", "Parent Process ID");
	printf("%-50s%-20s%-20s\n", "----------------------------------", "----------", "-----------");    
    
    do {
        printf("%-50s%-20d%-20d\n", pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);
    } while (Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
    return 1;
}

/**
 * Find process by name
 * Command: pc find [name_process]
 * 
 **/
int findProcessByName(char *name_process) {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    // handle error
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return 0;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);
    
    // Check the first result
    if (!Process32First(hProcessSnap, &pe32)) {
        return 0;
    }
    printf("%-50s%-20s%-20s\n", "Process Name", "Process ID", "Parent Process ID");
	printf("%-50s%-20s%-20s\n", "----------------------------------", "----------", "-----------");
    do {
        if (strcmp(name_process, pe32.szExeFile) == 0) {
            // Nếu pe32.szExeFile trùng với tên tiến trình thì in ra
            printf("%-50s%-20d%-20d\n", pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);
        }
    } while (Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
    return 1;
}

/**
 * Close process by ID
 * Command: pc kill [process_id]
 * 
 **/

int killProcessID(DWORD process_id) {
    HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    if (hprocess == NULL) {
        cout << "ERROR: Failed!" << endl;
        return 1;
    }
    if (!TerminateProcess(hprocess, 0)) {
        return 0;
    }
    return 1;
}

/**
 * Kill all processes with name
 * Command: pc kill [Name_Process]
 * 
 * */
int killProcess(char *name_process) {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32; // Describes an entry from a list of the processes residing in the system address space when a snapshot was taken.

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // Taken all processes
    // Handle error
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return 0;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);
    
    // Check the first one
    if (!Process32First(hProcessSnap, &pe32)) {
        return 0;
    }
    do {
        if (strcmp(name_process, pe32.szExeFile) == 0) {
            killProcessID(pe32.th32ProcessID);
        }
    } while (Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
    return 1;
}

/**
 * Suspend an ongoing process
 * Command: pc suspend [process_id]
 * 
 **/
int suspendProcess(DWORD process_id) {
    // Taken all threads
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 th32; //Describes an entry from a list of the threads executing in the system when a snapshot was taken.
	HANDLE hthread;
    // Handle error
	if (hThreadSnap == INVALID_HANDLE_VALUE) { 
		cout << "ERROR: CreateToolhelp32Snapshot" << GetLastError();
		return 0;
	}
	th32.dwSize = sizeof(THREADENTRY32);
	// Check the first one
	if (!Thread32First(hThreadSnap, &th32))
	{
		cout << "Thread32First Fail " <<  GetLastError(); // Nếu lỗi in ra lỗi
		CloseHandle(hThreadSnap);          // Đóng Handle snapshot
		return 0;
	}
	//Check the others
	do {
		if (th32.th32OwnerProcessID == process_id) {
			hthread = OpenThread(THREAD_ALL_ACCESS, FALSE, th32.th32ThreadID);
            if (SuspendThread(hthread) == -1) {
				return 0;
			}
		}
	} while (Thread32Next(hThreadSnap, &th32));
	CloseHandle(hThreadSnap);
	return 1;
}

/**
 *Resume a suspended process
 * Command pc resume [process_id]
 * 
 **/
int resumeProcess(DWORD process_id) {
    // Taken all threads
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 th32; //Describes an entry from a list of the threads executing in the system when a snapshot was taken.
	HANDLE hthread;
    // Handle error
	if (hThreadSnap == INVALID_HANDLE_VALUE) { 
		cout << "ERROR: CreateToolhelp32Snapshot" << GetLastError();
		return 0;
	}
	th32.dwSize = sizeof(THREADENTRY32);
	// Check the first one
	if (!Thread32First(hThreadSnap, &th32))
	{
		cout << "Thread32First Fail " <<  GetLastError(); 
		CloseHandle(hThreadSnap);        
		return 0;
	}
	//Check the others
	do {
		if (th32.th32OwnerProcessID == process_id) {
			hthread = OpenThread(THREAD_ALL_ACCESS, FALSE, th32.th32ThreadID); 
            if (ResumeThread(hthread) == -1) {
				return 0;
			}
		}
	} while (Thread32Next(hThreadSnap, &th32));
	CloseHandle(hThreadSnap);
	return 1;
}

/**
 * Create a child process
 * Command: pc bg [name_process/path](background mode)
 *           pc fg [name_process/path](foreground mode)
 * 
 **/
int createNewProcess(char **args) {
    // set wait time for process
    int wait_time;
    if (strcmp(args[1], "bg") == 0) {
        wait_time = 0;
    } else {
        wait_time = INFINITE;
    }
    char *run_file = combinePath(args, 2); // Ghép lại tên tiến trình hoặc đường dẫn
	STARTUPINFO si;
	PROCESS_INFORMATION pi;//Contains information about a newly created process and its primary thread. 

	ZeroMemory(&si, sizeof(si)); //Fills a block of memory with zeros.
	si.cb = sizeof(si);
	si.wShowWindow = SW_SHOW;//Activates the window and displays it in its current size and position.
	si.dwFlags = STARTF_USESHOWWINDOW; //The wShowWindow member contains additional information.
	si.lpTitle = args[1]; //For console processes, this is the title displayed in the title bar if a new console window is created. 
	ZeroMemory(&pi, sizeof(pi));

	// Khởi tạo tiến trình con
	if (!CreateProcess(NULL, run_file, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		int error = GetLastError(); // Kiểm tra lỗi
	    if (error == 2)
		    cout << "The batch file or execute file '" << run_file << "' is not found." << endl;
	    else
		    cout << "Can't run this file" << endl;
	    return 1;
	}
	// gán handle cho tiến trình con vừa tạo
	if (strcmp(args[1], "fg") == 0) {
		hForeProcess = pi.hProcess;
	}
	// Thời gian đợi 1 tiến trình con
	WaitForSingleObject(pi.hProcess, wait_time);

	// Đóng các handle
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
    return 1;
}
