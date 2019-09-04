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
/* General candidate elimination methods
/****************************************************************/

// loads training set for findS from given file
void cand_elim::load_training_set(std::vector<training_element> & training_set, std::string filename) {
    std::ifstream training(filename);
    std::string line, val;

    if (training.is_open()) {
        getline(training, line); // discard header line

        while (getline(training, line)) {
            str_vect instance(6, " ");
            bool result;

            std::istringstream temp(line);
            for (int i = 0; i < 6; ++i) {
                temp >> val;
                instance[i] = val;
            }

            temp >> val;
            if (val == "Yes")   result = true;
            else                result = false;
            
            training_set.push_back(training_element(instance, result));
        }
        training.close();
    }
}

void cand_elim::is_match(const str_vect & hypo, const str_vect & d) {
    for (int i = 0; i < hypo.size(); ++i) {
        if (hypo[i] != d[i] && hypo[i] != "?")    return false;
    }
    return true;
}

str_vect cand_elim::generalise(const str_vect & s, const str_vect & d) {
    str_vect new_s = s;
    for (int j = 0; j < s.size(); ++j) {
        // if there is a difference between elements
        if (d[j] != s[j]) {
            if (s[j] == "{}")  // if element is null set, take on value in instance
                s[j] = d[j];
            else if (s[j] != "?")  // if element differs and is not most general, make most general
                s[j] = "?";
        }
    }
    return new_s;
}

bool cand_elim::more_general(const str_vect & h1, const str_vect & h2) {
    int count1 = std::count(h1.begin(), h1.end(), "?");
    int count2 = std::count(h2.begin(), h2.end(), "?");
    return count1 > count2;
}



void cand_elim::positive_example(std::vector<str_vect> & s, std::vector<str_vect> & g, const str_vect & d) {
    for (str_vect hypo : g) {
        if (!is_match(hypo, d)) {
            auto it = std::find(g.begin(), g.end(), hypo);
            g.erase(it);
        }
    }

    for (str_vect hypo : s) {
        if (!is_match(hypo, d)) {
            auto it = std::find(g.begin(), g.end(), hypo);
            g.erase(it);

            str_vect new_s = generalise(hypo, d);
            s.push_back(new_s);
        }
    }
}

// adjusts hypothesis by comparing its elements with given training instance
void cand_elim::adjust_S(std::vector<hypothesis> & s, const training_element & element) {
    // only use positive instances
    if (element.result) {
        for (int j = 0; j < hypothesis.size(); ++j) {
            // if there is a difference between elements
            if (element.instance[j] != hypothesis[j]) {
                if (hypothesis[j] == "{}")  // if element is null set, take on value in instance
                    hypothesis[j] = element.instance[j];
                else if (hypothesis[j] != "?")  // if element differs and is not most general, make most general
                    hypothesis[j] = "?";
            }
        }
    }
}

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

// displays full training instance with result value
std::ostream & cand_elim::operator<<(std::ostream & os, const training_element & element) {
    os << "< " << element.instance << ", " << element.result << " >";
    return os;
}