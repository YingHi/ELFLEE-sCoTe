#include <iostream>
#include <cstdio>
#include <conio.h>
#include <array>
#include <string>

using namespace std;


bool is_Arab_Digit(string str) { // 아라비아 숫자 여부 확인
	return atoi(str.c_str()) != 0 || str.compare("0") == 0; // 값을 숫자로 변환할 수 있는지 체크함
}

int checkLevel(int i) {
	int num = i;
	int count = 0;
	do {
		num /= 10;
		count++;
	} while (num > 0);
	int result = 1;
	for (int i = 1; i < count; i++) result *= 10;
	return result;
}

int change_Romannum(string c) {
	if (c == "I" || c == "i") return 1;
	if (c == "V" || c == "v") return 5;
	if (c == "X" || c == "x") return 10;
	if (c == "L" || c == "l") return 50;
	if (c == "C" || c == "c") return 100;
	if (c == "D" || c == "d") return 500;
	if (c == "M" || c == "m") return 1000;
	if (c == "F" || c == "f") return 5000;
	return 0;
}
int is_Roman_Digit(string str) { // 로마 숫자 여부 확인
	int result = 0;
	for (int i = 0; i < str.size(); i++) { // 로마 숫자 내의 글자들만 있는지 체크
		string s = str.substr(i, 1);
		if (s == "I" || s == "i") break;
		if (s == "V" || s == "v") break;
		if (s == "X" || s == "x") break;
		if (s == "L" || s == "l") break;
		if (s == "C" || s == "c") break;
		if (s == "D" || s == "d") break;
		if (s == "M" || s == "m") break;
		if (s == "F" || s == "f") break;
		return 4000;
	}
	int level = 1; // 반복횟수 담는 int형 변수.
	for (int i = 0; i < str.size() - 1; i++) { // 문법 제대로 되어있는지 체크
		string now = str.substr(i, 1);
		string next = str.substr(i + 1, 1);
		if (now == next) level += 1; // 같은 글자가 반복되면 레벨 +1
		else {
			level = 1; // 같은 글자가 반복되지 않았다면 1로 초기화
		}
		if (level == 4) return 4000; // 만약 같은 글자가 네 번 반복되면 오류이므로 프로그램 종료
		int now_v = change_Romannum(now);
		int next_v = change_Romannum(next);
		if (now_v < next_v) {//앞 숫자가 뒷 숫자보다 작다면
			if (next_v / now_v == 2 || next_v / now_v == 5 || next_v / now_v == 10 || (next_v == 10 && now_v == 1)) break;
			return 4000;
		}
	}
	int lv = 10000;
	bool CanC = false;
	for (int i = 0; i < str.size(); i++) { // 실제 계산
		string now = str.substr(i, 1);
		int a = change_Romannum(now);
		int b = checkLevel(a);
		if (lv <= b) {
			if (!CanC) {
				return 4000;
			}
			CanC = false;
		}
		if (i < str.size() - 1) {
			string next = str.substr(i + 1, 1);
			int now_v = change_Romannum(now);
			int next_v = change_Romannum(next);
			if (now_v < next_v) {//앞 숫자가 뒷 숫자보다 작다면
				if (next_v / now_v == 2 || next_v / now_v == 5 || next_v / now_v == 10 || (next_v == 10 && now_v == 1)) {
					int a = checkLevel(next_v - now_v);
					if (lv <= a) return 4000;
					lv = a;
					result += (next_v - now_v);
					i += 1;
					continue;
				}
			}
			if (now_v = next_v) {
				CanC = true;
			}
		}
		lv = b;
		result += a;
	}
	return result;
}


void OutputMassage(string input) { // 메세지 출력
	cout << "아라비아 숫자는 로마 숫자로, 로마 숫자는 아라비아 숫자로 변환하는 프로그램입니다.\n";
	cout << "1~3999사이의 숫자를 입력하세요.\n종료를 원하신다면 ESC를 눌러주세요.\n";
	cout << "현재 입력 값 :" << input << "\n";
}

string inputString(string input) { // 값을 입력받는 함수
	char ch;
	OutputMassage(input); // 고정 메세지 출력
	while ((ch = _getch()) != 27 && ch != 13) { // Esc와 Enter가 아니라면 계속 입력받음
		if (ch == 8) {
			if (!input.empty()) input.pop_back();
		}
		else {
			input += ch;
		}
		system("cls");
		OutputMassage(input);
	}
	if (ch == 13) return input;
	else if (ch == 27) return "boom!";
}

string change_Roman(int integer) { // 로마자로 바꿔주는 함수
	int X = 0, C = 0, M = 0;
	string result = "";
	if (integer >= 1000) { // 천단위 체크
		M = integer / 1000;
		integer -= M * 1000;
	}
	for (int i = 0; i < M; i++) { // 천단위 추가
		result += "M";
	}
	if (integer >= 100) { // 백단위 체크
		C = integer / 100;
		integer -= C * 100;
	}
	if (C == 9) result += "CM";
	else if (C == 4) result += "CD";
	else {
		if (C >= 5) {
			result += "D";
			C -= 5;
		}
		for (int i = 0; i < C; i++) { // 백단위 추가
			result += "C";
		}
	}
	if (integer >= 10) {
		X = integer / 10;
		integer -= X * 10;
	}
	if (X == 9) result += "XC";
	else if (X == 4) result += "XL";
	else {
		if (X >= 5) {
			result += "L";
			X -= 5;
		}
		for (int i = 0; i < X; i++) { // 십단위 추가
			result += "X";
		}
	}
	if (integer == 9) result += "IX";
	else if (integer == 4) result += "IV";
	else {
		if (integer >= 5) {
			result += "V";
			integer -= 5;
		}
		for (int i = 0; i < integer; i++) { // 일단위 추가
			result += "I";
		}
	}
	return result;
}

void ResetProgram() {
	cout << "아무 키나 클릭시 다시 시작합니다.\n";
	system("pause");
	system("cls");
}

void main() {
	string input = "";
	bool is_arab, is_roman;
	int ci;
	while (1) {
		input = "";
		ci = 4000;
		is_arab = false;
		is_roman = false;
		input = inputString(input);
		if (input == "") {
			cout << "\n올바른 값을 입력해주세요.\n";
			ResetProgram();
			continue;
		}
		is_arab = is_Arab_Digit(input); // 아라비아 숫자인지 체크
		if (is_arab) { // 아라비아 숫자 였다면
			ci = stoi(input);
			if (ci > 0 && ci < 4000) {
				cout << "\n아라비아 숫자 " << ci << "의 변환 결과는 " + change_Roman(ci) + "입니다.\n";
				ResetProgram();
			}
			else {
				cout << "\n올바른 값을 입력해주세요.\n";
				ResetProgram();
			}
		}
		else {
			ci = is_Roman_Digit(input);
			if (ci > 0 && ci < 4000) {
				cout << "\n로마 숫자 " << input << "의 변환 결과는 " << ci << "입니다.\n";
				ResetProgram();
			}
			else {
				if (input == "boom!") {
					cout << "프로그램을 종료합니다.";
					break;
				}
				else cout << "\n올바른 값을 입력해주세요.\n";
				ResetProgram();
			}
		}
	}
}
