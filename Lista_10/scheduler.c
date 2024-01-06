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


struct queue QUE;

int pop(int i) {
    if (QUE.current_position == 0) {
        // Queue is empty
        return -1;
    }

    if (i < 0 || i >= QUE.current_position) {
        // Invalid index
        return -1;
    }

    // Shift elements after the specified index to fill the gap
    for (int j = i; j < QUE.current_position - 1; j++) {
        QUE.list[j] = QUE.list[j + 1];
    }

    QUE.current_position--;

    return 0;
}
void printf_process_info(struct proscess_info info ){
    printf("ID = %d\n", info.id);
    printf("Priority = %d\n", info.priority);
    printf("Exec time = %d\n", info.exec_time);
}

int FCFS(){
    if (QUE.current_position == 0) {
        // Queue is empty
        return -1;
    }

    printf_process_info(QUE.list[0]);
    QUE.list[0].exec_time = QUE.list[0].exec_time - 1;
    if (QUE.list[0].exec_time == 0){
        if(pop(0) == -1){
            return -1;
        };
    }
    return 0;
}

int SJF(){
    
}

int main(int argc, char* argv[]){
    char buffer[BUFSIZ];
    size_t len;
    int time =0;
    int type = 0;
    struct proscess_info current_process;
    
    QUE.current_position = 0;

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
                    i = i + 3;
                } else {
                    val = (buffer[i] - '0');
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
                    QUE.list[QUE.current_position] = current_process;
                    // printf_process_info(QUE.list[QUE.current_position]);
                    QUE.current_position = QUE.current_position +1;
                }
                
                type = (type +1)%3;

            }
        }

        FCFS();
        printf("\n--------------------------------------------\n");
        time++;
    }
    

    while (FCFS() != -1){printf("\n--------------------------------------------\n");}
    
    return 0;   

}