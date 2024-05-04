#include "console.hpp"
#include "json.hpp" // Make sure this points to the correct path of json.hpp
#include "rang.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
using namespace std;

using json = nlohmann::json;
using namespace std;
using namespace rang;

consoleSize console_size;

vector<string> partialMatch(json array, string substring) {
  vector<string> matches; // to hold any matches
  //size_t found;           // size_t is an integer position of
                          // found item. -1 if its not found.

  if (substring.size() == 0) {
    return matches;
  }
  for (auto &element : array.items()) {
    string key = element.key();

    if (key.find(substring) != string::npos) {

      for (int i = 0; i < substring.length(); i++) { // loop through array
        if (substring[i] == key[i] && i == substring.length() -1){
            matches.push_back(key);
          }
        else if (substring[i] == key[i] && i != substring.length() -1){
          continue;
        }
        else{
          break;
        }

      
      }
      
    }
  }
  return matches;
}

void titleBar(string title, int length = console_size.width) {
  string padding = string((length / 2) - (title.size() / 2), ' ');
  title = padding + title + padding;
  cout << bgB::black << fg::gray << title << fg::reset << bg::reset << endl;
}

void horizontalBar(int length = console_size.width) {
  string line = string(length, '_');
  cout << fg::gray << line << fg::reset << bg::reset << endl;
}

void printMenu(vector<string> options) {
  int i = 1;
  for (auto s : options) {
    cout << fg::black << style::bold << i << ". " << style::reset << fg::cyan
         << s << fg::reset << bg::reset << endl;
    i++;
  }
  cout << fg::reset << bg::reset;
}

void printHighlightedSubstr(string word, string substr, int loc) {
  for (int j = 0; j < word.size(); j++) {
    // if we are printing the substring turn it red
    if (j >= loc && j <= loc + substr.size() - 1) {
      cout << fg::red << style::underline << word[j] << fg::blue
           << style::reset;
    } else {
      cout << fg::blue << word[j] << fg::reset << style::reset;
    }
  }
}

void printCurrent(char k, string word) {
  cout << fg::green << style::bold << "KeyPressed: \t\t" << style::reset
       << fgB::yellow;
  if (int(k) == 127) {
    cout << "del";
  } else {
    cout << k;
  }
  cout << " = " << (int)k << fg::reset << endl;
  cout << fg::green << style::bold << "Current Substr: \t" << fg::reset
       << fgB::blue << word << fg::reset << style::reset << endl;
  cout << endl;
}

void errorMessage(string message) {
  cout << bgB::red << fgB::gray << message << fg::reset << bg::reset << endl;
  sleep(1);
}

json loadJsonFile(string filePath) {

  // Timer T;   // create a timer
  // T.Start(); // start it

  // Load your JSON object as shown in previous examples
  ifstream fileStream(filePath);
  string partialKey = " ";
  json myJson;
  if (fileStream.is_open()) {
    fileStream >> myJson;
    fileStream.close();
  } else {
    cerr << "Failed to open file: " << filePath << endl;
    return 1;
  }

  // Iterate over all key-value pairs
  for (auto &element : myJson.items()) {
    string key = element.key();

    // Check if the key contains the partialKey substring
    if (key.find(partialKey) != string::npos) {
      // Found a match, do something with it
      cout << "Found partial match: " << key << " -> " << element.value()
           << endl;
    }
  }

  return myJson;
}

int main() {
  console_size = getConsoleSize(); // get width and height of console
  json dictionary = loadJsonFile("./dictionary_2mb.json");
  char k;             // holder for character being typed
  string key;         // string version of char for printing
  string substr = ""; // var to concatenate letters to
  // vector<string> animals = loadAnimalsFast(); // array of animal names
  vector<string> matches; // any matches found in vector of animals
  int loc;                // location of substring to change its color
  bool deleting = false;
  string str = "";
  vector<string> mainMenu;
  mainMenu.push_back("Type letters and watch the results change.");
  mainMenu.push_back(
      "Hit the DEL key to erase a letter from your search string.");
  mainMenu.push_back(
      "When a single word is found, hit enter. (this is for real assignment.");

  clearConsole();
  titleBar("Getch Example", console_size.width);
  printMenu(mainMenu);

  // While capital Z is not typed keep looping

  while ((k = getch()) != 'Z') {

    clearConsole();
    titleBar("Getch Example", console_size.width);
    printMenu(mainMenu);

    cout << (int)k << endl;
    if ((int) k ==10){
      break;
    }
    // Tests for a backspace and if pressed deletes
    // last letter from "substr".
    if ((int)k == 127) {
      if (substr.size() > 0) {
        substr = substr.substr(0, substr.size() - 1);
        deleting = true;
      }
    } else {
      deleting = false;
      // Make sure a letter was pressed and only letter
      if (!isalpha(k)) {
        errorMessage("Letters only!");
        continue;
      }

      // We know its a letter, lets make sure its lowercase.
      // Any letter with ascii value < 97 is capital so we
      // lower it.
      if ((int)k < 97) {
        k += 32;
      }
      substr += k; // append char to substr
    }
    horizontalBar();
    printCurrent(k, substr);

    // Find any animals in the array that partially match
    // our substr word
    matches = partialMatch(dictionary, substr);

    if ((int)k != 32) { // if k is not a space print it

      key = to_string(k);

      horizontalBar();

      cout << style::bold << style::underline << fg::black
           << "MATCHES: " << fg::green << matches.size() << style::reset
           << fg::reset << endl
           << endl;

      for (auto &c : substr)
        c = tolower(c);
      // This prints out all found matches
      for (int i = 0; i < matches.size(); i++) {
        // find the substring in the substr
        loc = matches[i].find(substr);
        // if its found
        if (loc != string::npos) {
          printHighlightedSubstr(matches[i], substr, loc);
        }
        cout << " ";
      }
      cout << fg::reset << endl << endl << endl << endl;
      if (matches.size() == 1) {
        cout << "done?" << endl;
        // this isn't handled at all, just messin around
      }
    }
  }
  return 0;
}

// prints out some json file.
void printJson(json d) {
  for (auto &element : d.items()) {
    string key = element.key();
    string val = element.value();

    cout << "key: " << key << " :: val: " << val << endl;
  }
}

void printMatches(json d, string sub) {
  int count = 0;
  vector<string> matches; // to hold any matches
  bool found = true;

  for (auto &element : d.items()) {
    string key = element.key();
    string val = element.value();
    // Check if the key contains the partialKey substring
    if (key.find(sub) != string::npos) {
      count++;
      matches.push_back(key);

    }

    else {
      found = false;
    }
  }

  if (found == true) {
    cout << count << " words found" << endl << endl;

    for (int i = 0; i < matches.size(); i++) {
      cout << matches[i] << " " << endl;
    }
  } else if (found == false)
    cout << "No matches found" << endl;
}
