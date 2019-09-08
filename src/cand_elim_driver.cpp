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

bool cand_elim::more_spec_than_G(hypothesis & s) {
    for (int i = 0; i < G.size(); ++i) {
        if (!(s == G[i]) && s < G[i]) {
            return true;
        }
    }
    return false;
}

bool cand_elim::more_gen_than_S(hypothesis & g) {
    for (int i = 0; i < S.size(); ++i) {
        if (!(g == S[i]) && g > S[i]) {
            return true;
        }
    }
    return false;
}

void cand_elim::positive_example(const str_vect & d) {
    for (hypothesis g : G) {
        if (!(g % d)) {
            auto it = std::find(G.begin(), G.end(), g);
            G.erase(it);
        }
    }

    for (hypothesis s : S) {
        if (!(s % d)) {
            auto it = std::find(S.begin(), S.end(), s);
            S.erase(it);

            hypothesis new_s = s.min_generalise(d);
            if (new_s % d && more_spec_than_G(new_s))
                S.push_back(new_s);
        }
    }

    for (hypothesis s : S) {
        if (more_gen_than_S(s)) {
            auto it = std::find(S.begin(), S.end(), s);
            S.erase(it);
        }
    }
}

void cand_elim::negative_example(const str_vect & d) {
    for (hypothesis s : S) {
        if (s % d) {
            auto it = std::find(S.begin(), S.end(), s);
            S.erase(it);
        }
    }

    for (hypothesis g : G) {
        if (g % d) {
            auto it = std::find(G.begin(), G.end(), g);
            G.erase(it);

            vector<hypothesis> new_g = g.min_specialise(d);
            for (int i = 0; i < new_g.size(); ++i) {
                if (!(new_g[i] % d) && more_gen_than_S(new_g[i]))
                    G.push_back(new_g[i]);
            }
        }
    }

    for (hypothesis g : G) {
        if (more_spec_than_G(g)) {
            auto it = std::find(G.begin(), G.end(), g);
            G.erase(it);
        }
    }
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

        for (int i = 0; i < 2; ++i) {
            cout << "iteration " << i << endl;
            if (training_set[i].result)
                positive_example(training_set[i].instance);
            else 
                negative_example(training_set[i].instance);

            cout << S << endl;
            cout << G << endl;
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
