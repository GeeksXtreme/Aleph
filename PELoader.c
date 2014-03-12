#include <stdio.h>
#include <windows.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory.h>
#include <string.h>

int main(int argc, char *argv[])
{
FILE *filePointer;
int fileDescriptor;
int stat;
struct _stat buff;
void *spacePointer; 


//input safety
if (argc != 2){
printf("Usage: PELoader.exe <PE32file.exe>"); 
exit(0);
			 }		 

//open the exe to be run, null check on file pointer
filePointer = fopen(argv[1], "r");
if (filePointer == NULL){
printf("Bad file pointer, try again.");
exit(0);				}

//get file descriptor for reading, sanity check
fileDescriptor = _fileno(filePointer);
if (fileDescriptor == 0){
printf("Bad file descriptor, try again.");
exit(0);				}

//get data associated with the file descriptor so I can malloc for the right size
stat = _fstat(fileDescriptor, &buff);
printf("Size of file is: %d bytes\n", buff.st_size);

//allocate space according to file size, sanity check
spacePointer = malloc(buff.st_size);
if (spacePointer == NULL){
printf("Bad file pointer, try again.");
exit(0);				 }
printf("Malloc'd address is: %p \n", spacePointer);

//Copy executable into the space
fread(spacePointer, buff.st_size, 1, filePointer);



printf("Test success.");

//clean up 
fclose(filePointer);
free(spacePointer);
return 0;

}


/*
1. Take filename as argument.
2. Get size of file.
3. malloc for that much space 
4. Check to see if malloc pointer is null. If so, exit with an error message.
5. Memcopy the file into the malloc'd space, but with bounds checking
6. Call main function
*/
