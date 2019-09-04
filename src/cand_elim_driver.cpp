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

/****************************************************************/
/* Main candidate elimination program
/****************************************************************/

int main(int argc, char const *argv[]) {

    if (argc > 1) {
        string filename = string(argv[1]);
        load_training_set(filename);

        for (int i = 0; i < training_set.size(); ++i) {
            cout << training_set[i] << endl;
        }

        cout << endl;

        for (int j = 0; j < hypothesis::attributes.size(); ++j) {
            cout << hypothesis::attributes[j] << endl;
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
