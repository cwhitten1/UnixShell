#include "io_redir.h"

void redirect_in(char* fn){
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
	int fd;

	if(append)
		fd = open(fn, O_CREAT|O_WRONLY|O_APPEND);
	else
		fd = open(fn, O_CREAT|O_WRONLY);

	dup2(fd, 1);
	//close(fd);


}
void reset_in(){

}
void reset_out(){

}
void reset(){

}