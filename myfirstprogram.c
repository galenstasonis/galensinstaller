#include <stdio.h>
#include <string.h>

int main () {
    int studentID = 13202;
    int studentAge = 15;
    float studentGradePercent = 79.5;
    char studentGradeLetter = 'C';
    char studentGradeSign = '+';
    printf("Student ID: %d\n", studentID);
    printf("Student Age: %d\n", studentAge);
    printf("Student Grade Percent: %.1f\n", studentGradePercent);
    printf("Student Grade Letter: %c%c\n", studentGradeLetter, studentGradeSign);
    
    char answer[5];
    printf("Continue? [1/0]\n");
    scanf ("%s", answer);
    if (strcmp(answer, "0") == 0) {
        return 0;
    }
    else if (strcmp(answer, "1") == 0) {
        int length;
        int width;
        int area;
        length = 10;
        width = 5;
        area = length * width;
        printf("Area: %d\n", area);
    }
    return 0;
}