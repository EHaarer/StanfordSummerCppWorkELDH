// This code was written by Ethan Haarer and Aidan Mundy. It handles
// huffman encoding for assignment 7 of cs106b at stanford.

#include "encoding.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "filelib.h"

Map<char, int> buildFrequencyTable(istream& input){
    Map<char, int> freqTable;
    char ch;
    // Tallies the character frequencies
    while (input.get(ch)){
        freqTable[ch] = freqTable[ch] + 1;
    }
    return freqTable;
}

HuffmanNode* buildEncodingTree(Map<char, int>& freqTable){
    double treePriority;
    HuffmanNode* one;
    HuffmanNode* zero;
    PriorityQueue<HuffmanNode*> order;

    // Enqueues all of the initial huffman nodes.
    for (char ch : freqTable){
        order.enqueue(new HuffmanNode(ch), freqTable[ch]);
    }

    // Orders the nodes into a huffman tree
    while(order.size() > 1){
        treePriority = order.peekPriority();
        zero = order.dequeue();
        treePriority += order.peekPriority();
        one = order.dequeue();
        order.enqueue(new HuffmanNode(zero, one), treePriority);
    }

    // Sends the final remaining node as the resultant tree.
    return order.dequeue();
}

string flattenTreeToHeader(HuffmanNode* t){
    string header = "";
    // Recursively runs a string builder that builds the header.
    return (t->isLeaf() ?
                "." + charToString(t->ch) :
                "(" + flattenTreeToHeader(t->zero) + flattenTreeToHeader(t->one) + ")");
}

HuffmanNode* recreateTreeFromHeader(string str){
    // Base case, one single character left.
    if (str[0] == '.' && str.length() == 2){
        return new HuffmanNode(str[1]);
    } else { //recursive case
        str = str.substr(1, str.length()-2);
        HuffmanNode* zero = nullptr;
        HuffmanNode* one = nullptr;
        // If the left side is a leaf, this runs
        if(str[0] == '.'){
            zero = new HuffmanNode(str[1]);
            one = recreateTreeFromHeader(str.substr(2));
        } else { // Otherise this runs
            int parenthesesIndex = 0;
            // Counts and matches up parentheses.
            for(int i = 0; i < str.length(); i++){
                if(str[i] == '(') {
                    parenthesesIndex++;
                } else if(str[i] == ')'){
                    parenthesesIndex--;
                    if(parenthesesIndex <=0){ //Once parentheses are matched, assign appropriate children.
                        zero = recreateTreeFromHeader(str.substr(0, i+1));
                        one = recreateTreeFromHeader(str.substr(i+1));
                        break;
                    }
                }
            }
        }
        return new HuffmanNode(zero, one);
    }
}

void searchTree(Map<char, string>& encoding, const HuffmanNode* treePtr, string sequence){
    if (treePtr->isLeaf()){ // Base Case
        encoding[treePtr->ch] = sequence;
    } else { // Recursive Case
        searchTree(encoding, treePtr->zero, sequence + '0');
        searchTree(encoding, treePtr->one, sequence + '1');
    }
}

Map<char, string> buildEncodingMap(HuffmanNode* encodingTree){
    Map<char, string> encodingMap;
    searchTree(encodingMap, encodingTree, "");
    return encodingMap;
}

void freeTree(HuffmanNode* t){
    if(t->isLeaf()){ // Base case
        delete t;
    } else { // Recursive case
        freeTree(t->zero);
        freeTree(t->one);
        delete t;
    }
}

void compress(istream& input, HuffmanOutputFile& output){ 
    Map<char, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* huffmanTree = buildEncodingTree(freqTable);
    Map<char, string> encMap = buildEncodingMap(huffmanTree);
    output.writeHeader(flattenTreeToHeader(huffmanTree));
    rewindStream(input); // Rewunds stream to beginning
    char ch;
    while (input.get(ch)){ // Writes each character's bits.
        for(char bit : encMap[ch]){
            output.writeBit(charToInteger(bit));
        }
    }
    freeTree(huffmanTree);
}

void decompress(HuffmanInputFile& input, ostream& output){
    HuffmanNode* huffmanTree = recreateTreeFromHeader(input.readHeader());
    Map<char, string> encMap = buildEncodingMap(huffmanTree);
    Map<string, char> revEncMap;
    for(char oldKey : encMap.keys()){ // Reverse map of binary to characters.
        revEncMap[encMap[oldKey]] = oldKey;
    }

    string tempString = "";
    int bit = input.readBit();
    while(bit != -1){ // Reads each characters bits
        tempString += integerToString(bit);
        if(revEncMap.containsKey(tempString)){ // If a character is found, search for next character and output this one.
            output << revEncMap[tempString];
            tempString = "";
        }
        bit = input.readBit();
    }

    freeTree(huffmanTree);
}
