#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 5

char* readline();
char** split_string(char*);


/* This is 80 chars: ---------------------------------------------------------*/

struct map_node {
    char word[MAX_WORD_LENGTH + 1];   //key
    int num;                        //value
    struct map_node *next;
};

/**
 * @brief Create the first map_node and push the first word.
 * @param first_word = the first word to enter the map.
 * @return pointer to the head or NULL if out of memory.
 */
struct map_node *create_map(char *first_word) {
    struct map_node *head = malloc(sizeof(struct map_node));
    if (head == NULL) {
        return NULL;
    }
    strcpy(head->word, first_word);
    head->num = 1;
    head->next = NULL;
    return head;
}


/**
 * @brief Pushes a word-node at the end of the maplist or add 1 to
 * the num in exist word.
 * @param head= head of the mapList.
 * @param new_word= the new Word to push.
 * @return pointer to the head or NULL if out of memory.
**/
struct map_node *push_back(struct map_node *head, char *new_word) {
    struct map_node *current = head;
    if (current->next == NULL) {
        if (strcmp(current->word, new_word) == 0) {
            current->num++;
            return head;
        }
    }
    while (current->next != NULL) {
        if (strcmp(current->word, new_word) == 0) {
            current->num++;
            return head;
        }
        current = current->next;
    }
    // Current now points to last node
    // Create new node
    struct map_node *new_node = malloc(sizeof(struct map_node));
    if (new_node == NULL) {
        return NULL;
    }; // Fail in case of an error
    strcpy(new_node->word, new_word);
    new_node->num = 1;
    new_node->next = NULL;
    current->next = new_node;
    return head;
}

/**
 * @brief free the given map
 * @param head= head of the mapList
 * @return -
**/
void free_map(struct map_node *head) {
    while (head) {
        struct map_node *node_to_delete = head;
        head = head->next;
        free(node_to_delete);
    }
}

/**
 * @brief Get the number of accurrences of a word in the map
 * @param head= head of the mapList.
 * @param word= the word we search it accurences.
 * @return integer -the number of accurences.
**/
int get_num_of_occurences(struct map_node *head, char *word) {
    struct map_node *current = head;
    while (current->next != NULL) {
        if (strcmp(current->word, word) == 0) {
            return current->num;
        }
        current = current->next;
    }
    return 0;
}

/**
 * @brief Check if they can use the magazine for the ransome note.
 * @param magazine_count= Numbers of words in the magazine.
 * @param magazine= an array of words from the magazine
 * @param note_count= Numbers of words in the note.
 * @param note= an array of words from the note
 * @return -
**/

void checkMagazine(int magazine_count, char **magazine, int note_count, char **note) {
    if (magazine_count < note_count)
        printf("No");

    struct map_node *magazine_head = create_map(magazine[0]);
    if (magazine_head == NULL) return;
    for (int i = 1; i < magazine_count; i++) {
        if (push_back(magazine_head, magazine[i]) == NULL) {
            free_map(magazine_head);
            return;
        }
    }

    struct map_node *note_head = create_map(note[0]);
    if (note_head == NULL) {
        free_map(magazine_head);
        return;
    }
    for (int i = 1; i < note_count; i++) {
        if (push_back(note_head, note[i]) == NULL) {
            free_map(magazine_head);
            free_map(note_head);
            return;
        }
    }

    for (struct map_node *current_note_word = note_head;
         current_note_word != NULL;current_note_word=current_note_word->next) {
        int occur_num_in_magazine=get_num_of_occurences(magazine_head,
                                             current_note_word->word);
        if (occur_num_in_magazine < current_note_word->num) {
            printf("No");
            free_map(magazine_head);
            free_map(note_head);
            return;
        }
    }
    printf("Yes");
    free_map(magazine_head);
    free_map(note_head);
}

int main()
{
    char** mn = split_string(readline());

    char* m_endptr;
    char* m_str = mn[0];
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    char* n_endptr;
    char* n_str = mn[1];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** magazine_temp = split_string(readline());

    char** magazine = malloc(m * sizeof(char*));

    for (int i = 0; i < m; i++) {
        char* magazine_item = *(magazine_temp + i);

        *(magazine + i) = magazine_item;
    }

    int magazine_count = m;

    char** note_temp = split_string(readline());

    char** note = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* note_item = *(note_temp + i);

        *(note + i) = note_item;
    }

    int note_count = n;

    checkMagazine(magazine_count, magazine, note_count, note);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!line) {
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}
