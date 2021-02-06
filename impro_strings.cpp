#include "impro_strings.h"
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

// returns A.compare(B), while a is uppercase of A, and
// B is uppercase of B.
int __strcmpi(std::string a, std::string b) {
    std::transform(a.begin(), a.end(),a.begin(), std::toupper);
    std::transform(b.begin(), b.end(),b.begin(), std::toupper);
    return a.compare(b);
}

// reads floats from a string. Case insensitive "na" and "#n/a" converts to nanf("")
// Other string that cannot be parsed will be ignored.
// For example: input text as "0 whatever 1 2 na \n 3 whatever 4 5 whatever whatever", output vecFloat will be
// {0, 1, 2, nanf(""), 3, 4, 5}
void fromStringToVecFloatAcceptNaIgnoreOthers(
        std::string text,
        std::vector<float> & vecFloats,
        int & nFloats, int & nNanfs)
{
    // get text (entire content to a single string)
    std::stringstream ss;
    ss << text;
    vecFloats.clear();
    nFloats = 0;
    nNanfs = 0;
    while (true) {
        float tmp;
        std::string stmp;
        // check EOF of the stream
        if (ss.eof() == true) break;
        // read a string
        ss >> stmp;
        if (stmp.length() <= 0) continue;
        // convert to a float
        try {
            if(__strcmpi(stmp, std::string("NA")) == 0 || __strcmpi(stmp, std::string("#n/a")) == 0)
            {
                tmp = std::nanf("");
                nNanfs++;
            } else {
                tmp = std::stof(stmp);
                nFloats++;
            }
            // append to vector (including NA (nanf))
            vecFloats.push_back(tmp);
        }  catch (...) {
            // if neither NA nor a float, ignore it.
            // do nothing
        }
    }
}

// reads double from a string. Case insensitive "na" and "#n/a" converts to nan("")
// Other string that cannot be parsed will be ignored.
// For example: input text as "0 whatever 1 2 na \n 3 whatever 4 5 whatever whatever", output vecDouble will be
// {0, 1, 2, nanf(""), 3, 4, 5}
void fromStringToVecDoubleAcceptNaIgnoreOthers(
        std::string text,
        std::vector<double> & vecDoubles,
        int & nDoubles, int & nNans)
{
    // get text (entire content to a single string)
    std::stringstream ss;
    ss << text;
    vecDoubles.clear();
    nDoubles = 0;
    nNans = 0;
    while (true) {
        double tmp;
        std::string stmp;
        // check EOF of the stream
        if (ss.eof() == true) break;
        // read a string
        ss >> stmp;
        if (stmp.length() <= 0) continue;
        // convert to a double
        try {
            if(__strcmpi(stmp, std::string("na")) == 0 || __strcmpi(stmp, std::string("#n/a")) == 0)
            {
                tmp = std::nan("");
                nNans++;
            } else {
                tmp = std::stod(stmp);
                nDoubles++;
            }
            // append to vector (including NA (nanf))
            vecDoubles.push_back(tmp);
        }  catch (...) {
            // if neither NA nor a double, ignore it.
            // do nothing
        }
    }
}

