#include "command.h"

void addCommand(int ind, char* name, int id, int argnum, ...)
{
        struct command *cmd = &commands[ind];
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

        num_commands++;
}

void insertCommand(int ind, char* name, int id, int argnum, ...)
{
        //Shift everything to the right
        int k;
        for(k = MAX_COMMANDS - 1; k > ind; k--)
        {
                commands[k] = commands[k-1];
        }

        //Insert
        struct command *cmd = &commands[ind];
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

        num_commands++;
}

void removeCommand(int ind)
{
        struct command blank;
        if(ind == (MAX_COMMANDS -1))
        {
                commands[ind] = blank;
        }
        int i;

        //Shift the elements on the right of element to the left
        for(i = ind; i < MAX_COMMANDS; i++)
        {
                commands[i] = commands[i+1];
        }

        num_commands--;
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

void clearCommandTable(){
        int i;
        for(i = 0; i < MAX_COMMANDS; i++)
        {
                struct command *cmd = &commands[i];
                clearCommandArgs(cmd);
                cmd->name = NULL;
                cmd->id = 0;
                cmd->infile = NULL;
                cmd->outfile = NULL;
                cmd->appendOut = 0;
        }
}

void clearCommandArgs(struct command * cmd){
        int i;
        for(i = 0; i < MAX_ARGS; i++)
        {
                cmd->args[i] = NULL;
        }
        cmd->argnum = 0;
}     