#include "utils.h"

void memcpy(char* from,char*to,int bytes){
	int i=0;
	for(;i<bytes;i++){
		*(to+i) = *(from+i);
	}
}
