#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <utility>
#include <vector>
#include <map>
using namespace std;

// strcutures
typedef struct Node {
    char character;
    map<char, struct Node*> next_chars;
    bool word;
} node;

// Global Variables
map<char, node*> first_characters;

// method headers
vector<string> parse_string(string sentence);
vector<char> parse_word(string word);
map<char, node*> get_word_next(string word);
void get_all_words(node* start, vector<string> *all_words, string current);
void all_sub(node* start, vector<string> *all_words, string current, string substring);
void store_word(string word);
bool NotAlpha(char c) {return isalpha(c);}


// main method
int main(int argc, char** argv) {

    string input;

    // until the user ends, take new input
    while(true) {
        getline(cin, input);
        vector<string> parsed = parse_string(input);
        
        // STORE {string}
        // store a given word in the d.s.
        if(parsed[0] == "STORE") {
            store_word(parsed[1]);

        // COMPLETE {string}
        // find all endings for a given string
        } else if(parsed[0] == "COMPLETE") {
            node* start = new node;
            start->character = ' ';

            // get the final position of the given word in the d.s.
            start->next_chars = get_word_next(parsed[1]);
            start->word = false;
            vector<string> all_words;
            string current = "";

            get_all_words(start, &all_words, current);

            cout << "All auto-complete words:" << endl;

            for(int i = 0; i < all_words.size(); i++) {
                cout << parsed[1] + all_words[i] << endl;
            }
        
        // FULL {string}
        // find all words containing a given substring
        } else if(parsed[0] == "FULL") {
            node* start = new node;
            start->character = ' ';
            start->next_chars = first_characters;
            start->word = false;
            vector<string> all_words;
            string current = "";

            all_sub(start, &all_words, current, parsed[1]);

            cout << "All words containing (" << parsed[1] << "):" << endl;

            for(int i = 0; i < all_words.size(); i++) {
                cout << all_words[i] << endl;
            }
        
        // ALLWORDS
        // get all the words in the d.s.
        } else if(parsed[0] == "ALLWORDS") {
            node* start = new node;
            start->character = ' ';
            start->next_chars = first_characters;
            start->word = false;
            vector<string> all_words;
            string current = "";

            get_all_words(start, &all_words, current);

            for(int i = 0; i < all_words.size(); i++) {
                cout << all_words[i] << endl;
            }
        
        // READ {textfile + .txt}
        // read all the words in a text file into the d.s.
        } else if(parsed[0] == "READ") {
            
            fstream f;
            string file_name = parsed[1] + ".txt";
            f.open(file_name.c_str(), ios::in);
            string line;

            if(f.is_open()) {
                while(!f.eof()){
                    f >> line;
                    vector<string> parsed_line = parse_string(line);
                    for(int i = 0; i < parsed_line.size(); i++) {
                        store_word(parsed_line[i]);
                    }
                }
                f.close();
                cout << "Read file: " << file_name << endl;
            } else {
                cout << "Could not read: " << file_name << endl;
            }

        // QUIT
        // end the program
        } else if(parsed[0] == "QUIT") {
            break;
        } else {
            cout << "Command not recognized" << endl;
        }
    }

    return EXIT_SUCCESS;
}


/* Description: Create a vector of strings split by spaces.
 * Input: string: sentence.
 * Output: vector<string>: sentence split by spaces.
 * Effects: none.
*/
vector<string> parse_string(string sentence) {
    vector<string> output;
    char* sent_char = new char[1024];
    strcpy(sent_char, sentence.c_str());

    // use tok to check for the next space
    char* tok = new char[3], brkt;
    strcpy(tok, " \n");
    char* word = new char[1024];
    string fixed_word;

    // iterate through sentence, appending a new word at each space
    for(word = strtok(sent_char, tok); word; word = strtok(NULL, tok)) {
        fixed_word = "";
        for(int i = 0; i < strlen(word); i++) {
            if(isalnum(word[i])) {
                fixed_word += word[i];
            }
        }
        output.push_back(fixed_word);
    }

    // free allocated memory
    free(sent_char);
    free(tok);

    return output;
}


/* Description: create a vector of characters based on a given string.
 * Input: string: word.
 * Output: vector<char>.
 * Effects: none.
*/
vector<char> parse_word(string word) {
    vector<char> parsed;
    for(int i = 0; i < word.length(); i++) {
        parsed.push_back(word[i]);
    }
    return parsed;
}


/* Description: Given a string, append it to the Node strctuture, first_characters. Start by 
                checking for the last stored character in the word: if the word ends before
                the storage, indicate a word ends at that point; if there are more characters
                in the word then exist in the storage, create new nodes for each new character.
 * Input: string: word.
 * Output: none.
 * Effects: A new word is added to the first_characters data structure 
*/
void store_word(string word) {

    // parse the given word and create a reference to the stored words
    vector<char> parsed = parse_word(word);
    map<char, node*> *curr_nodes = &first_characters;

    for(int i = 0; i < parsed.size(); i++) {
        
        // if the current character is found in the current reference bank,
        // search the next bank for the next character in the word.
        if(curr_nodes->find(parsed[i]) != curr_nodes->end()) {
            curr_nodes = &curr_nodes->at(parsed[i])->next_chars;
        
        // if the character was not found, create a new node and append
        // it to the current reference bank
        } else {
            node* new_node = new node;
            if(i + 1 == word.length()) {
                new_node->word = true;
            } else
                new_node->word = false;
            
            new_node->character = parsed[i];
            curr_nodes->insert(pair<char, node*>(parsed[i], new_node));
            curr_nodes = &new_node->next_chars;
        }
    }
}


/* Description: Helper method to test the storage of new words. Return the list of
                next characters associated with the final character in a given word.
 * Input: string: word.
 * Output: vector<node>.
 * Effects: none.
*/
map<char, node*> get_word_next(string word) {
    vector<char> parsed = parse_word(word);
    map<char, node*> *curr_nodes = &first_characters;

    // continue through the length of the word until the last character is reached or
    // found in the datastructure.
    for(int i = 0; i < parsed.size(); i++) {
        if(curr_nodes->find(parsed[i]) != curr_nodes->end()) {
            curr_nodes = &curr_nodes->at(parsed[i])->next_chars;
        } else {
            break;
        }
    }

    return *curr_nodes;
}


/* Description: Method used to retrieve all words stored. Uses a depth-first search to
                build all possible words.
 * Input: node*: starting node, vector<string> *all_words: output vector containing all
 *        built words, string current: current word being built.
 * Output: none.
 * Effects: all_words will contain all stored words.
*/
void get_all_words(node* start, vector<string> *all_words, string current) {

    // base case, check if the current node is null
    if(start == NULL) {
        return;
    }

    // if not, add the current character to the current string
    current += start->character;

    // if the current node indicates a word is complete, add it the all_words
    if(start->word) {
        current += '\0';
        if(current[0] == ' ')
            (*all_words).push_back(current.erase(0, 1));
        else
            (*all_words).push_back(current);
    }

    // search all characters in the current nodes bank
    for(int i = 0; i < start->next_chars.size(); i++) {
        get_all_words(start->next_chars[i], all_words, current);
    }
}


/* Description: Method used to retrieve all words that contain a given substring. Uses
                a depth-first search to build all possible words.
 * Input: node*: starting node, vector<string> *all_words: output vector containing all
 *        built words, string current: current word being built, string substring: partial
 *        word compared against built words.
 * Output: none.
 * Effects: all_words will contain all stored words containing a given substring.
*/
void all_sub(node* start, vector<string> *all_words, string current, string substring) {

    // base case, check if the current node is null
    if(start == NULL) {
        return;
    }

    // if not, add the current character to the current string
    current += start->character;

    // if the current node indicates a word is complete, add it the all_words.
    // A second condition checks if the substring is found in the current stirng
    if(start->word && (current.find(substring) != std::string::npos)) {
        current += '\0';
        if(current[0] == ' ')
            (*all_words).push_back(current.erase(0, 1));
        else
            (*all_words).push_back(current);
    }

    // search all characters in the current nodes bank
    for(int i = 0; i < start->next_chars.size(); i++) {
        all_sub(start->next_chars[i], all_words, current, substring);
    }
}