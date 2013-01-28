/*
 *  uWave.h
 *  DarwiinRemote
 *
 *  Created by Guest on 6/23/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef UWAVE_H
#define UWAVE_H

//
//how to use this library:
//1. Set "recordFlag" to 1 to record gestures in a file as template. In this mode, input gestures will be written to files named "$index.umv"
//and become templates for recognition. The number of templates is fixed and defined as "NUM_TEMPLATES".
//Set "recordFlag" to 0 in uWave.c to enable recognization. Recognization result will be returned by endGesture() as the index of the template.
//The result is also writen into a file "log.txt".
//
//2. When a gesture begins, call beginGesture(); when the gesture finishes, call endGesture();
//put the acc samples during a gesure into accBuffer like the following(x, y, z are the acceleration in g):
//	accBuffer[accIndex][0] = x*10;
//	accBuffer[accIndex][1] = y*10;
//	accBuffer[accIndex][2] = (z-1)*10;
//	accIndex++;
//Make sure (accIndex < MAX_ACC_LEN)!!!!!!!!!!!!!!!!!!!!
//beginGesure() allocates memory and initializes variables; endGesture() record/recognize the input gesture and output results
//


//macros for quantization
//(8,4) applies to 100Hz; if 50Hz, change to (4,2)...
#define QUAN_WIN_SIZE 8
#define QUAN_MOV_STEP 4


//max number of gesture templates
#define NUM_TEMPLATES 20

/////////////////////////////////////////////////////////////////////////////
#define DIMENSION 3

#define MAX_ACC_LEN 500

extern int recordFlag;

extern int tempIndex;

struct GestureStruct {
	int** data;
	int length;
};
typedef struct GestureStruct Gesture;

// 2D array to hold ACC data, each row contains (x, y, z) of an ACC sample
extern int** accBuffer;
extern int accIndex;

int** allocAccBuf(int len);
void releaseAccBuf(int** p, int len);
void beginGesture();

int endGesture(); //return the gesture index if in recognizing mode (recordFlag = 0)

int quantizeAcc(int** acc_data, int length);
int DTWdistance(int** sample1, int length1, int** sample2, int length2, int i, int j, int* table);
int DetectGesture(int** input, int length, Gesture* templates, int templateNum);
Gesture readFile(int index);
int writeFile(int** data, int len, int index);


#endif

