#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_STACK_SIZE 10 //최대 스택사이즈 10
#define MAX_EXPRESSION_SIZE 20 //최대 문자열 크기 20 

typedef enum{ // 스택안과 스택 바깥에서의 우선순위 지정을 위해 따로 초기화 x
	lparen,  /* ( 왼쪽 괄호 */   //0부터 시작해서 1 만큼씩 올라감 
	rparen,  /* ) 오른쪽 괄호*/
	times,   /* * 곱셈 */
	divide,  /* / 나눗셈 */
	plus,    /* + 덧셈 */
	minus,   /* - 뺄셈 */
	operand  /* 피연산자 */
} precedence;

static int isp[]={0,9,7,7,5,5,1};  //스택내부에서의 우선순위
static int icp[]={10,9,7,7,5,5,1}; //스택외부에서의 우선순위
// '(' 가 스택에 들어올때는 높은 우선순위로 바로 들어오지만
//스택 내부에서는 낮은 우선순위로 다른 연산자들이 스택에 쌓일 수 있게 됨

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
	printf("[----- [이찬] [2019038029] -----]\n");
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

void charCat(char* c) //문자하나를 전달받아, 후위표기식에 추가
{
	if (postfixExp == '\0') //후위표기식 배열 비어있을때 = 처음으로 들어올때
		strncpy(postfixExp, c, 1); //들어온 문자 한칸 복사해옴
	else
		strncat(postfixExp, c, 1); //들어있는 문자열에 추가
}

void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp; 
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0')
	{   // 중위표기식 문자열 에서 문자 가져와 비교
		if(getPriority(*exp) == operand) // 숫자이면
		{
			x = *exp; 
        	charCat(&x); //바로 후위표기식 문자열에 넣어줌
		}

        else if(getPriority(*exp) == rparen) // ')' 이면
        {
        	while((x = postfixPop()) != '(') { //스택에서 팝을 해서 나오는 것이 '('가 될 때까지
        		charCat(&x); //나오는 문자를 후위표기식 문자열에 넣어줌
        	}// '(' 가 팝되지만 후위표기식에 문자가 들어가지는 않으면서 반복문 종료
        }
        else //나머지경우
        {   // 스택 안과 바깥에서의 우선순위를 다르게 설정해 '(' 가 들어올때 복잡함을 해결
            while( isp[ getPriority(postfixStack[postfixStackTop]) ] >=  icp[ getPriority(*exp) ] )
            {   //현재 스택안의 탑에있는 것의 우선순위가 들어온 문자의 우선순위보다 크거나 같다면, 작아질때까지 반복
            	x = postfixPop();//팝 실행 
            	charCat(&x);     //후위표기식 문자열에 넣어줌
            }
            postfixPush(*exp); //들어온 문자를 스택에 푸시 - else 조건에선 모두 실행됨
        }
        exp++; //중위표기식 문자열 다음위치로 이동, 위의 조건에서 마지막 널문자 나올때까지 반복
	}
    while(postfixStackTop != -1) //후위표기식에 붙여줘야하는 스택에 남아있는 문자가 있을 수 있기때문에
    {                            // 스택이 비워질때까지
    	x = postfixPop();
    	charCat(&x);     //남은 스택의 문자들 후위표기식에 붙여줌
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
	int opr1, opr2, i;               //피연산자 변수 2개 , 반복문용 i
	int len = strlen(postfixExp); // 완성된 후위표기식 길이 확인
	char symbol;        //후위표기식 문자하나를 보내줄 변수
	evalStackTop = -1;  // 계산용 스택 탑 초기화

	for(i = 0; i < len; i++) //후위연산식 길이 동안 반복
	{
		symbol = postfixExp[i]; //한문자 씩 후위표기식 문자열에서 뽑아냄
		if(getToken(symbol) == operand) { // 피연산자 일시 
			evalPush(symbol - '0');   // 계산용 스택에 푸시
			// 문자 계산이므로 문자에서 아스키문자값 47 ='0'을 빼면 원하는 숫자를 얻음
		}
		else { //연산자가 나오면 계산할 피연산자 두개 필요
			opr2 = evalPop(); // 스택에 나중에 쌓인 숫자 먼저 팝
			opr1 = evalPop(); // 먼저 쌓여있던 숫자 팝
			switch(getToken(symbol)) { //연산자에 따라 다른 계산결과를 계산용스택에 다시 push
			case plus: evalPush(opr1 + opr2); break;
			case minus: evalPush(opr1 - opr2); break;
			case times: evalPush(opr1 * opr2); break;
			case divide: evalPush(opr1 / opr2); break;
			default: break;
			}
		}
	}
	evalResult = evalPop(); //스택에들어있던 마지막으로 계산된 결과를 팝해서 최종결과에 대입
}