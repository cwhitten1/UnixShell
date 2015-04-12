NOT IMPLEMENTED
----------------------
-Piping does not actually set up the IO redirection between commands.
	- Illegal pipelines are recognized and handled however.
	- Any redirection after the pipeline is appropriately added to first and last command of the pipeline

-Wildcard matching was not implemented for anything besides '.' and './'

-None of the extra credit was implemented



IMPLEMENTED
--------------------------
- All built in commands
- Execution of external commands
- I/O redirection (not piping)
- Aliasing (can be used in I/O redirection and piping)
- Environment Variable Expansion (by itself, not like ${PATH}/file.txt)
- Error Handling


NOTES
---------------------------
Please make sure the test files you use have the "bye" command at the end followed by a newline


Execution Instructions
--------------------------
make clean;
make;
./shell.exe