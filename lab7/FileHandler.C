#include <iostream>
#include <string>
#include "SymbolList.h"
#include "FileHandler.h"
#include <stdio.h>
#include <stdlib.h>

FileHandler::FileHandler(SymbolList * defined, SymbolList * undefined)
{
	this->defined = defined;
	this->undefined = undefined;
}

bool FileHandler::isArchive(std::string filename)
{
	if(filename.substr(filename.find_last_of(".") + 1) == "a")
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool FileHandler::isObjectFile(std::string filename)
{
	if(filename.substr(filename.find_last_of(".") + 1) == "o")
	{
		return true;
	}
	else
	{
		return false;
	}
}

void FileHandler::handleObjectFile(std::string filename)
{
	FILE *fp;
	fp = popen(("nm " + filename).c_str(), "r");
    char line[120];
	char symName[30];
	char sType;
	if (fp == NULL) {std::cout << "popen failed\n"; exit(1); }
	while (fgets(line, sizeof(line), fp))
	{
            if(line[0] == ' ')
            {
                sscanf(line, " %c %s", &sType, symName);
            }
            else
            {
		sscanf(line, "%*x %c %s", &sType, symName);
            }
		handleObjectSymbol(symName, sType);
	}
	pclose(fp);
}

void FileHandler::handleArchive(std::string filename)
{
    FILE *fp; 
    char line[120];
    bool changed = false; 
//make temp directory to unload .a (into .o's)
    if(system("mkdir tmp") == -1)
    {
        std::cout << "mkdir tmp failed\n";
        exit(1);
    }
//How does it gather individual files to process them
  system("cd tmp; ar -x tmp.a");
    fp = popen(filename.c_str(), "r");     
    if (fp != NULL)
    {
        system("ls tmp");
    }

//Go through each file and add .o file (if any symbols are resolved? U -> D)
    do{
   
            while(fgets(line, sizeof(line), fp) != NULL)
            {
                //does object file contin undefined symbol info
            }

            changed = true;
           
         }  while(changed == true);
      
            pclose(fp);
}

//handles .a files; gets the list of .o; determines whether
//a .o should be added to the undefined and defined lists (by calling objectFileNeeded)
//if an object file should be added, it calls handleObjectFile to do the work


void FileHandler::handleObjectSymbol(std::string name, char type)
{
    char t;
    int n = 0;

//it is undefined (U undefined)
    if(type == 'U')
    {
	if(defined->getSymbol(name, &t)) //in defined list
        {
            return;//do nothing, already defined
        }
        if(undefined->getSymbol(name, &t)) //in undefined list
        {
            return; //do nothing, already undefined and in list
        }
        undefined->insertSymbol(name, type); //add to undefined list
        return;
    }
//it is strong global
    if(type == 'T' || type == 'D')
    {//it is defined (T, D Strong)
        if(defined->getSymbol(name, &t))
        {
            if(t == 'C')
            {
                defined->updateSymbol(name, type);
                return;
            }
            //double strong defined

            std::cout << "multiply defined symbol\n";
            return;
        }
        else
        {
            //Strong but not defined add to defined
            defined->insertSymbol(name, type);            
            //in undefined move to defined
            if(undefined->getSymbol(name, &t))
            {
            //was previously undefined
                undefined->removeSymbol(name);
                return;
            }
        }
    }
//it is weak global
    if(type == 'C')
    {
        //not already defined
        if(!defined->getSymbol(name, &t))
        {
            defined->insertSymbol(name, type);
            return;
        }
        //in undefined list
        if(undefined->getSymbol(name, &t))
        {
            undefined->removeSymbol(name);
            return;
        }
    }
//is local symbol
    if(type == 'b' || type == 'd')
    {
        name += n;
        defined->insertSymbol(name, type);
        n++;
        return;
    }
}
