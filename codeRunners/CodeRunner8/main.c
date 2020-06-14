#include <stdio.h>
#include <stdlib.h>


typedef struct Node
{
	char * _data; // pointer to data
	struct Node * _next;
} Node;

typedef struct Stack
{
	Node *_top;
} Stack;


int isEmptyStack(Stack* stack)
{
	if(stack != NULL)
	{
		return stack->_top == NULL;
	}
	else
	{
		return 0;
	}
}


Stack* stackAlloc()
{
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	if (stack != NULL) {
		stack->_top = NULL;
	}
	else
	{
		printf("no memory\n");
	}
	return stack;
}


void freeStack(Stack** stack)
{
	while(!isEmptyStack)
}

void push(Stack* stack, char *data)
{
	if(stack!=NULL)
	{
		Node *p = (Node *) malloc(sizeof(Node));
		if(p==NULL)
		{
			printf("no memory\n");
			return;
		}
		p->_data = data;
		p->_next = stack->_top;
		stack->_top=p;
	}
}

char* pop(Stack* stack)
{
	char *s;
	Node *p;
	if(stack == NULL || stack->_top == NULL)
	{
		return NULL;
	}
	s = stack ->_top->_data;
	p = stack->_top;
	stack ->_top = p->_next;
	free(p);
	return s;
}


int main()
{

	return 0;
}
