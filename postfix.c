/* postfix.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack �댁뿉�� �곗꽑�쒖쐞, lparen = 0 媛��� ��쓬 */
typedef enum{
	lparen = 0,  /* ( �쇱そ 愿꾪샇 */
	rparen = 9,  /* ) �ㅻⅨ履� 愿꾪샇*/
	times = 7,   /* * 怨깆뀍 */
	divide = 6,  /* / �섎닓�� */
	plus = 5,    /* + �㏃뀍 */
	minus = 4,   /* - 類꾩뀍 */
	operand = 1 /* �쇱뿰�곗옄 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression�� ���ν븯�� 諛곗뿴 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix濡� 蹂�寃쎈맂 臾몄옄�댁쓣 ���ν븯�� 諛곗뿴 */
char postfixStack[MAX_STACK_SIZE];	/* postfix濡� 蹂��섏쓣 �꾪빐 �꾩슂�� �ㅽ깮 */
int evalStack[MAX_STACK_SIZE];		/* 怨꾩궛�� �꾪빐 �꾩슂�� �ㅽ깮 */

int postfixStackTop = -1;  /* postfixStack�� top */
int evalStackTop = -1;	   /* evalStack�� top */

int evalResult = 0;	   /* 怨꾩궛 寃곌낵瑜� ���� */

void postfixpush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
	char command;

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;
}

char postfixPop()
{
    char x;
    if(postfixStackTop == -1)
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--];
    }
    return x;
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
}

int evalPop()
{
    if(evalStackTop == -1)
        return -1;
    else
        return evalStack[evalStackTop--];
}

/**
 * infix expression�� �낅젰諛쏅뒗��.
 * infixExp�먮뒗 �낅젰�� 媛믪쓣 ���ν븳��.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x)
{
	return getToken(x);
}

/**
 * 臾몄옄�섎굹瑜� �꾨떖諛쏆븘, postfixExp�� 異붽�
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);
	else
		strncat(postfixExp, c, 1);
}

/**
 * infixExp�� 臾몄옄瑜� �섎굹�� �쎌뼱媛�硫댁꽌 stack�� �댁슜�섏뿬 postfix濡� 蹂�寃쏀븳��.
 * 蹂�寃쎈맂 postfix�� postFixExp�� ���λ맂��.
 */
void toPostfix()
{
	/* infixExp�� 臾몄옄 �섎굹�⑹쓣 �쎄린�꾪븳 �ъ씤�� */
	char *exp = infixExp;
	char x; /* 臾몄옄�섎굹瑜� �꾩떆濡� ���ν븯湲� �꾪븳 蹂��� */

	/* exp瑜� 利앷��쒖폒媛�硫댁꽌, 臾몄옄瑜� �쎄퀬 postfix濡� 蹂�寃� */
	while(*exp != '\0')
	{
		/* �꾩슂�� 濡쒖쭅 �꾩꽦 */

	}

	/* �꾩슂�� 濡쒖쭅 �꾩꽦 */

}
void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset()
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';
         
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation()
{
	/* postfixExp, evalStack�� �댁슜�� 怨꾩궛 */
}
