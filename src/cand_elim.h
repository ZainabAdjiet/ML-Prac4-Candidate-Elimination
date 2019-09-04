#ifndef CAND_ELIM_H
#define CAND_ELIM_H

/****************************************************************/
/* Includes
/****************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

/****************************************************************/
/* Functions
/****************************************************************/

namespace cand_elim {

    typedef std::vector<std::string> str_vect;

    std::vector<training_element> & training_set;
    std::vector<hypothesis> & G;
    std::vector<hypothesis> & S;

    struct hypothesis {
        str_vect hypo;
        std::vector<str_vect> attributes;
        hypothesis(str_vect & h);
        void addAttributeValue(int index, std::string value);
        bool matches(hypothesis other);
    }
    
    struct training_element {
        str_vect instance;
        bool result;
        training_element(str_vect & instance, bool result);
    };

    void load_training_set(std::string filename);
    void positive_example(const str_vect & d);
    void negative_example(const str_vect & d);
    std::ostream & operator<<(std::ostream & os, const str_vect & d);
    std::ostream & operator<<(std::ostream & os, const training_element & element);
}

#endif
