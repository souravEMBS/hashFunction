#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


typedef struct myDataTag{
	int user; //0 - not used 1- used
	int key;
	char name[24];
} myData;

int hashKey(char *name)
{
	int key,i;

	key = 0;
	int len;
	len = strlen(name);
	for(i=0;i<len;i++)
	{
		key += name[i];
	}

	key %= 7;
	printf("hashkey for %s is %d\n",name,key);
	return key;
}

void initTable(char* fileName,int size)
{
	FILE *fp;
	myData data;
	memset(&data,0,sizeof(data));

	fp = fopen(fileName,"w+");
	if(fp == NULL)
	{
		perror("fopen init table:");
		exit(1);
	}
	int i;

	for(i = 0;i<size;i++){
		fwrite(&data,sizeof(myData),1,fp);
	}

}

void insertData(int key, char* name, char* fileName)
{
	FILE* fp;
	myData data,slot;

	int pos;
	pos = key;

	data.user = 1;
	data.key = key;
	strcpy(data.name, name);


	fp = fopen(fileName,"r+");
	if(fp == NULL)
	{
		perror("fopen insert data:");
		exit(1);
	}

	while(1)
	{
		fseek(fp,pos*sizeof(myData),SEEK_SET);;
		fread(&slot,sizeof(myData),1,fp);
		if(slot.user != 1){
			break;
		}
		printf("collision detected!!!\n");
		pos++;
		pos %= 10;
	}

	printf("position = %d\n",pos);
	fseek(fp, pos*sizeof(myData),SEEK_SET);
	fwrite(&data,sizeof(myData),1,fp);

	fclose(fp);


}



void printData(char* fileName)
{

	FILE *fp;
	myData data;
	int i;

	fp = fopen(fileName,"r+");
	if(fp == NULL)
	{
		perror("fopen insert data:");
		exit(1);
	}

	for(i = 0; i< 7;i++)
	{
		fseek(fp, i*sizeof(myData),SEEK_SET);
		fread(&data,sizeof(myData),1,fp);
		printf("car:%d user flag:%d key:%d name: %s\n",(i+1),data.user,data.key,data.name);
		memset(&data,0,sizeof(myData));
	}

}


int main(int argc, char const *argv[])
{
	/* code */

	char *carCompanies[7]={
		"ferarri",
		"lambo",
		"audi",
		"bmw",
		"jaguar",
		"ford",
		"maruti"
	};
	int i,key;

	initTable("myHashTable", 10);
	//insertData()
	for (i = 0; i< 7;i++)
	{
		key = hashKey(carCompanies[i]);
		printf("hashkey for %s is %d\n",carCompanies[i],key);
		insertData(key, carCompanies[i], "myHashTable");
	}

	printData("myHashTable");
	return 0;
}