#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
 

#define SYS_GPIO_DIR "/sys/class/gpio"

 

int main(int argc, char** argv){

    int port_num = 398, val, i;

    char buff[256];

    FILE *fp;

    

    //snprintf(buff, sizeof(buff), "%s/export", SYS_GPIO_DIR);
    snprintf(buff, sizeof(buff), "/sys/class/gpio/export");

    fp = fopen(buff, "w");

    //fprintf(fp, "%d\n", port_num);
    fprintf(fp, "398\n");

    fclose(fp);

#if 1    

    snprintf(buff, sizeof(buff), "%s/gpio%d/direction", SYS_GPIO_DIR, port_num);

    fp = fopen(buff, "w");

    fprintf(fp, "out\n");

    fclose(fp);

 

    snprintf(buff, sizeof(buff), "%s/gpio%d/value", SYS_GPIO_DIR, port_num);

    fp = fopen(buff, "w");

    setvbuf(fp, NULL, _IONBF, 0);

    

    val= 0;

    for(i = 0; i <10; i++){

        fprintf(fp, "%d\n", val);

        sleep(1);

        val = !val;

    }    

    fclose(fp);

    

    snprintf(buff, sizeof(buff), "%s/unexport", SYS_GPIO_DIR);

    fp = fopen(buff, "w");

    fprintf(fp, "%d\n", port_num);

    fclose(fp);
#endif
return 0;

    

}


