#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LENGTH 100
#define MAX_WORDS 1000

// BST Node
typedef struct Node {
    char word[MAX_WORD_LENGTH];
    char code[4]; // 3 characters + null terminator
    struct Node* left;
    struct Node* right;
} Node;

// Function prototypes
Node* createNode(const char* word, const char* code);
Node* insert(Node* root, const char* word, const char* code);
Node* search(Node* root, const char* key, int searchByCode);
void generateRandomCode(char code[]);
void encodeTextFile(const char* inputFileName, const char* outputFileName, Node* root);
void decodeTextFile(const char* inputFileName, const char* outputFileName, Node* root);
void displayBST(Node* root);
void displayDecodedText(const char* inputFileName, const char* decodedFileName);
void freeBST(Node* root);

// Function to create a new node
Node* createNode(const char* word, const char* code) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->word, word);
    strcpy(newNode->code, code);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to insert a new node into BST
Node* insert(Node* root, const char* word, const char* code) {
    if (root == NULL) {
        return createNode(word, code);
    }

    int compareResult = strcmp(word, root->word);
    if (compareResult < 0) {
        root->left = insert(root->left, word, code);
    } else if (compareResult > 0) {
        root->right = insert(root->right, word, code);
    }

    return root;
}

// Function to search for a node in BST by word or code using inorder traversal
Node* search(Node* root, const char* key, int searchByCode) {
    if (root == NULL) {
        return NULL;
    }

    Node* foundNode = search(root->left, key, searchByCode);
    if (foundNode != NULL) {
        return foundNode;
    }

    if ((searchByCode == 0 && strcmp(root->word, key) == 0) ||
        (searchByCode == 1 && strcmp(root->code, key) == 0)) {
        return root;
    }

    return search(root->right, key, searchByCode);
}


// Function to generate a random code
void generateRandomCode(char code[]) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
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
        char line[MAX_WORD_LENGTH];
        while (fgets(line, sizeof(line), inputFile) != NULL) {
            char* token = strtok(line, ", \n");
            while (token != NULL) {
                // Search for the word in the BST
                Node* node = search(root, token, 0); // Search by word
                if (node != NULL) {
                    // Write the code corresponding to the word
                    fprintf(outputFile, "%s ", node->code);
                } else {
                    // If word not found in BST, it's an error as each word should have a code
                    printf("Error: Word \"%s\" not found in the BST.\n", token);
                }
                token = strtok(NULL, ", \n");
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
            // Search for the code in the BST
            Node* currentNode = search(root, code, 1); // Search by code
            if (currentNode != NULL) {
                fprintf(outputFile, "%s ", currentNode->word);
            } else {
                printf("Error: Code \"%s\" not found in the BST.\n", code);
                fprintf(outputFile, "ERROR ");
            }
        }
        fclose(inputFile);
        fclose(outputFile);
    } else {
        printf("Error opening file: %s or %s\n", inputFileName, outputFileName);
    }
}


// Function to display the binary search tree (BST) containing encoded words and their codes
void displayBST(Node* root) {
    if (root != NULL) {
        displayBST(root->left);
        printf("Word: %s, Code: %s\n", root->word, root->code);
        displayBST(root->right);
    }
}

// Function to display decoded text from file and compare with input file
void displayDecodedText(const char* inputFileName, const char* decodedFileName) {
    FILE* inputFile = fopen(inputFileName, "r");
    FILE* decodedFile = fopen(decodedFileName, "r");

    if (inputFile && decodedFile) {
        char inputWord[MAX_WORD_LENGTH];
        char decodedWord[MAX_WORD_LENGTH];

        while (fscanf(inputFile, "%s", inputWord) != EOF && fscanf(decodedFile, "%s", decodedWord) != EOF) {
            // Compare the words from input and decoded files
            if (strcmp(inputWord, decodedWord) != 0) {
                // Words don't match, add missing punctuation
                printf("%s, ", decodedWord);
            } else {
                printf("%s ", decodedWord);
            }
        }

        // Check for any remaining words in the decoded file
        while (fscanf(decodedFile, "%s", decodedWord) != EOF) {
            printf("%s ", decodedWord);
        }

        printf("\n");

        fclose(inputFile);
        fclose(decodedFile);
    } else {
        printf("Error opening file: %s or %s\n", inputFileName, decodedFileName);
    }
}


// Function to recursively free BST nodes
void freeBST(Node* root) {
    if (root != NULL) {
        freeBST(root->left);
        freeBST(root->right);
        free(root);
    }
}

int main() {
    Node* root = NULL;
    srand(time(NULL)); // Seed for random number generation

    // Example text file names
    const char* inputFileName = "input.txt";
    const char* encodedFileName = "encoded.txt";
    const char* decodedFileName = "decoded.txt";

    // Read words from input file
    FILE* inputFile = fopen(inputFileName, "r");
    if (inputFile) {
        char line[MAX_WORD_LENGTH];
        while (fgets(line, sizeof(line), inputFile) != NULL) {
            char* token = strtok(line, ", \n");
            while (token != NULL) {
                // Check if word already exists in the BST
                Node* existingNode = search(root, token, 0);
                if (existingNode == NULL) {
                    char code[4];
                    generateRandomCode(code);
                    root = insert(root, token, code);
                }
                token = strtok(NULL, ", \n");
            }
        }
        fclose(inputFile);

        // Display the encoded words and their codes (BST)
        printf("Binary Search Tree (BST) containing encoded words:\n");
        displayBST(root);

        // Encode text file
        encodeTextFile(inputFileName, encodedFileName, root);
        printf("Text file encoded successfully.\n");

        // Decode encoded text file
        decodeTextFile(encodedFileName, decodedFileName, root);
        printf("Encoded text file decoded successfully.\n");

        // Display decoded text
        printf("Decoded text:\n");
        displayDecodedText(inputFileName,decodedFileName);

    } else {
        printf("Error opening file: %s\n", inputFileName);
    }

    // Cleanup BST
    freeBST(root);

    return 0;
}
