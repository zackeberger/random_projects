//
//  main.cpp
//  Boyer-Moore
//
//  Created by Zack Berger on 3/2/19.
//  Copyright © 2019 Berger Inc. All rights reserved.
//

#include <string>
#include <cassert>
#include <iostream>
using namespace std;

// Assume that characters are 1 byte and
// can be converted to integers
const int POSSIBLE_CHARACTERS = 256;

/*
 PROGRAM DESCRIPTION:
 - This program permits a user to search for a pattern within a text string
 - The search methods provided return the index (0 based) of the first occurence of the pattern
 in the text, or -1 if the pattern is not present.
 - The program is designed with the Boyer-Moore string searching algorithm
 
 BOYER-MOORE BACKGROUND:
 - String searching is typically done by checking each aligning a pattern and text can have,
 and seeing if that alignment shows that the pattern and text match
 - Boyer-Moore algorithm is an improved technique for string searching. It uses two rules, the
 "bad character rule" and the "good suffix rule" to skip alignments. For each alignment, each rule is checked
 and whatever rule yields the largest number of skips is used.
 - Both rules require preproceessing of the pattern
 - The naive approach has time complexity O(nm) where n is the length of the text and m the length of the pattern.
   The Boyer Moore algorithm has a markedly lower average time complexity due to the skipping of alignments
 - This program currently only implements the "bad character rule," although the interface for the
 "good suffix rule" is provided
 */

class StringFinder
{
public:
    StringFinder(string text, string pattern);
    // Construct a StringFinder object with a text document
    // and a pattern to be searched for within the text.
    
    int search();
    // User can search for a pattern stored in a StringFinder object
    // exists within the text member. Returns the index at which
    // the first instance of the pattern occurs.
    // Repeated calls to search() results in the same return value.
    
    int search(string newPattern);
    // User can specify a new pattern to be searched for within
    // the text member.
    
private:
    void badCharacterPreprocessing();
    // Helper function that performs preprocessing actions
    // on the pattern to build the bad character table.
    
    void goodSuffixPreprocessing();
    // Helper function that performs the preprocessing actions
    // on the pattern to build the good suffix table.
    
    int max(int a, int b);
    // Returns the greater of the two inputs. Used to determine
    // whether the bad character rule, or good suffix rule
    // permits the greatest number of alignment-skips.
    
    string  m_text;
    string  m_pattern;
    
    // Each character has
    //  - value of -1 if not in pattern, or
    //  - the position of the last occurence of it in the pattern
    int     m_lastOccurrences[256];
    
    // TODO: Implement member to handle good suffix rule
};

StringFinder::StringFinder(string text, string pattern)
: m_text(text), m_pattern(pattern)
{
    badCharacterPreprocessing();
    goodSuffixPreprocessing();
}

int StringFinder::search()
{
    // Store the size of the text and pattern
    int textSize = m_text.size();
    int patternSize = m_pattern.size();
    
    if (patternSize == 0)
        return -1;
    
    // Skip indicates current position in text
    int skip = 0;
    
    // If skip is more than this number of possible alignments, the pattern
    // will have moved past the end of the text
    int maxSkip = textSize - patternSize;
    
    while (skip <= maxSkip)
    {
        // p will be used to traverse the pattern backwards
        int p = patternSize - 1;
        
        // Keep traversing the pattern until we have either
        //      - exhausted the pattern
        //      - found a bad character
        while ( p >= 0 )
        {
            if (m_text[skip + p] != m_pattern[p])       // we found a bad character
                break;
            p--;
        }
        
        // Pattern completely traversed, so skip marks where
        // pattern exists in the text
        if (p < 0)
        {
            return skip;
        }
        
        // Bad character hit, so update skip to the next possible alignment
        //      - If bad character is not present in pattern,
        //        update skip so it marks the index 1 past the bad character
        //      - If last occurence of bad character is on left of current character,
        //        align that character in pattern with the bad character in text
        //        by updating skip appropriately
        //      - Else, last occurence of bad character is on right of current character,
        //        update yields a nonsensical negative answer, so simply update skip by 1
        
        int update = p - m_lastOccurrences[m_text[skip + p]];      // m_text[skip + p] gives the bad character
        if (update >= 1)
            skip += update;
        else
            skip++;
    }
    
    return -1;  // Pattern not found in text
}

int StringFinder::search(string newPattern)
{
    m_pattern = newPattern;
    
    badCharacterPreprocessing();
    goodSuffixPreprocessing();
    
    return search();
}


//////////////////////////////
// *** HELPER FUNCTIONS *** //
//////////////////////////////

void StringFinder::badCharacterPreprocessing()
{
    // Fill up the bad character array with -1
    for (int i = 0; i < POSSIBLE_CHARACTERS; i++)
        m_lastOccurrences[i] = -1;
    
    // Indicate the last occurrence of each character
    // within the pattern
    for (int i = 0; i < m_pattern.size(); i++)
        m_lastOccurrences[ m_pattern[i] ] = i;
}

void StringFinder::goodSuffixPreprocessing()
{
    // TODO: Implement good suffix preprocessing method
}

// If a is greater than or equal to b, return a
// Otherwise, return b
int StringFinder::max(int a, int b)
{
    return (a >= b) ? a : b;
}



//////////////////////////////////////////////////////
///// *** MAIN METHOD FOR TESTING OF PROGRAM *** /////
//////////////////////////////////////////////////////

int main()
{
    StringFinder s("", "");
    assert(s.search() == -1);
    assert(s.search("hello") == -1);
    
    StringFinder s1("1", "");
    assert(s1.search() == -1);
    assert(s1.search("1") == 0);
    assert(s1.search("Not here") == -1);
    
    StringFinder s2("Zack Berger is a student at University of California", "Zack");
    assert(s2.search() == 0);
    assert(s2.search("k Berger") == 3);
    assert(s2.search("is a stud") == 12);
    assert(s2.search("student at") == 17);
    assert(s2.search("ia") == 50);
    assert(s2.search("???") == -1);
    assert(s2.search("Student") == -1);
    
    StringFinder s3("mv0t9q3mytx1789mychqp3u,x9349u0qtx4u3hhqmq8qt h80t h h0h   0t qh7 0ht00 aaaa", "aaaa");
    assert(s3.search() == 72);
    assert(s3.search("mv") == 0);
    assert(s3.search(",x9349") == 23);
    
    cout << "All tests passed!" << endl;
}
