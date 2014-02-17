//
//  decoder.c
//  huff
//
//  Created by Adam Bradford on 2/3/14.
//  Copyright (c) 2014 Adam Bradford. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include "encoder.h"
#include "decoder.h"
#include "syscalls.h"

//turns a result array into a huff tree
void createDecodeTreeFromResultArray(huffResult *resultArray, huffNode out[511])
{
    memset(out, 0, sizeof(huffNode) * 511);
    //create a root node
    huffNode *rootNode = out;

    int nextFreeNode = 1;

    //iterate across the result array and place nodes where needed
    for(int i = 0; i < 256; i++)
    {
        huffResult *currentResult = &resultArray[i];
        char * valueString = currentResult->string;

        //simply walk down the tree adding nodes as needed until we reach the final character of the current encode string.
        huffNode *nextNode = rootNode;
        int stringLength = (int)strlen(valueString);
        for(int j = 0;  j < stringLength ;j++)
        {

            char currentBit = valueString[j];
            if (currentBit == '0')
            {
                if(nextNode->leftLeaf == NULL)
                {
                    nextNode->leftLeaf = &out[nextFreeNode];
                    nextNode++;
                }
                huffNode *parentNode = nextNode;
                nextNode = nextNode->leftLeaf;
                nextNode->parent = parentNode;
            }
            else if(currentBit == '1')
            {
                if(nextNode->rightLeaf == NULL)
                {
                    nextNode->rightLeaf = &out[nextFreeNode];
                    nextNode++;
                }
                huffNode *parentNode = nextNode;
                nextNode = nextNode->rightLeaf;
                nextNode->parent = parentNode;

            }
            nextNode->byte = i;
        }
    }
}

//returns the encodings from a givin file, returns null otherwise
void huffmanEncodingsFromFile(FILE *file, unsigned long long *decodedLength, char encodings[4096])
{
    rewind(file);
    //get the first 4 header characters
    char header[4];
    fread(header, 4, sizeof(char), file);

    //make sure it says HUFF
    if(!(header[0] == 'H' &&
       header[1] == 'U' &&
       header[2] == 'F' &&
       header[3] == 'F'))
    {
        fprintf(stderr, "Invliad File Header - missing \"HUFF\"");
        exit(-1);
    }

    //put the decoded length in the output variable
    fread(decodedLength, 1, sizeof(unsigned long long), file);

    //get the next 256 strings representing the encodings
    //create an index to keep track of where we are in copying the string
    int currentIndex = 0;
    for(int i = 0; i < 256; i ++)
    {


        //loop until we hit the /n, if we hit any other character besides 0,1 or /n then it should fail
        while(1)
        {
            char nextByte;
            xfread(&nextByte, 1, 1, file);
            if(nextByte == '0' ||
               nextByte == '1' ||
               nextByte == '\n')
            {


                //place the byte in the result
                encodings[currentIndex++] = nextByte;

                //is this string finished?
                if(nextByte == '\n')
                {
                    break;
                }
            }
            else{
                //we have found a problem, print a message and set the lenght to 0 and return null;
                fprintf(stderr,
                    "Invalid character: %c found in for encoding index: %d\n",
                    nextByte, i);
                *decodedLength = 0;
                exit(-1);
            }
        }
    }
}

//returns huffresults from given encodings
void createHuffResultArrayFromFileEncodings(
    char * encodings, huffResult resultArray[256])
{
    //holds current index from the encodings
    int stringIndex = 0;

    for(int i = 0; i < 256; i++)
    {

        //get the current result and set defaults
        huffResult * currentResult = &resultArray[i];
        currentResult->value = i;
        currentResult->string = "";

        int currentStartIndex = stringIndex;

        //find the next newline
        while (encodings[stringIndex] != '\n')
        {
            char testChar = encodings[stringIndex++];
            if(!(testChar == '0' || testChar == '1'))
            {
                fprintf(stderr, "Invalid character found in encoding: %c\n",
                    testChar);
                return;
            }
        }

        //copy the encoding into the the currentresult
        if(currentStartIndex< stringIndex)
        {
            int length = stringIndex - currentStartIndex;

            char * value = calloc(length+1,sizeof(char) * length);
            assert(value);
            memcpy(value, &encodings[currentStartIndex], length);
            currentResult->string = value;

        }
        //increment
        stringIndex++;
    }

    //just a test to make sure every value has a string.
    for(int i = 0; i < 256; i++)
    {
        huffResult * currentResult = &resultArray[i];
        assert(currentResult->string);
    }
}
