#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DOT '.'
#define DASH '-'
#define MORSE_CODES_SIZE 26

char* morse_codes[] = {
    ".-",    /* a */
    "-...",    /* b */
    "-.-.",    /* c */
    "-..",    /* d */
    ".",    /* e */
    "..-.",    /* f */
    "--.",    /* g */
    "....",    /* h */
    "..",    /* i */
    ".---",    /* j */
    "-.-",    /* k */
    ".-..",    /* l */
    "--",    /* m */
    "-.",    /* n */
    "---",    /* o */
    ".--.",    /* p */
    "--.-",    /* q */
    ".-.",    /* r */
    "...",    /* s */
    "-",    /* t */
    "..-",    /* u */
    "...-",    /* v */
    ".--",    /* w */
    "-..-",    /* x */
    "-.--",    /* y */
    "--.."    /* z */
    };

 /* Convert 0-25 to letters 'a' - 'z'*/

char digit_to_char(int digit) {
    if (0 <= digit && digit <= 25)
        {
        return digit + 97;
    }
     else
        return '\0';
}

/*Convert 'a' - 'z' and 'A' - 'Z' to 0-25 */

int char_to_digit(char ch) {
    ch = tolower(ch);
    if ('a' <= ch && ch <= 'z')
        {
        return ch - 97;
    }
    else
        return -1;
}


/* the node of binary tree */

typedef struct Node {
    char data;
    struct Node* left;
    struct Node* right;
} Node;

Node* node_new(char data, Node* left, Node* right) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    } else {
        node->data = data;
        node->left = left;
        node->right = right;
        return node;
    }
}

Node* node_new_default() {
    return node_new('\0', NULL, NULL);
}

/* Morse code binary tree
   '.' to left
   '-' to right */
typedef struct MorseTree {
    Node* root;
} MorseTree;

MorseTree* morse_tree_new() {
    MorseTree* tree = (MorseTree*)malloc(sizeof(MorseTree));
    if (tree == NULL) {
        return NULL;
    } else {
        tree->root = node_new_default();
        if (tree->root == NULL) {
            free(tree);
            return NULL;
        } else {
            return tree;
        }
    }
}

/*
  Initialize the binary tree according to the Morse code table */

void morse_tree_init(MorseTree* tree, char** morse_codes, int size) {
    for (int i = 0; i < size; i++) {
        char* morse_code = morse_codes[i];
        Node* node = tree->root;
        for (char* p_ch = morse_code; *p_ch != '\0'; p_ch++) {
            if (*p_ch == DOT) {
                if (node->left == NULL) {
                    node->left = node_new_default();
                }
                node = node->left;
            } else if (*p_ch == DASH) {
                if (node->right == NULL) {
                    node->right = node_new_default();
                }
                node = node->right;
            }
        }
        node->data = digit_to_char(i);
    }
}


/* Encode the letters into Morse code
   ch :The letter to encode
   morse_codes :List of Morse codes
   char* :Morse code for the letter, or NULL if there is no Morse code for the letter */

char* morse_encode(char ch, char** morse_codes) {
    int index = char_to_digit(ch);
    if (index == 0) {
        return NULL;
    } else {
        return morse_codes[index];
    }
}

/* Decode Morse code and get the Latin letter of Morse code
 *  morse_code : Morse code
 * tree :Morse code binary tree
 * char :
 */

char morse_decode(char* morse_code, MorseTree* tree) {
    char* p_ch = morse_code;
    Node* node = tree->root;
    while (*p_ch != '\0') {
        if (*p_ch == DOT) {
            node = node->left;
            if (node == NULL) {
                return '\0';
            }
        } else if (*p_ch == DASH) {
            node = node->right;
            if (node == NULL) {
                return '\0';
            }
        } else {
            return '\0';
        }
        p_ch++;
    }
    return node->data;
}

/* Encode the letters in the file */
void encode_by_file() {
    FILE* file = fopen("encode_input.txt", "r");
    char buf[1024];
    int i = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (isalpha(ch)) {
            buf[i++] = ch;
        }
    }
    buf[i] = '\0';
    printf("sentence: [%s]\n", buf);
    printf("morse codes:\n");
    for (int k = 0; k < i; k++) {
        char ch = buf[k];
        char* morse_code = morse_encode(ch, morse_codes);
        printf("%s ", morse_code);
    }
    printf("\n");
}

/* Decode the Morse code in the file */
void decode_by_file(MorseTree* tree) {
    FILE* file = fopen("decode_input.txt", "r");
    char buf[1024][1024];
    int i = 0;
    while (fscanf(file, "%s", buf + i) != -1) {
        i++;
    }

    printf("morse codes: \n");
    for (int k = 0; k < i; k++) {
        printf("%s ", buf[k]);
    }
    printf("\n");

    printf("sentence: [");
    for (int k = 0; k < i; k++) {
        printf("%c", morse_decode(buf[k], tree));
    }
    printf("]\n");
}


/* Read a string */
char* read_line() {
    long buf_size = 1024;
    char* line = (char*) malloc(sizeof(char) * buf_size + 1);
    int i = 0;
    while (true) {
        char ch = getchar();
        if (ch == '\n' || ch == EOF) {
            break;
        }
        if (i >= buf_size) {
            buf_size *= 2;
            line = (char*) realloc(line, sizeof(char) * buf_size + 1);
        }
        line[i++] = ch;

    }
    line[i] = '\0';
    return line;
}

void encode_by_user() {
    int alpha_cnt[26] = {0};
    int morse_cnt[2] = {0};

    printf("input sentence: ");
    char* line = read_line();
    printf("sentence: [%s]\n", line);
    printf("morse codes:\n");
    for (int k = 0; line[k] != '\0'; k++) {
        char ch = line[k];
        alpha_cnt[char_to_digit(ch)]++;
        char* morse_code = morse_encode(ch, morse_codes);
        for (char* p = morse_code; *p != '\0'; p++) {
            if (*p == DOT) {
                morse_cnt[0]++;
            } else {
                morse_cnt[1]++;
            }
        }
        printf("%s ", morse_code);
    }
    printf("\n");
    printf("alpha statistics: \n");
    for (int i = 0; i < 26; i++) {
        printf("%c=%d, ", digit_to_char(i), alpha_cnt[i]);
    }
    printf("\n");
    printf("morse statistics: \n");
    printf("[.]=%d, [-]=%d\n", morse_cnt[0], morse_cnt[1]);

    free(line);
}

void decode_by_user(MorseTree* tree) {
    int alpha_cnt[26] = {0};
    int morse_cnt[2] = {0};

    printf("input morse codes: ");
    char* line = read_line();
    int buf_size = 1024;
    char (*morse_codes)[10] = (char(*)[10])
        malloc(sizeof(char) * 10 * buf_size);
    char* token = strtok(line, " ");
    int i = 0;
    strcpy(morse_codes[i], token);
    i++;

    while (true) {
        token = strtok(NULL, " ");
        if (token == NULL) {
            break;
        }
        if (i >= buf_size) {
            buf_size *= 2;
            morse_codes = (char(*)[10]) realloc(morse_codes, sizeof(char) * 10 * buf_size);
        }
        strcpy(morse_codes[i], token);
        i++;
    }

    printf("morse codes: \n");
    for (int k = 0; k < i; k++) {
        printf("%s ", morse_codes[k]);
        for (char* p = morse_codes[k]; *p != '\0'; p++) {
            if (*p == DOT) {
                morse_cnt[0]++;
            } else {
                morse_cnt[1]++;
            }
        }
    }
    printf("\n");

    printf("sentence: [");
    for (int k = 0; k < i; k++) {
        char ch = morse_decode(morse_codes[k], tree);
        alpha_cnt[char_to_digit(ch)]++;
        printf("%c", ch);
    }
    printf("]\n");
    printf("alpha statistics: \n");
    for (int i = 0; i < 26; i++) {
        printf("%c=%d, ", digit_to_char(i), alpha_cnt[i]);
    }
    printf("\n");
    printf("morse statistics: \n");
    printf("[.]=%d, [-]=%d\n", morse_cnt[0], morse_cnt[1]);

    free(morse_codes);
    free(line);
}

int main()
{
    MorseTree* tree = morse_tree_new();                        /* Initialize */
    morse_tree_init(tree, morse_codes, MORSE_CODES_SIZE);
    printf("0: encode by file\n");
    printf("1: decode by file\n");
    printf("2: encode by user\n");
    printf("3: decode by user\n");
    printf("input your choice:\n");
    int choice = 0;
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 0: encode_by_file();
            printf("\n");              break;
        case 1: decode_by_file(tree);
            printf("\n");              break;
        case 2: encode_by_user();      break;
        case 3: decode_by_user(tree);  break;
        default: printf("Error choice!\n");
            printf("\n");               break;
        }
        printf("\n");
    return 0;
}
