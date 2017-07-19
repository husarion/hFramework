#include <hFramework.h>
void stressMalloc() {
    int v = 0;
    while (true) {
        char* p[10];
        for (int i=0; i < 10; i ++) {
            p[i] = (char*)malloc(13);
            for (int j=0; j < 13; j ++)
                p[i][j] = (char)(j + v + i);
        }
        for (int i=0; i < 10; i ++) {
            for (int j=0; j < 13; j ++)
                if (p[i][j] != (char)(j + v + i)) {
                    printf("ERROR\n");
                    sys.fault_handler();
                }
            free(p[i]);
        }
        v ++;
    }
}


void hMain()
{
    sys.taskCreate(stressMalloc);
    sys.taskCreate(stressMalloc);
}
