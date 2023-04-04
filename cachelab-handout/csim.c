// Author: Junwei Lan
// StudentID: PB20111689
// Date: 2023.04.03

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include "cachelab.h"

void parse_command(int argc, char **argv, int *s, int *E, int *b, char *t);

int main(int argc, char **argv)
{
    int s, E, b;
    char t[256];
    
    // parse command line and get arguments
    parse_command(argc, argv, &s, &E, &b, t);
    printSummary(0, 0, 0);
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
