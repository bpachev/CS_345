// os345fat.c - file management system
// ***********************************************************************
// **   DISCLAMER ** DISCLAMER ** DISCLAMER ** DISCLAMER ** DISCLAMER   **
// **                                                                   **
// ** The code given here is the basis for the CS345 projects.          **
// ** It comes "as is" and "unwarranted."  As such, when you use part   **
// ** or all of the code, it becomes "yours" and you are responsible to **
// ** understand any algorithm or method presented.  Likewise, any      **
// ** errors or problems become your responsibility to fix.             **
// **                                                                   **
// ** NOTES:                                                            **
// ** -Comments beginning with "// ??" may require some implementation. **
// ** -Tab stops are set at every 3 spaces.                             **
// ** -The function API's in "OS345.h" should not be altered.           **
// **                                                                   **
// **   DISCLAMER ** DISCLAMER ** DISCLAMER ** DISCLAMER ** DISCLAMER   **
// ***********************************************************************
//
//		11/19/2011	moved getNextDirEntry to P6
//
// ***********************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <assert.h>
#include "os345.h"
#include "os345fat.h"

// ***********************************************************************
// ***********************************************************************
//	functions to implement in Project 6
//
int fmsCloseFile(int);
int fmsDefineFile(char*, int);
int fmsDeleteFile(char*);
int fmsOpenFile(char*, int);
int fmsReadFile(int, char*, int);
int fmsSeekFile(int, int);
int fmsWriteFile(int, char*, int);

// ***********************************************************************
// ***********************************************************************
//	Support functions available in os345p6.c
//
extern int fmsGetDirEntry(char* fileName, DirEntry* dirEntry);
extern int fmsGetNextDirEntry(int *dirNum, char* mask, DirEntry* dirEntry, int dir);
extern int fmsSetDirEntry(char* mask, DirEntry* dirEntry);


extern int fmsMount(char* fileName, void* ramDisk);

extern void setFatEntry(int FATindex, unsigned short FAT12ClusEntryVal, unsigned char* FAT);
extern unsigned short getFatEntry(int FATindex, unsigned char* FATtable);

extern int fmsMask(char* mask, char* name, char* ext);
extern void setDirTimeDate(DirEntry* dir);
extern int isValidFileName(char* fileName);
extern void printDirectoryEntry(DirEntry*);
extern void fmsError(int);

extern int fmsReadSector(void* buffer, int sectorNumber);
extern int fmsWriteSector(void* buffer, int sectorNumber);

// ***********************************************************************
// ***********************************************************************
// fms variables
//
// RAM disk
unsigned char RAMDisk[SECTORS_PER_DISK * BYTES_PER_SECTOR];

// File Allocation Tables (FAT1 & FAT2)
unsigned char FAT1[FAT_SIZE];
unsigned char FAT2[FAT_SIZE];

char dirPath[128];							// current directory path
FDEntry OFTable[NFILES];					// open file table

extern bool diskMounted;					// disk has been mounted
extern TCB tcb[];							// task control block
extern int curTask;							// current task #
extern int OFTalloc[];

int defineFile(char * fileName,int attributes);
int defineDir(char * fileName);
void parseFileName(char * fileName,DirEntry* dirEntry);


// ***********************************************************************
// ***********************************************************************
// This function closes the open file specified by fileDescriptor.
// The fileDescriptor was returned by fmsOpenFile and is an index into the open file table.
//	Return 0 for success, otherwise, return the error number.
//
int fmsCloseFile(int fileDescriptor)
{
//  printf("CLOSING file with file descriptor %d\n",fileDescriptor);
  DirEntry dirEntry;
  int error = 0;
  
  //if invalid filedescriptor
  if (fileDescriptor < 0 || fileDescriptor >= NFILES) return ERR52;
  //if file not open
  if (!OFTalloc[fileDescriptor]) return ERR63;
  if ((error = fmsGetDirEntry(OFTable[fileDescriptor].name, &dirEntry)) < 0) return error;
  
  if (dirEntry.fileSize != OFTable[fileDescriptor].fileSize)
  {
    setDirTimeDate(&dirEntry);
    dirEntry.fileSize = OFTable[fileDescriptor].fileSize;
    dirEntry.startCluster = OFTable[fileDescriptor].startCluster;
    if ((error = fmsSetDirEntry(OFTable[fileDescriptor].name,&dirEntry)) < 0) return error;
  }
  
  
//  dirEntry
  OFTable[fileDescriptor].name[0] = 0;
  OFTalloc[fileDescriptor] = 0;
  return 0;
} // end fmsCloseFile



// ***********************************************************************
// ***********************************************************************

//parses filename into name and extension
void parseFileName(char * fileName,DirEntry* dirEntry)
{
  int d,j,ext_pad,s_len,f_len,s;
  f_len = strlen(fileName);

  for (j = 0; j < f_len; j++)
  {
    if(fileName[j] != '.') fileName[j] = toupper(fileName[j]);
  }
  
  if (s = strchr(fileName, '.'))
  {
    s_len = strlen(s);
    d = f_len - s_len;
    memcpy(dirEntry->name,fileName, d);
    for (j = d; j < 8; j++)
    {
      dirEntry->name[j] = ' ';
    }
    
    memcpy(dirEntry->extension,s+1,s_len - 1);
    
    for (j = s_len - 1; j < 3; j++)
    {
      dirEntry->extension[j] = ' ';
    }
  }
  
  else
  {
    strncpy(dirEntry->name,fileName, 8);
    for (j = f_len; j < 8; j++)
    {
      dirEntry->name[j] = ' ';
    }
    
    for (j = 0; j < 3; j++)
    {
      dirEntry->extension[j] = ' ';
    }
    
  }
}

int defineFile(char * fileName,int attributes)
{    
  char buffer[BYTES_PER_SECTOR];
  int dirIndex, dirSector, error;
  int dirNum = 0;
  int new_sector = 0;  
  int loop = dirNum / ENTRIES_PER_SECTOR;
  int dir = CDIR;
  int dirCluster = dir;
  DirEntry curEntry;
  DirEntry newEntry;
  setDirTimeDate(&newEntry);  
  parseFileName(fileName, &newEntry);
  newEntry.fileSize = 0;
  newEntry.startCluster = 0;
  newEntry.attributes = attributes;
  if (attributes == DIRECTORY)
  {
    newEntry.startCluster = extendFile(0);
  }
  
  while(1)
  { // load directory sector
    if (dir)
    { // sub directory
      while(loop--)
      {
        error = getFatEntry(dirCluster, FAT1);
        if (error == FAT_EOC || error == 0)
        {
          if ((error = extendFile(dirCluster)) < 0) return error;
          new_sector = 1;
        }
        if (error == FAT_BAD) { printf("\nBAD FAT\n");
          return ERR54;}
        if (error < 2) 
        {
          printf("\nUninitialized or other fat error %d\n",error);
          return ERR54;
        }

        dirCluster = error;
      }
      
      dirSector = C_2_S(dirCluster);
    }
    else
    { // root directory
      dirSector = (dirNum / ENTRIES_PER_SECTOR) + BEG_ROOT_SECTOR;
      if (dirSector >= BEG_DATA_SECTOR) return ERR67;
    }

    if (new_sector)
    {
      memset(buffer,0,sizeof(buffer));
      new_sector = 0;
    }
    // read sector into directory buffer
    else if (error = fmsReadSector(buffer, dirSector)) return error;

    // find next matching directory entry
    while(1)
    { // read directory entry
      dirIndex = dirNum % ENTRIES_PER_SECTOR;
      memcpy(&curEntry, &buffer[dirIndex * sizeof(DirEntry)], sizeof(DirEntry));
      
      if (curEntry.name[0] == 0)  // open space
      {
        memcpy(&buffer[dirIndex * sizeof(DirEntry)], &newEntry, sizeof(DirEntry));
        fmsWriteSector(buffer,dirSector);
        return 0;
      }
      
      dirNum++;                            // prepare for next read
      // break if sector boundary
      if ((dirNum % ENTRIES_PER_SECTOR) == 0) break;
    }
    // next directory sector/cluster
    loop = 1;
   }
  return 0;
}

int defineDir(char* fileName)
{
  char buf[BYTES_PER_SECTOR];
  int error, fd;
  DirEntry entry1;
  DirEntry entry2;
  memset(buf, 0, BYTES_PER_SECTOR);
  if ((error = defineFile(fileName,DIRECTORY)) < 0) return error;
  
  strcpy(entry1.name,".        ");
  strcpy(entry2.name, "..      ");
  strcpy(entry1.extension,"   ");
  strcpy(entry2.extension,"   ");
  entry2.startCluster = CDIR;
  entry2.fileSize = 0;
  entry1.fileSize = 0;
  entry1.attributes = DIRECTORY;
  entry2.attributes = DIRECTORY;
  setDirTimeDate(&entry1);
  setDirTimeDate(&entry2);
  
  if ((fd = fmsOpenFile(fileName,1)) < 0) return fd;
  entry1.startCluster = OFTable[fd].startCluster;
  memcpy(buf, &entry1,sizeof(DirEntry));
  memcpy(buf+sizeof(DirEntry), &entry2, sizeof(DirEntry));
  
 // printf("OFTable[fd].startCluster %d\n", OFTable[fd].startCluster);
  
  if ((error = fmsWriteFile(fd,buf,2*sizeof(DirEntry))) < 1) return error;
  
  if ((error = fmsCloseFile(fd)) < 0) return error;
  
  return 0;
}


// If attribute=DIRECTORY, this function creates a new directory
// file directoryName in the current directory.
// The directory entries "." and ".." are also defined.
// It is an error to try and create a directory that already exists.
//
// else, this function creates a new file fileName in the current directory.
// It is an error to try and create a file that already exists.
// The start cluster field should be initialized to cluster 0.  In FAT-12,
// files of size 0 should point to cluster 0 (otherwise chkdsk should report an error).
// Remember to change the start cluster field from 0 to a free cluster when writing to the
// file.
//
// Return 0 for success, otherwise, return the error number.
//


int fmsDefineFile(char* fileName, int attribute)
{
  DirEntry ent;
  if (isValidFileName(fileName) <= 0) return ERR50;

  /*if (fmsMask(fileName,"FILE38  ","TXT"))
  {
    printf("\nI CAUGHT YOU REDHANDED DEFINING THIS FILE BEFORE!\n");
  }*/
  
  /*if (!fmsGetDirEntry(fileName, &ent)) {
    printf("\n%.8s %c",ent.name,ent.extension[2]);
    printDirectoryEntry(&ent);
    return ERR60;
  }*/
  
  if (attribute == DIRECTORY)
  {
    return defineDir(fileName); 
  }
  
  else
  {
    return defineFile(fileName, attribute);
  }
  

	return 0;
} // end fmsDefineFile




// ***********************************************************************
// ***********************************************************************
// This function deletes the file fileName from the current directory.
// The file name should be marked with an "E5" as the first character and the chained
// clusters in FAT 1 reallocated (cleared to 0).
// Return 0 for success; otherwise, return the error number.
//
int fmsDeleteFile(char* fileName)
{
  DirEntry dirEntry;
  int error;
  
  if ((error = fmsGetDirEntry(fileName, &dirEntry)) < 0) return error;
 // printf("\nmade it past the error\n");
  if (dirEntry.attributes == DIRECTORY)
  {
    return DeleteDir(&dirEntry,fileName);
  }
  
  else
  {
    return DeleteFile(&dirEntry, fileName);
  }
  
	return 0;
} // end fmsDeleteFile

int DeleteFile(DirEntry *dirEntry,char* fileName)
{
 // printf("Delete file is being called on %s\n",fileName);
  int nextCluster,error;
  int currentCluster = dirEntry->startCluster;
  dirEntry->name[0] = 0xe5;
  error = fmsSetDirEntry(fileName, dirEntry);
  //printf("%d\n",error);  
  while(currentCluster != FAT_EOC && currentCluster > 2)
  {
    nextCluster = getFatEntry(currentCluster,FAT1);
    setFatEntry(currentCluster,0,FAT1);
    currentCluster = nextCluster;
  }
  return 0;
}

int DeleteDir(DirEntry* dirEntry,char *fileName)
{
  char buffer[BYTES_PER_SECTOR];
  int dirIndex, dirSector, error;
  int dirNum = 0;  
  int loop = dirNum / ENTRIES_PER_SECTOR;
  int dir = dirEntry->startCluster;
  int dirCluster = dir;
  DirEntry curEntry;
  
  while(1)
  { // load directory sector
    if (dir)
    { // sub directory
      while(loop--)
      {
        error = getFatEntry(dirCluster, FAT1);
        if (error == FAT_EOC)
        {
          return DeleteFile(dirEntry,fileName);
        }
        if (error == FAT_BAD) return ERR54;
        if (error < 2) return ERR54;
        dirCluster = error;
      }
      dirSector = C_2_S(dirCluster);
    }
    else
    { // root directory
      dirSector = (dirNum / ENTRIES_PER_SECTOR) + BEG_ROOT_SECTOR;
      if (dirSector >= BEG_DATA_SECTOR) return ERR67;
    }

    // read sector into directory buffer
    if (error = fmsReadSector(buffer, dirSector)) return error;

    // find next matching directory entry
    while(1)
    { // read directory entry
      dirIndex = dirNum % ENTRIES_PER_SECTOR;
      memcpy(&curEntry, &buffer[dirIndex * sizeof(DirEntry)], sizeof(DirEntry));
      
      //found entry still in dir
      if(curEntry.name[0] && curEntry.name[0] != 0xe5 && curEntry.name[0] != '.')
      {
//        printf("\n%d",curEntry.name[0]);
        if(isValidDirEntry(&curEntry) == 0) {
          printDirectoryEntry(&curEntry);
          return ERR69;
        }
      }
      dirNum++;                            // prepare for next read
      
      if (curEntry.name[0] = 0) return DeleteFile(dirEntry,fileName);
      
      // break if sector boundary
      if ((dirNum % ENTRIES_PER_SECTOR) == 0) break;
    }
    // next directory sector/cluster
    loop = 1;
   }
   
   printf("Made it past the loop in deletedir\n");
  return DeleteFile(dirEntry,fileName);
  
}

// ***********************************************************************
// ***********************************************************************
// This function opens the file fileName for access as specified by rwMode.
// It is an error to try to open a file that does not exist.
// The open mode rwMode is defined as follows:
//    0 - Read access only.
//       The file pointer is initialized to the beginning of the file.
//       Writing to this file is not allowed.
//    1 - Write access only.
//       The file pointer is initialized to the beginning of the file.
//       Reading from this file is not allowed.
//    2 - Append access.
//       The file pointer is moved to the end of the file.
//       Reading from this file is not allowed.
//    3 - Read/Write access.
//       The file pointer is initialized to the beginning of the file.
//       Both read and writing to the file is allowed.
// A maximum of 32 files may be open at any one time.
// If successful, return a file descriptor that is used in calling subsequent file
// handling functions; otherwise, return the error number.
//
int fmsOpenFile(char* fileName, int rwMode)
{
  DirEntry dirEntry;
  int error = 0;
  int i = 0;
  
  if ((error = fmsGetDirEntry(fileName, &dirEntry))) return error;
  
  for (i = 0; i < NFILES; i++)
  {
    //Is the entry free?
    if (!OFTalloc[i]) break;
  }
  
  if (i == NFILES) return ERR70;
  
  memcpy(&OFTable[i].name,&dirEntry.name,sizeof(dirEntry.name));
  memcpy(&OFTable[i].extension,&dirEntry.extension,sizeof(dirEntry.extension));
  OFTable[i].attributes = dirEntry.attributes;
  OFTable[i].startCluster = dirEntry.startCluster;
  OFTable[i].currentCluster = dirEntry.startCluster;      
  OFTable[i].fileSize = dirEntry.fileSize;
  OFTable[i].pid = curTask;
  OFTable[i].mode = rwMode;
  OFTable[i].flags = 0;
  OFTalloc[i] = 1;
  
  switch (rwMode)
  {
    case 0:
      OFTable[i].fileIndex = 0;
      break;
    case 1:
      OFTable[i].fileIndex = 0;      
      break;
    case 2:
      if ((OFTable[i].fileIndex = fmsSeekFile(i, OFTable[i].fileSize)) < 0) 
      {
        printf("\nPROBLEMS with append access mode\n %d\n",OFTable[i].fileIndex);
        return OFTable[i].fileIndex;
      }
      break;
    case 3:
      OFTable[i].fileIndex = 0;
      break;
    default:
      break;      
  }

  fmsReadSector(OFTable[i].buffer, C_2_S(OFTable[i].currentCluster));
  
	return i;
} // end fmsOpenFile



// ***********************************************************************
// ***********************************************************************
// This function reads nBytes bytes from the open file specified by fileDescriptor into
// memory pointed to by buffer.
// The fileDescriptor was returned by fmsOpenFile and is an index into the open file table.
// After each read, the file pointer is advanced.
// Return the number of bytes successfully read (if > 0) or return an error number.
// (If you are already at the end of the file, return EOF error.  ie. you should never
// return a 0.)
//
int fmsReadFile(int fileDescriptor, char* buffer, int nBytes)
{
  int bytes_left, bufIndex, nextCluster, error;
  int bytes_read = 0;

  //if invalid file descriptor
  if (fileDescriptor < 0 || fileDescriptor >= NFILES) return ERR52;
  //if file not open
  if (!OFTalloc[fileDescriptor]) return ERR63;
  
  FDEntry* fd = &OFTable[fileDescriptor];
  
  if (fd->mode == 1 || fd->mode == 2) return ERR85; //write-only mode?
  
//  printf("\nREAD %d bytes from fs %d\n",nBytes,);


  
  
  while (nBytes > 0)
  {
    //read sector
    
    if (fd->fileSize == fd->fileIndex) 
    {
      return (bytes_read) ? bytes_read : ERR66; //EOF      
    }
    
    bufIndex = fd->fileIndex % BYTES_PER_SECTOR; 
    
    
    // The first part checks to see if on a cluster boundary.
    // The second part is a sanity check of sorts. If FileIndex = 0, we should be at the start of the file?
    // The second clause seems to have been created in response to some weird bug.
    if (bufIndex == 0 && (fd->fileIndex || !fd->currentCluster))
    {
      if (fd->currentCluster == 0)
      {
        if (fd->startCluster == 0) return ERR66; //no file at all?
        nextCluster = fd->startCluster;
        fd->fileIndex = 0;
      }
      
      else
      {
        nextCluster = getFatEntry(fd->currentCluster, FAT1);
        if (nextCluster == FAT_EOC) 
        {
          return bytes_read;
        }
      }
      
      if (fd->flags && BUFFER_ALTERED)
      {
        if ((error = fmsWriteSector(fd->buffer, C_2_S(fd->currentCluster)))) return error;
        fd->flags &= ~ BUFFER_ALTERED;
      }
      
      //update buffer
      if ((error = fmsReadSector(fd->buffer,C_2_S(nextCluster)))) return error;
      fd->currentCluster = nextCluster;
    }
    
    //calculate remaining bytes in sector
    bytes_left = BYTES_PER_SECTOR - bufIndex;
    //make sure we don't read more bytes than in the file or than asked to.
    bytes_left = (bytes_left > nBytes) ? nBytes : bytes_left;
    bytes_left = (bytes_left > fd->fileSize - fd->fileIndex) ? fd->fileSize - fd->fileIndex : bytes_left; 
    
    //copy memory to buffer
    memcpy(buffer, &fd->buffer[bufIndex], bytes_left);
    
    //update parameters
    nBytes -= bytes_left;
    buffer += bytes_left;
    bytes_read += bytes_left;
    fd->fileIndex += bytes_left;    
  }
  
	return bytes_read;
} // end fmsReadFile



// ***********************************************************************
// ***********************************************************************
// This function changes the current file pointer of the open file specified by
// fileDescriptor to the new file position specified by index.
// The fileDescriptor was returned by fmsOpenFile and is an index into the open file table.
// The file position may not be positioned beyond the end of the file.
// Return the new position in the file if successful; otherwise, return the error number.
//
int fmsSeekFile(int fileDescriptor, int index)
{
  char buf[BYTES_PER_SECTOR];
  memset(buf,0,sizeof(buf));
  if (!OFTalloc[fileDescriptor]) return ERR63;
  if(index < 0 || index > OFTable[fileDescriptor].fileSize) 
  {
    printf("\nfsize = %d, ind = %d\n", OFTable[fileDescriptor].fileIndex, index);
    return ERR85;
  }
  OFTable[fileDescriptor].fileIndex = index;
  int nextCluster = OFTable[fileDescriptor].startCluster;

  
  while (index > BYTES_PER_SECTOR)
  {
    if ((nextCluster = getFatEntry(nextCluster, FAT1)) < 0) return nextCluster;
    if (nextCluster == FAT_EOC)
    {
      nextCluster = extendFile(nextCluster);
      fmsWriteSector(buf,C_2_S(nextCluster));
    }
    index -= BYTES_PER_SECTOR;    
  }
  
  OFTable[fileDescriptor].currentCluster = nextCluster;
  fmsReadSector(OFTable[fileDescriptor].buffer, C_2_S(OFTable[fileDescriptor].currentCluster));
	return OFTable[fileDescriptor].fileIndex;
} // end fmsSeekFile



// ***********************************************************************
// ***********************************************************************
// This function writes nBytes bytes to the open file specified by fileDescriptor from
// memory pointed to by buffer.
// The fileDescriptor was returned by fmsOpenFile and is an index into the open file table.
// Writing is always "overwriting" not "inserting" in the file and always writes forward
// from the current file pointer position.
// Return the number of bytes successfully written; otherwise, return the error number.
//

int extendFile(int currentCluster);

int extendFile(int currentCluster)
{
  int i;
  for (i = 5; i < FAT_SIZE; i++)
  {
    if (getFatEntry(i,FAT1) == 0) //available spot
    {
      if (currentCluster)
      {
        setFatEntry(currentCluster,i,FAT1);
        setFatEntry(currentCluster,i,FAT2);
      }
      
      setFatEntry(i,FAT_EOC,FAT1);
      setFatEntry(i,FAT_EOC,FAT2);
      //copy fat 1 to fat 2
      memcpy(FAT2, FAT1,NUM_FAT_SECTORS * BYTES_PER_SECTOR);
      return i;
    }
  }
  return ERR65;
}


int fmsWriteFile(int fileDescriptor, char* buffer, int nBytes)
{
  int bytes_left, bufIndex, nextCluster, error,i;
  int bytes_written = 0;

  //if invalid file descriptor
  if (fileDescriptor < 0 || fileDescriptor >= NFILES) return ERR52;
  //if file not open
  if (!OFTalloc[fileDescriptor]) return ERR63;
  
  FDEntry* fd = &OFTable[fileDescriptor];
  
  if (fd->mode == 0) return ERR85; //write-only mode?
  
  
  if (fd->startCluster == 0) 
  {
    fd->currentCluster = extendFile(0);
    fd->startCluster = fd->currentCluster;
    memset(fd->buffer,0,BYTES_PER_SECTOR);
  }

  

  while (nBytes > 0)
  {
    bufIndex = fd->fileIndex % BYTES_PER_SECTOR; 
    
    
    // The first part checks to see if on a cluster boundary.
    // The second part is a sanity check of sorts. If FileIndex = 0, we should be at the start of the file?
    // The second clause seems to have been created in response to some weird bug.
    if (bufIndex == 0 && fd->fileIndex)
    {
      if (fd->currentCluster == 0)
      {
        if (fd->startCluster == 0) 
        {
          nextCluster = extendFile(0);
          fd->startCluster = nextCluster;
          memset(fd->buffer,0,BYTES_PER_SECTOR);          
        }
        
        else nextCluster = fd->startCluster;
        fd->fileIndex = 0;
      }
      
      else
      {
        nextCluster = getFatEntry(fd->currentCluster, FAT1);
        if (nextCluster == FAT_EOC)
        {
          if ((nextCluster = extendFile(fd->currentCluster)) < 0) return nextCluster;
        }
      }
      
//      if (fd->flags && BUFFER_ALTERED)
  //    {
        if ((error = fmsWriteSector(fd->buffer, C_2_S(fd->currentCluster)))) return error;
      //  fd->flags &= ~ BUFFER_ALTERED;
    //  }
      
      //update buffer
      if ((error = fmsReadSector(fd->buffer,C_2_S(nextCluster)))) return error;
      fd->currentCluster = nextCluster;
    }
    
    //calculate remaining bytes in sector
    bytes_left = BYTES_PER_SECTOR - bufIndex;
    //make sure we don't write more bytes than asked to.
    bytes_left = (bytes_left > nBytes) ? nBytes : bytes_left;
    
    //copy memory to buffer
    memcpy(&fd->buffer[bufIndex], buffer, bytes_left);
    fd->flags &= BUFFER_ALTERED;
    
    //update parameters
    nBytes -= bytes_left;
    buffer += bytes_left;
    bytes_written += bytes_left;
    fd->fileIndex += bytes_left;    
  }
  
  //write out last cluster read
  if ((error = fmsWriteSector(fd->buffer, C_2_S(fd->currentCluster)))) return error;
  fd->flags &= ~ BUFFER_ALTERED;  
  if (fd->fileIndex > fd->fileSize) fd->fileSize = fd->fileIndex;
  return bytes_written;
} // end fmsWriteFile
