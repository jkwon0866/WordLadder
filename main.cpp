#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <stack>
#include <queue>
#include <vector>

using namespace std;

//goes through the dictionary txt file and puts the words into hash table; hash function is the values of first letter of word corresponding to 0-25
void initialize_dictionary(const string &dictionaryFile, list<string>* dict_table){
	//put the dictionary text file through an ifstream
	ifstream inputStream;
	inputStream.open(dictionaryFile);
	if(!inputStream.is_open()){
		cout << "dictionary file could not be opened." << endl;
		return;
	}
	string word;
	int index = 0;
	//go through each word, and determine whether it is a 5-letter word or not
	while(!inputStream.eof()){
		inputStream >> word;
		//if it isn't a 5-letter word, then exit function
		if(word.size() != 5){
			cout << "this dictionary file contains a non 5-letter word." << endl;
			return;
		}
		//if it is, then apply hash function, and put into the correct index of hash table
		else{
			index = word.at(0) - 'a';
			dict_table[index].push_back(word);
		}
	}
	inputStream.close();
}

//--------------------------------HELPER FUNCTION FOR find_word_ladder--------------------------------//

//checks if the two words passed in are off by just one letter (i.e. happy, sappy)
bool is_one_off(const string &word1, const string &word2)
{
	if(word1.size() != 5 || word2.size() != 5){
  		return false;
  	}
  	//counts the number of corresponding letters that don't match
  	int count = 0;
  	for(int i = 0; i < 5; i++){
    	if(word1.at(i) != word2.at(i)){
    		count++;
    	}
  	}
  	if(count == 1){
    	return true;
  	}
  	else{
    	return false;
  	}
}

//-------------------------------------------------------------------------------------------------------//

//looks for a word ladder using bidirectional bfs search
void find_word_ladder(const string &startWord, const string &endWord, list<string>* dict_table){
	//if the start word is the same as the end word, then you already found the word ladder! Just print the word and exit function
	if(startWord == endWord){
		cout << "Word Ladder Found." << endl;
		cout << startWord;
		return;
	}

	//use your hash function to determine index of start word and end word
	int startIndex = startWord.at(0) - 'a';
	bool startExists = false;

	int endIndex = endWord.at(0) - 'a';
	bool endExists = false;

	//go to the index in hash table where start word should be located, and traverse through the list there, confirming whether the start word exists
	list<string>::iterator it;
	for(it = dict_table[startIndex].begin(); it != dict_table[startIndex].end(); it++){
		if(*it == startWord){
			startExists = true;
			break;
		}
	}
	//go to the index in hash table where end word should be located, and traverse through the list there, confirming whether the end word exists
	for(it = dict_table[endIndex].begin(); it != dict_table[endIndex].end(); it++){
		if(*it == endWord){
			endExists = true;
			break;
		}
	}
	//if either the start word or end word doesn't exist, then just exit the function
	if(!startExists || !endExists){
		cout << "either start or end word doesn't exist" << endl;
		return;
	}


	//create two word ladders from the start word and the end word
	queue< stack<string> > wordLadders_begin;
	queue< stack<string> > wordLadders_end;

	//enqueue a ladder with the start word to wordLadders_begin
	stack<string> frontLadder_begin;
	frontLadder_begin.push(startWord);
	wordLadders_begin.push(frontLadder_begin);

	//enqueue a ladder with the end word to wordLadders_end
	stack<string> frontLadder_end;
	frontLadder_end.push(endWord);
	wordLadders_end.push(frontLadder_end);

	//frontierWords are vectors that represent the words that have been checked going from either the start word or the end word.
	vector< stack<string> > frontierWords_begin;
	frontierWords_begin.push_back(frontLadder_begin);
	vector< stack<string> > frontierWords_end;
	frontierWords_end.push_back(frontLadder_end);

	//a start_table and end_table are created from the original dictionary table to be used by either the word ladder from the start word side or end word side.
	//this is to prevent overlaps in erasing a word from a table, and allow intersects to be properly checked for
	list<string> start_table[26];
	list<string> end_table[26];
	for(int i = 0; i < 26; i++){
		list<string>::iterator it;
		for(it = dict_table[i].begin(); it != dict_table[i].end(); it++){
			start_table[i].push_back(*it);
			end_table[i].push_back(*it);
		}
	}
	start_table[startIndex].remove(startWord);
	end_table[endIndex].remove(endWord);



	//while both word ladders are not empty
	while(!wordLadders_begin.empty() && !wordLadders_end.empty()){
		//get the top of the front ladder from both queues
		string topWord_begin = wordLadders_begin.front().top();
		string topWord_end = wordLadders_end.front().top();


		//Go through each word in the hash table and find all words that are off by one with the top word of the front ladder on the start word side.
		//when an off-by-one-letter word is found, then check if it is equal to the end word or not. 
		for(int i = 0; i < 26; i++){
			list<string>::iterator it;
			for(it = start_table[i].begin(); it != start_table[i].end(); it++){
				if(is_one_off(*it, topWord_begin)){

					//addedWordLadder_begin is a copy of the front ladder with the off-by-one word pushed onto it
					//it will either be added to the queue of word ladders or printed as the end result word ladder
					stack<string> addedWordLadder_begin = wordLadders_begin.front();
			        addedWordLadder_begin.push(*it);

			        //add the new off-by-one word found onto the frontierWords vector
			        frontierWords_begin.push_back(addedWordLadder_begin);

			        //if this off-by-one-letter word is the end word of the ladder, this new Stack contains the entire word ladder. It will print the word ladder found. You are done!
			        for(int j = 0; j < frontierWords_end.size(); j++){
			        	if(*it == frontierWords_end.at(j).top()){
			        		cout << "You've found the word ladder!" << endl;
			        		frontierWords_end.at(j).pop();
				          	while(!frontierWords_end.at(j).empty()){
				            	addedWordLadder_begin.push(frontierWords_end.at(j).top());
				            	frontierWords_end.at(j).pop();
				          	}
				          	stack<string> resultLadder;
				          	while(!addedWordLadder_begin.empty()){
				          		resultLadder.push(addedWordLadder_begin.top());
				          		addedWordLadder_begin.pop();
				          	}
				          	while(!resultLadder.empty()){
				            	cout << resultLadder.top() << endl;
				            	resultLadder.pop();
				          	}
				          	return;
				        }
			        }
			    	//otherwise, enqueue this new ladder and remove this word from the dictionary
				    wordLadders_begin.push(addedWordLadder_begin);
				    start_table[i].erase(it);
			        it--;				}
			}
		}
		//after checking every off-by-one word for the front of the queue, you are done with it. Dequeue it out.
		wordLadders_begin.pop();

		//NOTE: This next for loop mirrors what the earlier for loop does. It repeats the process on the end word side!
		//Go through each word in the hash table and find all words that are off by one with the top word of the front ladder on the end word side.
		//when an off-by-one-letter word is found, then check if it is equal to the start word or not. 
		for(int i = 0; i < 26; i++){
			list<string>::iterator it;
			for(it = end_table[i].begin(); it != end_table[i].end(); it++){
				if(is_one_off(*it, topWord_end)){

					//addedWordLadder_end is a copy of the front ladder with the off-by-letter word added to it
					//it will either be added to the queue of word ladders or printed as the end result word ladder
					stack<string> addedWordLadder_end = wordLadders_end.front();
			        addedWordLadder_end.push(*it);

			        frontierWords_end.push_back(addedWordLadder_end);

			        //if this off-by-one-letter word is the end word of the ladder, this new Stack contains the entire word ladder. You are done!
			        for(int j = 0; j < frontierWords_begin.size(); j++){
			        	if(*it == frontierWords_begin.at(j).top()){
			        		cout << "You've found the word ladder!" << endl;
			        		frontierWords_begin.at(j).pop();
				          	while(!frontierWords_begin.at(j).empty()){
				            	addedWordLadder_end.push(frontierWords_begin.at(j).top());
				            	frontierWords_begin.at(j).pop();
				          	}
				          	stack<string> resultLadder = addedWordLadder_end;
				          	while(!resultLadder.empty()){
				          		cout << resultLadder.top() << endl;
				          		resultLadder.pop();
				          	}
				          	return;
				        }
			        }
			    	//otherwise, enqueue this new ladder and remove this word from the dictionary
				    wordLadders_end.push(addedWordLadder_end);
				    end_table[i].erase(it);
			        it--;
				}
			}
		}
		//after checking every off-by-one word for the front of the queue, you are done with it. Dequeue it out.
		wordLadders_end.pop();
	}

	//if the outside of the while loop is reached, that means one of the word ladders finished looking through the dictionary table and couldn't find a path to the other word.
	//Therefore, "no word ladder found"
	cout << "no word ladder found." << endl;
	return;
}
 
int main(){
	//Get the name of dictionary text file
	string inputFile = "";
	cout << "Enter the dictionary file." << endl;
	cin >> inputFile;

	//populate the dictionary hash table
	list<string> dict_table[26];
	initialize_dictionary(inputFile, dict_table);

	string startWord = "";
	string endWord = "";

	//Get the starting word and end word
	cout << "Provide the start word: ";
	cin >> startWord;
	cout << "Provide the end word: ";
	cin >> endWord;

	//find the word ladder (the path from start word to end word) using bidirectional bfs
	find_word_ladder(startWord, endWord, dict_table);



	return 0;
}