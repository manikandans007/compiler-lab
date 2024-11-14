#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 64
#define ALPHABET_SIZE 26

// Global Variables
int **transitionMap;               // Stores state transitions: transitionMap[i][j] gives the state reached from i with symbol j
int **partitionTransitionMap;       // Partition-based transition map, using partition numbers as row indices
int startState;                     // Starting state
long int reachable;                 // Bitset representing reachable states
long int allStates;                 // Bitset representing all FSM states
long int finalStates;               // Bitset representing final states
long int nonFinalStates;            // Bitset representing non-final states
long int *P;                        // Array of partitions; each partition is a bitset of states

void initializeTransitionMaps() {
    transitionMap = (int**)malloc(MAX_STATES * sizeof(int*));
    partitionTransitionMap = (int**)malloc(MAX_STATES * sizeof(int*));
    for (int i = 0; i < MAX_STATES; i++) {
        transitionMap[i] = (int*)malloc(ALPHABET_SIZE * sizeof(int));
        partitionTransitionMap[i] = (int*)malloc(ALPHABET_SIZE * sizeof(int));
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            transitionMap[i][j] = -1;
            partitionTransitionMap[i][j] = -1;
        }
    }
}

void freeTransitionMaps() {
    for (int i = 0; i < MAX_STATES; i++) {
        free(transitionMap[i]);
        free(partitionTransitionMap[i]);
    }
    free(transitionMap);
    free(partitionTransitionMap);
}

void dfs(int v) {
    reachable |= (1 << v);
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (transitionMap[v][i] != -1 && (reachable & (1 << transitionMap[v][i])) == 0) {
            dfs(transitionMap[v][i]);
        }
    }
}

void readInput() {
    char buffer[125];
    fgets(buffer, sizeof(buffer), stdin);
    startState = atoi(buffer);

    fgets(buffer, sizeof(buffer), stdin);
    char *p = strtok(buffer, " ");
    while (p != NULL) {
        int state = atoi(p);
        finalStates |= 1 << state;
        p = strtok(NULL, " ");
    }

    int from, to;
    char symbol;
    while (fscanf(stdin, "%d %c %d", &from, &symbol, &to) != EOF) {
        transitionMap[from][symbol - 'a'] = to;
        allStates |= (1 << from) | (1 << to);
    }
}

void initializePartitions() {
    P = (long int*)malloc(MAX_STATES * sizeof(long int));
    for (int i = 0; i < MAX_STATES; i++) {
        P[i] = 0;
    }
    nonFinalStates = allStates & ~finalStates;
    P[0] = finalStates;
    P[1] = nonFinalStates;
}

void partitionStates() {
    int nextPartitionIndex = 2;
    for (int i = 0; i < MAX_STATES; i++) {
        if (P[i] == 0) break;

        long int newPartition = 0;
        for (int j = 63; j >= 0; j--) {
            long int staticState = (long int)1 << j;
            if ((P[i] & staticState) != 0) {
                partitionTransitionMap[i] = transitionMap[j];
                for (int k = j - 1; k >= 0; k--) {
                    long int otherState = (long int)1 << k;
                    if ((P[i] & otherState) != 0) {
                        for (int l = 0; l < ALPHABET_SIZE; l++) {
                            int staticNext = -1, otherNext = -1;
                            for (int m = 0; m < nextPartitionIndex; m++) {
                                if ((P[m] & (1 << transitionMap[j][l])) != 0) staticNext = m;
                                if ((P[m] & (1 << transitionMap[k][l])) != 0) otherNext = m;
                            }
                            if (transitionMap[j][l] != transitionMap[k][l] && (staticNext != otherNext)) {
                                P[i] &= ~(1 << k);
                                newPartition |= (1 << k);
                                break;
                            }
                        }
                    }
                }
                break;
            }
        }
        if (newPartition != 0) {
            P[nextPartitionIndex] = newPartition;
            nextPartitionIndex++;
        }
    }
}

void printResults(int nextPartitionIndex) {
    int startPartition = 0;
    for (int i = 0; i < nextPartitionIndex; i++) {
        if ((P[i] & (1 << startState)) != 0) {
            startPartition = i;
            break;
        }
    }
    printf("Start Partition: %d\n", startPartition);

    printf("Final Partitions: ");
    for (int i = 0; i < nextPartitionIndex; i++) {
        if ((P[i] & finalStates) != 0) {
            printf("%d ", i);
        }
    }
    printf("\nTransitions:\n");
    for (int i = 0; i < nextPartitionIndex; i++) {
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            if (partitionTransitionMap[i][j] != -1) {
                for (int k = 0; k < nextPartitionIndex; k++) {
                    if ((P[k] & (1 << partitionTransitionMap[i][j])) != 0) {
                        printf("%d %c %d\n", i, j + 'a', k);
                    }
                }
            }
        }
    }
}

int main() {
    initializeTransitionMaps();
    readInput();

    reachable = 0;
    dfs(startState);

    allStates &= reachable;
    finalStates &= reachable;

    initializePartitions();
    partitionStates();
    printResults(64);

    free(P);
    freeTransitionMaps();
    return 0;
}
