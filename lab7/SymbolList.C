#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include "SymbolList.h"

//lots of methods are missing that you need to add 
//See SymbolList.h for the declarations

SymbolList::SymbolList()
{
    first = NULL;
}

bool SymbolList::getSymbol(std::string symbolName, char * type)
{
    symbolEntry * ptr = first;

    while(ptr != NULL)
    {
        if(ptr->name == symbolName)
        {
           *type = ptr->type;
	   return true;
        }           
        ptr = ptr->next;
    }
    return false;
}

void SymbolList::updateSymbol(std::string symbolName, char type)
{
    symbolEntry * ptr = first;

    while(ptr != NULL)
    {
        if(ptr->name == symbolName)
        {
           ptr->type = type;
           return;
        }           
        ptr = ptr->next;
    }
}

void SymbolList::insertSymbol(std::string symbolName, char type)
{
    symbolEntry * ptr;
    symbolEntry *temp = new symbolEntry;
    temp->name = symbolName;
    temp->type = type;
    temp->next = NULL;

    if(first == NULL)
    {
       first = temp;
       return;
    }
    ptr = first;
    while(ptr->next != NULL)
    {
        ptr = ptr->next;
    }
    ptr->next = temp;
}

void SymbolList::removeSymbol(std::string symbolName)
{
   symbolEntry *ptr = first;
    while(ptr != NULL)
    {
        if(ptr->next->name == symbolName)
        {
           ptr->next = ptr->next->next;
           return;
        }
        ptr = ptr->next;
    }
}

void SymbolList::startIterate()
{
    if(first == NULL)
        return;
    iterate = first;
}

std::string SymbolList::getNext(char * type)
{
    symbolEntry *ptr = first;
    std::string hold;
    if(iterate == NULL)
        return NULL;
    
    ptr = iterate;
    *type = ptr->type;
    iterate = iterate->next; 
    return ptr->name;
}

/* printSymbols
 * prints a header and then the contents of the linked list
 */
void SymbolList::printSymbols(std::string header)
{
    symbolEntry * ptr = first;
    std::cout << header << " Symbol Table\n";
    std::cout << "-----------------------\n";
    while (ptr != NULL)
    {
       
       std::cout << std::setw(32) << std::left 
                 << ptr->name << " " << ptr->type 
                 << std::endl;
       ptr = ptr->next;
    }
}

