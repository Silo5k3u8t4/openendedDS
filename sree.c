#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CODE_LENGTH 5 // Define the length of the encoding code

// Define the TreeNode structure
struct TreeNode {
    char data;
    char* code;
    struct TreeNode* left;
    struct TreeNode* right;
};

// Function to generate a new TreeNode
struct TreeNode* newNode(char data) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    node->data = data;
    node->code = NULL;
    node->left = node->right = NULL;
    return node;
}

// Function to insert a TreeNode into the tree
struct TreeNode* insert(struct TreeNode* root, char data, char* code) {
    if (root == NULL) {
        root = newNode(data);
        root->code = strdup(code);
    } else if (strcmp(code, root->code) < 0) {
        root->left = insert(root->left, data, code);
    } else if (strcmp(code, root->code) > 0) {
        root->right = insert(root->right, data, code);
    } else {
        // Handle case where code is already in use
        free(code);
    }
    return root;
}

// Function to generate a random code
char* generateRandomCode(int length) {
    char* code = (char*)malloc((length + 1) * sizeof(char));
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    srand(time(NULL));

    for (int i = 0; i < length; ++i) {
        int index = rand() % (sizeof(charset) - 1);
        code[i] = charset[index];
    }
    code[length] = '\0';
    return code;
}

// Function to search for a TreeNode with given code
struct TreeNode* search(struct TreeNode* root, char code) {
    if (root == NULL || root->data == code) {
        return root;
    }
    if (code < root->data) {
        return search(root->left, code);
    }
    return search(root->right, code);
}

// Function to print the contents of the tree
void printTree(struct TreeNode* root) {
    if (root != NULL) {
        printf("Data: %c, Code: %s\n", root->data, root->code);
        printTree(root->left);
        printTree(root->right);
    }
}

// Function to encode a file
void encodeFile(char* inputFileName, char* outputFileName, struct TreeNode* root) {
    FILE *inputFile, *outputFile;
    inputFile = fopen(inputFileName, "r");
    outputFile = fopen(outputFileName, "wb");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening files.\n");
        exit(1);
    }

    char c;
    unsigned char buffer = 0; // Buffer to store bits
    int count = 0; // Count of bits written to buffer

    while ((c = fgetc(inputFile)) != EOF) {
        struct TreeNode* node = search(root, c);
        if (node == NULL) {
            printf("Error: Character '%c' not found in the tree.\n", c);
            exit(1);
        }
        char *code = node->code;
        while (*code != '\0') {
            // Shift buffer to left and add bit
            buffer = (buffer << 1) | (*code - '0');
            count++;
            // If buffer is full, write it to output file
            if (count == 8) {
                fwrite(&buffer, sizeof(unsigned char), 1, outputFile);
                buffer = 0;
                count = 0;
            }
            code++;
        }
    }

    // If there are remaining bits in buffer, write them to output file
    if (count > 0) {
        buffer <<= (8 - count);
        fwrite(&buffer, sizeof(unsigned char), 1, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);
}

// Function to decode a file
void decodeFile(char* inputFileName, char* outputFileName, struct TreeNode* root) {
    FILE *inputFile, *outputFile;
    inputFile = fopen(inputFileName, "rb");
    outputFile = fopen(outputFileName, "w");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening files.\n");
        exit(1);
    }

    struct TreeNode* currentNode = root;
    unsigned char buffer; // Buffer to store bits
    int count = 0; // Count of bits read from buffer

    if (root == NULL) {
        printf("Error: Tree is empty.\n");
        exit(1);
    }

    while (fread(&buffer, sizeof(unsigned char), 1, inputFile) > 0) {
        for (int i = 7; i >= 0; --i) {
            char bit = (buffer >> i) & 1; // Extract each bit from buffer
            if (bit == 0) {
                if (currentNode->left == NULL) {
                    printf("Error: Unexpected end of tree traversal. Exiting.\n");
                    exit(1);
                }
                currentNode = currentNode->left;
            } else if (bit == 1) {
                if (currentNode->right == NULL) {
                    printf("Error: Unexpected end of tree traversal. Exiting.\n");
                    exit(1);
                }
                currentNode = currentNode->right;
            } else {
                printf("Invalid bit '%c' in encoded file.\n", bit);
                exit(1);
            }
            if (currentNode->left == NULL && currentNode->right == NULL) {
                fprintf(outputFile, "%c", currentNode->data);
                currentNode = root;
            }
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}

// Function to free the memory allocated for the tree
void freeTree(struct TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root->code);
        free(root);
    }
}

// Main function
int main() {
    // Example usage
    char inputFileName[] = "input.txt";

    struct TreeNode* root = NULL;
    FILE *inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("Error opening input file.\n");
        exit(1);
    }
    char c;
    while ((c = fgetc(inputFile)) != EOF) {
        if (search(root, c) == NULL) {
            char* code = generateRandomCode(CODE_LENGTH);
            root = insert(root, c, code);
        }
    }
    fclose(inputFile);

    printf("Tree contents:\n");
    printTree(root);

    char encodedFileName[] = "encoded.txt"; // Encode into a text file
    encodeFile(inputFileName, encodedFileName, root);
    printf("File has been encoded.\n");

    char decodedFileName[] = "decoded.txt";
    decodeFile(encodedFileName, decodedFileName, root);
    printf("File has been decoded.\n");

    freeTree(root);

    return 0;
}
