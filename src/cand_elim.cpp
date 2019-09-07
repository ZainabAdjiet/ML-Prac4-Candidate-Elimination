/****************************************************************/
/* Includes
/****************************************************************/

#include "cand_elim.h"

using namespace cand_elim;

/****************************************************************/
/* training_element methods
/****************************************************************/

training_element::training_element(str_vect & in, bool res) :
    instance(in), result(res) {}

/****************************************************************/
/* hypothesis methods
/****************************************************************/

hypothesis::hypothesis(const str_vect & h) :
    hypo(h) {}

void hypothesis::set_num_attributes(int num) {
    attributes.resize(num);
}

void hypothesis::add_attribute_value(int index, std::string value) {
    str_vect attr = attributes[index];
    auto it = std::find(attr.begin(), attr.end(), value);
    if (it == attr.end())   attributes[index].push_back(value);
}

std::string hypothesis::get_diff_attribute(int index, std::string value) {
    for (int i = 0; i < num_attributes; ++i) {
        if (attributes[index][i] != value)  return attributes[index][i];
    }
    return "?";
}

bool hypothesis::operator==(const hypothesis & other) {
    return hypo == other.hypo;
}

bool hypothesis::operator%(const hypothesis & other) {
    for (int i = 0; i < num_attributes; ++i) {
        if (hypo[i] != other.hypo[i] && hypo[i] != "?" && other.hypo[i] != "?")
            return false;
    }
    return true;
}

bool hypothesis::operator>(const hypothesis & other) {
    int thisCount = std::count(hypo.begin(), hypo.end(), "?");
    int otherCount = std::count(other.hypo.begin(), other.hypo.end(), "?");
    return thisCount > otherCount;
}

bool hypothesis::operator<(const hypothesis & other) {
    int thisCount = std::count(hypo.begin(), hypo.end(), "?");
    int otherCount = std::count(other.hypo.begin(), other.hypo.end(), "?");
    return thisCount < otherCount;
}

hypothesis hypothesis::min_generalise(const str_vect & d) {
    str_vect new_s = hypo;
    for (int i = 0; i < num_attributes; ++i) {
        // if there is a difference between elements
        if (d[i] != new_s[i]) {
            if (new_s[i] == "{}")  // if element is null set, take on value in instance
                new_s[i] = d[i];
            else if (new_s[i] != "?")  // if element differs and is not most general, make most general
                new_s[i] = "?";
        }
    }
    return hypothesis(new_s);
}

std::vector<hypothesis> hypothesis::min_specialise(const str_vect & d) {
    std::vector<hypothesis> min_gs;
    str_vect new_g;
    int last_spec = -1;
    for (int i = 0; i < num_attributes; ++i) {
        if (hypo[i] == "?" && i != last_spec) {
            new_g = hypo;
            new_g[i] = get_diff_attribute(i, d[i]);
            last_spec = i;
            min_gs.push_back( hypothesis(new_g) );
        }
    }
    return min_gs;
}

// bool cand_elim::more_general(const str_vect & h1, const str_vect & h2) {
//     int count1 = std::count(h1.begin(), h1.end(), "?");
//     int count2 = std::count(h2.begin(), h2.end(), "?");
//     return count1 > count2;
// }

// void cand_elim::positive_example(std::vector<str_vect> & s, std::vector<str_vect> & g, const str_vect & d) {
//     for (str_vect hypo : g) {
//         if (!is_match(hypo, d)) {
//             auto it = std::find(g.begin(), g.end(), hypo);
//             g.erase(it);
//         }
//     }

//     for (str_vect hypo : s) {
//         if (!is_match(hypo, d)) {
//             auto it = std::find(g.begin(), g.end(), hypo);
//             g.erase(it);

//             str_vect new_s = generalise(hypo, d);
            
//         }
//     }
// }

// // adjusts hypothesis by comparing its elements with given training instance
// void cand_elim::adjust_S(std::vector<hypothesis> & s, const training_element & element) {
//     // only use positive instances
//     if (element.result) {
//         for (int j = 0; j < hypothesis.size(); ++j) {
//             // if there is a difference between elements
//             if (element.instance[j] != hypothesis[j]) {
//                 if (hypothesis[j] == "{}")  // if element is null set, take on value in instance
//                     hypothesis[j] = element.instance[j];
//                 else if (hypothesis[j] != "?")  // if element differs and is not most general, make most general
//                     hypothesis[j] = "?";
//             }
//         }
//     }
// }

/****************************************************************/
/* Stream operators
/****************************************************************/

// displays training instance attributes and hypothesis as a vector between '<' and '>'
std::ostream & cand_elim::operator<<(std::ostream & os, const str_vect & d) {
    os << "< ";
    int i = 0;
    for (; i < d.size() - 1; ++i) {
        os << d[i] << ", ";
    }
    os << d[i] << " >";
    return os;
}

// displays training instance attributes and hypothesis as a vector between '<' and '>'
std::ostream & cand_elim::operator<<(std::ostream & os, const hypothesis & h) {
    os << h.hypo;
    return os;
}

// displays training instance attributes and hypothesis as a vector between '<' and '>'
std::ostream & cand_elim::operator<<(std::ostream & os, const std::vector<hypothesis> & h_vect) {
    for (int i = 0; i < h_vect.size(); ++i) {
        os << h_vect[i] << std::endl;
    }
    return os;
}

// displays full training instance with result value
std::ostream & cand_elim::operator<<(std::ostream & os, const training_element & element) {
    os << "< " << element.instance << ", " << element.result << " >";
    return os;
}