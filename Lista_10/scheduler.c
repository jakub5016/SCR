#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define AMMOUT 10


struct proscess_info
{
    int id;
    int exec_time;
    int priority;
};


struct queue
{
    struct proscess_info list[AMMOUT];
    int current_position;
};


void printf_process_info(struct proscess_info info ){
    printf("ID = %d\n", info.id);
    printf("Priority = %d\n", info.priority);
    printf("Exec time = %d\n", info.exec_time);
}

int main(int argc, char* argv[]){
    char buffer[BUFSIZ];
    size_t len;
    int time =0;
    int type = 0;
    
    struct queue que;
    que.current_position = 0;
    struct proscess_info current_process;

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        len = strlen(buffer) - 2;
        printf("Time:%d\n", time);
        
        int i = 2;
        int val = 0;
        
        char next_line = 1;
        int type = 0; // 0 - ID, 1 - priority, 2 - exec time
        
        int id;
        int priority;
        int exec_time;
        while (next_line) {
            if (buffer[i] <= 32){
                next_line = 0;  // Break out of the inner loop
            } 
            else {
                if (buffer[i + 1] > 32) {
                    val = ((buffer[i] - '0') * 10) + (buffer[i + 1] - '0');
                    // printf("\n%c, %c\n", buffer[i], buffer[i+1]);
                    i = i + 3;
                } else {
                    val = (buffer[i] - '0');
                    // printf("\n%c\n", buffer[i]);
                    i = i +2;
                }

                switch (type)
                {
                case 0:
                    current_process.id = val;
                    break;
                case 1:
                    current_process.priority = val;
                    break;
                case 2:
                    current_process.exec_time = val;
                    break;
                default:
                    break;
                }

                if (type == 2){
                    que.list[que.current_position] = current_process;
                    printf_process_info(que.list[que.current_position]);
                    que.current_position = que.current_position +1;
                }
                
                type = (type +1)%3;

            }
        }

        // for (int i = 0; i < len - (len % 6); i = i +6){
        //     printf("ID=%c, priority=%c, exec=%c\n",buffer[2+i], buffer[4+i], buffer[6+i]);
        //     switch (type)
        //     {
        //     case 0:
        //         current_process.id = buffer[2+i] - '0';

        //         que.list[que.current_position] = current_process;
        //         que.current_position = que.current_position +1;
        //         break;
            
        //     default:
        //         break;
        //     }
        // }

        printf("\n--------------------------------------------\n");
        time++;
    }
    return 0;   

}