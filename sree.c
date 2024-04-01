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

// Function to encode user input and write to file
void encodeInputToFile(struct TreeNode* root, char* fileName) {
    FILE* outputFile = fopen(fileName, "w");
    if (outputFile == NULL) {
        printf("Error opening file for writing.\n");
        exit(1);
    }

    printf("Enter your text:\n");
    char c;
    while ((c = getchar()) != EOF && c != '\n') { // Read until newline
        struct TreeNode* node = search(root, c);
        if (node == NULL) {
            printf("Error: Character '%c' not found in the tree.\n", c);
            exit(1);
        }
        fprintf(outputFile, "%s", node->code);
    }
    fclose(outputFile);
    printf("Encoded content written to %s.\n", fileName);
}

// Function to read encoded data from file and decode
void decodeFile(struct TreeNode* root, char* fileName) {
    FILE* inputFile = fopen(fileName, "r");
    if (inputFile == NULL) {
        printf("Error opening file for reading.\n");
        exit(1);
    }

    printf("Decoded text: ");
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
            printf("%c", currentNode->data);
            currentNode = root;
        }
    }
    printf("\n");
    fclose(inputFile);
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

// Function to build Huffman tree from character frequencies
struct TreeNode* buildHuffmanTree(char* characters, int* frequencies, int numChars) {
    struct TreeNode* nodes[numChars];
    for (int i = 0; i < numChars; i++) {
        nodes[i] = newNode(characters[i]);
        nodes[i]->code = (char*)malloc(sizeof(char) * 100); // Arbitrary size for code
        snprintf(nodes[i]->code, 100, "%d", frequencies[i]); // Assign frequencies as initial codes
    }

    while (numChars > 1) {
        struct TreeNode* smallest1 = NULL;
        struct TreeNode* smallest2 = NULL;
        int index1, index2;
        for (int i = 0; i < numChars; i++) {
            if (nodes[i] != NULL) {
                if (smallest1 == NULL || frequencies[i] < frequencies[index1]) {
                    index2 = index1;
                    smallest2 = smallest1;
                    index1 = i;
                    smallest1 = nodes[i];
                } else if (smallest2 == NULL || frequencies[i] < frequencies[index2]) {
                    index2 = i;
                    smallest2 = nodes[i];
                }
            }
        }

        struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
        newNode->data = 0;
        newNode->code = (char*)malloc(sizeof(char) * 100);
        newNode->left = smallest1;
        newNode->right = smallest2;
        snprintf(newNode->code, 100, "%s%s", smallest1->code, smallest2->code);
        nodes[index1] = newNode;
        nodes[index2] = NULL;

        frequencies[index1] += frequencies[index2];
        frequencies[index2] = 0;

        numChars--;
    }

    return nodes[0];
}

int main() {
    int numChars;
    printf("Enter the number of characters in your alphabet: ");
    scanf("%d", &numChars);

    char characters[numChars];
    int frequencies[numChars];

    printf("Enter the characters and their frequencies:\n");
    for (int i = 0; i < numChars; i++) {
        scanf(" %c:%d", &characters[i], &frequencies[i]);
    }

    struct TreeNode* root = buildHuffmanTree(characters, frequencies, numChars);

    // User chooses to encode or decode
    int choice;
    printf("\n1. Encode\n2. Decode\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    char fileName[50]; // Buffer to store file name

    if (choice == 1) {
        printf("Enter the name of the output file to store encoded data: ");
        scanf("%s", fileName);
        encodeInputToFile(root, fileName);
    } else if (choice == 2) {
        printf("Enter the name of the file containing encoded data: ");
        scanf("%s", fileName);
        decodeFile(root, fileName);
    } else {
        printf("Invalid choice.\n");
    }

    // Free allocated memory
    freeTree(root);

    return 0;
}
