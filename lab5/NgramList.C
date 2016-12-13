
#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include "WordList.h"
#include "NgramList.h"

using namespace std;

/*
 * NgramList
 *
 * takes as input the size of the ngrams to be built and the list  
 * of words to build the ngrams from and builds a linked list of 
 * ngrams.
 *
 * param: int ngramSz - size of the ngram
 * param: const WordList & wl - list of the words use
 */
NgramList::NgramList(int ngramSz, const WordList & wl)
{
   this->ngramSz = ngramSz;
   WordList::const_iterator p;
   first = NULL;
   maxCount = 1;
   p = wl.begin();
   size = (wl.size()*.6);
   array = new Ngram_t*[size];
   for(int i=0;i < size; i++)
      array[i] = NULL;
   while (p != wl.end())
   {
      std::string ngram = getNextNgram(p, wl.end());
      p++;
      if (!ngram.empty()) insertNgram(ngram);
   }
}

/*
 * NgramList destructor
 *
 * automatically called when NgramList object goes out of scope
 * deletes the linked list
 */
NgramList::~NgramList()
{
   Ngram_t * nextNgram;
   while (first != NULL)
   {
      nextNgram = first->next;
      delete(first);
      first = nextNgram;
   }
   delete[] array;
}

/*
 * getNextNgram
 *
 * takes as input an iterator that points to the word to start the
 * ngram and an iterator that points to the end of the word list
 * and builds and returns the ngram. returns empty string if no
 * ngram can be built, for example, if there are insufficient words
 * to build one.
 *
 * param: WordList::const_iterator start - points to first word in ngram
 * param: WordList::const_iterator end - points to end of word list
 * return: std::string - returns ngram or empty string
 */
std::string NgramList::getNextNgram(WordList::const_iterator start, 
                                    WordList::const_iterator end)
{
   int i, len;
   string ngram = "";
   for (i = 0; i < ngramSz && start != end; i++)
   {
      std::string word = (*start);
      //see if the string ends with punctuation
      //don't create ngrams that continue after punctuation
      if (!isalpha(word[word.length()-1]) && i < ngramSz - 1) return "";

      //take off all ending punctuation
      len = word.length() - 1;
      while (len >= 0 && !isalpha(word[len])) 
      {
         //remove last character
         word = word.substr(0, word.length() - 1);
         len--;
      }
      if (len < 0) return "";  //give up

      //is the first word in the ngram?
      if (ngram == "") ngram = word;
      else ngram = ngram + " " + word;

      start++;
   }

   //could we build a long enough ngram?
   if (i < ngramSz) return "";

   //take off beginning punctuation
   while (ngram.length() > 0 && !isalpha(ngram[0])) 
      ngram = ngram.substr(1, ngram.length());
   return ngram;
}

/*
 * insertNgram
 *
 * looks for the ngram to be inserted. If it is already in
 * the linked list,  it increments the count. If not, it 
 * inserts it into the linked list.
 *
 * param: std::string s - ngram to be inserted
 * return: none
 */
void NgramList::insertNgram(std::string s)
{
   int hashValue = 0;
   Ngram_t * tNode = NULL;

//generates hashValue for index into array/map
   std::hash<std::string> str_hash;
   hashValue = str_hash(s) % size;

//If array is not empty at hashValue index
   if(array[hashValue] != NULL)
   {
      tNode = array[hashValue];
      while (tNode != NULL)
      {
//If current node is same string increment count of node
         if(tNode->ngram == s)
         {
            tNode->count++;
            if(tNode->count > maxCount)
               maxCount = tNode->count;
            return;
         }
//If current node is different from s
         else
         {
//If this is not the last node in linked list
            if(tNode->next != NULL)
            {
               tNode = tNode->next;
            }
//If last one is linked list adds to end of linked list
            else
            {
               Ngram_t * newNode = new Ngram_t();
               newNode->ngram = s;
               newNode->count = 1;
               tNode->next = newNode;
               return;
            }
         }
       }
   }
//Array is empty there so add to array
   else
   {
      Ngram_t * newNode = new Ngram_t();
      newNode->ngram = s;
      newNode->count = 1;
      array[hashValue] = newNode;
   }
}


/*
 * sortByCount
 *
 * performs a bubble sort on the linked list of ngrams, sorting the
 * nodes in the list by the count
 *
 * param: none
 * return: none (modfied private linked list)
 */
void NgramList::sortByCount()
{
   Ngram_t** tArray = new Ngram_t*[maxCount];
   for(int i=0;i < maxCount; i++)
      tArray[i] = NULL;
   Ngram_t * last = NULL;
   Ngram_t * index = NULL;   

//Array of nodes with counts in hash order
//iterate through list
   for(int i = 0; i < size; i++)
   {
//No linked list at location skip to next index
      if(array[i] == NULL)
         ;
//linked list at index location
      else
      {
         last = array[i];
//go through every item in array
         while(last != NULL)
         {
//if tArray[count] null then set array[count] = new node
            index = last->next;
//Creates memory leak, this is intentional as we want to "lose" the linked list
            last->next = tArray[last->count];
            tArray[last->count] = last;
//Last item in array/linked list
            last = index;
         }
      } 
   }
//tArray now array of sorted nodes

//tArray needs to be changed to link list

index = NULL;
last = NULL;
first = NULL;

   for(int i= maxCount; i >= 0; i--)
   {
//Creates memory leak that is the result of deliberatly setting equal to NULL pointer
      last = tArray[i];
      if(last == NULL)
      {
         ;
      }
      else
      {
         if(index != NULL)
            index->next = last;
         if(first == NULL)
         {
            first = last;
         }
         while(last->next != NULL)
         {
            last = last->next;
         }
         index = last;
      }
   }
   delete[] tArray;
}

/*
 * operator<<
 *
 * prints the list of ngrams
 *
 * param: std::ostream & os - output stream to direct the output to
 * param: const NgramList & nglst - ngram list object
 * return: std::ostream & - output stream
 */  
std::ostream& operator<<(std::ostream& os, const NgramList & nglst)
{
   cout << "List of " << nglst.ngramSz << " word ngrams and counts\n";
   cout << "--------------------------------\n";
   NgramList::Ngram_t * ptr = nglst.first;
   while (ptr != NULL)
   {
      cout << ptr->ngram << ", " << ptr->count << endl;
      ptr = ptr->next;
   } 
   return os;
}
