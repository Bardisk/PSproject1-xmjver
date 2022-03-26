#include<cstdio>
#include "base.h"
char lName[100],sName[100];
unsigned int node[100000];
int main(){
	puts("Enter the name of file you want to transvert");
	puts("This will change the map data to new form");
	scanf("%s",lName);
	FILE* fp = fopen(lName,"rb");
	int szN, szM;
	fread(&szN,sizeof(int),1,fp);
	fread(&szM,sizeof(int),1,fp);
	fread(node,sizeof(unsigned int),szN*szM,fp);
	fclose(fp);
	for(int i=0;i<szN*szM;i++){
		if (node[i]&0x0010)
			node[i]^=(0x0010|HAS_ITEM);
		if (node[i]&0x0020)
			node[i]^=(0x0020|SPEED_UP);
		if (node[i]&0x0040)
			node[i]^=(0x0040|LEVEL_UP);
	}
	puts("Enter the file name you want to save as");
	scanf("%s",sName);
	fp=fopen(sName,"wb");
	fwrite(&szN,sizeof(int),1,fp);
	fwrite(&szM,sizeof(int),1,fp);
	fwrite(node,sizeof(unsigned int),szN*szM,fp);
	fclose(fp);
	return 0;
}
