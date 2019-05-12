// Naeim Salib
// CSCI 335
// Assigment 3

#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include "quadratic_probing.h"
#include "double_hashing.h"

using namespace std;


template <typename HashTableType>
void TestComputeAdjacentWords(HashTableType &hash_table, 
			      const vector<string> &words) {
  // This is for computing timing.
  const auto begin = chrono::high_resolution_clock::now();

  hash_table.MakeEmpty();

  //..Insert your own code using algorithm described in Figure 4.73
  
  map<string,vector<string>> adjWords;
  map<int,vector<string>> wordsByLength;

  // Group the words by their length
  for( auto & str : words )
   wordsByLength[ str.length( ) ].push_back( str );

  // Work on each group separately
  for( auto & entry : wordsByLength )
  {
    const vector<string> & groupsWords = entry.second;
    int groupNum = entry.first;
    // Work on each position in each group
    for( int i = 0; i < groupNum; ++i )
      {
       // Remove one character in specified position, computing representative.
       // Words with same representatives are adjacent; so populate a map ...
       map<string,vector<string>> repToWord;

       for( auto & str : groupsWords )
       {
       string rep = str;
       rep.erase( i, 1 );
       repToWord[ rep ].push_back( str );
       }

       // and then look for map values with more than one string
       for( auto & entry : repToWord )
       {
         const vector<string> & clique = entry.second;
         if( clique.size( ) >= 2 )
         for( int p = 0; p < clique.size( ); ++p )
         for( int q = p + 1; q < clique.size( ); ++q )
         {
         adjWords[ clique[ p ] ].push_back( clique[ q ] );
         adjWords[ clique[ q ] ].push_back( clique[ p ] );
         }
       }
      }
  }

  const auto end = chrono::high_resolution_clock::now();
  cout << "Time to build map:"<<endl;
  cout << chrono::duration_cast<chrono::nanoseconds>(end-begin).count() << "ns" << endl;
  cout << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << "ms" << endl;

  // Verify
  string inputword;

  while(true){

  cout << "Input Word:";
  cin >> inputword;

    for( map<string,vector<string> >::const_iterator ptr=adjWords.begin();
         ptr!=adjWords.end(); ptr++) {
        
        // cout << "Input Word:";
        // cin >> inputword;

        if(ptr->first==inputword){
           
           for( vector<string>::const_iterator eptr=ptr->second.begin();
             eptr!=ptr->second.end(); eptr++)
                cout << *eptr << "\n";
        }
       

    }

  }


  
}

vector<string> ReadWordsFromFile(const string &words_filename) {
  vector<string> the_words;
  // Implement this...

  fstream dataFile;
  string name;

  dataFile.open(words_filename);
  if(dataFile.fail())
  {
       cout<<"File can not open.\n";
       return the_words;
  }

  while (!dataFile.fail() && !dataFile.eof())
  {
        dataFile >> name;
        the_words.push_back(name);
  }

  dataFile.close();
  return the_words;
}

// Sample main for program CreateAndTestHash
int
main(int argc, char **argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " words_file_name hash_type (quadratic or double)" << endl;
    return 0;
  }
  
  const string words_filename(argv[1]);
  const vector<string> the_words = ReadWordsFromFile(words_filename);

  const string param_flag(argv[2]);

  if (param_flag == "quadratic") {
    HashQuadraticProbing<string> quadratic_probing_table;    
    TestComputeAdjacentWords(quadratic_probing_table, the_words);
  } else if (param_flag == "double") {
    HashTableDouble<string> double_probing_table;
    TestComputeAdjacentWords(double_probing_table, the_words);    
  } else {
    cout << "Uknown hash type " << param_flag << " (User should provide quadratic, or double)" << endl;
  }
  return 0;
}
