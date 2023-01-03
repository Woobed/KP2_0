#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <windows.h>

typedef struct mbr {
	char name[8];
	int ride[7];
	int result = 0;
	int place;
	int flag[7];   /*��� �������� �����*/
	int DNQ[7];		/*���������������*/
	int ptr[7];		/*��� �������� ���������� ������ ��� ���������������*/
	int taken;
} mbr;

void put_table(int size,mbr* arr); /*������� ������� � �������*/
void sort_bubble(int size, mbr* arr);/*��������� ������ ��������*/
int count_result(int size, int j, mbr* arr); /*����������� ����� ����� 1 ����*/
int count_place(int size, mbr* arr); /*���������� ������ ���� ��� ������� �����*/
int put_for(int size, int fr, int to, mbr* arr, int i); /*���������� ������� ��� � ������ � �������� ���������*/
int count_DNQ(int size, int i, int j, mbr* arr); /*������������ ��������� ����� ��� ����������������*/
int best_in_ride(int size, int i, mbr* arr); /*���������� ������ ������ ���� � ������*/
int read_from_file(int size, mbr* arr, char* fname); /*��������� ������ �� �����*/
mbr* add_new_mas(int size, mbr* arr, int * newarr); /*������� ����� ������*/





int main() {
	setlocale(LC_ALL, "ru_RU");
	int flag = 0;
	int ptr;
	int fr,to;
	char txt[5] = ".txt";
	int size=0;
	char fname[30];
	char* fnam;
	mbr *membr;
	int j = 0;
	char s[200];
	puts("������� ��� �����");
	scanf("%s", &fname);
	strcat(fname, txt);
	fnam = fname;
	FILE* inf;
	inf = fopen(fname, "r");
	if (inf == NULL) {
		printf("����� �� ����������");
		return 0;
	}
	while (!feof(inf)) {
		fgets(s, 200, inf);
		size++;
	}
	fclose(inf);

	membr = (mbr*)malloc(size * sizeof(mbr));
	if (read_from_file(size, membr, fnam) == NULL) {
		printf("������ � ������ ���������� � ����");
		return 0;
	}
	for (int j = 0; j < size; j++) {
		printf("%s %d %d %d %d %d %d %d\n\n", membr[j].name, membr[j].ride[0], membr[j].ride[1], membr[j].ride[2], membr[j].ride[3], membr[j].ride[4], membr[j].ride[5], membr[j].ride[6]);

		for (int k = 0; k < 7; k++) { /*������� flag ��� ������*/
			if (membr[j].ride[k] % 10 == 0) {
				membr[j].ride[k] = (membr[j].ride[k] / 10)+1;
				membr[j].flag[k] = 1;
				continue;
			}
			if (membr[j].ride[k] % 10 != 0)
				membr[j].flag[k] = 0;
			
		}
	}
	for (int j = 0; j < size; j++) { /*������� DNQ ��� ������*/
		for (int k = 0; k < 7; k++) {
			if (membr[j].ride[k] < 0) membr[j].DNQ[k] = 1;
			else membr[j].DNQ[k] = 0;
		}
	}
	for (int j = 0; j < size; j++) { /*���������� ����� ��� ��������������� � ������� �����*/
			for (int k = 0; k < 7; k++) {
				if (membr[j].DNQ[k] == 1) membr[j].ride[k] = count_DNQ(size, j, k, membr);
			}
		membr[j].result=count_result(size,j,membr);
		membr[j].place = 0;
	}
		for (int j = 1; j <= size; j++) {
			membr[count_place(size,membr)].place = j;
	}
	while (flag == 0) { /*������� ����*/
		printf("������� ������� -> 1\n������������� -> 2\n������� ������� � ���������� ����� -> 3\n�������� ������� � ���� -> 4\n������ � ������ -> 5\n�������� ������ ������� -> 6\n������� ������ -> 7\n������� ��������� -> 8\n��������� -> 0\n");
		scanf("%d", &ptr);
		switch (ptr) {
		case 1: {
			system("cls");
			put_table(size,membr);
			break;
		}
		case 2: {
			sort_bubble(size,membr);
			system("cls"); 
			printf("������ �������������\n\n");
			put_table(size, membr);
			break;
		}
		case 3: {
			printf("������� ������� �������� ���������\n");
			scanf("%d", &fr);
			printf("������� ������� �������� ���������\n");
			scanf("%d", &to);
			for (int i = 0; i < size; i++) {
				if (put_for(size, fr, to, membr, i) == 1) printf("\n%s %d", membr[i].name, membr[i].result);
			}
			printf("\n\n");
			break;
		}
		case 4: { /*������ ������ � ��������� ���� */
			FILE* sec;
			char fnamesec[60];
			puts("������� ��� �����\n");
			scanf("%s", &fnamesec);
			strcat(fnamesec, txt);
			sec = fopen(fnamesec, "w");
			if (sec == NULL) {
				printf("ERROR\n");
				break;
			}
			for (int i = 0; i < size; i++) {
				fprintf(sec, "%s ", membr[i].name);
				for (int j = 0; j < 7; j++) {
						if (membr[i].flag[j] == 1) {
							fprintf(sec, "%d0 ", membr[i].ride[j]);
							continue;
						}
						if (membr[i].DNQ[j] == 1) {
							fprintf(sec, "%d ", -membr[i].ride[j]);
							continue;
						}
						if ((membr[i].flag[j] == 0) && (membr[i].DNQ[j] == 0)) {
							fprintf(sec, "%d ", membr[i].ride[j]);
							continue;
						}
					}
				fprintf(sec, "\n");
				}
				
			
			system("cls");
			printf("������� ��������\n\n");
			if (sec != NULL) fclose(sec);
			break;
		}
		case 5: {
			int h;
			system("cls");
			put_table(size,membr);
			puts("������� ����� ������\n");
			scanf("%d", &h);
			printf("������ � %d ������ ��� %s\n\n", h, membr[best_in_ride(size,h-1, membr)].name);
			break;
		}
		case 6: { /*��������� ������ � ���������� ������������ ������*/
			int m = 0;
			int n = 0;
			int n2;
			printf("������� ����� ��������� � ����� ������\n");
			scanf("%d %d", &m, &n);
			printf("������� ����� ��������\n");
			scanf("%d", &n2);
			membr[m - 1].ride[n - 1] = n2;

			for (int j = 0; j < size; j++) { 
				for (int k = 0; k < 7; k++) {
					if (membr[j].DNQ[k] == 1) membr[j].ride[k] = count_DNQ(size, j, k, membr);
				}
				membr[j].result = count_result(size, j, membr);
				membr[j].place = 0;
			}
			for (int j = 1; j <= size; j++) {
				membr[count_place(size, membr)].place = j;
			}

			put_table(size, membr);
			break;
		}
		case 7: {
			int snum;
				puts("������� ����� ������ ��� ���������\n");
				scanf("%d", &snum);
				if (snum > size) {
					printf("������ � ����� ������� �� ����������\n");
					break;
				}
				for (int i = snum - 1; i < size - 1; i++) {
					membr[i] = membr[i + 1];
				}
				size--;
			for (int j = 0; j < size; j++) { /*���������� ����� � ����*/
				for (int k = 0; k < 7; k++) {
					if (membr[j].DNQ[k] == 1) membr[j].ride[k] = count_DNQ(size, j, k, membr);
				}
				membr[j].result = count_result(size, j, membr);
				membr[j].place = 0;
			}
			for (int j = 1; j <= size; j++) {
				membr[count_place(size, membr)].place = j;
			}
			system("cls");
			printf("������ %d ������� (%s)\n\n", snum, membr[snum - 1].name);
			put_table(size, membr);
			break;
		}
		case 8: {
			int localflag = 0;
			int localsize = 0;
			int* newarrindex;
			int newcount = 0;
			for (int i = 0; i < size; i++) {
				membr[i].taken = 0;
			}
			newarrindex = (int*)malloc(size * sizeof(int));
			system("cls");
			while (localflag == 0) {
				int h;
				printf("�������� �������� ����������\n\n������� ��������������� -> 1\n������� �������� ����� -> 2\n�������� ����� -> 3\n�������� ���� -> 4\n������� ������ -> 8\n��������� � ������� ���� -> 0\n");
				scanf("%d", &h);
				switch (h) {
				case 1: {
					system("cls");
					for (int i = 0; i < size; i++) {
						for (int j = 0; j < 7; j++) {
							if (membr[i].DNQ[j] == 1 && membr[i].taken==0) {
								newarrindex[newcount] = i;
								newcount++;
								membr[i].taken = 1;
								break;
							}
						}
					}
					printf("��������� �� ���������������\n\n");
					break;
				}
				case 2: {
					system("cls");
					for (int i = 0; i < size; i++) {
						for (int j = 0; j < 7; j++) {
							if (membr[i].flag[j] == 1 && membr[i].taken == 0) {
								newarrindex[newcount] = i;
								newcount++;
								membr[i].taken = 1;
								break;
							}
						}
					}
					printf("��������� �� �������� �����\n\n");
					break;
				}
				case 3: {
					int min, max;
					printf("������� ������� ����� ");
					scanf("%d", &min);
					printf("������� �������� ����� ");
					scanf("%d", &max);
					for (int i = 0; i < size; i++) {
						if (membr[i].result >= min && membr[i].result <= max && membr[i].taken == 0) {
							newarrindex[newcount] = i;
							newcount++;
							membr[i].taken = 1;
						}
					}
					system("cls");
					printf("��������� �� %d �� %d �����\n\n",min,max);
					break;
				}
				case 4: {
					int minp, maxp;
					printf("������� ����������� ����� ");
					scanf("%d", &minp);
					printf("������� ������������ ����� ");
					scanf("%d", &maxp);
					for (int i = 0; i < size; i++) {
						if (membr[i].place >= minp && membr[i].place <= maxp && membr[i].taken == 0) {
							newarrindex[newcount] = i;
							newcount++;
							membr[i].taken = 1;
						}
					}
					system("cls");
					printf("��������� �� %d �� %d ����\n\n", minp, maxp);
					break;
				}
				case 8: {
					mbr* newmas;
					newmas = (mbr*)malloc(newcount * sizeof(membr));
					system("cls");
					newmas=add_new_mas(newcount , membr, newarrindex);
					puts("��������� ���������:\n");
					for (int i = 0; i < newcount; i++) {
						printf("%s %d %d %d %d %d %d %d %d\n", newmas[i].name, newmas[i].ride[0], newmas[i].ride[1], newmas[i].ride[2], newmas[i].ride[3], newmas[i].ride[4], newmas[i].ride[5], newmas[i].ride[6], newmas[i].result, newmas[i].place);
					}
					printf("\n");
					break;
				}
				case 0: {
					localflag = 1;
					break;
				}
				default: {
					printf("�������� ������������ �������\n");
				}
				}
			}
			printf("\n\n");
			break;
		}
		case 0: {
			flag = 1;
			break;
		}
		default: {
			system("cls");
			printf("������ ����������� ������\n");
			break;
		}
		}
	}
	free(membr);
	return 1;
}

void put_table(int size,mbr* arr) {
	printf("����     | 1 | 2 | 3 | 4 | 5 | 6 | 7 | ���� | �����\n");
	for (int i = 0; i < size; i++) {
		printf("%6s{%d} ", arr[i].name,i+1);
		for (int j = 0; j < 7; j++) {
			if (arr[i].flag[j] == 1) {
				printf("(%d) ", arr[i].ride[j]);
				continue;

			}
			if (arr[i].DNQ[j] == 1) {
				printf("!%d! ", arr[i].ride[j]);
				continue;
			}
			if ((arr[i].flag[j] == 0) && (arr[i].DNQ[j] == 0)) {
				printf(" %-3d", arr[i].ride[j]);
				continue;
			}
		}
		printf(" %3d", arr[i].result);
		printf("%7d\n", arr[i].place);
	}
	printf("\n");
	printf("x -  ���������� ������ �� �����;\n(x) - �������� �����\n!x! - ��������������� (DNQ)\n\n");
}
void sort_bubble(int size, mbr* arr) {
	mbr d;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - 1; j++) {
			if (arr[j].result > arr[j + 1].result) {
				d = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = d;
			}
		}
	}
}
int count_result(int size, int j, mbr* arr) {
	int sum = 0;
	for (int i = 0; i < 7; i++) {
		sum += arr[j].ride[i];
	}
	return sum;
}
int count_place(int size,mbr* arr) {
	int mI=0;
	int min = 1000;
	for (int i = 0; i < size; i++) {
		if ((arr[i].result < min) && (arr[i].place == 0)) {
			min = arr[i].result;
			mI = i;
		}
	}
	return mI;
}
int put_for(int size,int fr, int to, mbr* arr,int i) {
	if (arr[i].result >= fr && arr[i].result <= to) return 1;
	else return 0;
}
int count_DNQ(int size,int i, int j, mbr* arr) {
	int d = arr[i].ride[j];
		for (int k = 0; k < size; k++) {
			if ((arr[k].ride[j] > arr[i].ride[j]) && (arr[k].DNQ[j] == 0) && (arr[k].ptr[j]==0)) {
				arr[k].ride[j] -= 1;
				arr[k].ptr[j] = 1;
			}
		}
		d = size + 1;
		return d;
	
}
int best_in_ride(int size,int i,mbr* arr) {
	int min = 1000;
	int mi;
	for (int j = 0; j < size; j++) {
		if (arr[j].ride[i] < min) {
			min = arr[j].ride[i];
			mi = j;
		}
	}
	return mi;
}
int read_from_file(int size,mbr* arr,char * fname) {
	FILE* inf;
	int point;
	int i = 0;

		inf = fopen(fname, "r");
		if (inf == NULL) {
			printf("ERROR");
			return 0;
		}
		while(!feof(inf)) {
			fscanf(inf, "%s%d%d%d%d%d%d%d", &arr[i].name, &arr[i].ride[0], &arr[i].ride[1], &arr[i].ride[2], &arr[i].ride[3], &arr[i].ride[4], &arr[i].ride[5], &arr[i].ride[6]);
			i++;
		}
			fclose(inf); 
	for (int i=0;i<size;i++)
		for (int j = 0; j < 7; j++) {
			if (arr[i].ride[j] % 10 == 0) {
				if (fabs(arr[i].ride[j]/10) > size || arr[i].name == NULL) return 0;
			}
			else{
				if (fabs(arr[i].ride[j]) > size || arr[i].name == NULL) return 0;
			}
		}
	return 1;
}

mbr* add_new_mas(int size, mbr* arr, int* newarr) {
	mbr* newmas;
	newmas = (mbr*)malloc(size * sizeof(mbr));
	for (int i = 0; i < size; i++) {
		newmas[i] = arr[newarr[i]];
	}
	return newmas;
}