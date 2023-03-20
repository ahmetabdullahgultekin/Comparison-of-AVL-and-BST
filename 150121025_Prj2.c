// Ahmet Abdullah GÜLTEKÝN
// 150121025
// This program compares performance of the two special trees which are Adelson Velsky - Landis and Splay

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Preorder printing
struct Stack_Structure {
	
	//Values
	struct AVL_Tree_Structure* inputAvl;
	struct Splay_Tree_Structure* inputSplay;
	
	//Link next pointer
	struct Stack_Structure* nextInput;
};

//Construct the structure of the AVL tree
struct AVL_Tree_Structure {
	
	//Give the necessary variables
	int value, height;
	
	//AVL tree has two child, this feature belongs to BST
	struct AVL_Tree_Structure *leftchild_ptr, *rightchild_ptr; 
};

//Construct the structure of the Splay tree
struct Splay_Tree_Structure {
	
	//Give the necessary variable
	int data;
	
	//AVL tree has two child, this feature belongs to BST
	struct Splay_Tree_Structure *rightchild_ptr; 
	struct Splay_Tree_Structure *leftchild_ptr;
	
};

//Function prototypes
struct AVL_Tree_Structure *nodeInsertionAVL (int num, struct AVL_Tree_Structure *newLeaf, int* countCost, int checkExistance);
int checkViolationForAVL_TreeEquilibrium(struct AVL_Tree_Structure * certainNode);
struct AVL_Tree_Structure *rota_R(struct AVL_Tree_Structure *ptr);
struct AVL_Tree_Structure *rota_L(struct AVL_Tree_Structure *ptr);
struct Splay_Tree_Structure *nodeInsertionSplay(int num, struct Splay_Tree_Structure *latestNode, int *cmp);
int numberOfIterations(int data, int datas[], int size);
int oddOrEven(int iterations);
int findLengthOfInput(FILE* test_ptr, int* cntblnk);
void pushValue(struct AVL_Tree_Structure* treeAvlPtr, struct Splay_Tree_Structure *treeSplayPtr, struct Stack_Structure **inputPtr);
struct AVL_Tree_Structure *popTopAvl(struct Stack_Structure **popPtr, int treeType);
struct Splay_Tree_Structure *popTopSplay(struct Stack_Structure **popPtr, int treeType);
void printTreeAsPreorderTraversal(struct AVL_Tree_Structure * printedKeyAvl, struct Splay_Tree_Structure * printedKeySplay, int numbers);

//Insert the root recursively to avl tree and make tree balanced according the violation result
struct AVL_Tree_Structure *nodeInsertionAVL(int num, struct AVL_Tree_Structure *newLeaf, int *countCost, int checkExistance) {
	
	int numberOfChanges = 0;
	/*
	if(newLeaf != NULL)
	printf("num=%d,newleaf=%d\n", num, newLeaf->value);
	*/

    //If no root in tree or tree is empty, then return a new created node for root
    if (newLeaf == NULL) {
    	
        //Allocate
	    struct AVL_Tree_Structure *newChild = NULL;
	    newChild = (struct AVL_Tree_Structure*) malloc(sizeof(struct AVL_Tree_Structure));
	
	    //Throw alert for the allocation problem
	    if(newChild == NULL) {
	    	printf("Memory has not been allocated!");
	    }
	
	    //Initialize the variables
	    //Added node must be a leaf
	    (*newChild).height = 1;
	    (*newChild).value = num;
	
	    //Leaves do not have children
	    (*newChild).rightchild_ptr = NULL;
		(*newChild).leftchild_ptr = NULL;
        //*countCost += 1;
        return newChild;	
	}
    	
	//This if part goes left most of the root
	else if (num > (*newLeaf).value) {
		*countCost += 1;
		(*newLeaf).rightchild_ptr = nodeInsertionAVL(num, (*newLeaf).rightchild_ptr, countCost, checkExistance);
	}	
	
	//We find root so recursively we can add to tree our new leaf
	//This if part goes left most of the root
	else if ((*newLeaf).value > num) {
		*countCost += 1;
		(*newLeaf).leftchild_ptr = nodeInsertionAVL(num, (*newLeaf).leftchild_ptr, countCost, checkExistance);
	}					
		
	//If we find the same node with same data, we can leave it as is
	else {
		*countCost += 1;
	    return newLeaf;		
	}
	
	//We added a new node so we must revise our datas like heights of the nodes
	int rH, lH;
	
	//If the node and its children are exist then assign its heights to variables
	//If child is not exist then the height will be zero
	if((*newLeaf).rightchild_ptr != NULL) {//printf("+%d+", (*newLeaf).rightchild_ptr->value);
		rH = (*newLeaf).rightchild_ptr->height;
	}
	else
	    rH = 0;
		
	if((*newLeaf).leftchild_ptr != NULL) {//printf("-%d-", (*newLeaf).leftchild_ptr->value);
		lH = (*newLeaf).leftchild_ptr->height;
    }   
	else
	    lH = 0;
	
	//printf("lH=%d,rH=%d", lH, rH);
	
	int absx = 0, prev;
	//We can adjust the certain node's height by comparing heights of children
	if(rH > lH) {
		prev = (*newLeaf).height;
		(*newLeaf).height = rH + 1;
		absx = abs(prev - (*newLeaf).height);
		//*countCost += absx * 2;
	} else if(lH > rH) {
		prev = (*newLeaf).height;
		(*newLeaf).height = lH + 1;
		absx = abs(prev - (*newLeaf).height);
		//*countCost += absx * 2;
	} else {
		prev = (*newLeaf).height;
		(*newLeaf).height = rH + 1;
		absx = abs(prev - (*newLeaf).height);
		//*countCost += absx * 2;
	}
	
	//printf("*%d*", checkViolationForAVL_TreeEquilibrium(newLeaf));
	
	struct AVL_Tree_Structure *temp = NULL;
	//Next step is -if needed- rotate the tree
	//First left
	if(2 <= checkViolationForAVL_TreeEquilibrium(newLeaf)) {
		
		int leftData = newLeaf->leftchild_ptr->value;
		//Second Left
		if(leftData > num) {
			*countCost+=1;
			return rota_R(newLeaf);
		}
		//Second Right
		else if(leftData < num) {
			//Firstly left rotation and next right rotation
			temp = rota_L(newLeaf->leftchild_ptr);
			newLeaf->leftchild_ptr = temp;
			*countCost+=2;
			return rota_R(newLeaf);
		}
	}
	//First right
	else if(-2 >=checkViolationForAVL_TreeEquilibrium(newLeaf)) {
		
		int rightData = newLeaf->rightchild_ptr->value;
		//Second Left
		if(rightData > num) {//printf("AAA");
		    //Firstly right rotation and next left rotation
		    temp = rota_R(newLeaf->rightchild_ptr);
			newLeaf->rightchild_ptr = temp;
			*countCost+=2;
			return rota_L(newLeaf);
		}
		//Second Right
		else if(rightData < num) {//printf("BBB");
		    *countCost+=1;
			return rota_L(newLeaf);
		}
	}
	
	//Better to remember that newLeaf is root now
	struct AVL_Tree_Structure *root = NULL;
	root = newLeaf;
    return root;				            
}

//Return the violation valu which is the difference of heights of the left and right child
int checkViolationForAVL_TreeEquilibrium(struct AVL_Tree_Structure * certainNode) {
	
	//Initialization
	int difference = 0;
	int rightAltitude, leftAltitude;
	
	//If node is null the no need to check
	if(certainNode == NULL) return;
	
	//Take the key values of children while statements check whether the children is null or not
	if(certainNode->rightchild_ptr != NULL)
		rightAltitude = certainNode->rightchild_ptr->height;
	else
	    rightAltitude = 0;
	    
	if(certainNode->leftchild_ptr != NULL)
		leftAltitude = certainNode->leftchild_ptr->height;
	else
	    leftAltitude = 0;
	
	//Get difference and return value
	difference = leftAltitude - rightAltitude;
	
	//printf("+%d+", leftAltitude);
	
	return difference;
}

//Right rotation function for AVL tree
struct AVL_Tree_Structure *rota_R(struct AVL_Tree_Structure *ptr) {
	
	//Create two temporary pointer
	struct AVL_Tree_Structure *temp1 = NULL;
	struct AVL_Tree_Structure *temp2 = NULL;
	
	//For swaping or rotating assign to pointer left child of root and right child of left child of root
	temp1 = (*ptr).leftchild_ptr;
	temp2 = (*ptr).leftchild_ptr->rightchild_ptr;
	
	//Perform rotation
	(*temp1).rightchild_ptr = ptr;
	(*ptr).leftchild_ptr = temp2;
	
	//Check whether children are null or not
	//If not then update the heights
	if((*ptr).leftchild_ptr == NULL && (*ptr).rightchild_ptr == NULL)
		ptr->height = 1;
	else if(ptr->leftchild_ptr != NULL && ptr->rightchild_ptr != NULL)
		ptr->height = ((ptr->leftchild_ptr->height > ptr->rightchild_ptr->height) ? ptr->leftchild_ptr->height : ptr->rightchild_ptr->height) + 1;
	else {
		ptr->height = ((ptr->leftchild_ptr != NULL) ? ptr->leftchild_ptr->height : ptr->rightchild_ptr->height) + 1;
	}
	
	//Update for other child
	if((*temp1).leftchild_ptr == NULL && (*temp1).rightchild_ptr == NULL)
		temp1->height = 1;
	else if(temp1->leftchild_ptr != NULL && temp1->rightchild_ptr != NULL)
		temp1->height = ((temp1->leftchild_ptr->height > temp1->rightchild_ptr->height) ? temp1->leftchild_ptr->height : temp1->rightchild_ptr->height) + 1;
	else {
		temp1->height = ((temp1->leftchild_ptr != NULL) ? temp1->leftchild_ptr->height : temp1->rightchild_ptr->height) + 1;
	}
	
    //printf("ph%d*%d*,th%d*%d*R\n",ptr->value,ptr->height,temp1->value, temp1->height);
    
    //This is root now
	struct AVL_Tree_Structure *root = NULL;
	root = temp1;
	return root;
}

struct AVL_Tree_Structure* rota_L(struct AVL_Tree_Structure *ptr) {
	
	//Create two temporary pointer
	struct AVL_Tree_Structure *temp1 = NULL;
	struct AVL_Tree_Structure *temp2 = NULL;
	
	//For swaping or rotating assign to pointer left child of root and right child of left child of root
	temp1 = (*ptr).rightchild_ptr;
	temp2 = (*ptr).rightchild_ptr->leftchild_ptr;
	
	//Perform rotation
	(*temp1).leftchild_ptr = ptr;
	(*ptr).rightchild_ptr = temp2;
	
	//Check whether children are null or not
	//If not then update the heights
	if((*ptr).leftchild_ptr == NULL && (*ptr).rightchild_ptr == NULL)
		ptr->height = 1;
	else if(ptr->leftchild_ptr != NULL && ptr->rightchild_ptr != NULL)
		ptr->height = ((ptr->leftchild_ptr->height > ptr->rightchild_ptr->height) ? ptr->leftchild_ptr->height : ptr->rightchild_ptr->height) + 1;
	else {
		ptr->height = ((ptr->leftchild_ptr != NULL) ? ptr->leftchild_ptr->height : ptr->rightchild_ptr->height) + 1;
	}
	
	//Update for other child
	if((*temp1).leftchild_ptr == NULL && (*temp1).rightchild_ptr == NULL)
		temp1->height = 1;
	else if(temp1->leftchild_ptr != NULL && temp1->rightchild_ptr != NULL)
		temp1->height = ((temp1->leftchild_ptr->height > temp1->rightchild_ptr->height) ? temp1->leftchild_ptr->height : temp1->rightchild_ptr->height) + 1;
	else {
		temp1->height = ((temp1->leftchild_ptr != NULL) ? temp1->leftchild_ptr->height : temp1->rightchild_ptr->height) + 1;
	}
	
    //printf("ph%d*%d*,th%d*%d*L\n",ptr->value,ptr->height,temp1->value, temp1->height);
    
    //This is root now
	struct AVL_Tree_Structure *root = NULL;
	root = temp1;
	return root;
}

//Insert the root recursively and make tree balanced according the violation result
struct Splay_Tree_Structure *nodeInsertionSplay(int num, struct Splay_Tree_Structure *latestNode, int *cmp) {
	
	//If root is empty
	if(latestNode == NULL) {
		
		//Create new Node
		struct Splay_Tree_Structure *newRoot = NULL;
		newRoot = (struct Splay_Tree_Structure*)malloc((sizeof(struct Splay_Tree_Structure)));
		
		//Check memory allocation
		if(newRoot == NULL) {
			printf("Out of Memory Situation");
			return NULL;
		}
		
		//Assign nulls to new node
		(*newRoot).rightchild_ptr = NULL;
		(*newRoot).leftchild_ptr = NULL;
		(*newRoot).data = num;
		
		//Count comparison and return
		*cmp +=2;
		return newRoot;
	} 
	
	if((*latestNode).data == num) {
		//Count comparison and return
		*cmp +=2;
		return latestNode;
	}
	
	if(num > (*latestNode).data) {
		//Count comparison and return
		*cmp +=2;
		(*latestNode).rightchild_ptr = nodeInsertionSplay(num, (*latestNode).rightchild_ptr, cmp);
	}
	else if(num < (*latestNode).data) {
		//Count comparison and return
		*cmp +=1;
		(*latestNode).leftchild_ptr = nodeInsertionSplay(num, (*latestNode).leftchild_ptr, cmp);
	} 
	else {
		//Unreachable
		//Count comparison and return
		*cmp +=1;
		return latestNode;
	}
		            
    return latestNode;
}

//Find the key how many times occured 
int numberOfIterations(int data, int datas[], int size) {
	
	//Initialize
	int numIter = 0, i;
	
	//Search step by step
	for(i = 0; i < size; i++) {
		if(data == datas[i])
		    numIter++;
	}
	
	//return number of iterations
	return numIter;
}

//Check as boolean value and determine the new key is exist
int oddOrEven(int iterations) {
	
	//If not exist return one
	if(iterations == 1)
	    return 1;
	    
    //If exist return two
	else if(iterations > 1)
	    return 2;
	    
	//If there is a problem then throw alert to console
	else
	    printf("Inappropriate input!\t");
	    return;
}

//Determine there are how many numbers in input
int findLengthOfInput(FILE* test_ptr, int *cntblnk) {
	
	//Read char-by-char and count blanks
	int length = 0, countBlank = 0;
	char c;
	
	//If char exist then perform reading
	if(c = fgetc(test_ptr)) {
		
		//Determine what it is blank or number
		if((c >= '0' && c <= '9') || c == ' ')
		   	length++;
			
		//Check reading is halted
    	if(EOF == c) {
    		printf("Search has terminated!");
    		//Close file, process over
    		fclose(test_ptr);
    		return;
		}
		
		else {
		    //there is not a problem, complete reading
	    	while(EOF != c) {
	    		
	    	//Read char
         	c = fgetc(test_ptr);
         	
         	//Count blank
         	if(c == ' ') countBlank++;
         	
         	//Count length
    		if((c >= '0' && c <= '9') || c == ' ')
		   	length++;	
		    }
		}	
	}
	else {//Problem found
		printf("First char not found!");
	}
	
	//printf("%d", length);
	
	//change pointer integer value
	*cntblnk = countBlank;
	//Close file in order to read again
	fclose(test_ptr);
	
	return length;
}

//According to parameters, push values to certain tree
void pushValue(struct AVL_Tree_Structure* treeAvlPtr, struct Splay_Tree_Structure *treeSplayPtr, struct Stack_Structure **inputPtr) {
	
	//Choose right tree
	if(treeSplayPtr == NULL) {
        printf("%d ", (*treeAvlPtr).value);
	} else {
		printf("%d ", (*treeSplayPtr).data);
	}
	
	//Allocate memory
	struct Stack_Structure *tempptr = (struct Stack_Structure*) malloc(sizeof(struct Stack_Structure));
	
	//Run for splay and assign according to certain tree
	if(treeSplayPtr == NULL) {
		tempptr->inputAvl = treeAvlPtr;
		tempptr->inputSplay = NULL;
	} else {
		tempptr->inputSplay = treeSplayPtr;
		tempptr->inputAvl = NULL;
	}
	
	//Run for AVL and assign according to certain tree
	if(inputPtr == NULL) {
		(*tempptr).nextInput = NULL;
	} else {
		(*tempptr).nextInput = (*inputPtr);
	}
	(*inputPtr) = tempptr;
}

//Pop the top of AVL tree stack
struct AVL_Tree_Structure *popTopAvl(struct Stack_Structure **popPtr, int treeType) {

	//Create variables
	struct Stack_Structure *stackPtr = NULL;
	struct AVL_Tree_Structure *tmpptrAvl = NULL;
	
	//Do not change pointer which is given as a parameter
	stackPtr = *popPtr;
	
	//Halt if stack is empty
	if(!getTopAvlStack(&stackPtr)) {
		return NULL;
	}

	//Run function if it is AVL
	if(treeType == 1) {
		
		//Free pointer and return previous one
		tmpptrAvl = stackPtr->inputAvl;
		*popPtr = stackPtr->nextInput;
		free(stackPtr);
		return tmpptrAvl;
	}
	
	/*
	if(top == NULL) {
		printf("Underflow occured!");
	} else {
		int temp;
		temp = (*top).input;
		tmpptr = top;
		top = (*top).nextInput;
		free(tmpptr);
		return temp;
	}
	return -1;
	*/
}

struct Splay_Tree_Structure *popTopSplay(struct Stack_Structure **popPtr, int treeType) {
	
	//Assign variables
	struct Stack_Structure *stackPtr = NULL;
	struct Splay_Tree_Structure *tmpptrSplay = NULL;
	
	//Assign for protection
	stackPtr = *popPtr;
	
	//Halt if stack is empty
	if(!getTopSplayStack(&stackPtr)) {
		return NULL;
	}
	
	//Run if tree is splay
	if(treeType == 2) {
		
		//Free the top
		tmpptrSplay = stackPtr->inputSplay;
	    *popPtr = stackPtr->nextInput;
    	free(stackPtr);
    	return tmpptrSplay;
	}
}

//Call the top of stack
int getTopAvlStack(struct Stack_Structure **upperPtr) {
	
	//Return top value
	int value = -1;
	if(*upperPtr != NULL)
	    value = (**upperPtr).inputAvl->value;
	else
	    return 0;
	    
	return value;
}

int getTopSplayStack(struct Stack_Structure **upperPtr) {
	
	//Return top value
	int value = -1;
	if(*upperPtr != NULL)
	    value = (**upperPtr).inputSplay->data;
	else
	    return 0;
	    
	return value;
}

//Print whole tree as preorder
void printTreeAsPreorderTraversal(struct AVL_Tree_Structure * printedKeyAvl, struct Splay_Tree_Structure * printedKeySplay, int numbers) {
	
	int i;
	for(i = 0;i < numbers;i++) {
		//pushValue((*printedKey).value);
	}
	
	//Variables
	struct Stack_Structure *topNode = NULL;
	struct AVL_Tree_Structure *nextAvl = printedKeyAvl;
	
	//Assign height of AVL
	int heightAvl;
	if(printedKeyAvl != NULL) {
	    heightAvl = (*printedKeyAvl).height;	
	}
	
	//Variables
	int previous = 0;
	int boolean = 0;
	
	//Print for Avl
	if(printedKeySplay == NULL) {
		
		//Loop for each elemant
	    do {
	    	//Go to left subtree
		    if(nextAvl != NULL) {
			    previous = (*nextAvl).value;
		        pushValue(nextAvl, NULL, &topNode);
		        nextAvl = (*nextAvl).leftchild_ptr;
		        heightAvl-=1;
		        //printf("*%d*", height);
		    } else { //Go to right subtree
		    
		        //Finish loop if printing is over
		        if(topNode == NULL) {
		        	boolean = -1;
			    } else { //pop in order to go upper node
			    	//Call 1 for avl
			    	nextAvl = popTopAvl(&topNode, 1);
			        heightAvl+=1;
			        previous = (*nextAvl).value;
			        nextAvl = (*nextAvl).rightchild_ptr;				
			    }
	    	}
	    } while(!boolean);
	}
	
	//Variables
	struct Stack_Structure * topNodeSplay = NULL;
	struct Splay_Tree_Structure *nextSplay = printedKeySplay;
	
	//Print for Splay
	if(printedKeyAvl == NULL) {
		
		//Loop for each elemant
		do {
			//Go to left subtree
		    if(nextSplay != NULL) {
			    previous = (*nextSplay).data;
		        pushValue(NULL, nextSplay, &topNodeSplay);
		        nextSplay = (*nextSplay).leftchild_ptr;
		        //printf("*%d*", height);
		    } else { //Go to right subtree
		    
		        //Finish loop if printing is over
		        if(topNodeSplay == NULL) {
		        	boolean = -1;
			    } else { //pop in order to go upper node
			    	//Call 2 for splay
			    	nextSplay = popTopSplay(&topNodeSplay, 2);
			        previous = (*nextSplay).data;
			        nextSplay = (*nextSplay).rightchild_ptr;				
			    }
	    	}
	    } while(!boolean);
	}
	
	/*
	
	-Alternative way to print preorder-
	
	//Create two temporary pointer for both child
	struct AVL_Tree_Structure * tmpR = NULL;
	struct AVL_Tree_Structure * tmpL = NULL;
	
	//Terminate function if node is null
	if(printedKeySplay == NULL) return;

	//Recursively print each data of nodes
	printf("%d ", (*printedKeySplay).data);

	//Assign child
	if(printedKeySplay->leftchild_ptr != NULL) {
		tmpL = (*printedKeySplay).leftchild_ptr;
	}	
	//Assign child
	if((*printedKeySplay).rightchild_ptr != NULL) {
	    tmpR = (*printedKeySplay).rightchild_ptr;
	}
	
	//Recursive call for left
	printTreeAsPreorderTraversal(NULL, tmpL, numbers);

	//Recursive call for right
	printTreeAsPreorderTraversal(NULL, tmpR, numbers);
	*/
}

//Main function takes input file as an argument
int main(int argc, char *argv[]) {

    // Define variables for input
    char chr;
    int i, j, digit, number, numCol, prevDig, arrayLength = 0;
    int cntBlank = 0;
	FILE * input_ptr;
    
    // Open the input file in reading mode
    input_ptr = fopen(argv[1], "r");

    // Check file for error
    if (1 >= argc || input_ptr == NULL) {
        printf("Error occured! \n");
        return 0;
    }
    // If there is not an error, then perform
	arrayLength = findLengthOfInput(input_ptr, &cntBlank);
   	int nums[(intptr_t)cntBlank];
   	
   	//Error occures
	if(!(input_ptr = fopen(argv[1], "r"))) 
	printf("Error occured! \n");
    
    //Iinitialize variables
    char processArray[arrayLength];
    numCol = 0, number = 0;
    
    //Read each char and determine number of digits
    if(fgets(processArray, arrayLength, input_ptr)) {
    	for(i = 0, j = 0; i < arrayLength; i++) {
    		
    		//If it is number
    		if(processArray[i] >= '0' && processArray[i] <= '9') {
    			prevDig = number;
    			number = (int) processArray[i] - '0';
    			prevDig *= pow(10, numCol);
    			number += prevDig;
    			numCol++;
			}
			//If it is whitespace
			else if((processArray[i] == ' ') || !(processArray[i])) {
				nums[j] = number;
				j++;

				numCol = 0;
				number = 0;
				prevDig = 0;
			}
			else {
				//printf("-Inappropriate char!-\n");
				//This is not a whitespace or number
				continue;
			}
				
		}
	}
	//Alert
	else {
		printf("String has not been taken!");
	}
	
	/*
	for(i = 0; i < (intptr_t)cntBlank; i++) {
	printf("nums[%d] = %d\n", i, nums[i]);
	}
	*/
	
	//Variables for insertion
	int process, k = 0;
	int costAVL = 0, costSplay = 0;
	struct AVL_Tree_Structure *vertexAVL = NULL;
	
	//Run two times for two tree
	//First for AVL
	for(i = 0; i < (intptr_t)cntBlank; i++) {
		
	process = oddOrEven(numberOfIterations(nums[i], nums, sizeof(nums) / sizeof(nums[0])));
	//printf("i=%d, num=%d\n",i, nums[i]);
	    
	//vertex = createDataForTree(nums[i]);
    vertexAVL = nodeInsertionAVL(nums[i], vertexAVL, &costAVL, process);
    	    
    //if(i == 0)
    //    printTreeAsPreorderTraversal(vertex);
	}

	struct Splay_Tree_Structure *vertexSply = NULL;
	
	//Second for Splay
	for(i = 0; i < (intptr_t)cntBlank; i++) {
		
	process = oddOrEven(numberOfIterations(nums[i], nums, sizeof(nums) / sizeof(nums[0])));
	//printf("i=%d, num=%d\n",i, nums[i]);
	    
	//vertex = createDataForTree(nums[i]);
    vertexSply = nodeInsertionSplay(nums[i], vertexSply, &costSplay);
    
	}
	
	//Print trees
	
	//AVL
	printf("AVL tree:\n");
	printTreeAsPreorderTraversal(vertexAVL, NULL, cntBlank);
	printf("\nTotal cost: %d\n", costAVL);
	
	//Splay
	printf("\nSplay tree:\n");
	printTreeAsPreorderTraversal(NULL, vertexSply, cntBlank);
	printf("\nTotal cost: %d", costSplay);
 
    // Close the input file
    fclose(input_ptr);	

	return 0;
}
