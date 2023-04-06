// Author: Junwei Lan
// StudentID: PB20111689
// Date: 2023.04.03

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include "cachelab.h"

struct CacheLine {
    int valid;  // valid bit
    int tag;    // tag  bits
    int cnt;    // LRU counter
};

struct Result {
    int hit_cnt;
    int miss_cnt;
    int evict_cnt;
};

void parse_command(int argc, char **argv, int *s, int *E, int *b, char *t);
struct CacheLine **create_cache(int s, int E);
void get_operation(struct CacheLine **cache, struct Result *result, int s, int E, int b, char *t);
void simulate_cache(struct CacheLine **cache, struct Result *result, int s, int E, int b, unsigned int address);

int main(int argc, char **argv)
{
    int s, E, b;
    char t[256];

    // parse command line and get arguments
    parse_command(argc, argv, &s, &E, &b, t);
    // initial cache
    struct CacheLine **cache = create_cache(s, E);
    struct Result result = {0, 0, 0};
    get_operation(cache, &result, s, E, b, t);
    // free cache
    for (int i = 0, S = 1 << s; i < S; i++) {
        free(cache[i]);
    }
    free(cache);
    printSummary(result.hit_cnt, result.miss_cnt, result.evict_cnt);
    return 0;
}

void parse_command(int argc, char **argv, int *s, int *E, int *b, char *t) {
    int opt;
    while(-1 != (opt = getopt(argc, argv, "hvs:E:b:t:"))) {
        switch(opt) {
            case 'h':
                break;
            case 'v':
                break;
            case 's':
                *s = atoi(optarg);
                break;
            case 'E':
                *E = atoi(optarg);
                break;
            case 'b':
                *b = atoi(optarg);
                break;
            case 't':
                strcpy(t, optarg);
                break;
            default:
                printf("get unknown argument.\n");
                break;
        }
    }
    return;
}

struct CacheLine **create_cache(int s, int E) {
    int S = 1 << s;
    struct CacheLine ** cache = (struct CacheLine**)malloc(sizeof(struct CacheLine*) * S);
    for (int i = 0; i < S; i++) {
        cache[i] = (struct CacheLine*)malloc(sizeof(struct CacheLine) * E);
        for (int j = 0; j < E; j++) {
            cache[i][j].valid = 0;  // valid bit
            cache[i][j].tag = -1;   // tag bits
            cache[i][j].cnt = 0;    // LRU counter
        }
    }
    return cache;
}

void get_operation(struct CacheLine **cache, struct Result *result, int s, int E, int b, char *t) {
    FILE *file_ptr = fopen(t, "r");
    if (file_ptr == NULL) {
        return;
    }

    char operation;
    unsigned int address;
    int size;

    while(fscanf(file_ptr, " %c %x, %d", &operation, &address, &size) > 0) {
        switch(operation) {
            case 'I':
                break;
            case 'L':
                simulate_cache(cache, result, s, E, b, address);
                break;
            case 'M':
                simulate_cache(cache, result, s, E, b, address);
                simulate_cache(cache, result, s, E, b, address);
                break;
            case 'S':
                simulate_cache(cache, result, s, E, b, address);
                break;
            default:
                break;
        }
    }
    
    fclose(file_ptr);
    return;
}

void simulate_cache(struct CacheLine **cache, struct Result *result, int s, int E, int b, unsigned int address) {
    address = address >> b;     // b bits for block offset
    int mask_index = (1 << s) - 1;
    int index =  mask_index & address;
    int tag = address >> s;

    for (int j = 0; j < E; j++) {
        if (cache[index][j].valid == 1) {
            cache[index][j].cnt++;
        }
    }

    // if there is hit
    for (int j = 0; j < E; j++) {
        if (cache[index][j].valid == 1 && cache[index][j].tag == tag) {
            cache[index][j].cnt = 0;
            result->hit_cnt++;
            return;
        }
    }

    // if there is cold cache
    for (int j = 0; j < E; j++) {
        if (cache[index][j].valid == 0) {
            result->miss_cnt++;
            cache[index][j].valid = 1;
            cache[index][j].tag = tag;
            return;
        }
    }

    // there will be eviction 
    result->evict_cnt++;
    result->miss_cnt++;
    int max_cnt = cache[index][0].cnt;
    int max_j = 0;
    for (int j = 1; j < E; j++) {
        if (cache[index][j].cnt > max_cnt) {
            max_cnt = cache[index][j].cnt;
            max_j = j;
        }
    }
    // evict cache
    cache[index][max_j].tag = tag;
    cache[index][max_j].cnt = 0;
    return;
}