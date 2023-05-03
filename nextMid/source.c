#include <stdio.h>
#include <stdbool.h>

int vr_Scanf(const char* ch, void* data1, void* data2, void* data3, void* data4) {
    int data_number = 1;
    int index = 0;
    float temp_ch = 0;
    float num = 0;
    bool negative_number = false;
    while (*ch != '%' && *ch) ch++;

    while (*ch) {
        if (*ch == '%') {
            ch++;
            temp_ch = getc(stdin);

            // 엔터, 공백, \t일 경우 다음 버퍼로 넘어가서 계속 입력받음 
            while (temp_ch == ' ' || temp_ch == '\n' || temp_ch == '\t') {
                temp_ch = getc(stdin);
            }

            //입력 받을 변수 유형을 체크함
            switch (*ch)
            {
                //문자일때
            case 'c':
                if (data_number == 1) {
                    *((char*)data1) = (char)temp_ch;
                }
                else if (data_number == 2) {
                    *((char*)data2) = (char)temp_ch;
                }
                else if (data_number == 3) {
                    *((char*)data3) = (char)temp_ch;
                }
                else if (data_number == 4) {
                    *((char*)data4) = (char)temp_ch;
                }
                data_number++;
                break;
                //문자열일떄
            case 's':
                while (true) {
                    if (temp_ch == '\n' || temp_ch == '\0') {
                        if (data_number == 1) {
                            *((char*)data1 + index) = '\0';
                        }
                        else if (data_number == 2) {
                            *((char*)data2 + index) = '\0';
                        }
                        else if (data_number == 3) {
                            *((char*)data3 + index) = '\0';
                        }
                        else if (data_number == 4) {
                            *((char*)data4 + index) = '\0';
                        }
                        index = 0;
                        data_number++;
                        break;
                    }
                    else {
                        if (data_number == 1) {
                            *((char*)data1 + index) = (char)temp_ch;
                        }
                        else if (data_number == 2) {
                            *((char*)data2 + index) = (char)temp_ch;
                        }
                        else if (data_number == 3) {
                            *((char*)data3 + index) = (char)temp_ch;
                        }
                        else if (data_number == 4) {
                            *((char*)data4 + index) = (char)temp_ch;
                        }
                        index++;
                    }
                    temp_ch = getc(stdin);
                }
                data_number++;
                break;
                //정수일때
            case 'd':
                num = 0;

                if (temp_ch == '-') {
                    temp_ch = '0';
                }

                // 정수로 표현하고 그 사이 값이 0~ 9 사이 일경우
                while ((int)temp_ch >= '0' || (int)temp_ch <= sizeof(_CRT_INT_MAX)) {

                    //값을 num에다가 넣어주고 다시 문자를 읽어옴
                    num = (int)num * 10 + (int)temp_ch - '0';
                    temp_ch = getc(stdin);

                    if (temp_ch == ' ' || temp_ch == '\n') {
                        if (data_number == 1) {
                            *(int*)data1 = num;
                        }
                        else if (data_number == 2) {
                            *(int*)data2 = num;
                        }
                        else if (data_number == 3) {
                            *(int*)data3 = num;
                        }
                        else if (data_number == 4) {
                            *(int*)data4 = num;
                        }
                        data_number++;
                        break;
                    }
                }
                //실수일때
            case 'f':
                num = 0;

                if (temp_ch == '-') {
                    negative_number = true;
                    temp_ch = '0';
                }

                while ((float)temp_ch >= '0' || (float)temp_ch <= sizeof(_CRT_INT_MAX)) {

                    num = (float)num * 10 + (float)temp_ch - '0';

                    temp_ch = getc(stdin);

                    if (temp_ch == '.') {
                        temp_ch = getc(stdin);
                    }

                    if (temp_ch == ' ' || temp_ch == '\n') {
                        if (data_number == 1) {
                            *(float*)data1 = num / 10;
                        }
                        else if (data_number == 2) {
                            *(float*)data2 = num / 10;
                        }
                        else if (data_number == 3) {
                            *(float*)data3 = num / 10;
                        }
                        else if (data_number == 4) {
                            *(float*)data4 = num / 10;
                        }
                        data_number++;
                        break;
                    }
                }
            }
        }
        ch++;
    }
    return 1;
}

int main() {
    int a;
    float b;
    char c;
    char s[1024];

    vr_Scanf("%f %d %c %s", &b, &a, &c, s);
    printf("%f %d %c %s", b, a, c, s);

    return 0;
}
