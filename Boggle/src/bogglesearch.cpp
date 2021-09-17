// This code was written by Ethan Haarer and Aidan Mundy for Stanford's cs106b. It runs the logic for a human vs computer version of the word game Boggle.
#include "bogglegui.h"
#include <string>
#include "grid.h"
#include "lexicon.h"
#include "set.h"
using namespace std;

bool humanWordSearch(Grid<char>& board, Lexicon& dictionary, string word);
bool recursiveHumanWordSearch(const Grid<char>& board, string word, const int r, const int c, Grid<bool>& markers);
Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords);
void recursiveComputerWordSearch(const Grid<char>& board, const Lexicon& dictionary, const Set<string>& humanWords, Set<string>& foundWords, const int r, const int c, Grid<bool>& markers, string word);
int worthOfWord(const int length);
void falsifyBoolGrid(Grid<bool>& grid);

/**
 * Given a Boggle board, a dictionary, and a word, the function determines whether or not the
 * word can be found in both the dictionary and the Boggle board.
 *
 * Called by BoggleGUI to handle the logic for the human's turn in Boggle.
 *
 * @param board      The Boggle board to check.
 * @param dictionary The dictionary to use.
 * @param word       The word to find on the board and in the dictionary.
 * @return Whether or not the word was found.
 */
bool humanWordSearch(Grid<char>& board, Lexicon& dictionary, string word) {
    Grid<bool> markers(board.height(), board.width());
    BoggleGUI::clearHighlighting();

    // Checks to see that the word is both in the dictionary, and also long enough.
    if (word.length() < BoggleGUI::MIN_WORD_LENGTH || !dictionary.contains(word)){
        return false;
    }

    word = toUpperCase(word);

    // Runs a recursive check on all squares of the board, stopping when the target word is found or when all starting squares are exhausted.
    for (int h = 0; h < board.height(); h++) {
        for (int w = 0; w < board.width(); w++){
            falsifyBoolGrid(markers);
            if (recursiveHumanWordSearch(board, word, h, w, markers)){
                BoggleGUI::scorePointsHuman(worthOfWord(word.length()));
                return true;
            }
        }
    }
    // Only returns false if all starting squares are exhausted without finding the word on the boggle board.
    return false;
}

/**
 * Given a Boggle board, a word, and a location on the board, the function recursively
 * searches for the given word, tracking locations where it has already checked in the
 * markers grid and returns whether or not the word was found.
 *
 * @param board   The Boggle board to check.
 * @param word    The word to find on the board and in the dictionary.
 * @param r       The row value for the location on the board being checked.
 * @param c       The column value for the location on the board being checked.
 * @param markers A grid of booleans that tracks the search history for the recursive branch.
 * @return Whether or not the word was found.
 */
bool recursiveHumanWordSearch(const Grid<char>& board, string word, const int r, const int c, Grid<bool>& markers) {
    // If the previous call of recursiveHumanWordSearch found the last letter in the word, end the recursive search by returning true.
    if (word == "") {
        return true;
    }

    // If the inputs are out of bounds or the current location was already used, return false.
    if (!board.inBounds(r, c) || markers[r][c]) {
        return false;
    }

    // Highlights the word, then immediately unhighlights and returns false if the current location does not match the target letter.
    BoggleGUI::setHighlighted(r,c,true);
    if (word[0] != board[r][c]) {
        BoggleGUI::setHighlighted(r,c,false);
        return false;
    }

    // Marks the current location as used, cuts off the first letter of the word, and runs the recursive calls for the eight surrounding locations.
    markers[r][c] = true;
    word = word.substr(1); // Removes the first letter from the word. The word variable will not need to be backtracked because it is not passed by reference.
    bool valid = recursiveHumanWordSearch(board, word, r - 1, c - 1, markers)|| // If the word is found in any recursive path, valid will be true.
                 recursiveHumanWordSearch(board, word, r    , c - 1, markers)||
                 recursiveHumanWordSearch(board, word, r + 1, c - 1, markers)||
                 recursiveHumanWordSearch(board, word, r - 1, c    , markers)||
                 recursiveHumanWordSearch(board, word, r + 1, c    , markers)||
                 recursiveHumanWordSearch(board, word, r - 1, c + 1, markers)||
                 recursiveHumanWordSearch(board, word, r    , c + 1, markers)||
                 recursiveHumanWordSearch(board, word, r + 1, c + 1, markers);
    markers[r][c] = false; // Backtracks the marker grid by setting the current location to false.
    BoggleGUI::setHighlighted(r,c,valid); // Keeps the space highlighted if the path is valid, unhighlights if it isnt.
    return valid; // If any of the recursive functions found the word, returns true.
}

/**
 * Given a Boggle board, a dictionary, and a list of words used by the human,
 * the function finds all possible words on the Boggle board that are also in
 * the provided dictionary and havent been used by the human.
 *
 * Called by BoggleGUI to handle the logic for the computer's turn in Boggle.
 *
 * @param board      The Boggle board to check.
 * @param dictionary The dictionary to use.
 * @param humanWords A list of valid words used by the human.
 * @return A set of all words found by the function that are in the dictionary, on the board, and weren't used by the user.
 */
Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {
    Set<string> foundWords;
    Grid<bool> markers(board.height(), board.width());

    // Converts all words in the input set to uppercase for later use.
    // (As a note, we considered converting it to a lexicon since they ignore case, but we determined that this solution was quicker.)
    // THIS SHOULD BE REMOVED IF THIS INPUT IS CLARIFIED AS ALWAYS UPPERCASE (which it currently isnt according to the spec).
    // Technically this is unnecessary because our testing says that the values in humanWords are always uppercase, but we wanted to be sure.
    string tempWord;
    for (string word : humanWords){
        tempWord = toUpperCase(word);
        if (word != tempWord) { // Speeds the process up a little bit by not replacing already uppercase words.
            humanWords.remove(word);
            humanWords.add(tempWord);
        }
    }

    // Runs a recursive check on all squares of the board, stopping when starting squares and recursive paths are exhausted.
    for (int h = 0; h < board.height(); h++) {
        for (int w = 0; w < board.width(); w++){
            falsifyBoolGrid(markers);
            recursiveComputerWordSearch(board, dictionary, humanWords, foundWords, h, w, markers, "");
        }
    }
    return foundWords;
}

/**
 * Given a Boggle board, a dictionary, a list of words used by the human, and
 * a list of all words found by the computer, the function finds all possible
 * words on the Boggle board that are in the provided dictionary and havent been
 * used by the human.
 *
 * @param board      The Boggle board to check.
 * @param dictionary The dictionary to use.
 * @param humanWords The words used by the human.
 * @param foundWords An output parameter of all words found by the computer.
 * @param r          Row to begin recursive search from.
 * @param c          Column to begin recursive search from.
 * @param markers    A grid of booleans that tracks the search history for the recursive branch.
 * @param word       The word/prefix to continue to build on top of during the recursive search.
 */
void recursiveComputerWordSearch(const Grid<char>& board, const Lexicon& dictionary, const Set<string>& humanWords, Set<string>& foundWords, const int r, const int c, Grid<bool>& markers, string word) {
    // End this branch of the recursive path if the position was already used, the position is not in bounds, or the dictionary does not contain any word that starts with (or is) the current word.
    if (!board.inBounds(r, c) || markers[r][c] || !dictionary.containsPrefix(word + board[r][c])) {
        return;
    }

    word += board[r][c]; // Adds the current position's letter to the word. The word variable will not need to be backtracked because it is not passed by reference.
    markers[r][c] = true;

    // If the word is long enough, not in the human's list of words, not in the found list of words, and is in the dictionary, add the word to found words and add the respective score to the scoreboard.
    if (word.length() >= BoggleGUI::MIN_WORD_LENGTH && !humanWords.contains(word) && !foundWords.contains(word) && dictionary.contains(word)){
        foundWords.add(word);
        BoggleGUI::scorePointsComputer(worthOfWord(word.length()));
    }

    // Runs the recursive functions for the eight squares surrounding the current location.
    recursiveComputerWordSearch(board, dictionary, humanWords, foundWords, r - 1, c - 1, markers, word);
    recursiveComputerWordSearch(board, dictionary, humanWords, foundWords, r    , c - 1, markers, word);
    recursiveComputerWordSearch(board, dictionary, humanWords, foundWords, r + 1, c - 1, markers, word);
    recursiveComputerWordSearch(board, dictionary, humanWords, foundWords, r - 1, c    , markers, word);
    recursiveComputerWordSearch(board, dictionary, humanWords, foundWords, r + 1, c    , markers, word);
    recursiveComputerWordSearch(board, dictionary, humanWords, foundWords, r - 1, c + 1, markers, word);
    recursiveComputerWordSearch(board, dictionary, humanWords, foundWords, r    , c + 1, markers, word);
    recursiveComputerWordSearch(board, dictionary, humanWords, foundWords, r + 1, c + 1, markers, word);

    markers[r][c] = false; // Backtracks the marker grid by setting the current location back to false.
}

/**
 * Given a length input, determines the corresponding point value.
 * @param length The length of a word.
 * @return The point worth of the word according to Boggle's scoring rules.
 */
int worthOfWord(const int length){
    if (length <= 4){
        return 1;
    } else if (length == 5) {
        return 2;
    } else if (length == 6) {
        return 3;
    } else if (length == 7) {
        return 5;
    } else {//(length >= 8)
        return 11;
    }
}

/**
 * Makes all values in a boolean grid false.
 * @param grid The grid to falsify.
 */
void falsifyBoolGrid(Grid<bool>& grid){
    for (int h = 0; h < grid.height(); h++) {
        for (int w = 0; w < grid.width(); w++){
            grid[h][w] = false;
        }
    }
}
