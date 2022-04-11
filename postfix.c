#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_STACK_SIZE 10 //�ִ� ���û����� 10
#define MAX_EXPRESSION_SIZE 20 //�ִ� ���ڿ� ũ�� 20 

typedef enum{ // ���þȰ� ���� �ٱ������� �켱���� ������ ���� ���� �ʱ�ȭ x
	lparen,  /* ( ���� ��ȣ */   //0���� �����ؼ� 1 ��ŭ�� �ö� 
	rparen,  /* ) ������ ��ȣ*/
	times,   /* * ���� */
	divide,  /* / ������ */
	plus,    /* + ���� */
	minus,   /* - ���� */
	operand  /* �ǿ����� */
} precedence;

static int isp[]={0,9,7,7,5,5,1};  //���ó��ο����� �켱����
static int icp[]={10,9,7,7,5,5,1}; //���ÿܺο����� �켱����
// '(' �� ���ÿ� ���ö��� ���� �켱������ �ٷ� ��������
//���� ���ο����� ���� �켱������ �ٸ� �����ڵ��� ���ÿ� ���� �� �ְ� ��

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
	printf("[----- [����] [2019038029] -----]\n");
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

void charCat(char* c) //�����ϳ��� ���޹޾�, ����ǥ��Ŀ� �߰�
{
	if (postfixExp == '\0') //����ǥ��� �迭 ��������� = ó������ ���ö�
		strncpy(postfixExp, c, 1); //���� ���� ��ĭ �����ؿ�
	else
		strncat(postfixExp, c, 1); //����ִ� ���ڿ��� �߰�
}

void toPostfix()
{
	/* infixExp�� ���� �ϳ����� �б����� ������ */
	char *exp = infixExp; 
	char x; /* �����ϳ��� �ӽ÷� �����ϱ� ���� ���� */

	/* exp�� �������Ѱ��鼭, ���ڸ� �а� postfix�� ���� */
	while(*exp != '\0')
	{   // ����ǥ��� ���ڿ� ���� ���� ������ ��
		if(getPriority(*exp) == operand) // �����̸�
		{
			x = *exp; 
        	charCat(&x); //�ٷ� ����ǥ��� ���ڿ��� �־���
		}

        else if(getPriority(*exp) == rparen) // ')' �̸�
        {
        	while((x = postfixPop()) != '(') { //���ÿ��� ���� �ؼ� ������ ���� '('�� �� ������
        		charCat(&x); //������ ���ڸ� ����ǥ��� ���ڿ��� �־���
        	}// '(' �� �˵����� ����ǥ��Ŀ� ���ڰ� ������ �����鼭 �ݺ��� ����
        }
        else //���������
        {   // ���� �Ȱ� �ٱ������� �켱������ �ٸ��� ������ '(' �� ���ö� �������� �ذ�
            while( isp[ getPriority(postfixStack[postfixStackTop]) ] >=  icp[ getPriority(*exp) ] )
            {   //���� ���þ��� ž���ִ� ���� �켱������ ���� ������ �켱�������� ũ�ų� ���ٸ�, �۾��������� �ݺ�
            	x = postfixPop();//�� ���� 
            	charCat(&x);     //����ǥ��� ���ڿ��� �־���
            }
            postfixPush(*exp); //���� ���ڸ� ���ÿ� Ǫ�� - else ���ǿ��� ��� �����
        }
        exp++; //����ǥ��� ���ڿ� ������ġ�� �̵�, ���� ���ǿ��� ������ �ι��� ���ö����� �ݺ�
	}
    while(postfixStackTop != -1) //����ǥ��Ŀ� �ٿ�����ϴ� ���ÿ� �����ִ� ���ڰ� ���� �� �ֱ⶧����
    {                            // ������ �����������
    	x = postfixPop();
    	charCat(&x);     //���� ������ ���ڵ� ����ǥ��Ŀ� �ٿ���
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
	int opr1, opr2, i;               //�ǿ����� ���� 2�� , �ݺ����� i
	int len = strlen(postfixExp); // �ϼ��� ����ǥ��� ���� Ȯ��
	char symbol;        //����ǥ��� �����ϳ��� ������ ����
	evalStackTop = -1;  // ���� ���� ž �ʱ�ȭ

	for(i = 0; i < len; i++) //��������� ���� ���� �ݺ�
	{
		symbol = postfixExp[i]; //�ѹ��� �� ����ǥ��� ���ڿ����� �̾Ƴ�
		if(getToken(symbol) == operand) { // �ǿ����� �Ͻ� 
			evalPush(symbol - '0');   // ���� ���ÿ� Ǫ��
			// ���� ����̹Ƿ� ���ڿ��� �ƽ�Ű���ڰ� 47 ='0'�� ���� ���ϴ� ���ڸ� ����
		}
		else { //�����ڰ� ������ ����� �ǿ����� �ΰ� �ʿ�
			opr2 = evalPop(); // ���ÿ� ���߿� ���� ���� ���� ��
			opr1 = evalPop(); // ���� �׿��ִ� ���� ��
			switch(getToken(symbol)) { //�����ڿ� ���� �ٸ� ������� ���뽺�ÿ� �ٽ� push
			case plus: evalPush(opr1 + opr2); break;
			case minus: evalPush(opr1 - opr2); break;
			case times: evalPush(opr1 * opr2); break;
			case divide: evalPush(opr1 / opr2); break;
			default: break;
			}
		}
	}
	evalResult = evalPop(); //���ÿ�����ִ� ���������� ���� ����� ���ؼ� ��������� ����
}