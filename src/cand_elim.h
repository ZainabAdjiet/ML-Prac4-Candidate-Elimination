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
        static std::string get_diff_attribute(int index, std::string value);
        bool matches(hypothesis other);
        str_vect min_generalise(const str_vect & d);
        std::vector<str_vect> min_specialise(const str_vect & d);
    };
    
    struct training_element {
        str_vect instance;
        bool result;
        training_element(str_vect & instance, bool result);
    };

    const int num_attributes = 5;

    void load_training_set(std::string filename);
    std::ostream & operator<<(std::ostream & os, const str_vect & d);
    std::ostream & operator<<(std::ostream & os, const training_element & element);
}

#endif
