#include <sys/stat.h>
#include <string.h>
#include "envar.h"

char* searchPathForFile(char* fn);
int doesFileExist(char* pathname); /* Returns 1 if file exists (and is not a directory)*/