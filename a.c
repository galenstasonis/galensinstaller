#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

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
    bool isTrumpAmazing = true;
    bool doIHateTarun = false;
    printf(ANSI_COLOR_RED "Is Trump amazing? %d\n" ANSI_COLOR_RESET, isTrumpAmazing );
    printf("Do I hate Tarun? %d\n", doIHateTarun);
    printf("Are both booleans the same? %d\n", isTrumpAmazing == doIHateTarun);
    if (isTrumpAmazing == 0) {
        printf("Thank you for hating trump, you are a good person\n");
    }
    else if (doIHateTarun == 1) {
        printf("Thank you for hating Tarun, you are a good person, but why the fuck do you like trump?\n");
    }
    else {
        printf("You are a bad person for liking trump\n");
    }
    if (isTrumpAmazing == 1 && doIHateTarun == 0) {
        printf("genuinely fuck you end your like just die already\n");
    }
    int day;
    day = 7;
    switch (day) {
        case 1:
            printf("Monday\n");
            break;
        case 2:
            printf("Tuesday\n");
            break;
        case 3:
            printf("Wednesday\n");
            break;
        case 4:
            printf("Thursday\n");
            break;
        case 5:
            printf("Friday\n");
            break;
        case 6:
            printf("Saturday\n");
            break;
        case 7:
            printf("Sunday\n");
            break;
        default:
            printf("Invalid day\n");
    }
    int number;
    number = -100;
    while (number >0) {
        printf("%d\n", number);
        --number;
        usleep(20000);
    }   
    int multiplesOfFive[] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};
    multiplesOfFive[3] = 100;
    printf("Multiples of 5: %d\n", multiplesOfFive[3]);
    printf("Size of array: %zu bytes\n", sizeof(multiplesOfFive));
    char greetings[] = "I hate Trump";
    printf("%s\n", greetings);
    char str1[20] = "Hello, ";
    char str2[] = "World!";
    strcat(str1, str2);
    printf("%s\n", str1);
    char authorization[2];

    printf("Continue? [Y/n]: \n");
    scanf("%s", authorization);
    char yes[] = "y";
    char no[] = "n";
    if (strcmp(authorization, yes) == 0) {
        printf("You chose to continue\n");
    } else if (strcmp(authorization, no) == 0) {
        return 0;
    }
    printf("Continuing...\n");
    char character;
    printf("Enter a letter/number\n");
    scanf(" %c", &character);  
    if (isdigit(character)) {
        printf("You entered a number\n");
    } else if (isalpha(character)) {
        printf("You entered a letter\n");
    } else {
        printf("You entered a special character\n");
    }
    char name[50];
    printf("Enter your name: ");
    scanf("%s", name);
    printf("Hello, %s!\n", name);
    return 0;


}