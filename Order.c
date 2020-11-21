//Scott Buck

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define ALPHABETLENGTH 26
#define MAXNAMESIZE 100
//creation of new alphabet order array so that the string compare
//function has access to it without it being an input
char newAlphOrder [ALPHABETLENGTH];

//way to add one array to another
//used to copy strings
void addToArray (char ** names, char * tempNameAddress, int index, int strLength)
{
  for (int i = 0; i < strLength; i++)
  {
    names[index][i] = tempNameAddress[i];
  }
}
//custom function to compare strings with any given alphabet order
int customStringCompare (char * string1, char * string2)
{
  int length1 = strlen(string1);
  int length2 = strlen(string2);
  int smallestLength;
  int counter = 0;
  if (length1 < length2)
  {
    smallestLength = length1;
  }
  else
  {
    smallestLength = length2;
  }
  //go through each letter in the words
  for (int j = 0; j < smallestLength; j++)
  {
    //go through the whole alphabet
    for (int i = 0; i < ALPHABETLENGTH; i++)
    {
      //if letter of first word occurs in alphabet before letter of second word
      if (string1[j] == newAlphOrder[i] && string2[j] != newAlphOrder[i])
      {
        counter = 1;
        break;
      }
      //if letter of first word occurs in alphabet after letter of second word
      else if (string2[j] == newAlphOrder[i] && string1 [j] != newAlphOrder[i])
      {
        counter = 2;
        break;
      }
      //if the letters are the same
      else if(string2[j] == newAlphOrder[i] && string1[j] == newAlphOrder[i])
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
    else if (counter == 2)
    {
      return 2;
    }
  }
}
//part of the merge sort used to sort the names given at a much better big-oh constant
void merge (char ** names, int l, int m, int r)
{
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;
  //left and right string arrays for splitting for merge sort
  char ** leftName = calloc (n1+1, sizeof(char*));
  char ** rightName = calloc (n2+1, sizeof(char*));
  //allocating memory
  for (i = 0; i < n1; i++)
  {
    int length = strlen(names[l+i]);
    leftName[i] = calloc (length+1, sizeof(char));
    addToArray(leftName, names[l+i], i, length);
  }
  //allocating memeory
  for (j = 0; j < n2; j++)
  {
    int length = strlen(names[m+1+j]);
    rightName[j] = calloc (length+1, sizeof(char));
    addToArray(rightName, names[m+1+j], j, length);
  }
  i = 0;
  j = 0;
  k = l;
  //compare strings using the custom string compare and go through merge sort steps, comparing strings
  //and placing them into the proper place
  while(i < n1 && j < n2)
  {
    if(customStringCompare(leftName[i],rightName[j]) == 1 || customStringCompare(leftName[i],rightName[j]) == 0)
    {
      free(names[k]);
      int length = strlen(leftName[i]);
      names[k] = calloc(length, sizeof(char));
      names[k] = leftName[i];
      i++;
    }
    else if(customStringCompare(leftName[i],rightName[j]) == 2)
    {
      free(names[k]);
      int length = strlen(rightName[j]);
      names[k] = calloc(length, sizeof(char));
      names[k] = rightName[j];
      j++;
    }
    k++;
  }
  while(i < n1)
  {
    free(names[k]);
    int length = strlen(leftName[i]);
    names[k] = calloc(length, sizeof(char));
    addToArray(names, leftName[i], k, strlen(leftName[i]));
    i++;
    k++;
  }
  while(j < n2)
  {
    free(names[k]);
    int length = strlen(rightName[j]);
    names[k] = calloc(length, sizeof(char));
    addToArray(names, rightName[j], k, strlen(rightName[j]));
    j++;
    k++;
  }
  free(leftName);
  free(rightName);
}
//calling of the mergesort
void mergeSort(char ** names, int l, int r)
{
  if (l < r)
  {
    int m = (l+(r-l)/2);
    mergeSort(names, l, m);
    mergeSort(names, m+1, r);
    merge (names, l, m, r);
  }
}
//where the magic happens
int main()
{
  //asking for new order of alphabet
  for (int i = 0; i < ALPHABETLENGTH; i++)
  {
    scanf(" %c", &newAlphOrder[i]);
  }
  int numberOfNames;
  scanf("%d", &numberOfNames);
  //memory allocation and adding of names to the array of names
  char ** names = calloc(numberOfNames+1, sizeof(char*));
  char * tempNameAddress = calloc(MAXNAMESIZE+1, sizeof(char));
  for (int i = 0; i < numberOfNames; i++)
  {
    scanf("%s", tempNameAddress);
    int length = strlen(tempNameAddress);
    names[i] = calloc (length+1, sizeof(char));
    addToArray(names,tempNameAddress,i,length);
  }
  //calling the sort and the sort function calls the comparison function
  mergeSort(names, 0, numberOfNames-1);
  for (int i = 0; i < numberOfNames; i++)
  {
    printf("%s\n", names[i]);
  }
  //freeing all the memory I allocated
  for (int i = 0; i < numberOfNames; i++)
  {
    free(names[i]);
  }
  free(names);
  free (tempNameAddress);
  return 0;
}
