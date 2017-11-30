#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "queryresults.h"
#include "ngramcounter.h"
#include "linearhash.h"
#include "parser.h"

int parser(Trie *trie, char *initFile, char *queryFile) {
    FILE *iFile, *qFile;
    iFile = fopen(initFile, "r");
    if (iFile == NULL) {
        printf("fopen error %s\n", strerror(errno));
        exit(FOPEN_ERROR);
    }
    qFile = fopen(queryFile, "r");
    if (qFile == NULL) {
        printf("fopen error %s\n", strerror(errno));
        exit(FOPEN_ERROR);
    }

    char *firstLine = NULL;
    size_t lineSize = 0;
    getline(&firstLine, &lineSize, iFile);
    if (strncmp(firstLine, "DYNAMIC\n", 8) == 0) {
        dynamic_parser(trie, iFile, qFile);
    } else if (strncmp(firstLine, "STATIC\n", 7) == 0) {
        static_parser(trie, iFile, qFile);
    }
    fclose(iFile);
    fclose(qFile);
    free(firstLine);
}

int dynamic_parser(Trie *trie, FILE *iFile, FILE *qFile) {
    BloomFilter *bloomFilter = create_bloom_filter();
    QueryResults *queryResults = create_query_results(DEFAULT_LINES, DEFAULT_LINE_SIZE);
    NgramCounter *ngramCounter = create_ngram_counter();
    char *line = NULL;
    char *word = NULL;
    int topk = 3;
    size_t lineSize = 0;

    while (getline(&line, &lineSize, iFile) > 0) {
        insert_trie(trie, line);
    }

    while (getline(&line, &lineSize, qFile) > 0) {
        NgramArray *ngramArray = NULL;
        switch (line[0]) {
            case 'Q':
                query_trie_dynamic(trie, &line[2], bloomFilter, queryResults, ngramCounter);
                copy_results_to_buffer_query_results(queryResults);
                break;
            case 'A':
                insert_trie(trie, &line[2]);
                break;
            case 'D':
                delete_ngram_trie(trie, &line[2]);

                break;
            case 'F':
                word = strtok(line + 1, " \n");
                if (word != NULL) {
                    topk = atoi(word);
                    if (topk < 1) {
                        printf("Invalid top k given: %d\n", topk);
                        exit(-1); //todo swsto error code
                    }
                    ngramArray = copy_to_ngram_array(ngramCounter);
                }
                flush_query_results(queryResults);
                if (ngramArray != NULL) {
                    sort_topk(ngramArray, (unsigned int) topk);
//                    print_ngram_array(ngramArray);
                    destroy_ngram_array(ngramArray);
                }
                clear_ngram_counter(ngramCounter);
                break;
            default:
                printf("default\n");//todo na valoume swsto error message

        }
    }
    destroy_gram_counter(ngramCounter);
    free(line);
    destroy_query_results(queryResults);
    destroy_bloom_filter(bloomFilter);
    return SUCCESS;
}

int static_parser(Trie *trie, FILE *iFile, FILE *qFile) {
    BloomFilter *bloomFilter = create_bloom_filter();
    QueryResults *queryResults = create_query_results(DEFAULT_LINES, DEFAULT_LINE_SIZE);
    NgramCounter *ngramCounter = create_ngram_counter();
    char *line = NULL;
    char *word = NULL;
    int topk = 3;
    size_t lineSize = 0;

    while (getline(&line, &lineSize, iFile) > 0) {
        insert_trie(trie, line);
    }
    compress_trie(trie);
    //print_node_children_LinearHash(trie->linearHash);

    while (getline(&line, &lineSize, qFile) > 0) {
        NgramArray *ngramArray = NULL;
        switch (line[0]) {
            case 'Q':
                query_trie_static(trie, &line[2], bloomFilter, queryResults, ngramCounter);
                copy_results_to_buffer_query_results(queryResults);
                break;
            case 'F':
                word = strtok(line + 1, " \n");
                if (word != NULL) {
                    topk = atoi(word);
                    if (topk < 1) {
                        printf("Invalid top k given: %d\n", topk);
                        exit(-1); //todo swsto error code
                    }
                    ngramArray = copy_to_ngram_array(ngramCounter);
                }
                flush_query_results(queryResults);
                if (ngramArray != NULL) {
                    sort_topk(ngramArray, (unsigned int) topk);
//                    print_ngram_array(ngramArray);
                    destroy_ngram_array(ngramArray);
                }
                clear_ngram_counter(ngramCounter);
                break;
            default:
                printf("default\n");//todo na valoume swsto error message

        }
    }
    destroy_gram_counter(ngramCounter);
    free(line);
    destroy_query_results(queryResults);
    destroy_bloom_filter(bloomFilter);
    return SUCCESS;
}

// todo na mhn krasarei ama exei kenh grammh sto telos tou arxeiou