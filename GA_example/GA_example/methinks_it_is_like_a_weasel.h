#ifndef ME_THINKS_IT_IS_LIKE_A_WEASEL
#define ME_THINKS_IT_IS_LIKE_A_WEASEL

#include <string>
#include <cstdlib>
#include <cassert>
#include <algorithm>


/**
 * @brief   This is the string of all the possible characters that the target might contain.
 * 
 * @note    This includes the -space- character as well. 
 */
std::string allowed_chars = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/**
 * @brief    Selection class.
 * 
 * @details  The class contains the fitness function, encapsulates the target string and allows 
 *           access to its length from outside, reason for which it is not defined inside "fitness".
 *           It is only there for access control, therefore everything is static. 
 */
class selection
{
    public:
        /**
         * @brief      Fitness of the generated phenotype.
         *
         * @param[in]  candidate  The string to be analyzed.
         *
         * @return     Returns 0 if the string matches with the target, and a negative number if
         *             non-matching. It is calculated as the negated sum of the circular distances
         *             of the string letters with the destination letters.
         */
        static int fitness(std::string candidate)
        {
            assert(target.length() == candidate.length());
         
            int fitness_so_far = 0;
         
            /* POTENTIAL PARALLELIZATION OF THIS SECTION. */
            for (int i = 0; i < target.length(); ++i)
            {
                int target_pos = allowed_chars.find(target[i]); /* i=0, M, position = 13 */
                int candidate_pos = allowed_chars.find(candidate[i]); /* i=0, J, position = 10 */
                int diff = std::abs(target_pos - candidate_pos); /* 13 - 10 = 3 */
                /* min((27 - 3), 3) = 3 */
                fitness_so_far -= std::min(diff, int(allowed_chars.length()) - diff);
            }
         
            return fitness_so_far;
        }

        /**
         * @brief   Get the target string length.
         *
         * @return  Length in bytes of the target string.
         */
        static int target_length()
        { 
            return target.length();
        }
    private:
        static std::string target;
};

/* This is the string the population aims to evolve to. */
std::string selection::target = "METHINKS IT IS LIKE A WEASEL";

#endif /* ME_THINKS_IT_IS_LIKE_A_WEASEL */