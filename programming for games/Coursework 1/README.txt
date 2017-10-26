TO DO:

1. [x] Create a strategy generator
  1.1 [ ] Make it lense nonesenical
    1.1.1 [x] If two exit keywords appear in a row, then stop generating (one if no 'IF' statement)
    1.1.2 [x] Reduce probability of putting in a number, they look weird.
    1.1.3 [x] Reduce probability of '+' and '-' operators
    1.1.4 [ ] New way of generating seed.
    1.1.5 [ ] IF LAST = Z GOTO... followed by IF LAST = Z GOTO... is silly, stop this from happening
  1.2 [x] Make it object oriented
    1.2.1 [x] Remove the vector<vector<char>> part and just use a 1D array that stores current line's flags
  1.3 [x] Add in ITERATIONS and MYSCORE variables - change when flags [8] and [9] trigger when you do this
  1.4 [ ] When a variable has been printed, it should no longer be an option to print again (ie. the values of tempNum that chose it should be reassigned)
2. [x] Read in strategies - store words from file as a 2D matrix of strings?
3. [x] Use a BST to see which lines have been read to avoid an infinite loop
  3.1 [x] Combine points 3 & 4 - use a BST to find duplicate strategies faster - How are they ordered ?? Number of lines/ features ??
  3.2 [x] Template BST class to allow storage of comparables - line numbers and strategies
4. [x] Discard duplicate strategies, using overloading of == operator
  4.1 [x] To make strategy generation faster, as few strategies as possible should be discarded. Make them more likely to be unique
    4.1.1 [x] Make them longer
    4.1.2 [x] Make seed % by a larger number
5. [ ] Add error checking to interpreter - mustn't interpret a file with incorrect syntax etc.
6. [ ] Use threads to check for user input (exit) while tournament running
  6.1 [ ] Display est. run time?
7. [ ] Check that all heap variables are deleted
8. [x] Check what OS is in use and adjust ClearScreen() accordingly
9. [x] Display statsistics from each game
  8.1 [ ] Such as how many times someone stayed silent, betrayed, how many wins AND total score
10. [ ] Comment code
11. [ ] Offer default run settings option
12. [ ] Allow the user the option to run a tournament of the victors of the last tournament
13. [x] Output statistics to a file
  13.1 [x] Make a Table class and override << operator to send to a file
14. [x] GANG DILEMMA - Test all combinations of strategies against a random but constant set of strategies
15. [ ] Automatically print file at end with all scores in, for both.
