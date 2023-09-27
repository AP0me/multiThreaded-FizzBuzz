// OpenMP header
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct parallelFunctionInput{
  int num_threads;
  void (*theFunction)(struct parallelFunctionInput* ompParalelStruct);
  int* endNum;
  char result[100][100];
};

void omp_parallel(struct parallelFunctionInput* ompParalelStruct){
  #pragma omp parallel num_threads(ompParalelStruct->num_threads)
  ompParalelStruct->theFunction(ompParalelStruct);
}

void bufferFizzBuzz(int indexNum, int localTid, int endNum, struct parallelFunctionInput* ompParalelStruct){
  char indexNum_str[10]; 
  sprintf(indexNum_str, "%d", indexNum);
  if     ((indexNum%5==0)&&(indexNum%3==0)){ strcpy((ompParalelStruct->result)[indexNum], "fizzbuzz"  ); }
  else if( indexNum%5==0)                  { strcpy((ompParalelStruct->result)[indexNum], "buzz"      ); }
  else if( indexNum%3==0)                  { strcpy((ompParalelStruct->result)[indexNum], "fizz"      ); }
  else if((indexNum%5!=0)&&(indexNum%3!=0)){ strcpy((ompParalelStruct->result)[indexNum], indexNum_str); }
}

void fizzbuzz(struct parallelFunctionInput* ompParalelStruct){
  int endNum   = *(*ompParalelStruct).endNum;
  int localTid = omp_get_thread_num();
  int num_threads = ompParalelStruct->num_threads;

  int thisThreadShareBegin = localTid    *(endNum/num_threads);
  int thisThreadShareEnd   = (localTid+1)*(endNum/num_threads);
  for(int i=thisThreadShareBegin; i<thisThreadShareEnd; i++){
    int indexNum = i+1;
    bufferFizzBuzz(indexNum, localTid, endNum, ompParalelStruct);
  }
  
  int remaining = endNum%num_threads;
  int beginRemaining = endNum-remaining+localTid;
  if(localTid<remaining){
    int indexNum = beginRemaining+1;
    bufferFizzBuzz(indexNum, localTid, endNum, ompParalelStruct);
  }
}

int main(int argc, char* argv[]){
	int endNum=0;
  printf("enter number:");
  scanf("%d", &endNum);
  printf("input %d\n", endNum);

  void (*theFunction)(struct parallelFunctionInput*) = &fizzbuzz;
  struct parallelFunctionInput ompParalelStruct = {8, theFunction, &endNum};

  printf("--%s", &ompParalelStruct.result[0][0]);
  omp_parallel(&ompParalelStruct);
  for(int i=0; i<endNum; i++){ printf("%s\n", ompParalelStruct.result[i]); }
}
