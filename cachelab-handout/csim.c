#include "cachelab.h"
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
typedef struct {
int v;//valid bit
unsigned int tag;
int timestamp;
}cache_line;
 cache_line** cache_sim;//2d cache_line array
int s,S,E,b,B,m;//set bits,nub of set,lines,block bits,nub of bytes in block,address
int miss,hit,eviction;
void cache_init() 
{
	cache_sim = (cache_line**)malloc(S * sizeof( cache_line*)); 
	for( int i = 0; i < S;i++){
		cache_sim[i] =(cache_line*)malloc(E * sizeof(cache_line));
	}
	for(int i = 0; i < S ;i++){
		for(int j = 0;j < E;j++){
			cache_sim[i][j].v = 0;
			cache_sim[i][j].tag = 0;
			cache_sim[i][j].timestamp = 0;
		}
	
	}
}
// every line in set adds 1 to timestamp when a new reference of cache occurs
// then configure if or not set is full
// if set is full ,get line index which has largest timestamp
// if set is not full, get not valid index which is smallest
int add_timestamp(int set,int* full,int* next_index){
	int largest_index = 0;
	int largest = cache_sim[set][0].timestamp + 1;
	int valid_line = 0;
	for(int i = 0;i < E;i++){
		if(cache_sim[set][i].v == 1){
			cache_sim[set][i].timestamp++;//every line's timestamp add 1
			valid_line++;//count valid lines 
			if(cache_sim[set][i].timestamp > largest){
				largest = cache_sim[set][i].timestamp;//get largest timestamp
				largest_index = i;//get least recently used line index
			}
		}
	}
	if(valid_line == E){//if valid lines = E,this set is full
		*full = 1;
	}
	else{
		*full = 0;
	}
	*next_index = valid_line;//fist not valid line
		return largest_index;
}
void reference_cache(int set,int*tag,int* hit,int* miss,int* eviction){
	int status = 0;
	int evict_index = 0;
	int set_full = 0;
	int next_index;
	for(int i = 0;i < E;i++){//search in set 
		if(cache_sim[set][i].v == 1 && cache_sim[set][i].tag == *tag){//hit
			(*hit)++;
			status = 1;
			add_timestamp(set,&set_full,&next_index);
			cache_sim[set][i].timestamp = 0;
			printf(" hit");
		}
	}
	if (status == 0){//miss
		evict_index = add_timestamp(set,&set_full,&next_index);
		(*miss)++;
		printf(" miss");
		if(set_full == 1){//set is full,so need to evict least recently used line and replace it by new line
			cache_sim[set][evict_index].tag = *tag;
			cache_sim[set][evict_index].timestamp = 0;
			(*eviction)++;
			printf(" eviction");	
		}
		else{//set is not full,let new line replace empty line
			cache_sim[set][next_index].v = 1;
			cache_sim[set][next_index].tag = *tag;
			cache_sim[set][next_index].timestamp = 0;
		}
	}

}
int main(int argc, char** argv){
    int opt;
	char* t = NULL;//trace file name
	/* looping over arguments */
    while(-1 != (opt = getopt(argc,argv,"s:E:b:t:")))
	{ 
        switch(opt) {
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
			case 'b':
				b = atoi(optarg);
				break;
			case 't':
				t = optarg;
				break;
            default:
                printf("wrong arguement \n");
                break;
        }
    }
	S = 1 << s;//nub of set
	cache_init();
	 int set,tag;//set index,tag 
	FILE * pFile;
	pFile = fopen(t,"r");
	char identifier;//valid bit
	unsigned int address;// memory address
	int size;
	miss = 0;
	hit = 0;
	eviction = 0;
	while(fscanf(pFile," %c %x,%d",&identifier,&address,&size)>0){
		printf(" %c %x,%d ",identifier,address,size);
		set = (address>>b)&(0xffffffff>>(32-s));//get set index
		tag = address>>(s+b);//get tag
		//printf(" set = %x,tag = %x ",set,tag);
		if(identifier == 'L' || identifier == 'S'){
			reference_cache(set,&tag,&hit,&miss,&eviction);	
			printf("\n");
		}
		if(identifier == 'M'){
			reference_cache(set,&tag,&hit,&miss,&eviction);	
			reference_cache(set,&tag,&hit,&miss,&eviction);
			printf("\n");
		}
	}
	printSummary(hit,miss,eviction);
	for(int i = 0;i < S;i++){
		free(cache_sim[i]);
	}
	free(cache_sim);
	fclose(pFile);
	return 0;
}
