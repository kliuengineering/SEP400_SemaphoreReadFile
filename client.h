// named semaphore header

#ifndef _CLIENT_H_
#define _CLIENT_H_

const char SEM_NAME[] = "FileSem";
const char FILE_NAME[] = "MyFile.txt";
const int BUF_LEN = 16;
const mode_t SEM_PERMS = (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

#endif