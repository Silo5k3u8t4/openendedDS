#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TreeNode {
    char data;
    char* code;
    struct TreeNode* left;
    struct TreeNode* right;
};

struct TreeNode* newNode(char data) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    node->data = data;
    node->code = NULL;
    node->left = node->right = NULL;
    return node;
}

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

struct TreeNode* search(struct TreeNode* root, char data) {
    if (root == NULL || root->data == data) {
        return root;
    }
    if (data < root->data) {
        return search(root->left, data);
    }
    return search(root->right, data);
}

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

void decodeFile(char* inputFileName, char* outputFileName, struct TreeNode* root) {
    FILE *inputFile, *outputFile;
    inputFile = fopen(inputFileName, "rb");
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

void freeTree(struct TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root->code);
        free(root);
    }
}

int main() {
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
            char code[2] = {c, '\0'};
            root = insert(root, c, code);
        }
    }
    fclose(inputFile);

    char encodedFileName[] = "encoded.txt";
    encodeFile(inputFileName, encodedFileName, root);
    printf("File has been encoded.\n");

    printf("Encoded content:\n");
    FILE* encodedFile = fopen(encodedFileName, "rb");
    if (encodedFile == NULL) {
        printf("Error opening encoded file.\n");
        exit(1);
    }
    while ((c = fgetc(encodedFile)) != EOF) {
        printf("%c", c);
    }
    printf("\n");
    fclose(encodedFile);

    char decodedFileName[] = "decoded.txt";
    decodeFile(encodedFileName, decodedFileName, root);
    printf("File has been decoded.\n");

    FILE* decodedFile = fopen(decodedFileName, "r");
    if (decodedFile == NULL) {
        printf("Error opening decoded file.\n");
        exit(1);
    }
    printf("Decoded content:\n");
    while ((c = fgetc(decodedFile)) != EOF) {
        printf("%c", c);
    }
    printf("\n");
    fclose(decodedFile);

    freeTree(root);

    return 0;
}
