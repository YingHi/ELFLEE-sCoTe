#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void loadTxt(char* loadBuffer1, char* loadBuffer2) { // txt파일 읽어오기
    FILE* fp = fopen("data.txt", "rt");
    fgets(loadBuffer1, 1024, fp); // 첫번째 줄 리딩
    fgets(loadBuffer2, 1024, fp); // 두번째 줄 리딩
    fclose(fp);
}

int checkString(char* str) { // 문자열 검사
    int length = strlen(str);
    int is_per = 0; // %를 만났는지
    for (int i = 0; i < length; i++) {
        if (str[i] == '%') { // 만약 %를 만났다면
            if (is_per) { // 전 문자가 %이거나 %n이라면
                printf("문자열에 오류가 있습니다.");
                return 1;
            }
            else is_per = 1;  //아니라면 표시
        }
        else if (str[i] == 's' || str[i] == 'c' || str[i] == 'd') { // 만약 s / c / d를 만났다면
            if (is_per == 1) { // 전 문자가 %라면
                is_per = 2;
            }
            else if(is_per == 2) { // 이미 %n이 완성된 상태라면
                is_per = 0;
            }
        }
        else {
            if (is_per == 1) { // 전 문자가 %라면
                printf("문자열에 오류가 있습니다.");
                return 1;
            }
            else if (is_per == 2) { // 이미 %n이 완성된 상태라면
                is_per = 0;
            }
        }
    }
    return 0;
}

void extractNumbers(const char* str, char* result) {
    int i, j;
    for (i = 0, j = 0; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';
}


void cutStr(char *mainbuffer, char *cuttingstr) { // 기존 문자열에서 특정 문자열을 자르기
    int i = strlen(cuttingstr);
    int size = strlen(mainbuffer) + 1;
    for (int j = 0; j < i; j++) {
        if (mainbuffer[0] == cuttingstr[j]) {
            for (int k = 1; k < size; k++) {
                mainbuffer[k - 1] = mainbuffer[k];
            }
        } 
    }
}

void checkStrCount(char* mainbuffer, int* pc, int* ic, int* cc, int* sc) { // %갯수 체크 후 동적할당
    int point = 0, pcc = 0, scc = 0, ccc = 0, icc = 0;
    while (1) {
        if (mainbuffer[point] == '%') { // 만약 해당 위치의 글자가 %라면
            pcc++; // 퍼센트 갯수 증가
            if (mainbuffer[point + 1] == 's') scc++;
            if (mainbuffer[point + 1] == 'c') ccc++;
            if (mainbuffer[point + 1] == 'd') icc++;
        }
        if (mainbuffer[point] == '\n') { // 만약 해당 위치의 글자가 %라면
            mainbuffer[point] = '\0';
            break;
        }
        point++; // 탐색 위치를 다음으로 변경
    }
    *pc = pcc;
    *ic = icc;
    *cc = ccc;
    *sc = scc;
}

void main()
{
    char firstbuffer[1024], secondbuffer[1024];
    loadTxt(firstbuffer, secondbuffer); // 문자열 로드
    if(checkString(firstbuffer)) return;
    int* i; // int 배열
    char* c; // char 포인터 배열
    char** s; // string 포인터 배열(이중 포인터)
    int pc = 0, ic = 0, cc = 0, sc = 0;
    checkStrCount(firstbuffer, &pc, &ic, &cc, &sc);
    int nowpercount = -1; // 현재까지 지나온 % 갯수
    i = (int*)malloc(sizeof(int) * ic); // 문자열 포인터 배열에 % 갯수만큼 메모리 할당
    c = (char*)malloc(sizeof(char) * cc); // 문자열 포인터 배열에 % 갯수만큼 메모리 할당
    s = (char**)malloc(sizeof(char*) * sc); // 문자열 포인터 배열에 % 갯수만큼 메모리 할당
    ic = 0, cc = 0, sc = 0;
    while (nowpercount < pc) {
        char* str; // 임시로 값을 저장할 문자열 선언
        if (nowpercount < 0) { // 첫시동에는 %전까지의 문자열을 잘라내야함
            char* search = strchr(firstbuffer, '%'); // % 위치 서치
            int leng = search - firstbuffer;
            str = (char*)malloc(leng + 1); // 문자열 메모리 동적할당
            strncpy(str, firstbuffer, leng); // str에 현재 위치의 직전만큼의 문자열을 할당
            str[leng] = '\0';
            cutStr(firstbuffer, str);
            cutStr(secondbuffer, str);
            free(str); // str에 값이 할당된 상태라면 free
        }
        else {
            char* search;
            if (nowpercount == pc - 1) {
                search = strchr(&firstbuffer[1], '\0'); // % 위치 서치
            }
            else {
                search = strchr(&firstbuffer[1], '%'); // % 위치 서치
            }
            if (search != NULL) {
                int leng = search - firstbuffer - 2;
                str = (char*)malloc(leng + 1); // 문자열 메모리 동적할당
                strncpy(str, &firstbuffer[2], leng); // str에 현재 위치의 직전만큼의 문자열을 할당
                str[leng] = '\0'; // 널문자 추가
                char* search2 = strstr(secondbuffer, str); // str 위치 서치
                if (firstbuffer[0] == '%') {
                    if (firstbuffer[1] == 's') {
                        s[sc] = malloc(strlen(str) + 1);
                        strncpy(s[sc], secondbuffer, search2 - secondbuffer);
                        s[sc][search2 - secondbuffer] = '\0';
                        cutStr(firstbuffer, "%s");
                        cutStr(secondbuffer, s[sc]);
                        sc++;
                    }
                    if (firstbuffer[1] == 'c') {
                        c[cc] = *secondbuffer;
                        cutStr(firstbuffer, "%c");
                        int size = strlen(secondbuffer);
                        for (int k = 1; k < size; k++) {
                            secondbuffer[k - 1] = secondbuffer[k];
                        }
                        cc++;
                    }
                    if (firstbuffer[1] == 'd') {
                        char* str3 = (char*)malloc(sizeof(char*) * (search2 - secondbuffer) + 1);
                        char extractedint[100];
                        strncpy(str3, secondbuffer, search2 - secondbuffer);
                        strcat(str3, "");
                        extractNumbers(str3, extractedint);
                        i[ic] = atoi(extractedint);
                        cutStr(firstbuffer, "%d");
                        cutStr(secondbuffer, str3);
                        ic++;
                    }
                }
                cutStr(firstbuffer, str);
                cutStr(secondbuffer, str);
                free(str); // str에 값이 할당된 상태라면 free
            }
        }
        nowpercount++; // 조건 증가
    }
    for (int v = 0; v < ic; v++) {
        printf("포함된 숫자 : %d\n", i[v]);
    }
    for (int v = 0; v < cc; v++) {
        printf("포함된 문자 : %c\n", c[v]);
    }
    for (int v = 0; v < sc; v++) {
        printf("포함된 문자열 : %s\n", s[v]);
    }
}
