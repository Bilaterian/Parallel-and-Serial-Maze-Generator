#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <unistd.h>

typedef struct node{
	int i;
	int j;
	struct node *next; //what's immediately underneath this stack
}stack;

void push(int i, int j, stack **top);
void pop(stack **top);
void shuffleNeighbors(int **neighbors, int seed);
void printTheMaze(char ***theMaze, int arraySize);

int main(int argc, char *argv[]){
	int arraySize = 11;
	int seed = 0;
	int i = 0;
	int j = 0;

	int numThreads = 4;
	
	char **theMaze;
	
	if(argc > 1){
		for(i = 0; i < argc - 1; i++){
			if(strcmp(argv[i],"-n") == 0){
				arraySize = atoi(argv[i + 1]);
			}
			if(strcmp(argv[i],"-s") == 0){
				seed = atoi(argv[i + 1]);
			}
		}
	}
	else{
		arraySize = 11;
		seed = 0;
	}
	
	srand(seed);
	
	theMaze = (char **)malloc(sizeof(char *) * arraySize);
	for(i = 0; i < arraySize; i++){
		theMaze[i] = (char *)malloc(sizeof(char) * arraySize);
	}

	for(i = 0; i < arraySize; i++){
		for(j = 0; j < arraySize; j++){
			theMaze[i][j] = '.';
		}
	}
	
	//fill maze here
	//from here to the line printing the maze must be parallel
#	pragma omp parallel num_threads(numThreads) private(i, j) shared(theMaze)
{
#	ifdef _OPENMP
	int myRank = omp_get_thread_num();
#	else
	int myRank = 0;
#	endif
	
	int *neighborPriority;
	int iter = 0;
	char letterRank = (char)(myRank + 48);
	int numPush = 0;
	
	stack *top;
	top = NULL;
	
	if(myRank == 0){
		push(1, 1, &top);
#	pragma omp critical
{
		if(theMaze[1][1] == '.'){
			theMaze[1][1] = letterRank;
			numPush = numPush + 1;
		}
}
	}
	else if(myRank == 1){
		push(1, arraySize - 2, &top);
#	pragma omp critical
{
		if(theMaze[1][arraySize - 2] == '.'){
			theMaze[1][arraySize - 2] = letterRank;
			numPush = numPush + 1;
		}
}
	}
	else if(myRank == 2){
		push(arraySize - 2, 1, &top);
#	pragma omp critical
{
		if(theMaze[arraySize - 2][1] == '.'){
			theMaze[arraySize - 2][1] = letterRank;
			numPush = numPush + 1;
		}
}
	}
	else if(myRank == 3){
		push(arraySize - 2, arraySize - 2, &top);
#	pragma omp critical
{
		if(theMaze[arraySize - 2][arraySize - 2] == '.'){
			theMaze[arraySize - 2][arraySize - 2] = letterRank;
			numPush = numPush + 1;
		}
}
	}
	
	neighborPriority = (int *)malloc(sizeof(int) * 4);
	for(i = 0; i < 4; i++){
		neighborPriority[i] = i + 1;
	}

	while(top!= NULL){
		for(i = 0; i < rand() % 4; i++){
			shuffleNeighbors(&neighborPriority, seed);
		}
		
		i = top->i;
		j = top->j;
		pop(&top);
		
		for(iter = 0; iter < 4; iter++){
			if(neighborPriority[iter] == 1){ //north
#	pragma omp critical
{
				if((i + 2 < arraySize - 1) && (theMaze[i + 2][j] == '.')){
					theMaze[i + 2][j] = letterRank;
					theMaze[i + 1][j] = letterRank;
					push(i + 2, j, &top);
					numPush = numPush + 1;
				}
}
			}
			else if(neighborPriority[iter] == 2){ //south
#	pragma omp critical
{
				if((i - 2 > 0) && (theMaze[i - 2][j] == '.')){
					theMaze[i - 2][j] = letterRank;
					theMaze[i - 1][j] = letterRank;
					push(i - 2, j, &top);
					numPush = numPush + 1;
				}
}
			}
			else if(neighborPriority[iter] == 3){ //east
#	pragma omp critical
{
				if((j + 2 < arraySize - 1) && (theMaze[i][j + 2] == '.')){
					theMaze[i][j + 2] = letterRank;
					theMaze[i][j + 1] = letterRank;
					push(i, j + 2, &top);
					numPush = numPush + 1;
				}
}
			}
			else{ //west
#	pragma omp critical
{
				if((j - 2 > 0) && (theMaze[i][j - 2] == '.')){
					theMaze[i][j - 2] = letterRank;
					theMaze[i][j - 1] = letterRank;
					push(i, j - 2, &top);
					numPush = numPush + 1;
				}
}
			}
		}
	}
	printf("process %d has claimed %d nodes\n", myRank, numPush);
}
	printTheMaze(&theMaze, arraySize);

	return 0;
}

void push(int i, int j, stack **top){
	stack *localTop = *top;
	stack *newNode = (stack *)malloc(sizeof(stack));
	newNode->i = i;
	newNode->j = j;
	if(localTop == NULL){
		newNode->next = NULL;
	}
	else{
		newNode->next = localTop;
	}
	*(top) = newNode;
}

void pop(stack **top){
	stack *temp = *top;
	if(top != NULL){
		*(top) = temp->next;
	}
	free(temp);
}

void shuffleNeighbors(int **neighbors, int seed){
	int *shuffle = *neighbors;
	int temp;
	int index1 = seed % 4;
	int index2 = rand() % 4;
	
	temp = shuffle[index1];
	shuffle[index1] = shuffle[index2];
	shuffle[index2] = temp;
}

void printTheMaze(char ***theMaze, int arraySize){
	char **mazeCopy = *theMaze;
	int i = 0; 
	int j = 0;
	
	for(i = 0; i < arraySize; i++){
		for(j = 0; j < arraySize; j++){
			printf(" %c", mazeCopy[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}