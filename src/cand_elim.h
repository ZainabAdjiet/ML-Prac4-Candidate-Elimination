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

    struct hypothesis {
        str_vect hypo;
        static std::vector<str_vect> attributes;
        hypothesis(str_vect & h);
        static void set_num_attributes(int num);
        static void add_attribute_value(int index, std::string value);
        bool matches(hypothesis other);
    };
    
    struct training_element {
        str_vect instance;
        bool result;
        training_element(str_vect & instance, bool result);
    };

    const int num_attributes = 5;

    void load_training_set(std::string filename);
    //void positive_example(const str_vect & d);
    //void negative_example(const str_vect & d);
    std::ostream & operator<<(std::ostream & os, const str_vect & d);
    std::ostream & operator<<(std::ostream & os, const training_element & element);
}

#endif
