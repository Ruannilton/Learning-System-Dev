#include "port_io.h"
#include "../drivers/screen.h"

void main_kernel(){
	
	clear_screen();
	int i=0;
	char* str = "Hello kernel\n";

	print_str(str);	
	
	
}
