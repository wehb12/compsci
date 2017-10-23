TO DO:

1. [x] Create a strategy generator
  1.1 [ ] Make it lense nonesenical
    1.1.1 [x] If two exit keywords appear in a row, then stop generating (one if no 'IF' statement)
    1.1.2 [x] Reduce probability of putting in a number, they look weird.
    1.1.3 [x] Reduce probability of '+' and '-' operators
    1.1.4 [ ] New way of generating seed.
    1.1.5 [ ] IF LAST = Z GOTO... followed by IF LAST = Z GOTO... is silly, stop this from happening
  1.2 [x] Make it object oriented
    1.2.1 [x] Remove the vector<vector<char>> part and just use a 1D array that stores current line's flags.
  1.3 [x] Add in ITERATIONS and MYSCORE variables - change when flags [8] and [9] trigger when you do this
  1.4 [ ] When a variable has been printed, it should no longer be an option to print again (ie. the values of tempNum that chose it should be reassigned)
2. [x] Read in strategies - store words from file as a 2D matrix of strings?
3. [ ] Use a BST to see which lines have been read to avoid an infinite loop.
