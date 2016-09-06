#include <stdlib.h>
#include <string.h>
#include "methinks_it_is_like_a_weasel.h"

/**
 * @brief      Finds the position of the given gene in the genotype.
 *
 * @param      gene_ptr    Pointer to the genotype where the gene is looked for.
 * @param[in]  gene        Gen we are looking for inside the genotype. 
 *
 * @return     If existent, the position of the first gene's occurrence in the string.
 */
static int find_gen_pos(const gene_t* genotype_ptr, const gene_t gene)
{
    int i = 0;
    int position = -1;

    while(genotype_ptr[i] != '\0')
    {
        if(genotype_ptr[i] == gene)
        {
            position = i;
            break;
        }
        else
        {
            i++;
        }
    }
    return position;
}

/**
 * @brief   Cyclically moves a parent's gene through "allowed_genes".
 *
 * @param[in/out]   gene       Pointer to a gene from "parent" to mutate to a close one from 
 *                            "allowed_genes".  
 * @param[in]       offset    Offset from the original given gene.
 */
static void mutate_gen(gene_t* gene, const gene_t* allowed_genes, int offset)
{
    int gen_pos;
    int num_allowed_genes = find_gen_pos(allowed_genes, '\0');
    /* Convert "offset" to a positive value to prevent indexing with a negative number later on */
    while(offset < 0)
        offset += num_allowed_genes;
    gen_pos = find_gen_pos(allowed_genes, *gene);
    *gene = allowed_genes[(gen_pos + offset) % num_allowed_genes];
}

/**
 * @brief      Mutates a genotype genes by scrolling their genes by a small random offset with 
 *             the given probability.
 *
 * @param[in/out]   parent         Parent genotype.
 * @param[in]       mutation_rate  Fixed mutation rate.
 *
 */
void mutate(const gene_t* parent, const gene_t* allowed_genes, float mutation_rate, gt_size genotype_size)
{
    /* POTENTIAL PARALLELIZATION OF THIS SECTION */
    /* Cycle through each of the genes from "parent" and perform mutation operation. */
    for (int i = 0; i < genotype_size; ++i)
    {
        if (rand() / (RAND_MAX + 1.0) < mutation_rate)
        {
            int offset = (rand() % 3) + 1;
            if(rand() % 2 == 0)
                mutate_gen(&parent[i], allowed_genes, offset);
            else
                mutate_gen(&parent[i], allowed_genes, -offset);
        }
    }
}

/**
 * @brief      Fitness of the generated phenotype.
 *
 * @param[in]  candidate  Pointer to the genotype to be analyzed.
 * 
 * @caveat     This function assumes that candidate and target genotypes are of the same size.
 *
 * @return     Returns 0 if the string matches with the target, and a negative number if
 *             non-matching. It is calculated as the negated sum of the circular distances
 *             of the string letters with the destination letters.
 */
int fitness_determination(const gene_t *candidate, const gene_t *target, gt_size genotype_size)
{
    int fitness_so_far = 0;

    /* POTENTIAL PARALLELIZATION OF THIS SECTION. */
    for (int i = 0; i < genotype_size; ++i)
    {
        int target_gen_pos = find_gen_pos(target, target[i]); /* i=0, J, position = 10 */
        int candidate_gen_pos = find_gen_pos(candidate, candidate[i]); /* i=0, M, position = 13 */
        int diff = abs(target_gen_pos - candidate_gen_pos); /* 13 - 10 = 3 */
        /* min(3, (27 - 3)) = 3 */
        fitness_so_far -= min(diff, (genotype_size - diff));
    }
 
    return fitness_so_far;
}

/**
 * @brief      Obtain the most fit individual in a given group.
 *
 * @note       There is no possibility for childs less fit than the parent. For that reason
 *             if no child better fit is found, the last child with the same
 *             fitness is used as a parent for the next generation.
 *             
 * @param[in]      childs    Pointer to the childs population. Bidimensional array.
 * @param[in/out]  parent    Pointer to the current generation's parent.
 * @param[in]      sample_population    Number of individuals product of this generation.
 *
 */
void most_fit(const generation_t* childs, gene_t *parent, const gene_t *target, const int sample_population, gt_size genotype_size)
{
    int i;
    int child_fitness;
    int most_fit_child = -1;
    int best_fitness = fitness_determination(parent, target, genotype_size);
    /* POTENTIAL PARALLELIZATION OF THIS SECTION */
    for(i = 0; i < sample_population; ++i)
    {
        child_fitness = fitness_determination((gene_t *)&childs[i * genotype_size], target, genotype_size);
        if(child_fitness >= best_fitness)
        {
            most_fit_child = i;
        }
    }
    /* 
     * If there was a child better fit than the parent then replace it. Otherwise, the last
     * child with the same fitness will replace the parent, always, for the sake of simplicity. 
     */
    memcpy(parent, &childs[most_fit_child * genotype_size], genotype_size);
}