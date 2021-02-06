#ifndef IMPRO_STRINGS_H
#define IMPRO_STRINGS_H

#include <string>
#include <vector>
#


// returns A.compare(B), while a is uppercase of A, and
// B is uppercase of B.
int __strcmpi(std::string a, std::string b) ;

// reads floats from a string. Case insensitive "na" and "#n/a" converts to nanf("")
// Other string that cannot be parsed will be ignored.
// For example: input text as "0 whatever 1 2 na \n 3 whatever 4 5 whatever whatever", output vecFloat will be
// {0, 1, 2, nanf(""), 3, 4, 5}
void fromStringToVecFloatAcceptNaIgnoreOthers(
        std::string text,
        std::vector<float> & vecFloats,
        int & nFloats, int & nNanfs);

// reads double from a string. Case insensitive "na" and "#n/a" converts to nan("")
// Other string that cannot be parsed will be ignored.
// For example: input text as "0 whatever 1 2 na \n 3 whatever 4 5 whatever whatever", output vecDouble will be
// {0, 1, 2, nanf(""), 3, 4, 5}
void fromStringToVecDoubleAcceptNaIgnoreOthers(
        std::string text,
        std::vector<double> & vecDoubles,
        int & nDoubles, int & nNans);



#endif // IMPRO_STRINGS_H
