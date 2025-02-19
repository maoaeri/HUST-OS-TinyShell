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

#define MAX_CWD_LENS 128
#define MAX_BUFFER_SIZE 64
#define MAX_TOK_BUFSIZE 64
#define TOKEN_DELIMETERS " \t\r\n\a"

using namespace std;

/////////////////////////////////////
////////// Xử lý câu lệnh ///////////
/////////////////////////////////////


/**
 * Fix command
 * VD: C:\Users\Admin> $           cd E:\ 
 * ==> C:\Users\Admin> $ cd E:\
 * 
 **/
void fixCmd(char* cmd){

    int length = strlen(cmd);
    int first;
    for(int i=0; i<length; i++){
        if((cmd)[i] != ' '){
            first = i;
            break;
        }
    }
    int real_len = length - first, i=0;
//			"     Ta Ton" -> "Ta Tona Ton"
    while(real_len--){
        (cmd)[i] = (cmd)[i+first];
        i++;
    }
    (cmd)[length - first] = '\0'; //"Ta Tona Ton" -> "Ta Ton"
}

/**
 * print just a little welcome text
 **/
void printWelcomeText(){
    cout << "\n"
 " _____ _             _____ _          _ _ \n"
"|_   _(_)           /  ___| |        | | |\n"
 "  | |  _ _ __  _   _\\ `--.| |__   ___| | |\n"
 "  | | | | '_ \\| | | |`--. \\ '_ \\ / _ \\ | |\n"
 "  | | | | | | | |_| /\\__/ / | | |  __/ | |\n"
"  \\_/ |_|_| |_|\\__, \\____/|_| |_|\\___|_|_|\n"
"                __/ |                     \n"
"               |___/                      \n";
    cout << "Welcome to my Tiny Shell <3" << endl;
}

/**
 * Print working directory 
 * (VD: C:\Users\Admin\.....> [command])
 **/
int printPrompt(char *cur_directory){
    if(GetCurrentDirectory(MAX_CWD_LENS,cur_directory) == 0){
        cout << "Reading of current working directory failed.\n";
        return -1;
    }
    cout << "\n" << cur_directory << ">";
    
    return 1;
}

/**
 * Separate commands by: (" ","\t","\r","\n","\a")
 * 
 **/
char **separate_line(char *line){
    int bufsize = MAX_TOK_BUFSIZE;
    int position = 0;
    char **tokens = (char**)malloc(bufsize*sizeof(char*));
    char *token;

    if(!tokens){
        cout << "Allocation Failed" << endl; /* Không đủ bộ nhớ cấp phát */
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOKEN_DELIMETERS); /*Con trỏ trỏ tới args[0] của lệnh cmd VD: cd, dir*/
    while(token != NULL){
        tokens[position] = token; /* Lưu các con trỏ chứa thành phần của lệnh cmd */
        // cout << tokens[position] << endl;
        position++; 

        if(position >= bufsize){ /* số thành phần args[i] trong lệnh cmd lớn hơn số bufsize dự tính*/
            bufsize += MAX_TOK_BUFSIZE; /* Tăng số bufsize */
            tokens = (char**)realloc(tokens, bufsize); /* Cấp phát thêm bộ nhớ cho tokens */
            if(!tokens){
                cout << "Allocation Failed" << endl; /* Không đủ bộ nhớ cấp phát */
                exit(EXIT_FAILURE);
            } 
        }

        token = strtok(NULL,TOKEN_DELIMETERS); /* Trỏ token tới thành phần args tiếp theo trong của cmd*/
    }
    tokens[position] = NULL; /*Kết thúc danh sách mã*/
    return tokens;
}


/**
 * Chuẩn hóa đường dẫn [path], tránh trường hợp như:
 * VD: cd E:\New folder\ => [path] = 'E:\New'
 **/
char *combinePath(char **args, int start){

    char *path = (char *)calloc(MAX_CWD_LENS,sizeof(char)); /*Các thành phần không bị ngăn cách*/
    char *pivot = (char*)" "; /* Dấu ngăn cách */

    /* Kết hợp từng thành phần trong path */
    path = strcat(path, args[start]);
    int i = start + 1;
    while(args[i] != NULL){
        path = strcat(path,pivot);
        path = strcat(path, args[i]);
        ++i;
    }
    return path;
}


/**
 * Change word color
 * 
 **/
void setShellWordColor() {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
