#include <stdio.h>	
#include "parser.tab.h"
extern int yyparse();

void main() {
	yyparse();
	getchar();
}
