#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TRACKS 100

// Structure for stack node
struct StackNode {
    char data[256];
    struct StackNode* next;
};

// Structure for stack
struct Stack {
    struct StackNode* top;
};

// Function to initialize a stack
struct Stack* createStack() {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->top = NULL;
    return stack;
}

// Function to check if stack is empty
int isEmpty(struct Stack* stack) {
    return stack->top == NULL;
}

// Function to push an item to stack
void push(struct Stack* stack, char* data) {
    struct StackNode* newNode = (struct StackNode*)malloc(sizeof(struct StackNode));
    strcpy(newNode->data, data);
    newNode->next = stack->top;
    stack->top = newNode;
}

// Function to pop an item from stack
char* pop(struct Stack* stack) {
    if (isEmpty(stack)) {
        return NULL;
    }
    struct StackNode* temp = stack->top;
    stack->top = stack->top->next;
    char* popped = (char*)malloc(strlen(temp->data) + 1);
    strcpy(popped, temp->data);
    free(temp);
    return popped;
}

// Function to display the contents of the stack
void displayStack(struct Stack* stack) {
    struct StackNode* temp = stack->top;
    int count = 1;
    while (temp != NULL) {
        printf("%d. %s\n", count++, temp->data);
        temp = temp->next;
    }
}

int main() {
    FILE *csvFile = fopen("spotify_tracks.csv", "r");
    if (!csvFile) {
        printf("Error opening the CSV file.\n");
        return 1;
    }

    struct Stack* trackStack = createStack();
    int numTracks = 0;

    // Read track links from the CSV and push them onto the stack
    char trackLink[256];
    while (fgets(trackLink, sizeof(trackLink), csvFile)) {
        // Remove newline character
        trackLink[strcspn(trackLink, "\n")] = '\0';
        push(trackStack, trackLink);
        numTracks++;
    }

    fclose(csvFile);

    // Display the track links
    printf("Spotify Track Links:\n");
    displayStack(trackStack);

    // Ask user for choice and play the track
    int choice;
    while (1) {
        printf("Enter the number of the track you want to play (0 to exit): ");
        scanf("%d", &choice);

        if (choice == 0) {
            break;
        } else if (choice >= 1 && choice <= numTracks) {
            struct Stack* tempStack = createStack();
            // Pop tracks until the desired track is reached
            for (int i = 1; i < choice; i++) {
                push(tempStack, pop(trackStack));
            }
            char* chosenTrack = pop(trackStack); // Get the chosen track
            if (chosenTrack == NULL) {
                printf("Error: Track not found.\n");
            } else {
                printf("Playing: %s\n", chosenTrack);
                // Open the track link in the default web browser
                char command[512];
                #ifdef _WIN32
                    snprintf(command, sizeof(command), "start %s", chosenTrack);
                #elif __APPLE__
                    snprintf(command, sizeof(command), "open %s", chosenTrack);
                #else
                    snprintf(command, sizeof(command), "xdg-open %s", chosenTrack);
                #endif
                system(command);
                free(chosenTrack);
            }
            // Restore the remaining tracks to the main stack
            while (!isEmpty(tempStack)) {
                push(trackStack, pop(tempStack));
            }
            free(tempStack);
        } else {
            printf("Invalid choice. Please enter a valid track number.\n");
        }
    }

    // Free memory used by the stack
    while (!isEmpty(trackStack)) {
        free(pop(trackStack));
    }
    free(trackStack);

    return 0;
}
