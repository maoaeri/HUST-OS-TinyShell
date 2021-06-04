#include "Analyse.h"
#include "Command.h"
#include "Process.h"
using namespace std;
// HANDLE hForeProcess;


// /* Các câu lệnh liên quan đến tiến trình */
// int findProcessID(char *name_process);
// int getProcessListAll();
// int killProcessID(DWORD process_id);
// int killProcess(char *name_process);
// int suspendProcess(DWORD process_id);
// int resumeProcess(DWORD process_id);
// int createNewProcess(char **args);

int main (int argc, char** argv) {
    char cur_dir[MAX_CWD_LENS];
    /* Ctrl + C => STOP */
    signal(SIGINT, sighandler);

    /* set màu chữ cho shell */
    char color[] = "green";
    setColor(color);

    char *cmd = (char*)calloc(MAX_BUFFER_SIZE,sizeof(char)); /*Cấp phát bộ nhớ để nhập dòng lệnh command VD: cd, dir, ....*/
    
    int stop = 0; /**/
    while(!stop) { /* Nếu vẫn chưa stop thì thực hiện nhập và chạy lệnh */
        if(printPrompt(cur_dir) == -1){ /* Kiểm tra, in ra đường dẫn VD: C:\Users\Admin\> $ .....*/
            break;
        }
        fflush(stdin);
        cin.getline(cmd, MAX_BUFFER_SIZE); /* Nhập lệnh command */
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

