#include <string.h>

class c2gIt{
	public:
		c2gIt(int argc,char * argv[]);
		c2gIt(char *name);
		~c2gIt();
		init();
		
		float alpha;
		bool quantize;
		int q_colors;
		float theta;
		int r;
		char name[30];
		int mu;
};

c2gIt::c2gIt(int argc,char * argv[]){
	
}
c2gIt::c2gIt(char *name){
	init();
	for(int i=0;i<strlen(name);i++){
		this->name[i] = name[i];
	}
}
c2gIt::~c2gIt(){
	init();
}
c2gIt::init(){
	alpha = 10;
	quantize = true;
	q_colors = 64;
	theta = 3.14/4.0;
	r = 0;
	mu = 10;
}
