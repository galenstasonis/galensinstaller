#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main() {
    float f1= 35e3;
    printf("%.0f\n", f1);
    printf("%zu bytes memory usage\n", sizeof(f1));
    float result =(float) 5/2;
    printf("%.1f\n", result);
    time_t currentTime; 
    time(&currentTime);
    printf("Current time: %s", ctime(&currentTime));
    int division = 5 / 2;
    int remainder = 5 % 2;
    printf("Answer: %d, with a remainder of %d\n", division, remainder);
    double decimal = 5.0 / 2.0;
    printf("Answer: %.1f\n", decimal);
    printf("%zu bytes memory usage\n", sizeof(decimal));
    int x;
    x = 5;  
    printf ("X = %d\n", x);
    ++x;
    printf ("X = %d\n", x);
    bool isTrumpAmazing = false;
    bool doIHateTarun = true;
    printf(ANSI_COLOR_RED "Is Trump amazing? %d\n" ANSI_COLOR_RESET, isTrumpAmazing );
    printf("Do I hate Tarun? %d\n", doIHateTarun);
    printf("Are both booleans the same? %d\n", isTrumpAmazing == doIHateTarun);\
    if (isTrumpAmazing == 0) {
        printf("Thank you for hating trump, you are a good person\n");
    }
    else if (doIHateTarun == 1) {
        printf("Thank you for hating Tarun, you are a good person, but why the fuck do you like trump?\n");
    }
    else {
        printf("You are a bad person for liking trump\n");
    }
    return 0;

}