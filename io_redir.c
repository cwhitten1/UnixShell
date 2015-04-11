#include "io_redir.h"

void redirect_in(char* fn){
	if(fn == NULL)
		return;

	int fd = open(fn, O_RDONLY);

	if(fd == -1)
	{
		printf("\tFile %s not readable or does not exist.\n", fn);
		return;
	}

	printf("\tFile %s found!\n", fn);

	dup2(fd, 0);
	close(fd);
}
void redirect_out(char* fn, int append){
	if(fn == NULL)
		return;
	
	int fd;

	if(append)
		fd = open(fn, O_CREAT | O_WRONLY | O_APPEND,  S_IREAD | S_IWRITE);
	else
		fd = open(fn, O_CREAT | O_WRONLY,  S_IREAD | S_IWRITE);

	dup2(fd, 1);
	//close(fd);


}
void reset_in(){

}
void reset_out(){

}
void reset(){

}