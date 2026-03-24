#ifndef MYLIB_H
#define MYLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//для использование библиотеки введите #include "mylib.h"



// ===================== STACK =====================
// typedef struct {
//     void* arr;     - массив элементов (сырые данные)
//     int count;     - текущее количество элементов
//     int capacity;  - вместимость (сколько всего можно хранить)
//     int size_el;   - размер одного элемента (в байтах)
// } Stack;
//
// StackCreate(capacity, size_el) - создать стек
// StackResize(stack)             - увеличить capacity в 2 раза
// StackPush(stack, &value)       - добавить элемент
// StackPop(stack, &out)          - извлечь элемент
// StackPeek(stack, &out)         - посмотреть верхний элемент
// StackFree(stack)               - освободить память


// ===================== HASH MAP =====================
// typedef struct Entry {
//     unsigned int hash; - хеш ключа
//     void* key;         - ключ
//     void* value;       - значение
//     int key_size;      - размер ключа
//     int value_size;    - размер значения
//     Entry* next;       - следующий элемент (цепочка)
// } Entry;
//
// typedef struct {
//     Entry** table; - массив списков (бакеты)
//     int capacity;  - размер таблицы
// } HashMap;
//
// MapCreate(capacity)                         - создать map
// MapPut(map, &key, key_size, &value, size)   - добавить/обновить
// MapGet(map, &key, key_size, &out)           - получить значение
// MapRemove(map, &key, key_size)              - удалить
// MapFree(map)                                - очистить память


// ===================== BLOOM FILTER =====================
// typedef struct {
//     unsigned char* bits; - битовый массив
//     int size;            - количество битов
// } BloomFilter;
//
// CreateBloom(size)            - создать фильтр
// BloomAdd(bf, data, size)     - добавить элемент
// BloomCheck(bf, data, size)   - проверить наличие
// BloomFree(bf)                - освободить память
//
// ВАЖНО:
// - используется 3 хеш-функции
// - возможны false positive (ошибки "есть")
// - если вернул 0 → элемента точно нет
// - работает с любыми данными (через void*)

// ===================== GRAPH =====================
// Матричный граф (Adjacency Matrix)
// typedef struct {
//     int** matrix;   - матрица смежности (0/1)
//     int vertices;   - число вершин
//     int max_v;      - максимальное число вершин
// } GraphMatrix;
//
// GraphMatrix* GraphMatrixCreate(max_v) - создать граф
// GraphMatrixAddVertex(g)               - добавить вершину
// GraphMatrixAddEdge(g, from, to)      - добавить ребро
// GraphMatrixRemoveEdge(g, from, to)   - удалить ребро
// GraphMatrixPrint(g)                   - вывести матрицу
// GraphMatrixFree(g)                    - очистить память
//
// Списочный граф (Adjacency List)
// typedef struct Node { int vertex; Node* next; } Node;
// typedef struct { Node** list; int vertices; int max_v; } GraphList;
// GraphList* GraphListCreate(max_v)
// GraphListAddVertex(g)
// GraphListAddEdge(g, from, to)
// GraphListRemoveEdge(g, from, to)
// GraphListPrint(g)
// GraphListFree(g)


// ===================== ОБЩИЕ ПРАВИЛА =====================
// - void* = универсальные данные (любые типы)
// - всегда передаём адрес (&value)
// - size = sizeof(type)
// - memcpy копирует БАЙТЫ, а не типы
// - ответственность за память лежит на пользователе


//Stack
typedef struct {
    void* arr;
    int count;
    int capacity;
    int size_el;
} Stack;

// создание стека
Stack* StackCreate(int capacity, int size_el) {
    if (capacity <= 0) return NULL;

    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) return NULL;

    stack->arr = malloc(capacity * size_el);
    if (!stack->arr) {
        free(stack);
        return NULL;
    }

    stack->count = 0;
    stack->capacity = capacity;
    stack->size_el = size_el;

    return stack;
}

// увеличение размера
int StackResize(Stack* stack) {
    int new_capacity = stack->capacity * 2;
    void* new_arr = realloc(stack->arr, new_capacity * stack->size_el);
    if (!new_arr) return 0;

    stack->arr = new_arr;
    stack->capacity = new_capacity;
    return 1;
}

// push
int StackPush(Stack* stack, void* element) {
    if (stack->count >= stack->capacity) {
        if (!StackResize(stack)) return 0;
    }

    void* target = (char*)stack->arr + stack->count * stack->size_el;
    memcpy(target, element, stack->size_el);

    stack->count++;
    return 1;
}

// pop
int StackPop(Stack* stack, void* out_element) {
    if (stack->count == 0) return 0;

    stack->count--;

    void* source = (char*)stack->arr + stack->count * stack->size_el;
    memcpy(out_element, source, stack->size_el);

    return 1;
}

// peek (верхний элемент)
int StackPeek(Stack* stack, void* out_element) {
    if (stack->count == 0) return 0;

    void* source = (char*)stack->arr + (stack->count - 1) * stack->size_el;
    memcpy(out_element, source, stack->size_el);

    return 1;
}

// освобождение памяти
void StackFree(Stack* stack) {
    if (!stack) return;
    free(stack->arr);
    free(stack);
}

//Hash Map
typedef struct Entry {
    unsigned int hash;
    void* key;
    void* value;
    int key_size;
    int value_size;
    struct Entry* next;
} Entry;

typedef struct {
    Entry** table;
    int capacity;
} HashMap;

unsigned int hash_code(const char* data, int size) {
    unsigned int hash = 0;
    for (int i = 0; i < size; i++) {
        hash += (unsigned char)data[i] * (i + 1);
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

HashMap* MapCreate(int capacity) {
    if (capacity <= 0) return NULL;

    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    if (!map) return NULL;

    map->table = (Entry**)calloc(capacity, sizeof(Entry*));
    if (!map->table) {
        free(map);
        return NULL;
    }

    map->capacity = capacity;
    return map;
}

int MapPut(HashMap* map, void* key, int key_size, void* value, int value_size) {
    int hash = hash_code((char*)key, key_size);
    int index = (hash & 0x7fffffff) % map->capacity;

    Entry* curr = map->table[index];

    // обновление
    while (curr) {
        if (curr->hash == hash &&
            curr->key_size == key_size &&
            memcmp(curr->key, key, key_size) == 0) {

            memcpy(curr->value, value,
                   curr->value_size < value_size ? curr->value_size : value_size);
            return 1;
        }
        curr = curr->next;
    }

    // новый элемент
    Entry* e = (Entry*)malloc(sizeof(Entry));
    if (!e) return 0;

    e->key = malloc(key_size);
    e->value = malloc(value_size);
    if (!e->key || !e->value) {
        free(e->key);
        free(e->value);
        free(e);
        return 0;
    }

    memcpy(e->key, key, key_size);
    memcpy(e->value, value, value_size);

    e->hash = hash;
    e->key_size = key_size;
    e->value_size = value_size;

    e->next = map->table[index];
    map->table[index] = e;

    return 1;
}

int MapGet(HashMap* map, void* key, int key_size, void* out_value) {
    int hash = hash_code((char*)key, key_size);
    int index = (hash & 0x7fffffff) % map->capacity;

    Entry* curr = map->table[index];

    while (curr) {
        if (curr->hash == hash &&
            curr->key_size == key_size &&
            memcmp(curr->key, key, key_size) == 0) {

            memcpy(out_value, curr->value, curr->value_size);
            return 1;
        }
        curr = curr->next;
    }

    return 0;
}

int MapRemove(HashMap* map, void* key, int key_size) {
    int hash = hash_code((char*)key, key_size);
    int index = (hash & 0x7fffffff) % map->capacity;

    Entry** curr = &map->table[index];

    while (*curr) {
        if ((*curr)->hash == hash &&
            (*curr)->key_size == key_size &&
            memcmp((*curr)->key, key, key_size) == 0) {

            Entry* tmp = *curr;
            *curr = (*curr)->next;

            free(tmp->key);
            free(tmp->value);
            free(tmp);
            return 1;
        }
        curr = &(*curr)->next;
    }

    return 0;
}

void MapFree(HashMap* map) {
    if (!map) return;

    for (int i = 0; i < map->capacity; i++) {
        Entry* curr = map->table[i];
        while (curr) {
            Entry* tmp = curr;
            curr = curr->next;

            free(tmp->key);
            free(tmp->value);
            free(tmp);
        }
    }

    free(map->table);
    free(map);
}

//BloomFilter
typedef struct {
    unsigned char* bits; // битовый массив
    int size;            // количество битов
} BloomFilter;

void set_bit(unsigned char* bits, int pos) {
    bits[pos / 8] |= (1 << (pos % 8));
}

int get_bit(unsigned char* bits, int pos) {
    return bits[pos / 8] & (1 << (pos % 8));
}

unsigned int hash1(const char* data, int size) {
    unsigned int hash = 5381;
    for (int i = 0; i < size; i++)
        hash = ((hash << 5) + hash) + (unsigned char)data[i];
    return hash;
}

unsigned int hash2(const char* data, int size) {
    unsigned int hash = 0;
    for (int i = 0; i < size; i++)
        hash = data[i] + (hash << 6) + (hash << 16) - hash;
    return hash;
}

unsigned int hash3(const char* data, int size) {
    unsigned int hash = 0;
    for (int i = 0; i < size; i++)
        hash = (hash * 33) ^ (unsigned char)data[i];
    return hash;
}

BloomFilter* CreateBloom(int size) {
    if (size <= 0) return NULL;

    BloomFilter* bf = (BloomFilter*)malloc(sizeof(BloomFilter));
    if (!bf) return NULL;

    int byte_size = (size + 7) / 8;

    bf->bits = (unsigned char*)calloc(byte_size, 1);
    if (!bf->bits) {
        free(bf);
        return NULL;
    }

    bf->size = size;
    return bf;
}

void BloomAdd(BloomFilter* bf, void* data, int size) {
    unsigned int h1 = hash1((char*)data, size) % bf->size;
    unsigned int h2 = hash2((char*)data, size) % bf->size;
    unsigned int h3 = hash3((char*)data, size) % bf->size;

    set_bit(bf->bits, h1);
    set_bit(bf->bits, h2);
    set_bit(bf->bits, h3);
}

int BloomCheck(BloomFilter* bf, void* data, int size) {
    unsigned int h1 = hash1((char*)data, size) % bf->size;
    unsigned int h2 = hash2((char*)data, size) % bf->size;
    unsigned int h3 = hash3((char*)data, size) % bf->size;

    if (get_bit(bf->bits, h1) &&
        get_bit(bf->bits, h2) &&
        get_bit(bf->bits, h3)) {
        return 1; // возможно есть
    }

    return 0; // точно нет
}

void BloomFree(BloomFilter* bf) {
    if (!bf) return;
    free(bf->bits);
    free(bf);
}

typedef struct {
    int** matrix;
    int vertices;
    int max_v;
} GraphMatrix;

GraphMatrix* GraphMatrixCreate(int max_v) {
    if (max_v <= 0) return NULL;

    GraphMatrix* g = (GraphMatrix*)malloc(sizeof(GraphMatrix));
    g->matrix = (int**)malloc(sizeof(int*) * max_v);
    for (int i = 0; i < max_v; i++) {
        g->matrix[i] = (int*)calloc(max_v, sizeof(int));
    }
    g->vertices = 0;
    g->max_v = max_v;
    return g;
}

int GraphMatrixAddVertex(GraphMatrix* g) {
    if (g->vertices >= g->max_v) return 0;
    g->vertices++;
    return 1;
}

void GraphMatrixAddEdge(GraphMatrix* g, int from, int to) {
    if (from >= g->vertices || to >= g->vertices) return;
    g->matrix[from][to] = 1;
    g->matrix[to][from] = 1; // для неориентированного графа
}

void GraphMatrixRemoveEdge(GraphMatrix* g, int from, int to) {
    if (from >= g->vertices || to >= g->vertices) return;
    g->matrix[from][to] = 0;
    g->matrix[to][from] = 0;
}

void GraphMatrixPrint(GraphMatrix* g) {
    printf("Adjacency Matrix:\n");
    for (int i = 0; i < g->vertices; i++) {
        for (int j = 0; j < g->vertices; j++)
            printf("%d ", g->matrix[i][j]);
        printf("\n");
    }
}

void GraphMatrixFree(GraphMatrix* g) {
    for (int i = 0; i < g->max_v; i++) free(g->matrix[i]);
    free(g->matrix);
    free(g);
}

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct {
    Node** list;
    int vertices;
    int max_v;
} GraphList;

GraphList* GraphListCreate(int max_v) {
    GraphList* g = (GraphList*)malloc(sizeof(GraphList));
    g->list = (Node**)malloc(sizeof(Node*) * max_v);
    for (int i = 0; i < max_v; i++) g->list[i] = NULL;
    g->vertices = 0;
    g->max_v = max_v;
    return g;
}

int GraphListAddVertex(GraphList* g) {
    if (g->vertices >= g->max_v) return 0;
    g->vertices++;
    return 1;
}

void GraphListAddEdge(GraphList* g, int from, int to) {
    if (from >= g->vertices || to >= g->vertices) return;
    Node* node = (Node*)malloc(sizeof(Node));
    node->vertex = to;
    node->next = g->list[from];
    g->list[from] = node;

    node = (Node*)malloc(sizeof(Node));
    node->vertex = from;
    node->next = g->list[to];
    g->list[to] = node;
}

void GraphListRemoveEdge(GraphList* g, int from, int to) {
    if (from >= g->vertices || to >= g->vertices) return;

    Node** curr = &g->list[from];
    while (*curr) {
        if ((*curr)->vertex == to) {
            Node* tmp = *curr;
            *curr = (*curr)->next;
            free(tmp);
            break;
        }
        curr = &(*curr)->next;
    }

    curr = &g->list[to];
    while (*curr) {
        if ((*curr)->vertex == from) {
            Node* tmp = *curr;
            *curr = (*curr)->next;
            free(tmp);
            break;
        }
        curr = &(*curr)->next;
    }
}

void GraphListPrint(GraphList* g) {
    printf("Adjacency List:\n");
    for (int i = 0; i < g->vertices; i++) {
        printf("%d: ", i);
        Node* curr = g->list[i];
        while (curr) {
            printf("%d ", curr->vertex);
            curr = curr->next;
        }
        printf("\n");
    }
}

void GraphListFree(GraphList* g) {
    for (int i = 0; i < g->max_v; i++) {
        Node* curr = g->list[i];
        while (curr) {
            Node* tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }
    free(g->list);
    free(g);
}


#endif
