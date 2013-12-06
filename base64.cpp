#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define L(n,m) (n>>(8-m)) 
#define R(n,m) (((n<<(8-m))&255) >> (8-m))

#define L64(n,m) (n>>(6-m)) 
#define R64(n,m) (((n<<(6-m))&63) >> (6-m))

static char idx_table[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
int get_idx(char c){
	for(int i=0;i<64;i++)
		if(c == idx_table[i])
			return i;
}
char get_at(int idx){
	return idx_table[idx];
}

void encode(const unsigned char *src,int src_size,char *dst,int dst_size){
	int offset = 0, offset2 = 0;
	int r = 8,r2 = 6;
	unsigned char s = 0;

	memset(dst, 0, dst_size);
	while( offset != src_size ){
		if( r >= r2 ){
			int i;
			if( r2 >= r )	i = s | R(src[offset],r2);
			else			i = s | L(src[offset],r2);
			dst[offset2] = get_at( i );
			r -= r2;
			r2 = 6;
			s=0;
			offset2 ++;
		}
		else{
			r2 -= r;
			r = 8;
			s = (R(src[offset], r) << r2) & 63;
			offset ++;
		}
	}
	if(s)
		dst[offset2] = get_at(s<<r2);
}
void decode(const char *src,int src_size,char *dst,int dst_size){
	int offset=0,offset2=0;
	int r = 8,r2 = 6;

	memset(dst, 0, dst_size);
	while(offset2 != src_size && src[offset2] != '='){
		if( r >= r2 ){
			dst[offset] |= R64(get_idx(src[offset2]), (r2)) << (r-r2);
			r -= r2;
			r2 = 6;
			offset2 ++;
		}
		else{
			dst[offset] |= L64(get_idx(src[offset2]),r);
			r2 -= r;
			r = 8;
			offset ++;
		}
	}
}

int main(){
	char msg[] = "hello? i am a kimchi man! do you know psy?";
	char decoded[1024];
	char encoded[1024];

	encode(
		msg, strlen(msg),
		encoded, 1024);
	decode(
		encoded, strlen(encoded),
		decoded, 1024);

	printf("|%s|\n", encoded);
	printf("|%s|\n", decoded);
    return 0;
}
