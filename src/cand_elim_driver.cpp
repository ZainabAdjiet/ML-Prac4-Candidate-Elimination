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
            for (int i = 0; i < num_attributes; ++i) {
                temp >> val;
                instance[i] = val;
                hypothesis::add_attribute_value(i, val);
            }

            temp >> val;
            if (val == "P")   result = true;
            else              result = false;
            
            training_set.push_back(training_element(instance, result));
        }
        training.close();
    }
}

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

void cand_elim::positive_example(const str_vect & d) {
    vector<hypothesis> temp;
    for (int i = 0; i < G.size(); ++i)
        if (G[i] % d)   temp.push_back(G[i]);

    G.clear();
    G.insert(G.begin(), temp.begin(), temp.end());
    temp.clear();

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

    for (int i = 0; i < S.size(); ++i) {
        if (!more_gen_than_S(S[i], false))     temp.push_back(S[i]);
    }
    
    S.clear();
    S.insert(S.begin(), temp.begin(), temp.end());
    temp.clear();
}

void cand_elim::negative_example(const str_vect & d) {
    vector<hypothesis> temp;
    for (int i = 0; i < S.size(); ++i)
        if (!(S[i] % d))   temp.push_back(S[i]);

    S.clear();
    S.insert(S.begin(), temp.begin(), temp.end());
    temp.clear();

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

    if (argc > 1) {
        string filename = string(argv[1]);
        load_training_set(filename);

        str_vect g(num_attributes, "?");
        str_vect s(num_attributes, "{}");

        S.push_back(s);
        G.push_back(g);

        cout << "Initial setup:" << endl;
        cout << "G = " << G << endl;
        cout << "S = " << S << endl;
        cout << endl;

        for (int i = 0; i < training_set.size(); ++i) {
            cout << "Iteration " << i+1 << ": " << training_set[i] << endl << endl;
            if (training_set[i].result)
                positive_example(training_set[i].instance);
            else 
                negative_example(training_set[i].instance);

            cout << "G = " << G << endl;
            cout << "S = " << S << endl;
            cout << endl;
        }

        // ofstream out_file("output.txt");
        // if (out_file.is_open()) {

        //     // start with most specific hypothesis
        //     // '{}' used as symbol for null set
        //     vector<string> hypothesis = { "{}", "{}", "{}", "{}", "{}", "{}" };
        //     out_file << "Starting hypothesis: " << hypothesis << endl;

        //     out_file << endl << "Example training set:" << endl;

        //     int i = 0;
        //     for (; i < 4; ++i) { // iterate over example training instances
        //         out_file << endl;
        //         out_file << "Training instance: " << training_set[i] << endl;

        //         adjust_hypothesis(hypothesis, training_set[i]);
        //         out_file << "Adjusted hypothesis: " << hypothesis << endl;
        //     }

        //     out_file << endl << "Additional training instances:" << endl;

        //     for (; i < training_set.size(); ++i) {  // iterate over extra training instances to get target concept
        //         out_file << endl;
        //         out_file << "Training instance: " << training_set[i] << endl;

        //         adjust_hypothesis(hypothesis, training_set[i]);
        //         out_file << "Adjusted hypothesis: " << hypothesis << endl;
        //     }

        //     out_file << endl << "Target concept learnt with " << i-4 << " additional instance(s)" << endl;

        //     out_file.close();
        //     cout << "output written to 'output.txt'" << endl;
        // }
        // else {
        //     cout << "could not write to 'output.txt'" << endl;
        //     return 1;
        // }
        return 0;
    }
    else {
        cout << "no input file name specified" << endl;
        return 1;
    }
    
}
