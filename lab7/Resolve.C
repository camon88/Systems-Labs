#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include "SymbolList.h"
#include "FileHandler.h"
#include "Resolve.h"

//you need to implement printSymbolsAtEnd and fileExists.
//The stubs for these are below.

/* Resolve
 * Driver for the resolution process.
 * Uses a FileHandler object to handle each .a and .o
 * Maintains an undefined and defined list for symbols
 */
Resolve::Resolve(int argc, char * argv[])
{
   undefined = new SymbolList();
   defined = new SymbolList();
   handler = new FileHandler(defined, undefined);
   for (int i = 1; i < argc; i++)
   {
      
       if (!fileExists(argv[i]) || 
          (!handler->isArchive(argv[i]) && !handler->isObjectFile(argv[i])))
       {
          std::cout << "invalid file: "<< argv[i] << std::endl;
          exit(1);
       }
       if (handler->isArchive(argv[i])) 
           handler->handleArchive(argv[i]);

       if (handler->isObjectFile(argv[i])) 
           handler->handleObjectFile(argv[i]);
   }
   printSymbolsAtEnd();
 
}
/*
 * printSymbolsAtEnd
 * Checks to see if main is defined and has type T
 * Prints the contents of the undefined list
 * Prints the contents of the defined list
 */
void Resolve::printSymbolsAtEnd()
{
   char sType;
   //add code to see if main is undefined (it should be in the
   //defined list with type 'T')
   if(defined->getSymbol("main", &sType) && sType == 'T')
   {
       ;//do nothing
   }
   else
   {
       std::cout << "Main not defined"<< std::endl;
   }
      
   //add code to iterate through undefined list and print out
   //those symbol names
   undefined->printSymbols("Undefined");

   //error messages should be in form:
   // : undefined reference to <symbol>
   
   //print the contents of the defined list
   defined->printSymbols("Defined");
}

/* fileExists
 * returns true if filename exists 
 */
bool Resolve::fileExists(std::string filename)
{
    std::ifstream infile(filename);
    return infile.good();
}

