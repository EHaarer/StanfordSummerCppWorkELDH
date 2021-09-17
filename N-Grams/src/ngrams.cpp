// This n-Grams program was written by Aidan Mundy and Ethan Haarar

// Imports for all used libraries, primarily the Stanford C++ Library
#include <iostream>
#include "console.h"
#include "map.h"
#include "simpio.h"
#include "filelib.h"
#include <fstream>
#include "strlib.h"
#include "queue.h"
#include "vector.h"
#include "random.h"

using namespace std;

// Prototypes for all defined functions.
void printIntro();
void initializeFile(Queue<string>& fileVar);
int getngramLength(const int fileLength);
void makeMap(Queue<string>& fileVar, Map<Queue<string>, Vector<string> >& map, const int ngramLength);
void generateText(const Map<Queue<string>, Vector<string> >& map, const int ngramLength);
int getNumberOfWordsOrQuit();

int main() {
    // Defining variables to be passed into functions.
    Map<Queue<string>, Vector<string> > ngramMap;
    Queue<string> processedFile;

    printIntro();
    initializeFile(processedFile);
    int ngramLength = getngramLength(processedFile.size()); // Placed here to make sure console outputs are in the right order.
    makeMap(processedFile, ngramMap, ngramLength);
    generateText(ngramMap, ngramLength);

    cout << "Exiting." << endl;
    return 0;
}

// Prints the intro text to the console
void printIntro(){
    cout << "Welcome to CS 106B/X Random Writer ('N-Grams')!" << endl;
    cout << "This program generates random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl << endl;
}

// Takes in a file, dumping all words into a queue for later processing.
void initializeFile(Queue<string>& fileVar){
    ifstream inFile;
    string word;

    promptUserForFile(inFile, "Input file name? ");
    while(inFile >> word){
        fileVar.enqueue(word);
    }
    inFile.close();
}

// Gets the length of the ngrams from the user.
int getngramLength(const int fileLength){
    int ngramLength = getInteger("Value of N? ", "Illegal integer format. Try again.");
    // Loops until the user provides a valid input.
    while(ngramLength < 2 || ngramLength > fileLength){
        if(ngramLength < 2){
            cout << "N must be 2 or greater." << endl;
        } else if (ngramLength > fileLength){
            cout << "N cannot be greater than the number of words in the file" << endl;
        }
        ngramLength = getInteger("Value of N? ", "Illegal integer format. Try again.");
    }
    cout << endl;
    return ngramLength;
}

// Makes a map from the file with the appropriate length of ngrams.
void makeMap(Queue<string>& fileVar, Map<Queue<string>, Vector<string> >& map, const int ngramLength){
    Queue<string> tempKey;
    Queue<string> wrapAround;

    // Preps the tempKey and the wrapAround buffer with the appropriate number of words.
    for (int i = 1; i < ngramLength; i++) {
        tempKey.enqueue(fileVar.peek());
        wrapAround.enqueue(fileVar.dequeue());
    }

    // Creates the map for all ngrams until the files ends.
    int fileVarSize = fileVar.size();
    for (int i = 0; i < fileVarSize; i++) {
        map[tempKey].add(fileVar.peek());
        tempKey.dequeue();
        tempKey.enqueue(fileVar.dequeue());
    }

    // Processes the wrapAround buffer into the map.
    int wrapAroundSize = wrapAround.size();
    for (int i = 0; i < wrapAroundSize; i++) {
        map[tempKey].add(wrapAround.peek());
        tempKey.dequeue();
        tempKey.enqueue(wrapAround.dequeue());
    }

}

// Generates text based on the map of ngrams.
void generateText(const Map<Queue<string>, Vector<string> >& map, const int ngramLength){
    Queue<string> currentKey;
    Queue<string> tempKey;

    int numberOfWords = getNumberOfWordsOrQuit();
    // Loops until the user decides to quit by entering 0.
    while(numberOfWords != 0){
        cout << "... ";
        currentKey = randomKey(map); // Randomly chooses the starting key / starting point.
        tempKey = currentKey; // Creates a copy of the currentKey since it will be modified

        // Prints out the key word by word, stopping if the desired number of words is reached.
        for (int i = 0; !tempKey.isEmpty() && i < numberOfWords; i++) {
            cout << tempKey.dequeue() << " ";
        }

        // If more words need to be printed, this prints the remainder.
        for (int i = 0; i <= numberOfWords - ngramLength; i++) {
            currentKey.add(map[currentKey][randomInteger(0, map[currentKey].size() - 1)]); // Randomly chooses the next word.
            currentKey.dequeue();
            cout << currentKey.back() << " ";
        }

        cout << "..." << endl << endl;
        numberOfWords = getNumberOfWordsOrQuit();
    }
}

// Gets the user input for the number of words to generate, or quit if the user enters 0.
int getNumberOfWordsOrQuit(){
    int numberOfWords = getInteger("# of random words to generate (0 to quit)? ", "Illegal integer format. Try again.");
    while(numberOfWords != 0 && numberOfWords < 4){ // If the user does not want to quit, this ensures that the number of words to generate is long enough.
        cout << "Must be at least 4 words." << endl << endl;
        numberOfWords = getInteger("# of random words to generate (0 to quit)? ", "Illegal integer format. Try again.");
    }
    return numberOfWords;
}
