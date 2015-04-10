#include "command.h"

void addCommand(char* name, int id, int argnum, ...)
{
        int cmd_ind = cmdtab_curr % MAX_COMMANDS;
        struct command *cmd = &commands[cmd_ind];
        cmd->name = name;
        cmd->id = id;
        cmd->argnum = argnum;

        va_list args;
        va_start(args, argnum);
        int i;
        for(i = 0; i < argnum; i++){
                cmd->args[i] = va_arg(args, char*);
        }
        va_end(args);
}

void addArgToCommand(int cmd_ind, char* arg)
{
        struct command *cmd = &commands[cmd_ind];
        cmd->args[cmd->argnum] = strdup(arg); //argnum will point to the next empty slot in args array
        cmd->argnum += 1;
}

void addInputRedirection(int cmd_index, char* in_fn)
{
        struct command *cmd = &commands[cmd_index];
        cmd->infile = in_fn;
}       

void addOutputRedirection(int cmd_index, char* out_fn, int appendOut)
{
        struct command *cmd = &commands[cmd_index];
        cmd->outfile = strdup(out_fn);
        cmd->appendOut = appendOut;
}       