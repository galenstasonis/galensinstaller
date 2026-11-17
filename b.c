#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char buffer[1024];
    printf("Welcome, \n This program is intended to make your arch installation quicker/easier.\n");
    FILE *fp = popen("lsblk", "r");
    if (fp == NULL) {
        printf("lsblk not found, please install.");
        return 1;
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }
    pclose(fp);
    char diskName[30];
    printf("Please Select the disk you would like to partition (enter n to skip):\n");
    scanf("%s", diskName);
    printf("/dev/%s selected\n", diskName);
    char cfdiskCommand[] = "sudo cfdisk /dev/";
    strcat(cfdiskCommand, diskName);
    int status = system(cfdiskCommand);
    printf("returned to program with status %d\n", status);
    return 0;
}