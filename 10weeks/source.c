#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cutStr(char *mainbuffer, char *cuttingstr) {
    int i = strlen(cuttingstr);
    int size = strlen(mainbuffer);
    for (int j = 0; j < i; j++) {
        if (mainbuffer[0] == cuttingstr[j]) {
            for (int k = 1; k < size; k++) {
                mainbuffer[k - 1] = mainbuffer[k];
            }
        } 
    }
}

void inputStr(char* mainbuffer, char* dest, int count) {
    char* c = malloc(count);
    for (int i = 0; i < count; i++) {
        char a = mainbuffer[0];
        c[i] = a;
        cutStr(mainbuffer, a);
    }
}

void main()
{
    FILE* fp = fopen("data.txt", "rt");
    char firstbuffer[1024], secondbuffer[1024];
    fgets(firstbuffer, 1024, fp); // 첫번째 줄 리딩
    fgets(secondbuffer, 1024, fp); // 두번째 줄 리딩
    char** percounts; // 문자열 포인터 배열(이중 포인터)
    int* i; // int 배열
    char** s; // string 포인터 배열(이중 포인터)
    char* c; // char 포인터 배열
    int percount = 0; // %로 시작하는 단어 갯수
    int icount = 0, scount = 0, ccount = 0; // 각 배열의 갯수
    int point = 0, count = 2; // 버퍼 탐색 위치, 조건변수
    while (count > 0) { // "가 두번 나올때까지 반복 (malloc용 데이터 수집)
        if (firstbuffer[point] == '%') { // 만약 해당 위치의 글자가 %라면
            percount++; // 퍼센트 갯수 증가
            if (firstbuffer[point + 1] == 's') scount++; // 다음 글자가 s라면 string 배열 길이 증가
            if (firstbuffer[point + 1] == 'c') ccount++; // 다음 글자가 c라면 char 배열 길이 증가
            if (firstbuffer[point + 1] == 'i') icount++; // 다음 글자가 i라면 int 배열 길이 증가
        }
        if (firstbuffer[point] == '"') count--; // 만약 해당 위치의 글자가 "라면 조건 감소
        point++; // 탐색 위치를 다음으로 변경
    }
    percounts = (int**)malloc(sizeof(char*) * percount); // 문자열 포인터 배열에 % 갯수만큼 메모리 할당
    i = (int*)malloc(sizeof(int) * icount); // 문자열 포인터 배열에 % 갯수만큼 메모리 할당
    s = (char**)malloc(sizeof(char*) * scount); // 문자열 포인터 배열에 % 갯수만큼 메모리 할당
    c = (char*)malloc(sizeof(char) * ccount); // 문자열 포인터 배열에 % 갯수만큼 메모리 할당
    int is_per = 0; // 1이면 포인터 탐색차례
    int nowpercount = 0; // 현재까지 지나온 % 갯수
    icount = 0;
    scount = 0;
    ccount = 0;
    while (nowpercount < percount) {
        char* str; // 임시로 값을 저장할 문자열 선언
        if (nowpercount == 0) { // 첫시동에는 %전까지의 문자열을 잘라내야함
            char* search = strchr(firstbuffer, '%'); // % 위치 서치
            int leng = search - firstbuffer;
            str = (char*)malloc(leng+1); // 문자열 메모리 동적할당
            strncpy(str, firstbuffer, leng); // str에 현재 위치의 직전만큼의 문자열을 할당
            str[leng] = '\0'; // 널문자 추가
            cutStr(firstbuffer, str);
            cutStr(secondbuffer, str);
            free(str); // str에 값이 할당된 상태라면 free
            nowpercount++; // 조건 증가
        }
        else {
            char* search = strchr(&firstbuffer[1], '%'); // % 위치 서치
            int leng = search - firstbuffer - 2;
            str = (char*)malloc(leng + 1); // 문자열 메모리 동적할당
            strncpy(str, &firstbuffer[2], leng); // str에 현재 위치의 직전만큼의 문자열을 할당
            str[leng] = '\0'; // 널문자 추가
            printf("%s\n", str);
            char str2[1024];
            strtok(str2, str);
            printf("%s", str2);
            printf("%s", secondbuffer);

            if (firstbuffer[1] == 's') {
                strcpy(&s[scount], str);
                scount++;
                cutStr(firstbuffer, "%s");
            }
            else if (firstbuffer[1] == 'i') {
                int inteeger = atoi(str);
                i[icount] = inteeger;
                icount++;
                cutStr(firstbuffer, "%i");
            }
            else if (firstbuffer[1] == 'c') {
                c[ccount] = *str;
                ccount++;
                cutStr(firstbuffer, "%c");
            }
            free(str); // str에 값이 할당된 상태라면 free
            nowpercount++; // 조건 증가
        }

    }

    printf("%s\n", firstbuffer);
    printf("%s\n", secondbuffer);

    fclose(fp); //파일 포인터 닫기
}
