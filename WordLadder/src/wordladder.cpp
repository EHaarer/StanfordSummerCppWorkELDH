// This Wordladder program was written by Ethan Haarer and Aidan Mundy

// Imports for all used libraries, primarily the Stanford C++ Library
#include <iostream>
#include <fstream>
#include "filelib.h"
#include "console.h"
#include "lexicon.h"
#include "simpio.h"
#include "stack.h"
#include "queue.h"
#include "strlib.h"

using namespace std;

// Prototypes for all defined functions.
void printIntro();
void processDictionaryFile(Lexicon& lexicon);
bool getInputs(string& startWord, string& endWord, const Lexicon& lexicon);
void findLadder(const string startWord, const string endWord, const Lexicon& lexicon);
void printFinalStack(const string startWord, const string endWord, Stack<string>& stringStack);

int main() {
    // Defining variables to be passed into functions.
    Lexicon lexicon;
    string startWord;
    string endWord;

    printIntro();
    processDictionaryFile(lexicon);

    // While the user has not chosen to quit, attempt to find a ladder between the chosed words.
    while(getInputs(startWord, endWord, lexicon)){
        findLadder(startWord, endWord, lexicon);
    }

    cout << "Have a nice day." << endl;
    return 0;
}

// Prints the intro text to the console
void printIntro() {
    cout << "Welcome to CS 106B/X Word Ladder!" << endl;
    cout << "Please give me two English words, and I will convert the" << endl;
    cout << "first into the second by modifying one letter at a time." << endl << endl;
}

// Processes a user defined file into a lexicon. The file should be, but does not have to be, a dictionary.
void processDictionaryFile(Lexicon& lexicon) {
    ifstream inFile;
    promptUserForFile(inFile, "Dictionary file name: ");
    cout << endl; // Required to match expected output files.
    lexicon.addWordsFromFile(inFile);
    inFile.close();
}

// Gets valid startWord and endWord inputs from the user, returns false if user wishes to quit and true otherwise.
bool getInputs(string& startWord, string& endWord, const Lexicon& lexicon) {
    // Loop will run until user decides to quit or provides valid inputs.
    while(true){

        startWord = toLowerCase(getLine("Word 1 (or Enter to quit): "));
        if(startWord == ""){ // Checks if user wants to quit.
            return false;
        }
        endWord = toLowerCase(getLine("Word 2 (or Enter to quit): "));
        if(endWord == ""){ // Checks if user wants to quit.
            return false;
        }

        // Checks for valid inputs. If inputs are not valid, loop will re run, otherwise it returns true.
        if(startWord == endWord) {
            cout << "The two words must be different." << endl << endl;
        } else if(startWord.size() != endWord.size()) {
            cout << "The two words must be the same length." << endl << endl;
        } else if(!lexicon.contains(startWord) || !lexicon.contains(endWord)){
            cout << "The two words must be found in the dictionary." << endl << endl;
        } else {
            return true;
        }
    }
}

// This function runs the logic that finds the word ladder.
void findLadder(const string startWord, const string endWord, const Lexicon& lexicon) {
    Queue<Stack<string> > stackQueue;
    Stack<string> stringStack;
    Lexicon usedWords;
    Stack<string> tempStack;
    string tempWord;

    // Prep the previoudly defined variables so that the logic loop runs properly.
    stringStack.push(startWord);
    usedWords.add(startWord);
    stackQueue.enqueue(stringStack);

    // Loop runs until a ladder is found or all ladder options are expended.
    while(!stackQueue.isEmpty()){
        stringStack = stackQueue.dequeue();

        // Iterates through each character position in the word at the top of the stack.
        for (int i = 0; i < stringStack.peek().size(); i++) {
            tempWord = stringStack.peek();

            // Iterates through the alphabet in the current position of the word.
            for (char ch = 'a'; ch <= 'z'; ch++) {
                tempWord[i] = ch;
                if(lexicon.contains(tempWord) && !usedWords.contains(tempWord)){ // Ensures that the guessed word is a valid word and has not already been used.
                    if(tempWord == endWord){ // Checks to see if a ladder as been found.
                        stringStack.push(tempWord);
                        printFinalStack(startWord, endWord, stringStack);
                        return;
                    } else {
                        tempStack = stringStack;
                        tempStack.push(tempWord);
                        usedWords.add(tempWord);
                        stackQueue.enqueue(tempStack);
                    }
                }
            }
        }
    }
    //The code will only print this if all possible ladders have been expended without finding one that works.
    cout << "No word ladder found from " << endWord << " back to " << startWord << "." << endl << endl;
}

// Prints the final stack that contains the word ladder.
void printFinalStack(const string startWord, const string endWord, Stack<string>& stringStack){
    cout << "A ladder from " << endWord << " back to " << startWord << ":" << endl;
    while(!stringStack.isEmpty()){
        cout << stringStack.pop() << " ";
    }
    cout << endl << endl;
}
