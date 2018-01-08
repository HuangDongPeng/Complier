#include <stdio.h>	
#include <string>
#include "parser.tab.h"
extern int yyparse();

void main() {
	
	yyparse();
	getchar();
}
