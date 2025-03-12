// There are a number of trade-offs between dynamic arrays (AKA vectors) and
// linked lists. The preferred data structure depends entirely on the
// application, although vectors offer excellent general-purpose
// characteristics, especially on modern hardware. For more details, see
// https://stackoverflow.com/questions/19039972/linked-list-vs-vector, or
// consult any introductory algorithms textbook.

#include <err.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct Vector {
    size_t capacity;
    size_t length;
    int *data;
};

struct Vector create(void) {
    size_t capacity = 1;
    int *data = malloc(capacity * sizeof *data);
    if (NULL == data) {
        err(EXIT_FAILURE, "malloc");
    }
    struct Vector vector = {
        .capacity = capacity,
        .length = 0,
        .data = data,
    };
    return vector;
}

void delete(struct Vector vector) {
    free(vector.data);
}

int peek(struct Vector *vector) {
    return vector->length <= 0 ? INT_MIN : vector->data[vector->length - 1];
}

int pop(struct Vector *vector) {
    return vector->length <= 0 ? INT_MIN : vector->data[--vector->length];
}

void print(struct Vector vector) {
    for (size_t index = 0; index < vector.length; index++) {
        printf("%d%c",
               vector.data[index],
               index < vector.length - 1 ? ' ' : '\n');
    }
}

void push(struct Vector *vector, int datum) {
    if (vector->capacity <= vector->length) {
        size_t capacity = 2 * vector->capacity;
        int *data = reallocarray(vector->data, capacity, sizeof *data);
        if (NULL == data) {
            err(EXIT_FAILURE, "malloc");
        }
        vector->capacity = capacity;
        vector->data = data;
    }
    vector->data[vector->length++] = datum;
}

int main(void) {
    struct Vector vector = create();
    print(vector);
    push(&vector, 1);
    print(vector);
    push(&vector, 2);
    print(vector);
    push(&vector, 3);
    print(vector);
    printf("%d\n", peek(&vector));
    printf("%d\n", pop(&vector));
    print(vector);
    push(&vector, 4);
    print(vector);
    push(&vector, 8);
    print(vector);
    printf("%d\n", peek(&vector));
    printf("%d\n", pop(&vector));
    printf("%d\n", peek(&vector));
    printf("%d\n", pop(&vector));
    printf("%d\n", peek(&vector));
    printf("%d\n", pop(&vector));
    printf("%d\n", peek(&vector));
    printf("%d\n", pop(&vector));
    printf("%d\n", peek(&vector));
    printf("%d\n", pop(&vector));
    delete(vector);
}
