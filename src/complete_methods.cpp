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


vector<string> parse_string(string sentence) {
    vector<string> output;
    char* sent_char = new char[1024];
    strcpy(sent_char, sentence.c_str());
    char* tok = new char[3], brkt;
    strcpy(tok, " \n");
    char* word = new char[1024];
    string fixed_word;
    for(word = strtok(sent_char, tok); word; word = strtok(NULL, tok)) {
        fixed_word = "";
        for(int i = 0; i < strlen(word); i++) {
            if(isalnum(word[i])) {
                fixed_word += word[i];
            }
        }
        output.push_back(fixed_word);
    }
    free(sent_char);
    free(tok);

    return output;
}

vector<char> parse_word(string word) {
    vector<char> parsed;
    for(int i = 0; i < word.length(); i++) {
        parsed.push_back(word[i]);
    }
    return parsed;
}

void store_word(string word) {
    vector<char> parsed = parse_word(word);
    map<char, node*> *curr_nodes = &first_characters;

    for(int i = 0; i < parsed.size(); i++) {
        if(curr_nodes->find(parsed[i]) != curr_nodes->end()) {
            curr_nodes = &curr_nodes->at(parsed[i])->next_chars;
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

map<char, node*> get_word_next(string word) {
    vector<char> parsed = parse_word(word);
    map<char, node*> *curr_nodes = &first_characters;

    for(int i = 0; i < parsed.size(); i++) {
        if(curr_nodes->find(parsed[i]) != curr_nodes->end()) {
            curr_nodes = &curr_nodes->at(parsed[i])->next_chars;
        } else {
            break;
        }
    }

    return *curr_nodes;
}

// method to create all words in storage
// perform a depth first search, 
void get_all_words(node* start, vector<string> *all_words, string current) {

    if(start == NULL) {
        return;
    }
    current += start->character;
    if(start->word) {
        current += '\0';
        if(current[0] == ' ')
            (*all_words).push_back(current.erase(0, 1));
        else
            (*all_words).push_back(current);
    }

    for(int i = 0; i < start->next_chars.size(); i++) {
        get_all_words(start->next_chars[i], all_words, current);
    }
}

// method to find all words with a substring
void all_sub(node* start, vector<string> *all_words, string current, string substring) {

    if(start == NULL) {
        return;
    }
    current += start->character;
    if(start->word && (current.find(substring) != std::string::npos)) {
        current += '\0';
        if(current[0] == ' ')
            (*all_words).push_back(current.erase(0, 1));
        else
            (*all_words).push_back(current);
    }

    for(int i = 0; i < start->next_chars.size(); i++) {
        all_sub(start->next_chars[i], all_words, current, substring);
    }
}