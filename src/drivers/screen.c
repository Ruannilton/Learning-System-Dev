#include "screen.h"
#include "../kernel/port_io.h"
#include "../kernel/utils.h"

void print_char(char character,int col,int row, char attribute_byte){
	unsigned char *vidmem = (unsigned char*) VIDERO_MEMORY;
	if(!attribute_byte){
		attribute_byte = WHITE_ON_BLACK;
	}
	int offset;
	if(col>=0 && row>=0){
		offset = get_screen_offset(col,row);
	}else{
		offset=get_cursor();
	}
	
	if(character == '\n'){
		int r = offset/(2*MAX_COLS);
		offset = (get_screen_offset(79,r));
	}else{
		vidmem[offset] = character;
		vidmem[offset+1] = attribute_byte;
	}
	offset +=2;
	offset = handle_scrolling(offset);
	set_cursor(offset);
}

int get_screen_offset(int col,int row){
	return 2 * (row * MAX_COLS + col);
}

int get_cursor() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; 
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2; 
}

void set_cursor(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void print_str_at(char* str,int col,int row){
	if(col>=0 && row>=0){
		set_cursor(get_screen_offset(col,row));
	}
	int i=0;
	while(str[i]!=0){
		print_char(str[i],col,row,WHITE_ON_BLACK);
		i++;
	}
}

void print_str(char* str){
	print_str_at(str,-1,-1);
}

void clear_screen() {
	int row = 0;
	int col = 0;
	
	for (row =0; row < MAX_ROWS ; row ++) {
		for (col =0; col < MAX_COLS ; col ++) {
			print_char (' ', col , row , WHITE_ON_BLACK );
		}
	}
	
	set_cursor ( get_screen_offset (0, 0));
}

int handle_scrolling(int offset){
	if(offset < MAX_COLS*MAX_ROWS*2){
		return offset;
	}
	int i;
	for(i=1;i<MAX_COLS;i++){
		memcpy(get_screen_offset(0,i)+VIDERO_MEMORY,get_screen_offset(0,i-1)+VIDERO_MEMORY,MAX_COLS*2);
	}
	char* last = get_screen_offset(0,MAX_COLS-1)+VIDERO_MEMORY;
	for(i=0;i<MAX_COLS*2;i++){
		last[i]=0;
	}
	offset -=2*MAX_COLS;
	return offset;
}
