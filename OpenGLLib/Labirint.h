#pragma once  //нужно , чтобы файл подключался только один раз 
#include<stdio.h>
#include<stdlib.h>
struct walls* readfromfile(const char* fname );
struct walls
{
	int kolstrok;
	int kolstolb;
	bool** p; // указывать ,где содержатся стены по этому укзателю будут лежать другие указатели 
};


struct walls* readfromfile(const char* fname) // возвращать указатель на walls
{
	FILE *fp = fopen(fname,"r");
	if (fp == NULL)
		return 0;
	struct walls *sten=(struct walls*)malloc(sizeof(struct walls));//указатель на стены
	fscanf(fp, "%d %d", &sten->kolstrok,&sten->kolstolb);
	sten->p=(bool**)malloc(sizeof(bool*)*sten->kolstrok);//не учитывает количество элементов // выделение памяти для указателя на кажду строку 
	for (int i = 0; i < sten->kolstrok; i++)     //*(sten).kolstrok
	{
		sten->p[i] = (bool *)malloc(sizeof(bool)*sten->kolstolb);
	}
	for (int i = 0; i < sten->kolstrok;i++)    // внешний по строкам
	{
		for (int j = 0; j < sten->kolstolb; j++) // по элементам в cтолбцам 
		{
			fscanf(fp, "%d", &sten->p[i][j]);
		}                                          
	}
	fclose(fp);
	return(sten);
}


