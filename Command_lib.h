#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string>
#include <windows.h>
#include <time.h>
#include <sys/types.h>
#include <psapi.h>
#include <tlhelp32.h>
#include "Util.h"
#include "Process.h"
#include <tchar.h>

#define MAX_CWD_LENS 128
#define MAX_BUFFER_SIZE 64
#define MAX_TOK_BUFSIZE 64
#define TOKEN_DELIMETERS " \t\r\n\a"

using namespace std;

/* Mảng các câu lệnh command được hỗ trợ */
const char *command[] = {
    "cd",     
    "clear",    
    "echo",  
    "exit", 
    "help", 
    "del",
    "dir",
    "mkdir", 
    "date",
    "time",
    "pc",
    "calc",
    "run",
    "env",
};

/* All commands*/
int (*activate_command[])(char **) = { 
  &cd,
  &clr,
  &echo,
  &exit,
  &help,
  &del,
  &dir,
  &mk_dir,
  &date,
  &time_cmd,
  &pc,
  &calc,
  &runbat,
  &envvar,
}; 

/**
 * Run command. Eg: cd, calc,...
 **/
int execute(char ** args){
    int t; //a counter to check if command existed. 0 if not, otherwise 1.
    if (args[0] == NULL){
        return 0;
    }
    for(int i=0; i < size_of_command() ; i++){
        if(strcmp(args[0],command[i]) == 0){ //Check if user typed the correct command
            t = 1;
            return (*activate_command[i])(args);
        } else { //else return a reminder
            t = 0;
        }
    }
    if (!t) {
        printf("Command not found. Press 'help' for more instruction.\n");
    }
    return 0;
}


/**
 * return the size of now existing commands
 **/
int size_of_command(){
    return sizeof(command) / sizeof(char *);
}

//Command hereeeeee

int help(char **args){
    if (args[1] == NULL)
	{
		printf("Type \"help -[command]\" for more information about a specific command.\n");
		printf("Supported commands:\n cd\n date\n time\n dir\n clr\n echo\n del\n mkdir\n pc\n run\n env\n exit\n ");
		printf("Usage:\n\t <command> [option]\n\tEXAMPLE: help cd\n");
		printf("%-10s%s\n", " cd",
			"Change current directory.");
		printf("%-10s%s\n", " date",
			"Show today's date.");
		printf("%-10s%s\n", " time",
			"Show the current time.");
		printf("%-10s%s\n", " dir",
			"Show all files and folders in the current directory.");
        printf("%-10s%s\n", " clr",
			"Clear the console screen.");
        printf("%-10s%s\n", " echo",
			"Print a message on the screen.");
        printf("%-10s%s\n", " del",
			"Delete a file or folder in the current directory.");
        printf("%-10s%s\n", " mkdir",
			"Make a new directory.");
        printf("%-10s%s\n", " run",
			"Run .bat file only.");
        printf("%-10s%s\n%-10s%s\n", " pc",
            "Create process.", " ",
            "You must enter the options in the 2nd argument, such as f and b");
        printf("%-10s%s\n%-10s%s\n", " env",
            "Change environment variables.", " ",
            "You must enter the options in the 2nd and 3rd arguments, such as get and set");
		printf("%-10s%s\n", " exit", "Exit this tiny shell :((((");
	}
    else if(!strcmp(args[1],"cd")){
        cout << "Change the current directory." << endl;
        cout << "       cd           : Show the current directory" << endl;
        cout << "       cd ..        : Show the parent directory of the current directory" << endl;
        cout << "       cd [path]    : Change the current directory to [path]" << endl;
        cout << "EXAMPLE: cd C:\\Users\\Admin\\ => change current directory to C:\\User\\Admin" << endl;
    }
    else if(!strcmp(args[1],"date")){
        cout << "Display the current date to screen." << endl;
        cout << "This command does not support any options." << endl;
    }
    else if(!strcmp(args[1],"time")){
        cout << "Diplay the current time to screen." << endl;
        cout << "This command does not support any options." << endl;
    }
    else if(!strcmp(args[1],"dir")){
        cout << "Display the list of files and folder in the directory to the screen." << endl;
        cout << "       dir         : Show the files and folders in the current directory" << endl;
        cout << "       dir [path]  : Show the files and folders in the [path]" << endl;
        cout << "EXAMPLE: \"dir\", \"dir C:\\Users\\Admin\"" << endl;
    }
    else if(!strcmp(args[1],"clr")){
        cout << "Clear all line displaying on the console screen." << endl;
        cout << "This command does not support any options." << endl;
    }
    else if(!strcmp(args[1],"echo")){
        cout << "Print the message on the screen." << endl;
        cout << "This command does not support any options." << endl;
        cout << "EXAMPLE: \"echo Hello World\"" << endl;
        cout << "       => \"Hello World\"" << endl;
    }
    else if(!strcmp(args[1],"del")){
        cout << "Delete file or folder in the current directory." << endl;
        cout << "Recommend: Folder/File name contains no space." << endl;
        cout << "       del [Folder/Filename] : Delete folder/file (Folder/Filename) in the current dirrectory" << endl;
        cout << "EXAMPLE: \"del ABC\"" << endl; 
    }
    else if(!strcmp(args[1],"mkdir")){
        cout << "Make a new directory in the current directory." << endl;
        cout << "Recommend: Foldername contains no space." << endl;
        cout << "       mkdir [Foldername] : Make folder [Foldername] in the current directory" << endl;
        cout << "EXAMPLE: \"mkdir ABC\"" << endl;
    }
    else if (!strcmp(args[1], "run")){
        cout << "Run .bat file only with commands which our shell supports" << endl;
        cout << "       run [Filename.bat] : Run .bat file with commands which our shell supports " << endl;
        cout << "EXAMPLE: \"run command.bat\"" << endl;
    }
    else if(!strcmp(args[1],"pc")){
        cout << "Supported options:" << endl;
        cout << "        all      Show all running processes" << endl;
		cout << "        find     Find a process information by name" << endl;
		cout << "        suspend  Suspend a process by process id" << endl;
		cout << "        resume   Resume a process by process id" << endl;
		cout << "        kill     Terminate all processes by name" << endl;
        cout << "        killid   Terminate a process by process id" << endl;
        cout << "        bg       Run a process in background mode" << endl;
		cout << "        fg       Run a process in foregound mode" << endl;
        cout << "EXAMPLE: \"pc fg C:\\Users\\Admin\\ABC\"" << endl;
    }
    else if(!strcmp(args[1],"env")){
        cout << "Supported options:" << endl;
        cout << "        get [name]           Get environment variable by name" << endl;
		cout << "        set [name] [value]     Set environment variable" << endl;
		cout << "        get all              Get all environment variables" << endl;
        cout << "EXAMPLE: \"env set path1=C:\\Users\\Admin" << endl;
    }
    else if(!strcmp(args[1],"exit")){
        cout << "Exit the TinyShell." << endl;
        cout << "This command does not support any options." << endl; 
    }
    return 0;
}


/**
 * Change current directory
 * Command: cd [path] 
 * cd : return current directory
 * cd .. :return parent directory
 * cd [path]: Change to path's directory
 * 
 **/
int cd(char **args){
    if(args[1] == NULL){ /* cd */
        system("cd");
        cout << endl;
        return EXIT_SUCCESS;
    }
    /* \ cd [path] */
    else {
        char* path = combinePath(args,1); /* Chuẩn hóa path */
        if(SetCurrentDirectory(path)==FALSE){ /* Tìm đường dẫn nếu có */
            // cout << args[1] << endl;
            fprintf(stdout,"Not able to set current working directory\n");
        }
    }

    return 0;
}

/**
 * List all files and folders in directory
 * Command: dir [path] 
 * 
 **/
int dir(char **args){
    char *cur_dir = (char*)malloc(MAX_CWD_LENS*sizeof(char));
    GetCurrentDirectory(MAX_CWD_LENS, cur_dir);
	WIN32_FIND_DATA data;//Contains information about the file that is found by the FindFirstFile, FindFirstFileEx, or FindNextFile function.
	_FILETIME time_lastwrite; //Contains a 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 (UTC).
	_SYSTEMTIME convert_time; //Specifies a date and time, using individual members for the month, day, year, weekday, hour, minute, second, and millisecond. The time is either in coordinated universal time (UTC) or local time, depending on the function that is being called.
	string date;
	string time;
	char *char_date = (char *)calloc(15, sizeof(char));
	char *char_time = (char *)calloc(15, sizeof(char));
	const char add[] = "\\*";
    char *path = (char*)malloc(MAX_CWD_LENS*sizeof(char));
    if(args[1] == NULL){
        strcpy(path, cur_dir);
        strcat(path,"\\*");
    }
    else {
        path = combinePath(args,1);
        strcat(path,"\\*");
    }
	HANDLE han = FindFirstFile(path, &data);
	printf("%-15s%-15s%-15s%-15s\n", "Date", "Time", "Type", "Name");
	printf("=========================================================================\n");
	if (han != INVALID_HANDLE_VALUE)
	{
		do
		{
			time_lastwrite = data.ftLastWriteTime;
			FileTimeToSystemTime(&time_lastwrite, &convert_time);

			// Convert date, time to string & char *
			date = to_string(convert_time.wDay) + '/' + to_string(convert_time.wMonth) + '/' + to_string(convert_time.wYear);
			time = to_string(convert_time.wHour) + ':' + to_string(convert_time.wMinute);
			strcpy(char_date, date.c_str());
			strcpy(char_time, time.c_str());

			if (data.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE)
			{
				printf("%-15s%-15s%-15s%-15s\n", char_date, char_time, "<FILE>", data.cFileName);
			}
			if (data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				printf("%-15s%-15s%-15s%-15s\n", char_date, char_time, "<FOLDER>", data.cFileName);
			}
		} while (FindNextFileA(han, &data) != 0);
        delete cur_dir;
		FindClose(han);
		delete char_time;
		delete char_date;
		return EXIT_SUCCESS;
	}
	else
	{
        delete cur_dir;
        delete char_time;
		delete char_date;
		return EXIT_FAILURE;
	}
}


/**
 * create new folder in current directory
 * Command: mkdir [foldername]
 * 
 **/
int mk_dir(char **args){
    if(args[1] == NULL){ /* Only mkdir */
        cout << "ERROR: Command mkdir need filename" << endl;
        cout << "Command: mkdir [filename]" << endl;
        cout << "Recommend: filename should not have any space" << endl;
        return 0;
    }
    mkdir(args[1]); /*  */
    return 0;
}


/**
 * Print a message
 * Command: echo [message]
 *  
 **/
int echo(char **args){

    if(args[1] == NULL){
        cout << "ERROR: Echo + [message]" << endl;
        return 0;
    }
    int i=0;
    while(args[++i] != NULL){
        for(int j=0; j<strlen(args[i]); j++){
            cout << args[i][j];
        }
        cout << " ";
    }
    cout << endl;
    return 0;
}

/**
 * Clear console screen
 * Command: clr
 **/
int clr(char **args){ 
    
    if(strcmp(args[0],"clr") == 0){
        HANDLE hConsole; 
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        SMALL_RECT scrollRect;
        COORD scrollTarget;
        CHAR_INFO fill;

        // Get the number of character cells in the current buffer.
        if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        {
            return 0;
        }

        // Scroll the rectangle of the entire buffer.
        scrollRect.Left = 0;
        scrollRect.Top = 0;
        scrollRect.Right = csbi.dwSize.X;
        scrollRect.Bottom = csbi.dwSize.Y;

        // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
        scrollTarget.X = 0;
        scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

        // Fill with empty spaces with the buffer's default text attribute.
        fill.Char.UnicodeChar = TEXT(' ');
        fill.Attributes = csbi.wAttributes;

        // Do the scroll
        ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

        // Move the cursor to the top left corner too.
        csbi.dwCursorPosition.X = 0;
        csbi.dwCursorPosition.Y = 0;

        SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
    }
    return 0;
}


/**
 * Print current date
 * Command: date
 * 
 **/
int date(char **args){
    if (args[1] != NULL)
	{
		cout << "Command \"date\" does not support any option !\n";
		return EXIT_FAILURE;
	}
	time_t t = time(0);
	struct tm * now = localtime(&t);
	cout << "Current Date: " << now->tm_mday << '/'
		<< (now->tm_mon + 1) << '/'
		<< (now->tm_year + 1900)
		<< endl;
	return 0;
}


/**
 * Print current time (hour:minute:second)
 * Command: time
 * 
 */
int time_cmd(char **args){
    if (args[1] != NULL)
	{
		cout << "Command \"time\" does not support any option !\n";
		return EXIT_FAILURE;
	}
	time_t t = time(0);
	struct tm * now = localtime(&t);
	cout << "Current time: " << now->tm_hour << ':' << now->tm_min << ':' << now->tm_sec << endl;
	return 0;
}

/**
 * Delete file hoặc folder
 * Command: del [file/foldername]
 *  
 **/
int del(char **args){

    if(args[1] == NULL){
        cout << "ERROR: Command 'del' need path" << endl;
        cout << "Command: del [path]" << endl;
        return 0;
    }
    else if (args[2] != NULL){
        cout << "ERROR: Command 'del' cannot delete more than 1 file" << endl;
        cout << "Command: del [path]" << endl;
        return 0;
    }
    if(remove(args[1]) == -1 && rmdir(args[1]) == -1){ /* Nếu arg[1] là file thì xóa file, folder thì xóa folder. Nếu k có thì in chỗ dưới */
        cout << "ERROR: Unable to find file to be deleted. Try again" << endl;
        cout << "Command: del [path]" << endl;
        return 0;
    }
    return 0;
}


/**
 * Exit
 * Command: exit
 * 
 **/
int exit(char **args){
    char* term = (char*)malloc(64*sizeof(char));
    if(args[1] != NULL){
        term = combinePath(args,1);
        cout << "ERROR: Term " << term << "is not recognized for EXIT" << endl;
        cout << "Command: exit" << endl;
        return 0;
    }
    /* Trả 1 về cho biết stop trong main */
    return 1;
}


/**
 * Commands with process
 * command: pc (Process)
 * 
 **/
int pc(char **args) {
    if (args[1] == NULL) {
        cout << "ERROR: Too few arguments" << endl;
        return 0;
    }
    if (strcmp(args[1], "all") == 0) {
        if (getAllProcesses()) {
            return 0;
        }
        return 1;
    }
    if (strcmp(args[1], "find") == 0){
		if (args[2] == NULL)
		{
			printf("ERROR: Too few arguments\n");
			return 0;
		}
		// Tìm ID Process
		if (findProcessByName(args[2]))
			return 0;
        return 1;
	}

    if (strcmp(args[1], "kill") == 0) {
        if (args[2] == NULL) {
            cout << "ERROR: Too few argument" << endl;
            return 0;
        }

        if (killProcess(args[2])) {
            return 0;
        }
        return 1;
    }

    if (strcmp(args[1], "killid") == 0) {
        if (args[2] == NULL) {
            cout << "ERROR: Too few argument" << endl;
            return 0;
        }
        DWORD process_id = atoi(args[2]);
        if (killProcessID(process_id)) {
            return 0;
        }
        return 1;
    }

    if (strcmp(args[1], "suspend") == 0) {
        if (args[2] == NULL) {
            cout << "ERROR: Too few argument" << endl;
            return 0;
        }
        DWORD process_id = atoi(args[2]);
        suspendProcess(process_id);
        return 0;
    }

    if (strcmp(args[1], "resume") == 0) {
        if (args[2] == NULL) {
            cout << "ERROR: Too few argument" << endl;
            return 0;
        }
        DWORD process_id = atoi(args[2]);
        resumeProcess(process_id);
        return 0;
    }

    if (strcmp(args[1], "bg") == 0 || strcmp(args[1], "fg") == 0) {
        if(args[2] == NULL) {
            cout << "ERROR: Too few argument" << endl;
            return 0;
        } else {
            createNewProcess(args);
        }
        return 0;
    } 
    else {
        cout << "ERROR: Too few arguments" << endl;
        return 0;
    }
}

/**
 *  Open a calculator
 **/
int calc(char **args){
    system("calc");
    return 0;
}

/**
 *  Command with environment variables
 * Command: env [get/set/del] [var]
 **/
int envvar(char **args){
    char *val;
    int bufsize = MAX_BUFFER_SIZE;
    if (args[1] == NULL) {
        cout << "ERROR: Too few arguments";
        return 0;
    }
    if (strcmp(args[1], "get") == 0){
        if(args[2] == NULL) {
            cout << "ERROR: Too few arguments";
            return 0;
        } else {
            if (!strcmp(args[2], "all")){
                    LPTSTR lpszVariable; 
                    LPTCH lpvEnv; 
                
                    // Get a pointer to the environment block. 
                
                    lpvEnv = GetEnvironmentStrings();

                    // If the returned pointer is NULL, exit.
                    if (lpvEnv == NULL)
                    {
                        printf("ERROR: GetEnvironmentStrings failed (%d)\n", GetLastError()); 
                        return 0;
                    }
                
                    // Variable strings are separated by NULL byte, and the block is 
                    // terminated by a NULL byte. 

                    lpszVariable = (LPTSTR) lpvEnv;

                    while (*lpszVariable)
                    {
                        _tprintf(TEXT("%s\n"), lpszVariable);
                        lpszVariable += lstrlen(lpszVariable) + 1;
                    }
                    FreeEnvironmentStrings(lpvEnv);
                    return 0;
            } else {
                    DWORD dwRet, dwErr;
                    LPTSTR pszOldVal; 
                    DWORD dwFlags=0;

                    pszOldVal = (LPTSTR) malloc(bufsize*sizeof(TCHAR));
                    if(NULL == pszOldVal)
                    {
                        printf("Out of memory\n");
                        return FALSE;
                    }

                    dwRet = GetEnvironmentVariable(TEXT(args[2]), pszOldVal, bufsize*sizeof(TCHAR));

                    if(0 == dwRet)
                    {
                        dwErr = GetLastError();
                        if( ERROR_ENVVAR_NOT_FOUND == dwErr )
                        {
                            printf("Environment variable does not exist.\n");
                        }
                    }
                    else if(bufsize < dwRet)
                    {
                        pszOldVal = (LPTSTR) realloc(pszOldVal, dwRet*sizeof(TCHAR));   
                        if(NULL == pszOldVal)
                        {
                            printf("Out of memory\n");
                        }
                        dwRet = GetEnvironmentVariable(TEXT(args[2]), pszOldVal, dwRet);
                        if(!dwRet)
                        {
                            printf("GetEnvironmentVariable failed (%d)\n", GetLastError());
                        }
                    } else {
                        _tprintf(TEXT("%s=%s\n"), args[2],pszOldVal);
                    }
                    
            }
        }
        return 0;
    }
    if (strcmp(args[1], "set") == 0){
        if(args[2] == NULL) {
            cout << "ERROR: Too few arguments";
            return 0;
        } else {
            if (! SetEnvironmentVariable(TEXT(args[2]), TEXT(args[3]))) 
        {
            printf("SetEnvironmentVariable failed (%d)\n", GetLastError()); 
            return FALSE;
        } //overwrite var
        }
        return 0;
    }
    return 0;
}


//////////////////////////////////////////
////////  .bat ////////
//////////////////////////////////////////

/**
 * Check if the command is supported with this shell
 * 
 **/
bool cmdCheck(char **args){
    if (args[0] == NULL){
        return 0;
    }
    for(int i=0; i < size_of_command() ; i++){
        if(strcmp(args[0],command[i]) == 0){ 
            return true;
        } 
    }
    return false;
}

/**
 * Run file .bat
 * Command: run [Filename.bat]
 * 
 **/

int runbat(char **args){
    char w[255],a[255];
    char *run_file = combinePath(args, 1);
    FILE *f=fopen(run_file,"rt");
    if(f==NULL)
	{
        printf("\nFile not found.\n");
        // getch();
        return 0;
	}
    else
    {
        while(!feof(f))
        {
            fgets(w,255,f);
            strcpy(a,w);
            char **arg = separate_line(a); 
            printf("");
            if(cmdCheck(arg)){
                int stop = execute(arg); // system ???
            }
            else {
                system(w);
                
            }
        }
        fclose(f);
    }
    return 0;
}
