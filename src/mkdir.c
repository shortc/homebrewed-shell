#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>

getenv("PWD");

mkdir() //here's the start
{
  
  so...let's first check for what arguments we have.
    no args?
    
    how many files we got?
    loop for the number of files:
  
      if the directory name we plug in, already exists, then no error, it's already been made
        return 0
  
        else we take the directory and add it to the preexisting file path
          then mkdir using that file path and all access
          return 0
          
     else
       which arg?
       {parents}
        how many files we got?
        loop for each file
          
        if the directory name we plug in, already exists, then no error, it's already been made
        return 0
          
        else we take the directory and add it to the preexisting file path -1 file
          then mkdir using that file path and all access
          return 0
          
        {verbose}  
        how many files we got?
        loop for each file
          
        if the directory name we plug in, already exists, then no error, it's already been made
        print that it already exists
        return 0
  
        else we take the directory and add it to the preexisting file path file
          then mkdir using that file path and all access
          print that we did it.
          return 0
  
} 





int main(int argc, char* argv){

	mkdir();

	return 0;

}
