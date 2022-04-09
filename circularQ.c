#include <stdio.h>
#include <stdlib.h>
#define MAX_QUEUE_SIZE 4 //ť�� ������, ����ť���� 3������ ����

typedef char element; //�ڷ��� char�� ������ element�� ����
typedef struct {
	element queue[MAX_QUEUE_SIZE]; // ����ü �� ���ڿ� queue ����
	int front, rear;
}QueueType;  //QueueType �ڷ��� ����ü ����

QueueType* createQueue();    // ť ���� �Լ�+���� �Ҵ�
int freeQueue(QueueType* cQ);// ť�� �����޸� ���� �Լ�
int isEmpty(QueueType* cQ);  // ť�� ��á���� Ȯ���ϴ� �Լ�
int isFull(QueueType* cQ);   // ť�� ������� Ȯ���ϴ� �Լ�
void enQueue(QueueType* cQ, element item); //ť�� ���Ҹ� �����ϴ� �Լ�
void deQueue(QueueType* cQ, element* item);//ť�� ���Ҹ� �����ϴ� �Լ� 
void printQ(QueueType* cQ);  // ť�� ���� ����Լ�
void debugQ(QueueType* cQ);  // ť�� ���γ��� ����Լ�
element getElement();        // �Է��� ���Ҹ� �޴� �Լ�

int main(void)
{
	printf("[----- [����] [2019038029] -----]\n");
	QueueType* cQ = createQueue(); //QueueType�� ������ cQ ������, ť�����Լ� ȣ���� �����Ҵ� �� ���� �� �ʱ�ȭ
	element data; // ����ڰ� ť�� �Է��� ����-������
	char command; // ����� ���-Ŀ�ǵ�

	do {
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //������� ��� �Է�

		switch (command) {
		case 'i': case 'I': 
			data = getElement(); // �Լ�ȣ���� ť�� ������� ������ �� �޾ƿ�
			enQueue(cQ, data);   // ť ���� ���� �Լ� ȣ��
			break;               // ����ġ�� Ż��
		case 'd': case 'D':
			deQueue(cQ, &data);  // ť ���� �����Լ� ȣ��
			printf("%c",cQ->queue[data]);
			break;				 // ����ġ�� Ż��
		case 'p': case 'P':
			printQ(cQ);          // ť ����Լ� ȣ��
			break;				 // ����ġ�� Ż��
		case 'b': case 'B':      
			debugQ(cQ);          // ť �������� ����Լ� ȣ��
			break;				 // ����ġ�� Ż��
		case 'q': case 'Q':
			freeQueue(cQ);       // ť�� ���� �޸� ���� �Լ� ȣ��
			break;				 // ����ġ�� Ż�� -> �ݺ��� ���ǿ� ���� �ݺ��� ����
		default:    //���� �� �ٸ� �� �Է� ��
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;  // ����ġ�� Ż��
		}
	} while (command != 'q' && command != 'Q'); //q �Է½� ���� �ݺ��� ����
	return 1;// �����Լ� ����
}
QueueType* createQueue()
{
	QueueType* cQ; // QueueType�� ������ cQ ����
	cQ = (QueueType*)malloc(sizeof(QueueType)); //QueueType����ü ������ cQ�� ����Ű�� ���� ���� �޸� �Ҵ�
	cQ->front = 0; //ť�� ����� front �ʱ�ȭ 
	cQ->rear = 0;  //ť�� ����� rear �ʱ�ȭ
	return cQ;     // cQ ��ȯ
}
int freeQueue(QueueType* cQ) 
{
	if (cQ == NULL) return 1; // cq�� �ƹ��͵� ����Ű���ʴ´ٸ� �Լ� �ٷ�����
	free(cQ); // cQ�� ����Ű�� ������ �����޸� ����
	return 1; //�Լ�����
}
element getElement()
{
	element item; //char�� ���� item
	printf("Input element = ");
	scanf(" %c", &item);
	return item; //�Է� �޾� item ��ȯ
}
int isEmpty(QueueType* cQ)
{
	if (cQ->front == cQ->rear) { //���� ť�� ����־� ������¿���  front�� rear �� ���ٸ�
		printf("Circular Queue is empty!"); 
		return 1; // 1 ��ȯ�ϸ� �Լ�����
	}
	else
		return 0; // �ƴ϶�� 0��ȯ�ϸ� �Լ�����
}
int isFull(QueueType* cQ)
{
	cQ->rear = ((cQ->rear) + 1) % MAX_QUEUE_SIZE; //modulo ������ �̿��� ť�� rear 1ĭ �̵���Ŵ
	if (cQ->front == cQ->rear) { //���� 1ĭ �̵���Ų rear�� front�� ���ٸ�
		//1ĭ �̵� ����� �Լ��� ����á���� �˸��� �Լ�����
		if (cQ->rear == 0) //1ĭ �̵��� rear�� 0 �̶��(�̵� ���� rear�� (�ִ� ťũ��-1))
			cQ->rear = MAX_QUEUE_SIZE - 1;
		else
			cQ->rear--; // �� �� ��� �� 1�� ����
		printf("Circular Queue is full!"); 
		return 1; //1 ��ȯ�ϸ� �Լ�����
	}
	else
		return 0; //�̵���Ųrear != front �϶� 0��ȯ
}
void enQueue(QueueType* cQ, element item)
{
	if (isFull(cQ)) { //�Լ� ȣ���� ť�� ����á���� Ȯ��
	//���� á�ٸ� 1 ��ȯ�Ǿ� �ٷ� �Ʒ����� ����
		return ; //�Լ�����
	}
	cQ->queue[cQ->rear] = item; //ť�� ������ �������� �� �̵��� rear �κп� �Էµ� �� �ʱ�ȭ
	return ;//�Լ�����
}
void deQueue(QueueType* cQ, element* item)
{
	if (isEmpty(cQ)) //�Լ� ȣ���� ť�� ����ִ��� Ȯ��
	//����ִٸ� 1 ��ȯ�Ǿ� �ٷ� �Ʒ����� ���� -> �Լ�����
		return ;//�Լ�����
	
	cQ->front = ((cQ->front) + 1) % MAX_QUEUE_SIZE; //ť�� ������� �ʴٸ� front 1ĭ ����(�̵�)
	//*item = cQ->queue[cQ->front];  = ������ front��ġ�� ť ���� ������� ����� ���� *item (data)�� �ʱ�ȭ
	//  �� �ַ�ǿ��� ������ ���� ������ �ڵ�� ���� ť ������ �־ � �뵵���� �������� ���� �ּ�ó���Ͽ���.
	return; // �Լ�����
}
void printQ(QueueType* cQ)
{
	int i, first, last; 

	first = (cQ->front + 1) % MAX_QUEUE_SIZE; //first�� front ����ĭ
	last = (cQ->rear + 1) % MAX_QUEUE_SIZE;   //last�� rear ����ĭ

	printf("Circular Queue : [");
	i = first;
	while (i != last) {//���� ť������ front��ġ ĭ�� ������� ������
	// �� ����ĭ���� rear ��ġ���� ����� �� ����ĭ���� �ݺ��� ���� 
		printf("%3c", cQ->queue[i]);
		i = (i + 1) % MAX_QUEUE_SIZE; //modulo �������� �迭 �ִ���ġ ����ĭ = 0���� ����
	}
	printf(" ]\n");
}
void debugQ(QueueType* cQ)
{
	//����ü ���� queue �迭�� ������ �� �ִ� ���� ���
	printf("\n---DEBUG\n");
	for (int i = 0; i < MAX_QUEUE_SIZE; i++) 
	{   //i =0 ���� ����ϴ� front��ġ ������ front�� ���
		if (i == cQ->front) { 
			printf("  [%d] = front\n", i);
			continue; //�Ʒ� ���� ����, �ݺ��� ����
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);
	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); //���� front, rear ���
}

