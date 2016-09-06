#ifndef ME_THINKS_IT_IS_LIKE_A_WEASEL
#define ME_THINKS_IT_IS_LIKE_A_WEASEL

/* Genotype size. */
typedef int gt_size;
/* Gene form a genotype. */
typedef char gene_t;
/* 
 * This type will be used to create a container to store the population of each generation's 
 * childs. 
 */
typedef gene_t generation_t;

void mutate(const gene_t* parent, const gene_t* allowed_genes, float mutation_rate, gt_size genotype_size);
int fitness_determination(const gene_t *candidate, const gene_t *target, gt_size genotype_size);
void most_fit(const generation_t* childs, gene_t *parent, const gene_t *target, const int sample_population, gt_size genotype_size);

#endif /* ME_THINKS_IT_IS_LIKE_A_WEASEL */