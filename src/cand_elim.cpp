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

// sets the number of attributes for hypotheses
void hypothesis::set_num_attributes(int num) {
    attributes.resize(num);
}

// adds valid value for the indexed attribute if it doesnt already exist
void hypothesis::add_attribute_value(int index, std::string value) {
    str_vect attr = attributes[index];
    auto it = std::find(attr.begin(), attr.end(), value);
    if (it == attr.end())   attributes[index].push_back(value);
}

// returns different attribute for constructing hypotheses that don't match training instance
std::string hypothesis::get_diff_attribute(int index, std::string value) {
    for (int i = 0; i < num_attributes; ++i) {
        if (attributes[index][i] != value)  return attributes[index][i];
    }
    return "?";
}

// check if hypotheses are equal (have exact same attribute values)
bool hypothesis::operator==(const hypothesis & other) {
    return hypo == other.hypo;
}

// check if instance matches hypothesis
bool hypothesis::operator%(const str_vect & other) {
    for (int i = 0; i < num_attributes; ++i) {
        if (hypo[i] != other[i] && hypo[i] != "?" && other[i] != "?")
            return false;
    }
    return true;
}

// check if hypothesis matches another
// used for detecting if one is a more specific/general version of the other
bool hypothesis::operator%(const hypothesis & other) {
    int thisCount = std::count(hypo.begin(), hypo.end(), "?");
    int otherCount = std::count(other.hypo.begin(), other.hypo.end(), "?");

    int neededCols = num_attributes - std::max(thisCount, otherCount);
    int matchingCols = 0;
    for (int i = 0; i < num_attributes; ++i) {
        if (hypo[i] != "?" || other.hypo[i] != "?")
            if (hypo[i] == other.hypo[i])
                ++matchingCols;
    }
    return matchingCols >= neededCols;
}

// check if this hypothesis is more general than the other
bool hypothesis::operator>(const hypothesis & other) {
    int thisCount = std::count(hypo.begin(), hypo.end(), "?");
    int otherCount = std::count(other.hypo.begin(), other.hypo.end(), "?");
    return *this % other && thisCount > otherCount;
}

// check if this hypothesis is more specific than the other
bool hypothesis::operator<(const hypothesis & other) {
    int thisCount = std::count(hypo.begin(), hypo.end(), "?");
    int otherCount = std::count(other.hypo.begin(), other.hypo.end(), "?");
    return *this % other && thisCount < otherCount;
}

// check if this hypothesis is more or equally general compared to the other
bool hypothesis::operator>=(const hypothesis & other) {
    if (*this == other)
        return true;
    else
        return *this > other;
}

// check if this hypothesis is more or equally specific compared to the other
bool hypothesis::operator<=(const hypothesis & other) {
    if (*this == other)
        return true;
    else
        return *this < other;
}

// generate minimal generalisations of a hypothesis
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

// generate minimal specialisations of a hypothesis
std::vector<hypothesis> hypothesis::min_specialise(const str_vect & d) {
    std::vector<hypothesis> min_gs;
    str_vect new_g;
    int last_spec = -1; // index of last attribute that was specialised for this hypothesis
    for (int i = 0; i < num_attributes; ++i) {
        // for each '?' attribute in hypothesis,
        // make new hypothesis with this attribute specialised to not match d, since its a negative example
        if (hypo[i] == "?" && i != last_spec) {
            new_g = hypo;
            new_g[i] = get_diff_attribute(i, d[i]);
            last_spec = i;
            min_gs.push_back( hypothesis(new_g) );
        }
    }
    return min_gs;
}

/****************************************************************/
/* Stream operators
/****************************************************************/

// displays training instance attributes as a vector between '<' and '>'
std::ostream & cand_elim::operator<<(std::ostream & os, const str_vect & d) {
    os << "< ";
    int n = 0;
    for (; n < d.size() - 1; ++n)
        os << d[n] << ", ";
    os << d[n] << " >";
    return os;
}

// displays hypothesis as a vector between '<' and '>'
std::ostream & cand_elim::operator<<(std::ostream & os, const hypothesis & h) {
    os << h.hypo;
    return os;
}

// displays list of hypotheses as a set between '{' and '}'
std::ostream & cand_elim::operator<<(std::ostream & os, const std::vector<hypothesis> & h_vect) {
    os << "{ ";
    int n = 0;
    if (h_vect.size() > 0) {
        for (; n < h_vect.size() - 1; ++n)
            os << h_vect[n] << "," << std::endl;
        os << h_vect[n];
    }
    os << " }";
    return os;
}

// displays full training instance with result value
std::ostream & cand_elim::operator<<(std::ostream & os, const training_element & element) {
    os << "< " << element.instance << ", " << element.result << " >";
    return os;
}