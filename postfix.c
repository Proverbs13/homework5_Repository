#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10 //�ִ� ���û����� 10
#define MAX_EXPRESSION_SIZE 20

/* stack ������ �켱����, lparen = 0 ���� ���� */
typedef enum{
	lparen,  /* ( ���� ��ȣ */
	rparen,  /* ) ������ ��ȣ*/
	times,   /* * ���� */
	divide,  /* / ������ */
	plus,    /* + ���� */
	minus,   /* - ���� */
	operand  /* �ǿ����� */
} precedence;

static int isp[]={0,9,7,7,5,5,1};
static int icp[]={10,9,7,7,5,5,1};


char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression�� �����ϴ� ���ڿ� */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix�� ����� ���ڿ��� �����ϴ� ���ڿ� */
char postfixStack[MAX_STACK_SIZE];	/* postfix�� ��ȯ�� ���� �ʿ��� ���� */
int evalStack[MAX_STACK_SIZE];		/* ����� ���� �ʿ��� ���� */
int postfixStackTop = -1;  /* postfixStack�� top */
int evalStackTop = -1;	   /* evalStack�� top */
int evalResult = 0;	   /* ��� ����� ���� */

void postfixpush(char x); //����ǥ��� ���� Ǫ�� �Լ�
char postfixPop();        //����ǥ��� ���� �� �Լ� - ���� ��
void evalPush(int x);     //���� ���� Ǫ�� �Լ�
int evalPop();            //���� ���� �� �Լ� - ����� ��
void getInfix();          //���� ǥ��� �Է� �Լ�
precedence getToken(char symbol); // ���ڿ����� ������ ���ڿ����� �������� ��� �Լ�
precedence getPriority(char x);   // ����ǥ��Ĺ��ڿ����� ������ ���ڿ����� �������� ��� �Լ�
// �� getToken ȣ��
void charCat(char* c); //����ǥ���->����ǥ������� ���鶧 ���ڿ� �߰� �Լ�
void toPostfix();      //����ǥ���->����ǥ��� ��ȯ �Լ�
void debug();          //�Է�&������� ���γ��� ���
void reset();          //�Էµ� ���� �ʱ�ȭ �Լ�
void evaluation();     //����ǥ����� ����ϴ� �Լ�
int main()
{
	char command; //����ڰ� �Է��� ���
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
			getInfix(); //����ǥ����Է�
			break;
		case 'p': case 'P':
			toPostfix(); //����ǥ���->����ǥ������� ��ȯ
			break;
		case 'e': case 'E':
			evaluation(); //����ǥ��� ���
			break;
		case 'd': case 'D': //���γ��� ���
			debug();
			break;
		case 'r': case 'R': //�Էµ� �� �ֱ�ȭ
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
    postfixStack[++postfixStackTop] = x; //�������� ���ڿ� ���� Ǫ��
} //�������꽺�� ž�� ��ĭ �ø���, �޾ƿ� �� �������

char postfixPop()
{
    char x; 
    if(postfixStackTop == -1) //���ÿ� �ƹ��͵� ������� ������
        return '\0'; //������ ��������� �ǹ��ϴ� �ι��� ����
    else { //������ ������� ������
    	x = postfixStack[postfixStackTop--];
    } //�̾Ƴ��� ��ġ�� ���� x�� �ް�,top�� ��ĭ ����
    return x; //���� ���� x ��ȯ
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x; //���� ���ڿ� ���� Ǫ��
}// ���� ž�� ��ĭ �ø���, �޾ƿ� �� �������

int evalPop()
{
    if(evalStackTop == -1) //���ÿ� �ƹ��͵� ������� ������
        return -1; //�����߻� - �Լ�����
    else
        return evalStack[evalStackTop--];
} //�̾Ƴ��� ��ġ�� �� ���� / top�� ��ĭ���� 

/*
 * infix expression�� �Է¹޴´�.
 * infixExp���� �Էµ� ���� �����Ѵ�.
 */
void getInfix()
{
    printf("Type the expression >>> "); 
    scanf("%s",infixExp); //���� ǥ��� �Է¹���   
}

precedence getToken(char symbol) 
{   // ���� �Ѱ� �޾ƿ� ���������� ��ȯ
	switch(symbol) { 
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand; //�����ڰ� �ƴ� �� operand ��ȯ
	}
}

precedence getPriority(char x)
{   //getToken�� ���� ����
	return getToken(x); 
}  

/*
 * �����ϳ��� ���޹޾�, postfixExp�� �߰�
 */
void charCat(char* c)
{
	if (postfixExp == '\0') //����ǥ��� �迭 ��������� = ó������ ���ö�
		strncpy(postfixExp, c, 1); //���� ���� ��ĭ �����ؿ�
	else
		strncat(postfixExp, c, 1); //����ִ� ���ڿ��� �߰�
}

/**
 * infixExp�� ���ڸ� �ϳ��� �о�鼭 stack�� �̿��Ͽ� postfix�� �����Ѵ�.
 * ����� postfix�� postFixExp�� ����ȴ�.
 */
void toPostfix()
{
	/* infixExp�� ���� �ϳ����� �б����� ������ */
	char *exp = infixExp; 
	char x; /* �����ϳ��� �ӽ÷� �����ϱ� ���� ���� */

	/* exp�� �������Ѱ��鼭, ���ڸ� �а� postfix�� ���� */
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
            {   //���� ž���ִ� ��ȣ�켱������ ���ε��� �༮�� �켱�������� ũ�ų� ����������
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
{   //���γ��� ����Լ�
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp); //�Է��� ����ǥ���
	printf("postExp =  %s\n", postfixExp);//��ȯ�� ����ǥ���
	printf("eval result = %d\n", evalResult); // �����
	printf("postfixStack : "); //����ǥ��� ���� ���γ��� ���
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);
	printf("\n");
}

void reset()
{
	infixExp[0] = '\0';  //����ǥ��� ���ڿ� ����
	postfixExp[0] = '\0';//����ǥ��� ���ڿ� ����

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0'; //����ǥ��� ���� ����
    //���ڿ��ε���0��ġ�� '\0' �ʱ�ȭ�ϸ� ���ڿ��� �̿�(strcpy��)���� ����� ��ó�� �̿밡��������
	//�������� ����Ҷ��� ���ڿ��迭�� ���ִ� ��簪�� �ʱ�ȭ �صξ�� �ϱ� ������ �ݺ������� ���     
	postfixStackTop = -1; // ����ǥ��� ���� ž �ʱ�ȭ
	evalStackTop = -1;    // ���� ���� ž �ʱ�ȭ
	evalResult = 0;       // ����� �ʱ�ȭ
}

void evaluation()
{
	/* postfixExp, evalStack�� �̿��� ��� */
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

