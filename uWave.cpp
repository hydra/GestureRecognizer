/*
 *  DarwiinRemote
 *
 *  Created by Guest on 6/23/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
// DTWLib.cpp : Defines the entry point for the console application.
//
#include "uWave.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE* flog;
int recordFlag = 0; // record gesture to file or recognize it
int tempIndex = 0; //index of template when recording gesture

int** accBuffer;
int accIndex;

////////////////////////////////////////////////////////////////////////////////////////////////////
int** allocAccBuf(int len){
	int** ret = (int**)malloc(sizeof(int*)*len);
	int i;
	for( i = 0; i < len; i++)
		ret[i] = (int*)malloc(sizeof(int)*DIMENSION);
	return ret;
}

void releaseAccBuf(int** p, int len) {
	int i;

	if (!(p && len)) {
	  return;
	}

	for( i = 0; i < len; i++)
		free(p[i]);
	free(p);
}

//routine at beginning
void beginGesture() {
	accBuffer = allocAccBuf(MAX_ACC_LEN);
	accIndex = 0;

	//output results to a file
	flog = fopen("log.txt", "a");
	if( flog != NULL )
		fprintf(flog, "**********begin gesture\n");

}

//routine at end: record gesture or detect gesture
int endGesture() {
	int ret = -1, i;
	if( recordFlag == 1) {
	//write to file
		writeFile(accBuffer, accIndex,tempIndex);
		tempIndex++;
	} else {
		if( flog != NULL)
			fscanf(flog, "reach end of Gesture\n");
	//recognize gesture
		// 1) quantize acc
		accIndex = quantizeAcc(accBuffer, accIndex);
		if( flog != NULL)
			fprintf(flog, "finish quantizing AccBuffer\n");

		// 2) reads in templates, assume fixed number of templates now
		Gesture templates[NUM_TEMPLATES];
		for( i = 0; i < NUM_TEMPLATES; i++) {
			templates[i] = readFile(i);
			if( flog != NULL)
				fprintf(flog, "finish reading template %d\n", i);
			templates[i].length = quantizeAcc(templates[i].data, templates[i].length);
		}
		ret = DetectGesture(accBuffer, accIndex, templates, NUM_TEMPLATES);
		if( flog != NULL)
			fprintf(flog, "gesture is # %d\n", ret);
		for(i = 0; i < NUM_TEMPLATES; i++)
			releaseAccBuf(templates[i].data, templates[i].length);
	}

	if( flog != NULL)
		fclose(flog);
	releaseAccBuf(accBuffer, MAX_ACC_LEN);
	return ret;
}
//quantize

int quantizeAcc(int** acc_data, int length) {
	int i=0, j, k = 0, l, window = QUAN_WIN_SIZE, sum;
	int **temp = (int**)malloc(sizeof(int*)*(length/QUAN_MOV_STEP + 1));
	for(i = 0; i < length/QUAN_MOV_STEP + 1; i++)
		temp[i] = (int*)malloc(sizeof(int)*DIMENSION);
	//take moving window average
	i = 0;
	while(i < length) {
		if( i + window > length)
			window = length - i;
		for( l = 0; l < DIMENSION; l++) {
			sum = 0;
			for( j = i; j < window+i; j++)
				sum += acc_data[j][l];
			temp[k][l] = sum*1.0/window;
		}
		k++;
		i += QUAN_MOV_STEP;
	}//while
	//nonlinear quantization and copy quantized value to original buffer
	for( i = 0; i < k; i++)
		for( l = 0; l < DIMENSION; l++) {
			if( temp[i][l] > 10 ) {
				if( temp[i][l] > 20)
					temp[i][l] = 16;
				else
					temp[i][l] = 10 + (temp[i][l]-10)/10*5;
			} else if( temp[i][l] < -10) {
				if( temp[i][l] < -20)
					temp[i][l] = -16;
				else
					temp[i][l] = -10 + (temp[i][l] + 10)/10*5;
			}
			acc_data[i][l] = temp[i][l];
		}
	return k;
}//quantize

//DTW algorithm, return distance
int DTWdistance(int** sample1, int length1, int** sample2, int length2, int i, int j, int* table) {

	if( i < 0 || j < 0)
		return 100000000;
	int tableWidth = length2;
	int localDistance = 0;
	int k;
	for( k = 0; k < DIMENSION; k++)
		localDistance += ((sample1[i][k]-sample2[j][k])*(sample1[i][k]-sample2[j][k]));

	int sdistance, s1, s2, s3;

	if( i == 0 && j == 0) {
		if( table[i*tableWidth+j] < 0)
			table[i*tableWidth+j] = localDistance;
		return localDistance;
	} else if( i==0) {
		if( table[i*tableWidth+(j-1)] < 0)
			sdistance = DTWdistance(sample1, length1, sample2, length2, i, j-1, table);
		else
			sdistance = table[i*tableWidth+j-1];
	} else if( j==0) {
		if( table[(i-1)*tableWidth+ j] < 0)
			sdistance = DTWdistance(sample1, length1, sample2, length2, i-1, j, table);
		else
			sdistance = table[(i-1)*tableWidth+j];
	} else {
		if( table[i*tableWidth+(j-1)] < 0)
			s1 = DTWdistance(sample1, length1, sample2, length2, i, j-1, table);
		else
			s1 = table[i*tableWidth+(j-1)];
		if( table[(i-1)*tableWidth+ j] < 0)
			s2 = DTWdistance(sample1, length1, sample2, length2, i-1, j, table);
		else
			s2 = table[(i-1)*tableWidth+ j];
		if( table[(i-1)*tableWidth+ j-1] < 0)
			s3 = DTWdistance(sample1, length1, sample2, length2, i-1, j-1, table);
		else
			s3 = table[(i-1)*tableWidth+ j-1];
		sdistance = s1 < s2 ? s1:s2;
		sdistance = sdistance < s3 ? sdistance:s3;
	}
	table[i*tableWidth+j] = localDistance + sdistance;
	return table[i*tableWidth+j];
}

//write/read gesture file
Gesture readFile(int index) {
	FILE *fp;
	char fname[10];
	int data[DIMENSION*MAX_ACC_LEN], i=0,j, n;
	Gesture ret;

	ret.data = 0;
	ret.length = 0;


	sprintf(fname, "%d", index);

	fp = fopen(strcat(fname, ".uwv"), "r");
	if( fp == NULL)
		return ret;
	while(!feof(fp)) {
		fscanf(fp, "%d %d %d \n", &data[i], &data[i+1], &data[i+2]);
		i += 3;
	}
	n = (i-3)/DIMENSION;
	ret.length = n;
	ret.data = allocAccBuf(n);
	for(i = 0; i < n; i++)
		for(j = 0; j < DIMENSION; j++)
			ret.data[i][j] = data[(i*DIMENSION)+j];

	fclose(fp);
	return ret;
}

int writeFile(int** data, int len, int index){
	FILE *fp;
	int i,j;
	char fname[10];
	sprintf(fname, "%d.uwv", index);
	fprintf(flog, "writing to %s\n", fname);
	fp = fopen(fname, "w");
	if( fp == NULL) {
	  fprintf(fp, "failed\n");
		return -1;
	}
	for(i = 0; i < len; i++) {
		for( j = 0; j < DIMENSION; j++) {
			fprintf(fp, "%d ", data[i][j]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
	return 0;
}

//detect gesture, return detected template index
int DetectGesture(int** input, int length, Gesture* templates, int templateNum)
{
	if( length <= 0)
		return -1;
	int i, ret = 0,j;

	int distances[NUM_TEMPLATES];
	//int table[MAX_ACC_LEN/QUAN_MOV_STEP*MAX_ACC_LEN/QUAN_MOV_STEP];
	int* table;
	for( i = 0; i < templateNum; i++) {
		table = (int*) malloc(length * templates[i].length*sizeof(int));
		for( j = 0; j < length*templates[i].length; j++)
			table[j] = -1;

		distances[i] = DTWdistance(input, length, templates[i].data, templates[i].length, length-1, templates[i].length-1, table);
		distances[i] /= (length + templates[i].length);
		free(table);
	}

	for( i = 1; i < templateNum; i++) {
		if( distances[i] < distances[ret]) {
			ret = i;
		}
	}

	return ret;
}

