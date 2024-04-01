#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// BST Node
typedef struct Node {
    char key;
    char code[4]; // 3 characters + null terminator
    struct Node* left;
    struct Node* right;
} Node;

// Function prototypes
Node* createNode(char key);
Node* insert(Node* root, char key);
Node* search(Node* root, char key);
void generateRandomCode(char code[]);
void encodeTextFile(const char* inputFileName, const char* outputFileName, Node* root);
void decodeTextFile(const char* inputFileName, const char* outputFileName, Node* root);

int main() {
    Node* root = NULL;
    srand(time(NULL)); // Seed for random number generation

    // Example text file names
    const char* inputFileName = "input.txt";
    const char* encodedFileName = "encoded.txt";
    const char* decodedFileName = "decoded.txt";

    // Build BST and encode text file
    FILE* inputFile = fopen(inputFileName, "r");
    if (inputFile) {
        char ch;
        while ((ch = fgetc(inputFile)) != EOF) {
            root = insert(root, ch);
        }
        fclose(inputFile);
        encodeTextFile(inputFileName, encodedFileName, root);
        printf("Text file encoded successfully.\n");

        // Decode encoded text file
        decodeTextFile(encodedFileName, decodedFileName, root);
        printf("Encoded text file decoded successfully.\n");

        // Cleanup BST
        // (Deletion of tree nodes and freeing memory should be implemented if necessary)

    } else {
        printf("Error opening file: %s\n", inputFileName);
    }

    return 0;
}

// Function to create a new node
Node* createNode(char key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to insert a new node into BST
Node* insert(Node* root, char key) {
    if (root == NULL) {
        return createNode(key);
    }

    if (key < root->key) {
        root->left = insert(root->left, key);
    } else if (key > root->key) {
        root->right = insert(root->right, key);
    }

    return root;
}

// Function to search for a node in BST
Node* search(Node* root, char key) {
    if (root == NULL || root->key == key) {
        return root;
    }

    if (key < root->key) {
        return search(root->left, key);
    }

    return search(root->right, key);
}

// Function to generate a random code
void generateRandomCode(char code[]) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+-=[]{}|;':,.<>?";
    const size_t charsetSize = strlen(charset);

    for (int i = 0; i < 3; ++i) {
        code[i] = charset[rand() % charsetSize];
    }
    code[3] = '\0';
}

// Function to encode text file
void encodeTextFile(const char* inputFileName, const char* outputFileName, Node* root) {
    FILE* inputFile = fopen(inputFileName, "r");
    FILE* outputFile = fopen(outputFileName, "w");

    if (inputFile && outputFile) {
        char ch;
        while ((ch = fgetc(inputFile)) != EOF) {
            Node* node = search(root, ch);
            if (node != NULL) {
                fprintf(outputFile, "%s", node->code);
            } else {
                // Handle cases where character not found in BST
                // For simplicity, let's just write the character as is
                fprintf(outputFile, "%c", ch);
            }
        }
        fclose(inputFile);
        fclose(outputFile);
    } else {
        printf("Error opening file: %s or %s\n", inputFileName, outputFileName);
    }
}

// Function to decode encoded text file
void decodeTextFile(const char* inputFileName, const char* outputFileName, Node* root) {
    FILE* inputFile = fopen(inputFileName, "r");
    FILE* outputFile = fopen(outputFileName, "w");

    if (inputFile && outputFile) {
        char code[4];
        while (fscanf(inputFile, "%3s", code) == 1) {
            // Search for the node with the given code
            // For simplicity, assume there are no duplicate codes in the BST
            Node* currentNode = root;
            while (currentNode != NULL) {
                if (strcmp(currentNode->code, code) == 0) {
                    fprintf(outputFile, "%c", currentNode->key);
                    break;
                } else if (strcmp(currentNode->code, code) < 0) {
                    currentNode = currentNode->right;
                } else {
                    currentNode = currentNode->left;
                }
            }
            if (currentNode == NULL) {
                // If code not found, write the code itself as is
                fprintf(outputFile, "%s", code);
            }
        }
        fclose(inputFile);
        fclose(outputFile);
    } else {
        printf("Error opening file: %s or %s\n", inputFileName, outputFileName);
    }
}
