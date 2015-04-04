#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "envar.h"

void print_env();
void change_dir();
void set_env(char* env_var, char* val);
void unset_env(char* env_var);