using namespace std;
#include "Util_lib.h"
#include "Command_lib.h"
#include "Process_lib.h"



int main (int argc, char** argv) {
    char cur_dir[MAX_CWD_LENS];


    char *cmd = (char*)calloc(MAX_BUFFER_SIZE,sizeof(char)); //memory allocation for command lines
    
    int stop = 0;
    printWelcomeText();
    while(!stop) { 
        // stop process when press ctrl+C
        signal(SIGINT, signalhandler);
        setShellWordColor(); 
        if(printPrompt(cur_dir) == -1){ /* Check and print current dir. Eg: C:\Users\Admin\> $ .....*/
            break;
        }
        fflush(stdin);

        cin.getline(cmd, MAX_BUFFER_SIZE); /* Command*/
        cin.clear();
        // cout << "HUST" << endl;
        // sleep(1); 

        // fixCmd(cmd); /* Chỉnh sửa câu lệnh tránh các lỗi nhập cách khoảng */
        char **args =  separate_line(cmd); /* Chia câu lệnh */
            /**
             * VD: dir C:\Users\Admin .....
             * => args[0] : dir
             
             * => args[1] : C:\Users\Admin
             * .....
             **/
        stop = execute(args); /* Thực hiện lệnh, trả về 0 nếu tiếp tục, trả về 1 để thoát*/
        free(args); /*Giải phóng các con trỏ lệnh*/
    }
    free(cmd); /*Giải phóng lệnh*/

    return EXIT_SUCCESS;
}

