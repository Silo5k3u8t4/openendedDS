#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define structure for BST node
struct TreeNode {
    char data;
    char* code;
    struct TreeNode* left;
    struct TreeNode* right;
};

// Function to create a new BST node
struct TreeNode* newNode(char data) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    node->data = data;
    node->code = NULL;
    node->left = node->right = NULL;
    return node;
}

// Function to insert a node into BST
struct TreeNode* insert(struct TreeNode* root, char data, char* code) {
    if (root == NULL) {
        root = newNode(data);
        root->code = strdup(code);
    } else if (data < root->data) {
        root->left = insert(root->left, data, code);
    } else if (data > root->data) {
        root->right = insert(root->right, data, code);
    }
    return root;
}

// Function to search for a node in BST
struct TreeNode* search(struct TreeNode* root, char data) {
    if (root == NULL || root->data == data) {
        return root;
    }
    if (data < root->data) {
        return search(root->left, data);
    }
    return search(root->right, data);
}

// Function to encode the input file using BST
void encodeFile(char* inputFileName, char* outputFileName, struct TreeNode* root) {
    FILE *inputFile, *outputFile;
    inputFile = fopen(inputFileName, "r");
    outputFile = fopen(outputFileName, "w");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening files.\n");
        exit(1);
    }

    char c;
    while ((c = fgetc(inputFile)) != EOF) {
        struct TreeNode* node = search(root, c);
        if (node == NULL) {
            printf("Error: Character '%c' not found in the tree.\n", c);
            exit(1);
        }
        fprintf(outputFile, "%s", node->code);
    }

    fclose(inputFile);
    fclose(outputFile);
}

// Function to decode the encoded file using BST
void decodeFile(char* inputFileName, char* outputFileName, struct TreeNode* root) {
    FILE *inputFile, *outputFile;
    inputFile = fopen(inputFileName, "r");
    outputFile = fopen(outputFileName, "w");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening files.\n");
        exit(1);
    }

    struct TreeNode* currentNode = root;
    char bit;
    while ((bit = fgetc(inputFile)) != EOF) {
        if (bit == '0') {
            currentNode = currentNode->left;
        } else if (bit == '1') {
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

    fclose(inputFile);
    fclose(outputFile);
}

// Function to free memory allocated for BST
void freeTree(struct TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root->code);
        free(root);
    }
}

int main() {
    // Input file name
    char inputFileName[] = "input.txt";

    // Build the BST with characters from the input file
    struct TreeNode* root = NULL;
    FILE *inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("Error opening input file.\n");
        exit(1);
    }
    char c;
    while ((c = fgetc(inputFile)) != EOF) {
        if (search(root, c) == NULL) {
            char code[2] = {c, '\0'};
            root = insert(root, c, code);
        }
    }
    fclose(inputFile);

    // Encode input file
    char encodedFileName[] = "encoded.txt";
    encodeFile(inputFileName, encodedFileName, root);
    printf("File has been encoded.\n");

    // Print encoded content
    printf("Encoded content:\n");
    FILE* encodedFile = fopen(encodedFileName, "r");
    if (encodedFile == NULL) {
        printf("Error opening encoded file.\n");
        exit(1);
    }
    while ((c = fgetc(encodedFile)) != EOF) {
        printf("%c", c);
    }
    printf("\n");
    fclose(encodedFile);

    // Decode encoded file
    char decodedFileName[] = "decoded.txt";
    decodeFile(encodedFileName, decodedFileName, root);
    printf("File has been decoded.\n");

    // Free allocated memory
    freeTree(root);

    return 0;
}
