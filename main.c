#!/bin/bash
#include <stdio.h>

main() {
	shell_init();

	while(1) {
		printShellSymbol();
		yyparse();
	}
}

shell_init() {

}

getCommand() {

}