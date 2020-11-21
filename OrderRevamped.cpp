//Scott Buck
//Updated Order code and converted into C++

#include <string>
#include <stdlib.h>
#include <vector>
#include <iostream>

//length of the alphabet
#define ALPHABETLENGTH 26

// custom function to compare strings with any given alphabet order
int customStringCompare(std::string string1, std::string string2, std::string newAlphOrder)
{
    int length1 = string1.length();
    int length2 = string2.length();
    int smallestLength;
    int counter = 0;
    //determine smallest length, dont want to look past the last letter on the smallest string
    if (length1 < length2)
    {
        smallestLength = length1;
    }
    else
    {
        smallestLength = length2;
    }
    //go until you reach the last letter of the smallest string
    for (int j = 0; j < smallestLength; j++)
    {
        //compare each letter of both strings to each letter in the alphabet to determine proper placement
        for (int i = 0; i < ALPHABETLENGTH; i++)
        {
            //if letter of first word occurs in alphabet before letter of second word
            if ((string1.at(j) == newAlphOrder.at(i)) && (string2.at(j) != newAlphOrder.at(i)))
            {
                counter = 1;
                break;
            }
            //if letter of first word occurs in alphabet after letter of second word
            else if ((string2.at(j) == newAlphOrder.at(i)) && (string1.at(j) != newAlphOrder.at(i)))
            {
                counter = 2;
                break;
            }
            //if the letters are the same
            else if ((string2.at(j) == newAlphOrder.at(i)) && (string1.at(j) == newAlphOrder.at(i)))
            {
                counter = 0;
                break;
            }
        }
        if (counter == 0)
        {
            if (j >= smallestLength)
            {
                return 0;
            }
            continue;
        }
        else if (counter == 1)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
}
//part of the merge sort used to sort the names given at a much better big-oh constant
//something that I had to deal with was remembering to pass 'names' by reference.
//I was having an issue where i was passing names but names wasn't a pointer
//this would alter names in the code, but the original names would stay unaltered
//once converting it to a pointer it worked perfectly fine because it started altering the memory location
void merge(std::vector<std::string> * names, int l, int m, int r, std::string newAlphOrder)
{
    int i, j, k;
    int compareResult;
    int n1 = m - l + 1;
    int n2 = r - m;
    //instead of char ** arrays we just use a vector of strings and allocate the memory needed so we don't access out of bounds
    std::vector<std::string> leftNames;
    leftNames.resize(n1);
    std::vector<std::string> rightNames;
    rightNames.resize(n2);
    //fill the arrays
    for (i = 0; i < n1; i++)
    {
        leftNames.at(i) = names->at(l + i);
    }
    for (j = 0; j < n2; j++)
    {
        rightNames.at(j) = names->at(m + 1 + j);
    }
    i = 0;
    j = 0;
    k = l;
    //changed it from previous code so that the compare result is cached and the lengthy custom string compare
    //isn't being called multiple times
    //also rest of general merge sort code, changing the words in a vector, much easier than changing words in a char **
    while (i < n1 && j < n2)
    {
        compareResult = customStringCompare(leftNames.at(i), rightNames.at(j), newAlphOrder);
        if (compareResult == 1 || compareResult == 0)
        {
            names->at(k) = leftNames.at(i);
            i++;
        }
        else if (compareResult == 2)
        {
            names->at(k) = rightNames.at(j);
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        names->at(k) = leftNames.at(i);
        i++;
        k++;
    }
    while (j < n2)
    {
        names->at(k) = rightNames.at(j);
        j++;
        k++;
    }
}
//calling of the mergesort
void mergeSort(std::vector<std::string> * names, int l, int r, std::string newAlphOrder)
{
    if (l < r)
    {
        int m = (l + (r - l) / 2);
        mergeSort(names, l, m, newAlphOrder);
        mergeSort(names, m + 1, r, newAlphOrder);
        merge(names, l, m, r, newAlphOrder);
    }
}
//where the magic happens
int main()
{
    //on the original assignment our professor didnt want any input to screen, but here i decided to give users more direction
    std::cout << "Input the new alphabet\n";
    std::string newAlphOrder;
    std::cin >> newAlphOrder;
    //this is to make sure the alphabet inputted is long enough, if it isnt there can be out of bounds issues in the compare function
    while (newAlphOrder.length() < ALPHABETLENGTH)
    {
        std::cout << "Inputted Alphabet is not long enough, please input again\n";
        std::cin >> newAlphOrder;
    }
    int numberOfNames = 0;
    //pointer names that gets passed to mergesort
    std::vector<std::string> * names = new std::vector<std::string>();
    //need to reserve memory so push_back doesnt cause any issues when adding strings to the vector
    names->reserve(numberOfNames);
    std::string s;
    std::cout << "Input the words, -1 to end\n";
    //instead of asking for a number of words, you now enter -1 to stop inputting words, much more user friendly, also made sure -1 isnt included in names
    while (true)
    {
        std::cin >> s;
        if (s == "-1")
        {
            break;
        }
        names->push_back(s);
        numberOfNames++;
    }
    //calling the sort and the sort function calls the comparison function
    //newAlphOrder is not a global variable anymore because global variables can cause problems and are generally not good practice
    mergeSort(names, 0, numberOfNames - 1, newAlphOrder);
    std::cout << "Sorted words according to new alphabet";
    for (int i = 0; i < numberOfNames; i++)
    {
        std::cout << "\n" << names->at(i);
    }
    //free the data from the names pointer
    delete(names);
    return 0;
}
