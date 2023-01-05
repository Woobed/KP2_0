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
	int flag[7];    /*для контроля штрафных очков*/
	int DNQ[7];		/*контроля дисквалификаций*/
	int ptr[7];		/*для контроля уменьшения баллов при дисквалификации*/
	int taken;
} mbr;

void put_table(int size,mbr* arr);						/*Выводит таблицу в консоль*/
void sort_bubble(int size, mbr* arr);					/*Сортирует массив структур*/
int count_result(int size, int j, mbr* arr);			/*Высчитывает сумму очков указанной яхты*/
int count_place(int size, mbr* arr);					/*Возвращает индекс яхты для указанного места*/
int put_for(int size, int fr, int to, mbr* arr, int i); /*Возвращает индексы яхт с очками в заданном диапазоне*/
int count_DNQ(int size, int i, int j, mbr* arr);		/*просчитывант изменения очков при дисквалификациях*/
int best_in_ride(int size, int i, mbr* arr);			/*Возвращает индекс лучшей яхты в заезде*/
int read_from_file(int size, mbr* arr, char* fname);	/*Считывает данные из файла*/
mbr* add_new_mas(int size, mbr* arr, int * newarr);		/*Создает новый массив*/

int add_for_dnq(int size, mbr* arr, int* newarrindex, int newcount);							/*Добавляет в массив индексы яхт имеющих дисквалификацию*/
int add_for_wp(int size, mbr* arr, int* newarrindex, int newcount);								/*Добавляет в массив индексы яхт имеющих штрафные очки*/
int add_for_point_area(int size, mbr* arr, int* newarrindex, int newcount, int min, int max);	/*Добавляет в массив индексы яхт имеющих очки в заданном диапазоне*/
int add_for_place_area(int size, mbr* arr, int* newarrindex, int newcount, int minp, int maxp); /*Добавляет в массив индексы яхт имеющих место в заданном диапазоне*/
int del_str(int size, mbr* arr, int snum);														/*Удаляет строку таблицы*/
int put_in_file(int size, mbr* arr, char* fnamesec);											/*Записывает таблицу в файл*/





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
	puts("Введите имя файла");
	scanf("%s", &fname);
	strcat(fname, txt);
	fnam = fname;
	FILE* inf;
	inf = fopen(fname, "r");
	if (inf == NULL) {
		printf("Файла не существует");
		return 0;
	}
	while (!feof(inf)) {
		fgets(s, 200, inf);
		size++;
	}
	fclose(inf);

	membr = (mbr*)malloc(size * sizeof(mbr));
	if (read_from_file(size, membr, fnam) == NULL) {
		printf("Ошибка в данных занесенных в файл");
		return 0;
	}
	for (int j = 0; j < size; j++) {
		printf("%s %d %d %d %d %d %d %d\n\n", membr[j].name, membr[j].ride[0], membr[j].ride[1], membr[j].ride[2], membr[j].ride[3], membr[j].ride[4], membr[j].ride[5], membr[j].ride[6]);

		for (int k = 0; k < 7; k++) {			/*Просчет flag для данных считанных из файла*/
			if (membr[j].ride[k] % 10 == 0) {
				membr[j].ride[k] = (membr[j].ride[k] / 10)+1;
				membr[j].flag[k] = 1;
				continue;
			}
			if (membr[j].ride[k] % 10 != 0)
				membr[j].flag[k] = 0;
			
		}
	}
	for (int j = 0; j < size; j++) {			/*просчет DNQ для данных считанных из файла*/
		for (int k = 0; k < 7; k++) {
			if (membr[j].ride[k] < 0) membr[j].DNQ[k] = 1;
			else membr[j].DNQ[k] = 0;
		}
	}
	for (int j = 0; j < size; j++) {			/*Перерасчет очков при дисквалификации*/
			for (int k = 0; k < 7; k++) {
				if (membr[j].DNQ[k] == 1) membr[j].ride[k] = count_DNQ(size, j, k, membr);
			}
		membr[j].result=count_result(size,j,membr);
		membr[j].place = 0;
	}
		for (int j = 1; j <= size; j++) {		/*Перерасчет места*/
			membr[count_place(size,membr)].place = j;
	}
	while (flag == 0) {		/*Главное меню*/
		printf("Вывести таблицу -> 1\nОтсортировать -> 2\nВывести команды с диапазоном очков -> 3\nЗаписать таблицу в файл -> 4\nЛучший в заезде -> 5\nИзменить данные таблицы -> 6\nУдалить строку -> 7\nСоздать подмассив -> 8\nЗавершить -> 0\n");
		scanf("%d", &ptr);
		switch (ptr) {
		case 1: {			/*Вывод таблицы*/
			system("cls");
			put_table(size,membr);
			break;
		}
		case 2: {			/*Сортировка*/
			sort_bubble(size,membr);
			system("cls"); 
			printf("Табица отсторирована\n\n");
			put_table(size, membr);
			break;
		}
		case 3: {			/*Вывод по диапазону очков*/
			printf("Введите меньшее значение диапазона\n");
			scanf("%d", &fr);
			printf("Введите большее значение диапазона\n");
			scanf("%d", &to);
			for (int i = 0; i < size; i++) {
				if (put_for(size, fr, to, membr, i) == 1) printf("\n%s %d", membr[i].name, membr[i].result);
			}
			printf("\n\n");
			break;
		}
		case 4: {			/*запись данных в указанный файл */
			char fnamesec[60];
			puts("Введите имя файла\n");
			scanf("%s", &fnamesec);
			strcat(fnamesec, txt);
			if (put_in_file(size, membr, fnamesec) != 1) {
				printf("Ошибка записи файла");
				return 0;
			}
			system("cls");
			printf("Успешно записано\n\n");
			break;
		}
		case 5: {			/*Вывод лучшего участиника по заезду*/
			int h;
			system("cls");
			put_table(size,membr);
			puts("Введите номер заезда\n");
			scanf("%d", &h);
			printf("Лучшим в %d заезде был %s\n\n", h, membr[best_in_ride(size,h-1, membr)].name);
			break;
		}
		case 6: {			/*Изменение данных*/
			int m = 0;
			int n = 0;
			int n2;
			printf("Введите номер участника и номер заезда\n");
			scanf("%d %d", &m, &n);
			printf("Введите новое значение\n");
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
		case 7: {			/*Удаление строки*/
			int snum;
			puts("Введите номер строки для удаленния\n");
			scanf("%d", &snum);
			if (snum > size) {
				printf("Строки с таким номером не существует\n");
				break;
			}
			size = del_str(size, membr, snum);
			for (int j = 0; j < size; j++) {		/*Перерасчет очков при дисквалификации*/
				for (int k = 0; k < 7; k++) {
					if (membr[j].DNQ[k] == 1) membr[j].ride[k] = count_DNQ(size, j, k, membr);
				}
				membr[j].result = count_result(size, j, membr);
				membr[j].place = 0;
			}
			for (int j = 1; j <= size; j++) {		/*Перерасчет места*/
				membr[count_place(size, membr)].place = j;
			}
			system("cls");
			printf("Строка %d удалена (%s)\n\n", snum, membr[snum - 1].name);
			put_table(size, membr);
			break;
		}
		case 8: {		/*Создание подмассива*/
			int localflag = 0;
			int* newarrindex;
			int newcount = 0;
			for (int i = 0; i < size; i++) {
				membr[i].taken = 0;
			}
			newarrindex = (int*)malloc(size * sizeof(int));
			system("cls");
			while (localflag == 0) {
				int h;
				printf("Выберете критерии подмассива\n\nНаличие дисквалификаций -> 1\nНаличие штрафных очков -> 2\nДиапазон очков -> 3\nДиапазон мест -> 4\nСоздать массив -> 8\nВернуться в главное меню -> 0\n");
				scanf("%d", &h);
				switch (h) {
				case 1: {	/*Добавление яхт по наличию дисквалификации*/
					system("cls");
					newcount=add_for_dnq(size,membr,newarrindex,newcount);
					for (int i = 0; i < size; i++) {
						printf("%d ", newarrindex[i]);
					}
					printf("Добавлено по дисквалификации\n\n");
					break;
				}
				case 2: {	/*Добавление яхт по наличию штрафных очков*/
					system("cls");
					newcount = add_for_wp(size, membr, newarrindex, newcount);
					printf("Добавлено по штрафным очкам\n\n");
					break;
				}
				case 3: {	/*Добавление яхт по диапазону очков*/
					int min, max;
					printf("Введите минимум очков ");
					scanf("%d", &min);
					printf("Введите максимум очков ");
					scanf("%d", &max);
					newcount = add_for_point_area(size, membr, newarrindex, newcount, min, max);
					system("cls");
					printf("Добавлено от %d до %d очков\n\n",min,max);
					break;
				}
				case 4: {	/*Добавление яхт по диапазону мест*/
					int minp, maxp;
					printf("Введите минимальное место ");
					scanf("%d", &minp);
					printf("Введите максимальное место ");
					scanf("%d", &maxp);
					newcount = add_for_place_area(size, membr, newarrindex, newcount, minp, maxp);
					system("cls");
					printf("Добавлено от %d до %d мест\n\n", minp, maxp);
					break;
				}
				case 8: {	/*Создание и вывод подмассива по заданным ранее критериям*/
					mbr* newmas;
					newmas = (mbr*)malloc(newcount * sizeof(membr));
					system("cls");
					newmas=add_new_mas(newcount , membr, newarrindex);
					puts("Созданный подамссив:\n");
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
					printf("Выберете существующий вариант\n");
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
			printf("Введен неизвестный символ\n");
			break;
		}
		}
	}
	free(membr);
	return 1;
}

void put_table(int size,mbr* arr) {
	printf("Яхта     | 1 | 2 | 3 | 4 | 5 | 6 | 7 | Очки | Место\n");
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
	printf("x -  количество баллов за заезд;\n(x) - штрафные баллы\n!x! - дисквалификация (DNQ)\n\n");
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

int add_for_dnq(int size,mbr* arr,int* newarrindex,int newcount) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 7; j++) {
			if (arr[i].DNQ[j] == 1 && arr[i].taken == 0) {
				newarrindex[newcount] = i;
				newcount++;
				arr[i].taken = 1;
				break;
			}
		}
	}
	
	return newcount;
}
int add_for_wp(int size, mbr* arr, int* newarrindex, int newcount) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 7; j++) {
			if (arr[i].flag[j] == 1 && arr[i].taken == 0) {
				newarrindex[newcount] = i;
				newcount++;
				arr[i].taken = 1;
				break;
			}
		}
	}
	return newcount;
}
int add_for_point_area(int size, mbr* arr, int* newarrindex, int newcount,int min,int max) {
	for (int i = 0; i < size; i++) {
		if (arr[i].result >= min && arr[i].result <= max && arr[i].taken == 0) {
			newarrindex[newcount] = i;
			newcount++;
			arr[i].taken = 1;
		}
	}
	return newcount;
}
int add_for_place_area(int size, mbr* arr, int* newarrindex, int newcount,int minp,int maxp) {
	for (int i = 0; i < size; i++) {
		if (arr[i].place >= minp && arr[i].place <= maxp && arr[i].taken == 0) {
			newarrindex[newcount] = i;
			newcount++;
			arr[i].taken = 1;
		}
	}
	return newcount;
}
int del_str(int size, mbr* arr,int snum) {
	for (int i = snum - 1; i < size - 1; i++) {
		arr[i] = arr[i + 1];
	}
	size--;
	return size;
}
int put_in_file(int size,mbr* arr,char* fnamesec) {
	FILE* sec;
	sec = fopen(fnamesec, "w");
	for (int i = 0; i < size; i++) {
		fprintf(sec, "%s ", arr[i].name);
		for (int j = 0; j < 7; j++) {
			if (arr[i].flag[j] == 1) {
				fprintf(sec, "%d0 ", arr[i].ride[j]);
				continue;
			}
			if (arr[i].DNQ[j] == 1) {
				fprintf(sec, "%d ", -arr[i].ride[j]);
				continue;
			}
			if ((arr[i].flag[j] == 0) && (arr[i].DNQ[j] == 0)) {
				fprintf(sec, "%d ", arr[i].ride[j]);
				continue;
			}
		}
		fprintf(sec, "\n");
	}
	if (sec != NULL) {
		fclose(sec);
		return 1;
	}
}
