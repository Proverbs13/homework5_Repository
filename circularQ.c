/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */
#include <stdio.h>
#include <stdlib.h>
#define MAX_QUEUE_SIZE 4 //큐의 사이즈, 원형큐에선 3개공간 가능

typedef char element; //자료형 char의 별명을 element로 설정
typedef struct {
	element queue[MAX_QUEUE_SIZE]; // 구조체 내 문자열 queue 선언
	int front, rear;
}QueueType;  //QueueType 자료형 구조체 선언

QueueType* createQueue();    // 큐 생성 함수+동적 할당
int freeQueue(QueueType* cQ);// 큐의 동적메모리 해제 함수
int isEmpty(QueueType* cQ);  // 큐가 꽉찼는지 확인하는 함수
int isFull(QueueType* cQ);   // 큐가 비었는지 확인하는 함수
void enQueue(QueueType* cQ, element item); //큐의 원소를 삽입하는 함수
void deQueue(QueueType* cQ, element* item);//큐의 원소를 삭제하는 함수 
void printQ(QueueType* cQ);  // 큐의 내용 출력함수
void debugQ(QueueType* cQ);  // 큐의 세부내용 출력함수
element getElement();        // 입력할 원소를 받는 함수

int main(void)
{
	printf("[----- [이찬] [2019038029] -----]\n");
	QueueType* cQ = createQueue(); //QueueType형 포인터 cQ 선언후, 큐생성함수 호출해 동적할당 및 내부 값 초기화
	element data; // 사용자가 큐에 입력할 원소-데이터
	char command; // 사용의 명령-커맨드

	do {
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //사용자의 명령 입력

		switch (command) {
		case 'i': case 'I': 
			data = getElement(); // 함수호출해 큐에 집어넣을 데이터 값 받아옴
			enQueue(cQ, data);   // 큐 원소 삽입 함수 호출
			break;               // 스위치문 탈출
		case 'd': case 'D':
			deQueue(cQ, &data);  // 큐 원소 삭제함수 호출
			break;				 // 스위치문 탈출
		case 'p': case 'P':
			printQ(cQ);          // 큐 출력함수 호출
			break;				 // 스위치문 탈출
		case 'b': case 'B':      
			debugQ(cQ);          // 큐 세부정보 출력함수 호출
			break;				 // 스위치문 탈출
		case 'q': case 'Q':
			freeQueue(cQ);       // 큐의 동적 메모리 해제 함수 호출
			break;				 // 스위치문 탈출 -> 반복문 조건에 따라 반복문 종료
		default:    //조건 외 다른 것 입력 시
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;  // 스위치문 탈출
		}
	} while (command != 'q' && command != 'Q'); //q 입력시 까지 반복문 실행
	return 1;// 메인함수 종료
}

QueueType* createQueue()
{
	QueueType* cQ; // QueueType형 포인터 cQ 선언
	cQ = (QueueType*)malloc(sizeof(QueueType)); //QueueType구조체 포인터 cQ가 가리키는 공간 동적 메모리 할당
	cQ->front = 0; //큐에 사용할 front 초기화 
	cQ->rear = 0;  //큐에 사용할 rear 초기화
	return cQ;     // cQ 반환
}

int freeQueue(QueueType* cQ) 
{
	if (cQ == NULL) return 1; // cq가 아무것도 가리키지않는다면 함수 바로종료
	free(cQ); // cQ가 가리키는 공간의 동적메모리 해제
	return 1; //함수종료
}

element getElement()
{
	element item; //char형 변수 item
	printf("Input element = ");
	scanf(" %c", &item);
	return item; //입력 받아 item 반환
}

int isEmpty(QueueType* cQ)
{
	if (cQ->front == cQ->rear) { //만일 큐가 비어있어 현재상태에서  front와 rear 가 같다면
		return 1; // 1 반환하며 함수종료
	}
	return 0; // 아니라면 0반환하며 함수종료
}

int isFull(QueueType* cQ)
{
	cQ->rear = ((cQ->rear) + 1) % MAX_QUEUE_SIZE; //modulo 연산을 이용해 큐의 rear 1칸 이동시킴
	if (cQ->front == cQ->rear) { //만일 1칸 이동시킨 rear가 front와 같다면
		//1칸 이동 취소후 함수종료
		if (cQ->rear == 0) //1칸 이동한 rear가 0 이라면(이동 전의 rear는 (최대 큐크기-1))
			cQ->rear = MAX_QUEUE_SIZE - 1;
		else
			cQ->rear--; // 그 외 경우 는 1씩 빼줌
		return 1; //1 반환하며 함수종료
	}
	return 0; //이동시킨rear != front 일시 0반환
}

void enQueue(QueueType* cQ, element item)
{
	
	if (isFull(cQ)) { //함수 호출해 큐가 가득찼는지 확인
	//가득 찼다면 1 반환되어 바로 아래문장 실행
		printf("Circular Queue is full!"); 
		return ; //함수종료
	}
	cQ->queue[cQ->rear] = item; //큐의 공간이 남아있을 때 큐의 다음칸에 받아온 item값 넣어줌
	return ;//함수종료
}

void deQueue(QueueType* cQ, element* item)
{
	if (isEmpty(cQ)) { //함수 호출해 큐가 비어있는지 확인
	//비어있다면 1 반환되어 바로 아래문장 실행, 
		printf("Circular Queue is empty!");
		return ;//함수종료
	}
	cQ->front = ((cQ->front) + 1) % MAX_QUEUE_SIZE; //큐가 비어있지 않다면 front 1칸 증가(이동)
	// 증가전 front위치의 값 자체를 삭제하지는 않음
	return ; // 함수종료
}

void printQ(QueueType* cQ)
{
	int i, first, last; 

	first = (cQ->front + 1) % MAX_QUEUE_SIZE; //first는 front 다음칸
	last = (cQ->rear + 1) % MAX_QUEUE_SIZE;   //last는 rear 다음칸

	printf("Circular Queue : [");
	i = first;
	while (i != last) {
	// front위치의 큐는 어짜피 비어있으므로 그 다음칸부터 rear 위치까지 출력후 그 다음칸에서 반복문 종료 
		printf("%3c", cQ->queue[i]);
		i = (i + 1) % MAX_QUEUE_SIZE; //modulo 연산으로 배열 최대위치 다음칸 = 0으로 계산됨
	}
	printf(" ]\n");
}

void debugQ(QueueType* cQ)
{
	//구조체 내부 queue 배열에 실제로 들어가 있는 내용 출력
	printf("\n---DEBUG\n");
	for (int i = 0; i < MAX_QUEUE_SIZE; i++) 
	{   //i =0 부터 출력하다 front위치 나오면 front로 출력
		if (i == cQ->front) { //
			printf("  [%d] = front\n", i);
			continue; //아래 라인 배제, 반복문 실행
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);
	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); //현재 front, rear 출력
}

