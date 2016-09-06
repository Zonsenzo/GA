#include <stdio.h>
#include <time.h>
#include "methinks_it_is_like_a_weasel.h"

#define POPULATION 100

/* This is the genotype the population aims to evolve to. */
const gene_t target_genotype[] = "METHINKS IT IS LIKE A WEASEL";

/**
 * @brief   This is the collection of all the possible genes that the "target_genotype" might contain.
 * 
 * @note    This also includes the -space- "gen" (character). 
 */
const gene_t allowed_genes[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";

generation_t childs[POPULATION * sizeof(target_genotype)];
gene_t parent[sizeof(target_genotype)];

int main()
{
    time_t t;
    int const initial_fitness = fitness_determination(parent, target_genotype, sizeof(target_genotype));

    /* Seeds the random number generator with the POSIX system time. */
    srand((unsigned int)time(&t));
    
    /* Generation of the parent string out of random characters. */
    for (int i = 0; i < sizeof(target_genotype); ++i)
    {
        /* Modulo operand used to prevent indexing with a bigger number than the string's size. */
        parent[i] = allowed_genes[rand() % sizeof(allowed_genes)];
    }
   
    /* Loop until fitness reaches 0. */
    for(int fitness = initial_fitness; fitness < 0; fitness = fitness_determination(parent, target_genotype, sizeof(target_genotype)))
    {
        printf("%s: %d \n", parent , fitness);
        float const mutation_rate = 0.02f + (0.9f * fitness) / initial_fitness;

        for (int i = 0; i < POPULATION; ++i)
        {
            /* 
             * Copy each mutation of "parent" to each of the individuals in "childs". Note that the
             * "mutate" function doesn't change the "parent" string size.
             */
            mutate(parent, allowed_genes, mutation_rate, sizeof(target_genotype));
            memcpy(&childs[i * sizeof(target_genotype)], parent, sizeof(parent));
        }
        /* Keep the most fit child and make it the new parent. All other childs DIE. */
        most_fit(childs, parent, target_genotype, (int)POPULATION, sizeof(parent));
    }
    printf("Final genotype: %s \n", parent);

    /* This is just for halting execution after completion. Investigate a better implementation. */
    while (1)
    {

    }
}