#include <stdio.h>

int main() {
    // Define a character array to store the name
    char name[50];

    // Prompt the user for their name
    printf("What is your name? ");
    
    // Read the name from the user input
    scanf("%s", name);

    // Greet the user
    printf("Hello, %s!\n", name);

    return 0;
}