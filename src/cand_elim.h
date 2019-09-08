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
#include <cmath>

/****************************************************************/
/* Functions
/****************************************************************/

namespace cand_elim {

    typedef std::vector<std::string> str_vect;

    // hypothesis struct to keep hypothesis and different values for each attribute
    struct hypothesis {
        str_vect hypo;
        static std::vector<str_vect> attributes;
        hypothesis(const str_vect & h);
        static void set_num_attributes(int num);
        static void add_attribute_value(int index, std::string value);
        static std::string get_diff_attribute(int index, std::string value);
        bool operator==(const hypothesis & other);
        bool operator%(const str_vect & other);
        bool operator%(const hypothesis & other);
        bool operator>(const hypothesis & other);
        bool operator<(const hypothesis & other);
        bool operator>=(const hypothesis & other);
        bool operator<=(const hypothesis & other);
        hypothesis min_generalise(const str_vect & d);
        std::vector<hypothesis> min_specialise(const str_vect & d);
    };
    
    // training element to keep instance and example type
    struct training_element {
        str_vect instance;
        bool result;
        training_element(str_vect & instance, bool result);
    };

    const int num_attributes = 5;   // constant number of attributes for hypotheses and instances

    // found in cand_elim_driver.cpp
    void load_training_set(std::string filename);
    bool more_spec_than_G(hypothesis & s, bool equal);
    bool more_gen_than_S(hypothesis & g, bool equal);
    void positive_example(const str_vect & d);
    void negative_example(const str_vect & d);

    // found in cand_elim.cpp
    std::ostream & operator<<(std::ostream & os, const str_vect & d);
    std::ostream & operator<<(std::ostream & os, const hypothesis & h);
    std::ostream & operator<<(std::ostream & os, const std::vector<hypothesis> & h_vect);
    std::ostream & operator<<(std::ostream & os, const training_element & element);
}

#endif
