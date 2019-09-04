/****************************************************************/
/* Includes
/****************************************************************/

#include "cand_elim.h"

using namespace cand_elim;
using namespace std;

/****************************************************************/
/* Main candidate elimination program
/****************************************************************/

int main(int argc, char const *argv[]) {
    vector<training_element> training_set;

    if (argc > 1) {
        string filename = string(argv[1]);
        load_training_set(training_set, filename);

        ofstream out_file("output.txt");
        if (out_file.is_open()) {

            // start with most specific hypothesis
            // '{}' used as symbol for null set
            vector<string> hypothesis = { "{}", "{}", "{}", "{}", "{}", "{}" };
            out_file << "Starting hypothesis: " << hypothesis << endl;

            out_file << endl << "Example training set:" << endl;

            int i = 0;
            for (; i < 4; ++i) { // iterate over example training instances
                out_file << endl;
                out_file << "Training instance: " << training_set[i] << endl;

                adjust_hypothesis(hypothesis, training_set[i]);
                out_file << "Adjusted hypothesis: " << hypothesis << endl;
            }

            out_file << endl << "Additional training instances:" << endl;

            for (; i < training_set.size(); ++i) {  // iterate over extra training instances to get target concept
                out_file << endl;
                out_file << "Training instance: " << training_set[i] << endl;

                adjust_hypothesis(hypothesis, training_set[i]);
                out_file << "Adjusted hypothesis: " << hypothesis << endl;
            }

            out_file << endl << "Target concept learnt with " << i-4 << " additional instance(s)" << endl;

            out_file.close();
            cout << "output written to 'output.txt'" << endl;
        }
        else {
            cout << "could not write to 'output.txt'" << endl;
            return 1;
        }
        return 0;
    }
    else {
        cout << "no input file name specified" << endl;
        return 1;
    }
    
}
