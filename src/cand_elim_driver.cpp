/****************************************************************/
/* Includes
/****************************************************************/

#include "cand_elim.h"

using namespace cand_elim;
using namespace std;

std::vector<training_element> training_set;
std::vector<hypothesis> G;
std::vector<hypothesis> S;
vector<str_vect> hypothesis::attributes;

/****************************************************************/
/* General candidate elimination methods
/****************************************************************/

// loads training set for findS from given file
void cand_elim::load_training_set(std::string filename) {
    std::ifstream training(filename);
    std::string line, val;

    if (training.is_open()) {
        getline(training, line); // discard header line

        hypothesis::set_num_attributes(num_attributes);
        while (getline(training, line)) {
            str_vect instance(num_attributes, " ");
            bool result;

            std::istringstream temp(line);
            // add each attribute value to instance and add value to list of hypothesis attribute values
            for (int i = 0; i < num_attributes; ++i) {
                temp >> val;
                instance[i] = val;
                hypothesis::add_attribute_value(i, val);
            }

            // get example type
            temp >> val;
            if (val == "P")   result = true;
            else              result = false;
            
            training_set.push_back(training_element(instance, result));
        }
        training.close();
    }
}

// if equal is true, returns if s is more or equally as specific as all hypotheses in G
// else, returns if s is more specific than all hypotheses in G
bool cand_elim::more_spec_than_G(hypothesis & s, bool equal) {
    for (int i = 0; i < G.size(); ++i) {
        if (equal && s <= G[i]) {
            return true;
        }
        else if (!equal && s < G[i]) {
            return true;
        }
    }
    return false;
}

// if equal is true, returns if g is more or equally as general as all hypotheses in S
// else, returns if g is more general than all hypotheses in S
bool cand_elim::more_gen_than_S(hypothesis & g, bool equal) {
    for (int i = 0; i < S.size(); ++i) {
        if (equal && g >= S[i]) {
            return true;
        }
        else if (!equal && g > S[i]) {
            return true;
        }
    }
    return false;
}

// algorithm for positive training example
void cand_elim::positive_example(const str_vect & d) {
    vector<hypothesis> temp; // used to add back hypotheses that shouldn't be removed

    // add back hypotheses in G that do match d
    for (int i = 0; i < G.size(); ++i)
        if (G[i] % d)   temp.push_back(G[i]);

    G.clear();
    G.insert(G.begin(), temp.begin(), temp.end());
    temp.clear();

    // add hypotheses in S that do match
    // otherwise, add minimal generalisations that match and are more specific than some hypothesis in G
    for (int i = 0; i < S.size(); ++i) {
        if (S[i] % d)   temp.push_back(S[i]);
        else {
            hypothesis new_s = S[i].min_generalise(d);
            if (new_s % d && more_spec_than_G(new_s, true))
                temp.push_back(new_s);
        }
    }
    S.clear();
    S.insert(S.begin(), temp.begin(), temp.end());
    temp.clear();

    // remove hypotheses in S that are more general than any other in S
    // add back hypotheses that are less or equally as general as any other in S
    for (int i = 0; i < S.size(); ++i) {
        if (!more_gen_than_S(S[i], false))     temp.push_back(S[i]);
    }
    
    S.clear();
    S.insert(S.begin(), temp.begin(), temp.end());
    temp.clear();
}

void cand_elim::negative_example(const str_vect & d) {
    vector<hypothesis> temp; // used to add back hypotheses that shouldn't be removed

    // add back hypotheses in S that dont match d
    for (int i = 0; i < S.size(); ++i)
        if (!(S[i] % d))   temp.push_back(S[i]);

    S.clear();
    S.insert(S.begin(), temp.begin(), temp.end());
    temp.clear();

    // add hypotheses in G that dont match
    // otherwise, add minimal specialisations that match and are more general than some hypothesis in S
    for (int i = 0; i < G.size(); ++i) {
        if (!(G[i] % d))    temp.push_back(G[i]);
        else {
            vector<hypothesis> new_g = G[i].min_specialise(d);
            for (int j = 0; j < new_g.size(); ++j) {
                if (!(new_g[j] % d) && more_gen_than_S(new_g[j], true))
                    temp.push_back(new_g[j]);
            }
        }
    }
    G.clear();
    G.insert(G.begin(), temp.begin(), temp.end());
    temp.clear();

    // remove hypotheses in G that are more specific than any other in G
    // add back hypotheses that are less or equally as specific as any other in G
    for (int i = 0; i < G.size(); ++i)
        if (!more_spec_than_G(G[i], false))    temp.push_back(G[i]);

    G.clear();
    G.insert(G.begin(), temp.begin(), temp.end());
    temp.clear();
}

/****************************************************************/
/* Main candidate elimination program
/****************************************************************/

int main(int argc, char const *argv[]) {

    if (argc > 2) {
        string filename = string(argv[1]);
        string outfile = string(argv[2]);
        load_training_set(filename);

        // populate S and G with most specific and most general hypotheses respectively
        str_vect g(num_attributes, "?");
        str_vect s(num_attributes, "{}");

        S.push_back(s);
        G.push_back(g);
        
        ofstream out_file(outfile);
        if (out_file.is_open()) {
            // print initial setup
            out_file << "Initial setup:" << endl;
            out_file << "G = " << G << endl;
            out_file << "S = " << S << endl;
            out_file << endl;

            // iterate over training data and stop when S = G and both are singleton sets
            for (int i = 0; i < training_set.size(); ++i) {
                out_file << "Iteration " << i+1 << ": " << training_set[i] << endl << endl;
                if (training_set[i].result)
                    positive_example(training_set[i].instance);
                else 
                    negative_example(training_set[i].instance);

                out_file << "G = " << G << endl;
                out_file << "S = " << S << endl;
                out_file << endl;

                if (G.size() == 1 && S.size() == 1 && G[0] == S[0]) {
                    out_file << "Candidate elimination algorithm learnt hypothesis: " << G[0] << endl;
                    out_file << "after " << i+1 << " training examples";
                    break;
                }
            }
            return 0;
        }
    }
    else {
        cout << "input file name or output file name not specified" << endl;
        return 1;
    }
    
}
