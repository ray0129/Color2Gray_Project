#include "c2gIt.h"
#include "RGB2Lab.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>

#define FileName "Sunrise128.ppm"

using namespace std;

struct RGB{
	int r;
	int g;
	int b;
};
struct LAB{
	float L;
	float a;
	float b;
	float grayL;
};

c2gIt *c2gTD;
const int kRound = 30;

float cDelta(struct LAB iLab,struct LAB jLab){
	int alpha = c2gTD->alpha;
	float aDelta, bDelta, x;
	
	aDelta = iLab.a - jLab.a;
	bDelta = iLab.b - jLab.b;
	x = pow(pow(aDelta,2)+pow(bDelta,2), 0.5);
	
	return x/2; 
}

float lDelta(struct LAB iLab,struct LAB jLab){
	float temp = iLab.L - jLab.L;	
	if(temp >= 0){
		return temp;
	}
	else{
		return -temp;
	}
}

void optimization(struct LAB **lab, int height, int width, int h, int w){
	int mu = c2gTD->mu; 
	int wLeft, wRight, hUp, hDown;
	float cd ,ld, grayVar;
	float sDelta[3] = {0,0,0};
	
	wLeft = w-mu;
	if(wLeft < 0) wLeft = 0;
	
	wRight = w+mu;
	if(wRight >= width) wRight = width-1;
	
	hDown = h-mu;
	if(hDown < 0) hDown = 0;
	
	hUp = h+mu;
	if(hUp >= height) hUp = height-1;
	
	for(int i = hDown; i <= hUp; i++){
		for(int j = wLeft; j <= wRight; j++){
			cd = cDelta(lab[i][j], lab[h][w]);
			ld = lDelta(lab[i][j], lab[h][w]);
			grayVar = lab[h][w].grayL - lab[i][j].grayL;
			if(grayVar<0){
				grayVar = -grayVar;
			}
			sDelta[0] += pow(grayVar-max(cd,ld), 2);
		}
	}
	
	struct LAB tryL;
	tryL.L = lab[h][w].L;
	tryL.a = lab[h][w].a;
	tryL.b = lab[h][w].b;
	tryL.grayL = max(lab[h][w].grayL-0.1f, 0.0f);
	
	for(int i = hDown; i <= hUp; i++){
		for(int j = wLeft; j <= wRight; j++){
			cd = cDelta(lab[i][j], tryL);
			ld = lDelta(lab[i][j], tryL);
			grayVar = tryL.grayL - lab[i][j].grayL;
			if(grayVar<0){
				grayVar = -grayVar;
			}
			sDelta[1] += pow(grayVar-max(cd,ld), 2);
		}
	}
	
	tryL.grayL = min(lab[h][w].grayL+0.1f, 100.0f);
	for(int i = hDown; i <= hUp; i++){
		for(int j = wLeft; j <= wRight; j++){
			cd = cDelta(lab[i][j], tryL);
			ld = lDelta(lab[i][j], tryL);
			grayVar = tryL.grayL - lab[i][j].grayL;
			if(grayVar<0){
				grayVar = -grayVar;
			}
			sDelta[2] += pow(grayVar-max(cd,ld), 2);
		}
	}
	
	if(sDelta[1] < sDelta[0]){
		lab[h][w].grayL = max(lab[h][w].grayL-0.1f, 0.0f);
	}
	
	if(sDelta[2] < sDelta[0]){
		lab[h][w].grayL = min(lab[h][w].grayL+0.1f, 100.0f);
	}
		
	return;
}

int main(int argc, char * argv[]) {
	char pSix[10],temp[10];		
	int width = 0;		
	int height = 0;		
	int maximum = 0;
	int pixelNumber = 0;
	
	if(argc>=2){
		//c2gTD = new c2gIt(argc,argv);
	}
	else{
		c2gTD = new c2gIt(FileName);
	}
	
	FILE *rawFile;
	char inAdr[30] = "picture/";
	strcat(inAdr,(c2gTD->name));
	rawFile = fopen(inAdr, "rb");
	
	FILE *grayFile;
	grayFile = fopen("picture/test.ppm", "wb");
	

	fscanf(rawFile,"%s",pSix); 
	fscanf(rawFile, "%d %d", &width, &height);  
	fscanf(rawFile, "%d", &maximum); 
	
	pixelNumber = width*height;
	
	fprintf(grayFile,"%s\n",pSix);
	fprintf(grayFile,"%d %d\n" ,width, height);
	fprintf(grayFile,"%d\n", maximum);
	
	struct RGB **rgbPixel = new struct RGB*[height];
	struct LAB **LabPixel = new struct LAB*[height];

	for(int i=0; i< height; i++){
		rgbPixel[i] = new struct RGB[width];
		LabPixel[i] = new struct LAB[width];
	}
	
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			rgbPixel[i][j].r=0;
			rgbPixel[i][j].g=0;
			rgbPixel[i][j].b=0;
		}
	}
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			LabPixel[i][j].L=0;
			LabPixel[i][j].a=0;
			LabPixel[i][j].b=0;
		}
	}
	
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			fread(&rgbPixel[i][j].r,1,1,rawFile);
			fread(&rgbPixel[i][j].g,1,1,rawFile);
			fread(&rgbPixel[i][j].b,1,1,rawFile);
		}
	}
	
	float Lp, ap, bp;
	float r, g, b;
	
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			r = float(rgbPixel[i][j].r);
			g = float(rgbPixel[i][j].g);
			b = float(rgbPixel[i][j].b);
			RGB2Lab(r, g, b, &Lp, &ap, &bp);
			LabPixel[i][j].L = Lp;
			LabPixel[i][j].a = ap;
			LabPixel[i][j].b = bp;
		}
	}
	
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			LabPixel[i][j].grayL = LabPixel[i][j].L;
		}
	}
	
	for(int k=0; k < kRound; k++){
		for(int i=0; i<height; i++){
			for(int j=0; j<width; j++){
				optimization(LabPixel,height,width,i,j);
			}
		}
	}
	
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			Lp = LabPixel[i][j].grayL;
			ap = 0;
			bp = 0;
			Lab2RGB(Lp, ap, bp, &r, &g, &b);
			rgbPixel[i][j].r = int(r);
			rgbPixel[i][j].g = int(g);
			rgbPixel[i][j].b = int(b); 
		}
	}
	
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			fwrite(&rgbPixel[i][j].r,1,1,grayFile);
			fwrite(&rgbPixel[i][j].g,1,1,grayFile);
			fwrite(&rgbPixel[i][j].b,1,1,grayFile);
		}
	}
	
	fclose(rawFile);
	fclose(grayFile);

	return 0;
}
