#ifdef aaa
#include "preproc.h"

int main(int argc, char *argv[]){
    while(--argc > 0){
        mcro_exec(argv[argc]);

    }
    return 0;
}

#endif