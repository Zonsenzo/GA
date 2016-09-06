#include "methinks_it_is_like_a_weasel.h"
#include <iostream>
#include <vector>
#include <ctime>

/**
 * @brief      Cyclically moves a parent's character through "allowed_chars".
 *
 * @param[in/out]   character Reference to a character from the "parent" to mutate to a nearby one.  
 * @param[in]       distance  The distance from the original given character.
 */
static void move_char(char& character, int distance)
{
    while (distance < 0)
      distance += allowed_chars.length();
    int char_pos = allowed_chars.find(character);
    character = allowed_chars[(char_pos + distance) % allowed_chars.length()];
}

/**
 * @brief      Mutates the string by moving the characters by a small random distance with the given
 *             probability.
 *
 * @param[in]  parent         Parent string.
 * @param[in]  mutation_rate  Fixed mutation rate.
 *
 * @return     Mutated parent string.
 */
static std::string mutate(std::string parent, float mutation_rate)
{
    /* POTENTIAL PARALLELIZATION OF THIS SECTION */
    /* Cycle through each of the parent's characters and perform mutation operation */
    for (int i = 0; i < parent.length(); ++i)
        if (std::rand() / (RAND_MAX + 1.0) < mutation_rate)
        {
            int distance = std::rand() % 3 + 1;
            if(std::rand() % 2 == 0)
                move_char(parent[i], distance);
            else
                move_char(parent[i], -distance);
        }
    return parent;
}

/**
 * @brief      Tells if the first argument is less "fit" than the second one.
 *
 * @param      s1    1st string.
 * @param      s2    2nd string.
 *
 * @return     Result of the boolean expression.
 */
static bool less_fit(std::string const& s1, std::string const& s2)
{
    return selection::fitness(s1) < selection::fitness(s2);
}
 
int main()
{
    int const Copies = 100;
    typedef std::vector<std::string> childvec;

    /* Seeds the random number generator with the POSIX system time. */
    std::srand(time(0));
   
    std::string parent;
    /* Generation of the parent out of random characters. */
    for (int i = 0; i < selection::target_length(); ++i)
    {
        /* Modulo operand used to prevent indexing with a bigger number than the string's size. */
        parent += allowed_chars[std::rand() % allowed_chars.length()];
    }
   
    int const initial_fitness = selection::fitness(parent);
   
    /* Loop until fitness reaches 0 */
    for(int fitness = initial_fitness;
        fitness < 0;
        fitness = selection::fitness(parent))
    {
        std::cout << parent << ": " << fitness << "\n";
        float const mutation_rate = 0.02 + (0.9*fitness)/initial_fitness;
        childvec childs;
        childs.reserve(Copies + 1);
     
        childs.push_back(parent);
        for (int i = 0; i < Copies; ++i)
          childs.push_back(mutate(parent, mutation_rate));
        /* Value of the iterator returned by "max_element". */
        parent = *std::max_element(childs.begin(), childs.end(), less_fit);
    }
    std::cout << "Final String: " << parent << "\n";

    /* This is just for halting execution after completion. */
    char exit = 1;
    while (exit != 'ESC')
    {
        std::cout << "Press 'esc' to exit";
        std::cin >> exit;
    }
}