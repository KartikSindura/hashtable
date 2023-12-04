#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAP 100

// hash function
int hash_function(char *str) {
    int i = 0;
    i += abs(str[0] - str[1]);
    return i % CAP;
}
struct ht_item {
    char *key;
    char *value;
};
struct ll {
    struct ht_item *item;
    struct ll *next;
};
struct ht {
    struct ht_item **items;
    int size;
    int count;
    struct ll **buckets;
};

struct ll **create_buckets(struct ht *table) {
    struct ll **buckets =
        (struct ll **)calloc(table->size, sizeof(struct ll *));

    for (int i = 0; i < table->size; i++) buckets[i] = NULL;

    return buckets;
}

struct ll *allocate_ll(void) {
    struct ll *list = (struct ll *)malloc(sizeof(struct ll));
    return list;
}

struct ll *insert_ll(struct ll *list, struct ht_item *item) {
    if (!list) {
        struct ll *head = allocate_ll();
        head->item = item;
        head->next = NULL;
        list = head;
        return list;
    } else if (list->next == NULL) {
        struct ll *node = allocate_ll();
        node->item = item;
        node->next = NULL;
        list->next = node;
        // printf("%s",list->next->item->key);
        return list;
    }
    struct ll *temp = list;

    while (temp->next->next) {
        temp = temp->next;
    }
    struct ll *node = allocate_ll();
    node->item = item;
    node->next = NULL;
    list = node;
    return list;
}

struct ht_item *create_item(char *key, char *value) {
    struct ht_item *item = (struct ht_item *)malloc(sizeof(struct ht_item));
    item->key = strdup(key);
    item->value = strdup(value);

    return item;
}

struct ht *create_hashtable(int size) {
    struct ht *table = (struct ht *)malloc(sizeof(struct ht));
    table->size = size;
    table->count = 0;
    table->items =
        (struct ht_item **)calloc(table->size, sizeof(struct ht_item *));

    for (int i = 0; i < table->size; i++) table->items[i] = NULL;

    table->buckets = create_buckets(table);
    return table;
}

void handle_collision(struct ht *table, int index, struct ht_item *item) {
    struct ll *head = table->buckets[index];

    if (head == NULL) {
        head = allocate_ll();
        head->item = item;
        table->buckets[index] = head;
        return;
    } else {
        table->buckets[index] = insert_ll(head, item);
        return;
    }
}

char *search(struct ht *table, char *key) {
    int index = hash_function(key);
    struct ht_item *item = table->items[index];
    struct ll *head = table->buckets[index];
    if (item != NULL) {
        if (strcmp(item->key, key) == 0) {
            return item->value;
            if (head == NULL) return NULL;

            item = head->item;
            head = head->next;
        }
    }
    return NULL;
}

void insert(struct ht *table, char *key, char *value) {
    int index = hash_function(key);
    struct ht_item *item = create_item(key, value);
    struct ht_item *current_item = table->items[index];
    if (current_item == NULL) {
        if (table->count == table->size) {
            // full
            printf("\nHashtable is full.");
            return;
        } else {
            // insert
            table->items[index] = item;
            table->count++;
            printf("Directly Inserted %s at → %d\n", item->key, index);
        }

    } else {
        if (strcmp(current_item->key, key) == 0) {
            // same keys, update the value
            strcpy(current_item->value, value);
            printf("Updated key %s at → %d\n", item->key, index);

            return;
        } else {
            // collision

            handle_collision(table, index, item);
            printf("Collision Inserted %s at → %d\n", key, index);
            return;
        }
    }
}

void print_table(struct ht *table) {
    printf(
        "\nHash "
        "Table\n╔══════════════════════════════════════════════════════════════"
        "════╗\n\n");

    for (int i = 0; i < table->size; i++) {
        if (table->items[i]) {
            printf("\tIndex: %d, Key: %s, Value: %s\n", i, table->items[i]->key,
                   table->items[i]->value);
            struct ll *list = table->buckets[i];
            while (list) {
                printf("\t\t↳ Bucket key: %s, Bucket value: %s\n",
                       list->item->key, list->item->value);
                list = list->next;
            }
        }
    }
    printf(
        "\n╚══════════════════════════════════════════════════════════════════╝"
        "\n");
}
void print_search(struct ht *table, char *key) {
    char *val = search(table, key);
    if (val == NULL) {
        printf("Key:%s does not exist\n", key);
        return;
    } else {
        printf("Key:%s, Value:%s\n", key, val);
    }
}
int main(void) {
    struct ht *table = create_hashtable(CAP);
    insert(table, (char *)"Kartik", (char *)"21");
    insert(table, (char *)"Pranav", (char *)"24");
    insert(table, (char *)"Mayank", (char *)"20");
    insert(table, (char *)"Priyanshu", (char *)"20");
    insert(table, (char *)"Priya", (char *)"20");
    insert(table, (char *)"Mehul", (char *)"19");
    insert(table, (char *)"Meehal", (char *)"44");
    insert(table, (char *)"Mala", (char *)"124");
    insert(table, (char *)"Harshita", (char *)"823");
    insert(table, (char *)"Harshita", (char *)"23");
    insert(table, (char *)"Saksham", (char *)"19");
    insert(table, (char *)"Sakshi", (char *)"10");
    insert(table, (char *)"Jatin", (char *)"23");
    print_table(table);
}
