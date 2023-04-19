#include <iostream>
#include <array>
#include <stdlib.h>

using namespace std;

string map[5][5];
int player[2]; // Player 위치 (시작점)
int enemy[2]; // enemy 위치 (골인)

class Node {

public:
	int distance = 1000; // 거리, 기본값
	int before[2]; // 최단거리일때 이전노드.
	Node* upNode, * downNode, * leftNode, * rightNode;
	int x, y; // 해당 노드의 좌표
	int up[2] = { -1,-1 }; // 이동할 수 없는 위치라면 -1, -1
	int down[2] = { -1,-1 };
	int left[2] = { -1,-1 };
	int right[2] = { -1,-1 };
	Node(int a, int b) { // 생성자
		x = a;
		y = b;
		if (map[a][b] == "◆") {
			this->distance = -1;
		}
		if ((map[a][b + 1] == "▣" || map[a][b + 1] == "ㅤ") && b < 4) { // 해당 위치가 벽이거나 비어있거나 끝자리가 아니라면 해당 위치 지정
			this->right[0] = a;
			this->right[1] = b + 1;
		}
		if ((map[a][b - 1] == "▣" || map[a][b - 1] == "ㅤ") && b > 0) { // 해당 위치가 벽이거나 비어있거나 끝자리가 아니라면 해당 위치 지정
			this->left[0] = a;
			this->left[1] = b - 1;
		}
		if ((map[a + 1][b] == "▣" || map[a + 1][b] == "ㅤ") && a < 4) { // 해당 위치가 벽이거나 비어있거나 끝자리가 아니라면 해당 위치 지정
			this->down[0] = a + 1;
			this->down[1] = b;
		}
		if ((map[a - 1][b] == "▣" || map[a - 1][b] == "ㅤ") && a > 0) { // 해당 위치가 벽이거나 비어있거나 끝자리가 아니라면 해당 위치 지정
			this->up[0] = a - 1;
			this->up[1] = b;
		}
	}

	int Dijkstra(int c, Node* n) {
		int a = 0;
		if (this->x == enemy[0] && this->y == enemy[1]) { // 해당 위치가 적이라면
			this->distance = c + 1; // 현재 노드의 거리를 지정
			this->before[0] = n->x; // 이전 노드 위치 지정
			this->before[1] = n->y; // 이전 노드 위치 지정
			return 1; // 1 리턴으로 루프 종료를 알림
		}
		if (this->down[0] != -1 && this->downNode->distance > c + 1) { // 해당 위치가 비어있지 않고 거리가 더 가깝다면
			this->distance = c + 1; // 현재 노드의 거리를 지정
			this->before[0] = n->x; // 이전 노드 위치 지정
			this->before[1] = n->y; // 이전 노드 위치 지정
			a = this->downNode->Dijkstra(c + 1, this);
		}
		if (this->up[0] != -1 && this->upNode->distance > c + 1) { // 해당 위치가 비어있지 않고 거리가 더 가깝다면
			this->distance = c + 1; // 현재 노드의 거리를 지정
			this->before[0] = n->x; // 이전 노드 위치 지정
			this->before[1] = n->y; // 이전 노드 위치 지정
			a = this->upNode->Dijkstra(c + 1, this);
		}
		if (this->right[0] != -1 && this->rightNode->distance > c + 1) { // 해당 위치가 비어있지 않고 거리가 더 가깝다면
			this->distance = c + 1; // 현재 노드의 거리를 지정
			this->before[0] = n->x; // 이전 노드 위치 지정
			this->before[1] = n->y; // 이전 노드 위치 지정
			a = this->rightNode->Dijkstra(c + 1, this);
		}
		if (this->left[0] != -1 && this->leftNode->distance > c + 1) { // 해당 위치가 비어있지 않다면
			this->distance = c + 1; // 현재 노드의 거리를 지정
			this->before[0] = n->x; // 이전 노드 위치 지정
			this->before[1] = n->y; // 이전 노드 위치 지정
			a = this->leftNode->Dijkstra(c + 1, this);
		}
		return 0;
	}
};

Node* nodes[5][5]; // 노드들

void ResetGame() { // 게임 리셋
	for (int row = 0; row < 5; row++) {
		for (int col = 0; col < 5; col++) {
			map[row][col] = "ㅤ";
		}
	}
}

bool CheckMoveable(int A[2]) { // 플레이어나 적이 움직일 수 있는지 체크
	int count = 0;
	if (map[A[0] + 1][A[1]] != "ㅤ" || A[0] == 4) {
		count++;
	}
	if (map[A[0] - 1][A[1]] != "ㅤ" || A[0] == 0) {
		count++;
	}
	if (map[A[0]][A[1] + 1] != "ㅤ" || A[1] == 4) {
		count++;
	}
	if (map[A[0]][A[1] - 1] != "ㅤ" || A[1] == 0) {
		count++;
	}
	if (count == 4) return false;
	return true;
}

void SettingGame() { // 게임 세팅
	srand(time(0));
	int px = rand() % 5;
	int py = rand() % 5;
	map[px][py] = "◎"; // 플레이어 위치 세팅
	player[0] = px;
	player[1] = py;
	while (1) { // 적 위치 세팅
		int ex = rand() % 5;
		int ey = rand() % 5;
		if (map[ex][ey] == "ㅤ") {
			map[ex][ey] = "▣";
			enemy[0] = ex;
			enemy[1] = ey;
			break;
		}
	}
	for (int i = 0; i < 4; i++) { // 돌 세팅
		int sx = rand() % 5;
		int sy = rand() % 5;
		if (map[sx][sy] == "ㅤ") {
			map[sx][sy] = "◆";
		}
		else {
			i--;
			continue;
		}
		if (i == 5) { // 플레이어나 적이 움직일 수 없다면 돌 위치 리셋
			if (!CheckMoveable(player)) i = -1;
			if (!CheckMoveable(enemy)) i = -1;
		}
	}
}

void PaintMap(int px, int py) { // 화면 색칠
	if (px == player[0] && py == player[1]) {// 플레이어면 리턴(종료)
		return;
	}
	if (map[px][py] == "ㅤ") { // 빈칸이면 색칠
		map[px][py] = "■";
	}
	PaintMap(nodes[px][py]->before[0], nodes[px][py]->before[1]);
}

void PrintMap() { // 맵 출력
	for (int row = 0; row < 5; row++) {
		for (int col = 0; col < 5; col++) {
			nodes[row][col] = new Node(row, col);
		}
	}
	for (int row = 0; row < 5; row++) { // 노드에 각 노드와 연결된 포인터 연결
		for (int col = 0; col < 5; col++) {
			if (nodes[row][col]->down[0] != -1) {
				nodes[row][col]->downNode = nodes[nodes[row][col]->down[0]][nodes[row][col]->down[1]];
			}
			if (nodes[row][col]->up[0] != -1) {
				nodes[row][col]->upNode = nodes[nodes[row][col]->up[0]][nodes[row][col]->up[1]];
			}
			if (nodes[row][col]->left[0] != -1) {
				nodes[row][col]->leftNode = nodes[nodes[row][col]->left[0]][nodes[row][col]->left[1]];
			}
			if (nodes[row][col]->right[0] != -1) {
				nodes[row][col]->rightNode = nodes[nodes[row][col]->right[0]][nodes[row][col]->right[1]];
			}
		}
	}
	int i = 0;
	for(int a = 0; a < 20; a++){
		i = nodes[player[0]][player[1]]->Dijkstra(0, nodes[player[0]][player[1]]); // 루트노드로 다익스트라 실행
	}
	PaintMap(enemy[0],enemy[1]);
	cout << "□□□□□□□\n";
	for (int row = 0; row < 5; row++) {
		cout << "□";
		for (int col = 0; col < 5; col++) {
			cout << map[row][col];
		}
		cout << "□\n";
	}
	cout << "□□□□□□□\n";
	cout << "Player = ◎\nEnemy = ▣\nWall = ◆\nFastest Way = ■";
}

void main() {
	ResetGame();
	SettingGame();
	PrintMap();
}
