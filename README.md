# Lab 4: Candidate Elimination Algorithm

## What am I looking at here?

* After running the Candidate Elimination program on the 'table_1_data' in the training_sets folder, the output in 'japanese_economy_car' is obtained

* After running the Candidate Elimination program on the 'custom_training_data' in the training_sets folder, the output in 'japanese_sports_car' is obtained

### Q1

* The Japanese Sports Car concept was learnt after processing the 4 examples in 'custom_training_data'

### Q2

* The minimum is 4 examples as 2 positives change S to contain only the target concept and 2 negatives change G to contain only the target concept

## But how do I run it?

* First 'make' then run 'make run' to run:
    './candidate_elimination training_sets/custom_training_data.txt output.txt'

* This command reads the training data in 'custom_training_data', performs candidate elimination and outputs to 'output.txt'