#include<stdio.h>
#include<stdlib.h>
typedef struct ListNode { // 노드 타입
	int coef;
	int expon;
	struct ListNode* link;
} ListNode;
typedef struct ListType { // 리스트 헤더 타입
	int size;
	ListNode* head;
	ListNode* tail;
} ListType;
// 오류 함수
void error(char* message)
{
	fprintf(stderr, "%s\n", message);
	exit(1);
}
// 리스트 헤더 생성 함수
ListType* create()
{
	ListType* plist = (ListType*)malloc(sizeof(ListType));
	plist->size = 0;
	plist->head = plist->tail = NULL;
	return plist;
}
// plist는 연결 리스트의 헤더를 가리키는 포인터, coef는 계수,
// expon는 지수
void insert_last(ListType* plist, int coef, int expon)
{
	ListNode* temp =
		(ListNode*)malloc(sizeof(ListNode));
	if (temp == NULL) error("메모리 할당 에러");
	temp->coef = coef;
	temp->expon = expon;
	temp->link = NULL;
	if (plist->tail == NULL) {
		plist->head = plist->tail = temp;
	}
	else {
		plist->tail->link = temp;
		plist->tail = temp;
	}
	plist->size++;
}
// list3 = list1 + list2
void poly_add(ListType* plist1, ListType* plist2, ListType* plist3)
{
	ListNode* a = plist1->head;
	ListNode* b = plist2->head;
	int sum;
	while (a && b) {
		if (a->expon == b->expon) { // a의 차수 > b의 차수
			sum = a->coef + b->coef;
			if (sum != 0) insert_last(plist3, sum, a->expon);
			a = a->link; b = b->link;
		}
		else if (a->expon > b->expon) { // a의 차수 == b의 차수
			insert_last(plist3, a->coef, a->expon);
			a = a->link;
		}
		else { // a의 차수 < b의 차수
			insert_last(plist3, b->coef, b->expon);
			b = b->link;
		}
	}
	// a나 b중의 하나가 먼저 끝나게 되면 남아있는 항들을 모두
// 결과 다항식으로 복사
	for (; a != NULL; a = a->link)
		insert_last(plist3, a->coef, a->expon);
	for (; b != NULL; b = b->link)
		insert_last(plist3, b->coef, b->expon);
}
//
//
void poly_print(ListType* plist)
{
	ListNode* p = plist->head;
	printf("polynomial = ");
	for (; p; p = p->link) {
		if (p->link == NULL)
			printf("%d^%d  ", p->coef, p->expon);
		else printf("%d^%d + ", p->coef, p->expon);
	}
	printf("\n");
}
char* rvs(char* c2,int len) {
	char* c = malloc((len+1) * sizeof(char));
	int loop_i,loop_len=len-1;

	for (
		loop_i = 0; 
		loop_i < len;
		loop_i++, loop_len--
		) 
	{
		c[loop_i] = c2[loop_len];
	}
	c[loop_i] = '\0';
	return c;
}
void poly_save(ListType* list_x, char *x) {
	int j = 0;
	int coef_length;
	char* coef_c = malloc(2*sizeof(char));
	char* expon_c = malloc(2*sizeof(char));

	for (int i = 0;; i++) {
		if (x[i] == '\0')  break; 
		
		if (x[i] == 'x') {
			for (int coef_location = i-1,loop_i=1; ; coef_location--, loop_i++) {

				if (coef_location == -1 || x[coef_location] == '+') {
					coef_length = loop_i-1;
					coef_c[loop_i -1] = '\0';
					break; 
				}
				else {
					coef_c = realloc(coef_c, loop_i * sizeof(char));
					char c_num = x[coef_location];
					coef_c[loop_i - 1] = c_num;
				}
			}coef_c = rvs(coef_c, coef_length);//coef에 해당하는 부분을 정수로 바꾸기윈한 c 선언
			for (int expon_location = i+1, loop_i =1;;expon_location++, loop_i++) {
				if (x[expon_location] == '+' || x[expon_location] == '\0') {
					expon_c[loop_i -1] = '\0';
					i = expon_location;
					break;
				}
				else {
					expon_c = realloc(expon_c, loop_i * sizeof(char));
					char c_num = x[expon_location];
					expon_c[loop_i - 1] = c_num;
				}
			}

			int coef_number = atoi(coef_c);
			int expon_number = atoi(expon_c);

			insert_last(list_x, coef_number, expon_number);
		}
	}
	free(coef_c);
	//free(expon_c);
}
int main(void)
{
	ListType* list1, * list2, * list3;
	// 연결 리스트 헤더 생성
	list1 = create();
	list2 = create();
	list3 = create();
	// 다항식 1을 생성

	char c[100];
	printf("첫번째 다항식을 입력하십시오 : ");
	scanf_s("%s", &c,100);
	poly_save(list1, c);

	printf("두번째 다항식을 입력하십시오 : ");
	scanf_s("%s", &c,100);
	poly_save(list2, c);

	
	poly_print(list1);
	poly_print(list2);
	// 다항식 3 = 다항식 1 + 다항식 2
	poly_add(list1, list2, list3);
	poly_print(list3);
	free(list1); free(list2); free(list3);
}
