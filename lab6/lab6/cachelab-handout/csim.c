#include "cachelab.h"
#include <ctype.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <getopt.h>
#include <math.h>
#include <string.h>

//Jake Campos
//Jonathan Ward


typedef unsigned long tag;
int verbose = -1;


//cache structure for defining cache "type"
typedef struct {
    int numberOfSets;
    int setIndexBits;
    int numberOfLines; //associativity
    int numberBlockBits;
    int blockSize;
    tag** tags;
} cache;

//define functions here (no need for .h)
void cacheSetup(cache* cache, int sValue, int EValue, int bValue);
void parse(cache* cache, FILE* fp, int* h, int* m, int* e);
unsigned long getBits(int high, int low, unsigned long source);


int main(int argc, char * argv[]) {

        int hitCount = 0;
        int missCount = 0;
        int evictionCount = 0;
        int hflag = -1;
        int sValue = -1;
        int EValue = -1;
        int bValue = -1;
        char * tValue = malloc(80);
        int c;
        cache cache;

//grabs values from arguments when run and uses to call cacheSetup       
    while ((c = getopt(argc, argv, "hvs:E:b:t:")) != -1)
            switch (c) {
            case 'h':
                hflag = 1;
                break;
            case 'v':
                verbose = 1;
                break;
            case 's':
                sValue = atoi(optarg);
                break;
            case 'E':
                EValue = atoi(optarg);
                break;
            case 'b':
                bValue = atoi(optarg);
                break;
            case 't':
                tValue = optarg;
                break;
            default:
                abort();
            }
        cacheSetup(&cache, sValue, EValue, bValue);

        //For Testing Paramter Parsing.
        if(verbose == 1)
        {
         printf("hflag = %d\nvflag = %d\nsValue = %d\nEValue = %d\nbValue = %d\ntValue = %s\n",
         hflag, verbose, sValue, EValue, bValue, tValue);
        }
     

//open & check the file
        FILE * fp = fopen(tValue, "r");
        if (fp == NULL) {
            perror("Cannot Open File.");
            return (-1);
        }
        else {
            parse(&cache, fp, &hitCount, &missCount, &evictionCount);
        }
        printSummary(hitCount, missCount, evictionCount);
        fclose(fp);
        return (0);
    }   
    
//setup cache values for type of cache
void cacheSetup(cache* cache, int sValue, int EValue, int bValue)
{    
    cache->numberOfSets = pow(2,sValue);
    cache->setIndexBits = sValue;
    cache->numberOfLines = EValue;
    cache->blockSize = pow(2,bValue);
    cache->numberBlockBits = bValue;
    cache->tags = malloc(cache->numberOfSets * sizeof(tag*));
    for(int i = 0; i < cache->numberOfSets; i++)
    {
        cache->tags[i] = malloc(cache->numberOfLines * sizeof(tag));
        memset(cache->tags[i], -1, cache->numberOfLines * sizeof(tag));
    }
}

//read and parse lines from file
void parse(cache* cache, FILE* fp, int* h, int* m, int* e)
{
	char line[80];

//while the file isnt empty/over
	while(fgets(line, 80, fp))
	{
		if(line[0] != 'I')
		{
			unsigned long tSet, tTag, add;
			int tagExist = 0;
			tag* cSet;
			tag current, last;
			int setLoc, size;
			char op;
			
			if(verbose == 1)
			{
				puts(line);
			}
			
//grab a line and split it into pieces
			sscanf(line, " %c %lx, %d", &op, &add, &size);
//get values (bits) stolen from last semesters lab
			tSet = getBits(cache->numberBlockBits + cache->setIndexBits -1, cache->numberBlockBits, add);
			tTag = getBits(63, cache->numberBlockBits + cache->setIndexBits, add);
			cSet = cache->tags[tSet];
			
//searches cache for matching tag
			for(setLoc = 0; setLoc < cache->numberOfLines; setLoc++)
			{
				if(cSet[setLoc] == tTag)
				{
					tagExist = 1;
					if(verbose == 1)
					{
						puts("hit");
					}
					(*h)++;
					break;
				}
			}
//found the tag
			if(tagExist == 1)
			{
				current = cSet[setLoc];
				last = cSet[0];
				for(int i = 1; i <= setLoc; i++)
				{
					tag temp = last;
					last = cSet[i];
					cSet[i] = temp;
				}
				cSet[0] = current;
			}
//tag not yet in cache
			else
			{
				(*m)++;
				if(verbose == 1)
				{
					puts("miss");
				}
//if there is no room then evict
				if(cSet[cache->numberOfLines - 1] != -1)
				{
					(*e)++;
					if(verbose == 1)
					{
						puts("evict");
					}
				}
				last = cSet[0];
//put in cache
				for(int i = 1; i < cache->numberOfLines; i++)
				{
					tag temp = last;
					last = cSet[i];
					cSet[i] = temp;
				}
				cSet[0] = tTag;
			}
//if modify then pretend like we modified memory
			if(op == 'M')
			{
				if(verbose == 1)
				{
					puts("hit");
				}
				(*h)++;
			}
		}
		else
		{
			if(verbose == 1)
			{
				puts(line);
			}
		}	
	}
}

//stolen from old lab to get bits
unsigned long getBits(int high, int low, unsigned long source) {
    if (high > 63 || high < 0 || low < 0 || low > 63) {
        printf("Invalid.");
    }
    return (source << (63 - high)) >> (63 - high + low);
}
