#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
    struct Node* next_node;
    struct Node* previous_node;
    int value;
} Node;


Node* node_create(int value, Node* previous_node, Node* next_node) {
    Node* new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        perror("Failed to allocate memory for a new Node");
        exit(EXIT_FAILURE);
    }

    new_node->next_node = next_node;
    if (next_node != NULL) next_node->previous_node = new_node;

    new_node->previous_node = previous_node;
    if (previous_node != NULL) previous_node->next_node = new_node;

    new_node->value = value;
    return new_node;
}

Node* node_add (Node* node, int value) {
    if (node->next_node != NULL) return node_add(node->next_node, value);
    return node_create(value, node, NULL);
}

int node_insert_from_head(Node* node, int value, int position, int current_position) {
    if (current_position != position - 1) return node->next_node == NULL ? 1 : node_insert_from_head(node->next_node, value, position, current_position + 1);
    printf("Inserting from head\n");
    node_create(value, node, node->next_node);
    return 0;
}
int node_insert_from_tail(Node* node, int value, int position, int current_position) {
    if (current_position != position) return node->previous_node == NULL ? 1 : node_insert_from_tail(node->previous_node, value, position, current_position - 1);
    printf("Inserting from tail\n");
    node_create(value, node->previous_node, node);
    return 0;
}

void node_print (Node* node) {
    printf("%d%s", node->value, node->next_node != NULL ? ", " : "");
    if (node->next_node != NULL) node_print(node->next_node);
}

void node_free (Node* node) {
    if (node->next_node != NULL) node_free(node->next_node);
    free(node);
}


typedef struct LinkedList {
    struct Node* first_node;
    struct Node* last_node;
    int length;
} LinkedList;


LinkedList* linked_list_create() {
    LinkedList* linked_list = malloc(sizeof(LinkedList));
    if (linked_list == NULL) {
        perror("Failed to allocate memory for the linked list");
        exit(EXIT_FAILURE);
    }
    linked_list->first_node = NULL;
    linked_list->last_node = NULL;
    linked_list->length = 0;
    return linked_list;
}

int linked_list_add (LinkedList* linked_list, int value) {
    if (linked_list->first_node != NULL) {
        Node* last_node = node_add(linked_list->first_node, value);
        if (last_node == NULL) return 1;
        linked_list->last_node = last_node;
        linked_list->length++;
        return 0;
    }
    Node* first_node = node_create(value, NULL, NULL);
    first_node->value = value;
    linked_list->first_node = first_node;
    linked_list->last_node = first_node;
    linked_list->length++;
    return 0;
}

int linked_list_insert (LinkedList* linked_list, int value, int position) {
    if (position < 0 || position > linked_list->length) return 1;

    if (position == 0) {
        linked_list->first_node = node_create(value, NULL, linked_list->first_node);
        return 0;
    }

    if (position == linked_list->length) {
        linked_list->last_node = node_create(value, linked_list->last_node, NULL);
        return 0;
    }

    if (position > linked_list->length / 2) {
        return node_insert_from_tail(linked_list->last_node, value, position, linked_list->length - 1);
    }

    return node_insert_from_head(linked_list->first_node, value, position, 0);
}

void linked_list_print (LinkedList* linked_list) {
    printf("LinkedList {");
    if (linked_list->first_node != NULL) node_print(linked_list->first_node);
    printf("}\n");
}


void linked_list_free (LinkedList* linked_list) {
    if (linked_list->first_node == NULL) return;
    node_free(linked_list->first_node);
    free(linked_list);
}

void linked_list_clear (LinkedList* linked_list) {
    if (linked_list == NULL || linked_list->first_node == NULL) return;
    node_free(linked_list->first_node);
    linked_list->first_node = NULL;
    linked_list->last_node = NULL;
    linked_list->length = 0;
}

void test_insert (int length, int value, int position) {
    printf("\nTESTING INSERT -- | length: %d | value: %d | position: %d\n", length, value, position);
    LinkedList* linked_list = linked_list_create();
    for (int i = 1; i < length; i++) {
        linked_list_add(linked_list, i * 10);
    }
    linked_list_print(linked_list);
    if (linked_list_insert(linked_list, value, position) != 0) {
        printf("Error inserting value %d at position %d of linked_list with length %d. ", value, position, linked_list->length);
    }
    linked_list_print(linked_list);
}

int main () {

    test_insert(5, 0, 0);
    test_insert(5, 0, 1);
    test_insert(5, 0, 2);
    test_insert(5, 0, 3);
    test_insert(5, 0, 4);
    test_insert(5, 0, 5);

    test_insert(10, 0, 5);
    test_insert(10, 0, 4);

    return 0;
}