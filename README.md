# Word Ladder

## About:

Word ladder is a word game invented by Lewis Carroll. A word ladder puzzle begins with two words, and to solve the puzzle one must find a chain of other words to link the two, in which two adjacent words (that is, words in successive steps) differ by one letter.

This program takes a dictionary of 5-letter words, and given a start word and end word, constructs a word ladder from the dictionary for you.

## Implementation:

I used a bidirectional BFS search, which searches for a word ladder using the BFS approach from both the start word and end word side. [Here](https://www.youtube.com/watch?v=4i-2X73t3U0) is a video that gives a good visualization on what this looks like when finding a path on a street map. 

## How to Start:
1. Compile and run the main.cpp file from your directory:

```
g++ main.cpp
./a.out
```

2. Provide the name of the dictionary file, containing your words. Then, provide the starting word and end word of your word ladder when prompted.

3. If a word ladder could be generated, then it will print it out. Else, it will print out "no word ladder found."

## Trying it out:

I have provided a dictionary file in the repository, which I have  taken from [here](http://www.andrew.cmu.edu/course/15-121/dictionary.txt) and filtered out the non-5-lettered words.

Here are some examples of start and end words that have valid word ladders:

* steak steak
* bland blond
* hello motto
* chord piece
* songs poems