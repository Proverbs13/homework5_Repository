#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10 //최대 스택사이즈 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen,  /* ( 왼쪽 괄호 */
	rparen,  /* ) 오른쪽 괄호*/
	times,   /* * 곱셈 */
	divide,  /* / 나눗셈 */
	plus,    /* + 덧셈 */
	minus,   /* - 뺄셈 */
	operand  /* 피연산자 */
} precedence;

static int isp[]={0,9,7,7,5,5,1};
static int icp[]={10,9,7,7,5,5,1};


char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 문자열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 문자열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */
int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */
int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixpush(char x); //후위표기식 스택 푸시 함수
char postfixPop();        //후위표기식 스택 팝 함수 - 문자 팝
void evalPush(int x);     //계산용 스택 푸시 함수
int evalPop();            //계산용 스택 팝 함수 - 계산결과 팝
void getInfix();          //중위 표기식 입력 함수
precedence getToken(char symbol); // 문자열에서 가져온 문자에대한 열거형을 얻는 함수
precedence getPriority(char x);   // 중위표기식문자열에서 가져온 문자에대한 열거형을 얻는 함수
// ㄴ getToken 호출
void charCat(char* c); //중위표기식->후위표기식으로 만들때 문자열 추가 함수
void toPostfix();      //중위표기식->후위표기식 변환 함수
void debug();          //입력&계산결과의 세부내용 출력
void reset();          //입력된 정보 초기화 함수
void evaluation();     //후위표기식을 계산하는 함수
int main()
{
	char command; //사용자가 입력할 명령
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
			getInfix(); //중위표기식입력
			break;
		case 'p': case 'P':
			toPostfix(); //중위표기식->후위표기식으로 전환
			break;
		case 'e': case 'E':
			evaluation(); //후위표기식 계산
			break;
		case 'd': case 'D': //세부내용 출력
			debug();
			break;
		case 'r': case 'R': //입력된 값 최기화
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
    postfixStack[++postfixStackTop] = x; //후위연산 문자열 스택 푸시
} //후위연산스택 탑을 한칸 올리고, 받아온 값 집어넣음

char postfixPop()
{
    char x; 
    if(postfixStackTop == -1) //스택에 아무것도 들어있지 않으면
        return '\0'; //스택이 비어있음을 의미하는 널문자 리턴
    else { //스택이 비어있지 않으면
    	x = postfixStack[postfixStackTop--];
    } //뽑아내는 위치의 문자 x에 받고,top을 한칸 내림
    return x; //받은 문자 x 반환
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x; //계산용 문자열 스택 푸시
}// 스택 탑을 한칸 올리고, 받아온 값 집어넣음

int evalPop()
{
    if(evalStackTop == -1) //스택에 아무것도 들어있지 않으면
        return -1; //오류발생 - 함수종료
    else
        return evalStack[evalStackTop--];
} //뽑아내는 위치의 값 리턴 / top를 한칸내림 

/*
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
    printf("Type the expression >>> "); 
    scanf("%s",infixExp); //중위 표기식 입력받음   
}

precedence getToken(char symbol) 
{   // 문자 한개 받아와 열거형으로 반환
	switch(symbol) { 
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand; //연산자가 아닐 시 operand 반환
	}
}

precedence getPriority(char x)
{   //getToken과 같은 역할
	return getToken(x); 
}  

/*
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0') //후위표기식 배열 비어있을때 = 처음으로 들어올때
		strncpy(postfixExp, c, 1); //들어온 문자 한칸 복사해옴
	else
		strncat(postfixExp, c, 1); //들어있는 문자열에 추가
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp; 
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0')
	{
		if(getPriority(*exp) == operand)
		{
			x = *exp;
        	charCat(&x);
		}

        else if(getPriority(*exp) == rparen)
        {
        	while((x = postfixPop()) != '(') {
        		charCat(&x);
        	}
        }
        else
        {
            while(isp[getPriority(postfixStack[postfixStackTop])] >=  icp[ getPriority(*exp) ] )
            {   //현재 탑에있는 부호우선순위가 새로들어온 녀석의 우선순위보다 크거나 작을때까지
            	x = postfixPop();
            	charCat(&x);
            }
            postfixPush(*exp);
        }
        exp++;
	}
    while(postfixStackTop != -1)
    {
    	x = postfixPop();
    	charCat(&x);
    }
}
void debug()
{   //세부내용 출력함수
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp); //입력한 중위표기식
	printf("postExp =  %s\n", postfixExp);//변환한 후위표기식
	printf("eval result = %d\n", evalResult); // 계산결과
	printf("postfixStack : "); //후위표기식 스택 내부내용 출력
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);
	printf("\n");
}

void reset()
{
	infixExp[0] = '\0';  //중위표기식 문자열 비우기
	postfixExp[0] = '\0';//후위표기식 문자열 비우기

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0'; //후위표기식 스택 비우기
    //문자열인덱스0위치에 '\0' 초기화하면 문자열의 이용(strcpy등)에는 비워진 것처럼 이용가능하지만
	//스택으로 사용할때는 문자열배열에 들어가있는 모든값을 초기화 해두어야 하기 때문에 반복문으로 비움     
	postfixStackTop = -1; // 후위표기식 스택 탑 초기화
	evalStackTop = -1;    // 계산용 스택 탑 초기화
	evalResult = 0;       // 계산결과 초기화
}

void evaluation()
{
	/* postfixExp, evalStack을 이용한 계산 */
	int opr1, opr2, i;

	int length = strlen(postfixExp);
	char symbol;
	evalStackTop = -1;

	for(i = 0; i < length; i++)
	{
		symbol = postfixExp[i];
		if(getToken(symbol) == operand) {
			evalPush(symbol - '0');
		}
		else {
			opr2 = evalPop();
			opr1 = evalPop();
			switch(getToken(symbol)) {
			case plus: evalPush(opr1 + opr2); break;
			case minus: evalPush(opr1 - opr2); break;
			case times: evalPush(opr1 * opr2); break;
			case divide: evalPush(opr1 / opr2); break;
			default: break;
			}
		}
	}
	evalResult = evalPop();
	
}

