#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <thread>
#include "GameUtil.hpp"
#include <mmsystem.h>
#include <Digitalv.h>
#include <fstream>
#include <sstream>

#pragma comment(lib, "winmm.lib")

using namespace std;


namespace Game_Engine
{
	class Scene
	{
	private:
		bool is_GameRunning, is_rank, is_pause;// 게임이 진행중인지 체크
		Renderer r;
		Player p;
		Enemy e[7]; // 적
		Bead b[7]; // 여의주
		Background background[4];
		Background backgroundObj[3];
		UI ingameUI[5];
		UI rankUI[3][5];
		UI str[2];
		int game_state, respawnCool_e, respawnCool_b; // 게임 상태, 리스폰 쿨타임
		chrono::system_clock::time_point GenTimePoint_e; // 소환된 시간
		chrono::duration<double> RespawnTimer_e; // 리스폰 시간
		chrono::system_clock::time_point GenTimePoint_b; // 소환된 시간
		chrono::duration<double> RespawnTimer_b; // 리스폰 시간
	public:
		
		Scene() { is_GameRunning = false; }
		void Run()
		{
			Initialize();
			while (is_GameRunning)
			{
				Input();
				if (!is_pause) {
					Update();
				}
				Render();
			}
		}

	private:
		void Stop()
		{
			is_GameRunning = false;
		}
		void Initialize()
		{
			is_rank = false;
			is_GameRunning = true;
			is_pause = false;
			game_state = 1;
			respawnCool_e = 3.5;
			respawnCool_b = 2.5;
			for (int i = 0; i < 2; i++) {
				background[i].Set(i * 2 - 1, 0, 1, 2.2f, 0.016f, "Sky");
			}
			for (int i = 2; i < 4; i++) {
				background[i].Set((i-2) * 2 - 1, -1, 1, 2.2f, 0.022f, "Ground");
			}
			for (int i = 0; i < 2; i++) {
				backgroundObj[i].Set(i * 2 - 1, 0.7f + (0.15f * i), 0.2f + (0.1f * i), 0.3f + (0.1f * i), 0.023f + (0.005 * i), "Cloud");
			}
			backgroundObj[2].Set(1, 0, 0.7f, 0.55f, 0.008f, "Mountain");
			ingameUI[0].Set(-0.95f, 0.6f, 0.4f, 0.4f, "Score");
			str[0].Set(-0.25f, -0.25f, 0.5f, 0.5f, "Pause");
			str[1].Set(-0.45f, 0.4f, 0.6f, 0.6f, "Rank");
			rankUI[0][0].Set(-0.5, 0.3f, 0.35f, 0.35f, "1st");
			rankUI[1][0].Set(-0.5, 0.0f, 0.35f, 0.35f, "2nd");
			rankUI[2][0].Set(-0.5, -0.3f, 0.35f, 0.35f, "3rd");
			for (int j = 0; j < 3; j++) {
				rankUI[j][0].SetActive(false);
				for (int i = 1; i < 5; i++) {
					rankUI[j][i].Set(-0.3 + (i * 0.17f), 0.3f - (j * 0.3f) , 0.2f, 0.2f, "Zero");
					rankUI[j][i].SetActive(false);
				}
			}
			for (int i = 1; i < 5; i++) {
				ingameUI[i].Set(-0.7 + (i * 0.15f), 0.7f, 0.2f, 0.2f, "Zero");
			}
			GenTimePoint_e = chrono::system_clock::now();
			GenTimePoint_b = chrono::system_clock::now();
			PlaySound(TEXT("title.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}

		void Input()
		{
			if (GetAsyncKeyState(VK_RETURN))
			{
				if (!is_pause) {
					is_pause = true;
					str[0].SetActive(true);
				}
				else {
					is_pause = false;
					str[0].SetActive(false);
				}
			}
			if (GetAsyncKeyState(VK_ESCAPE))
			{
				Stop();
			}

			if (!is_pause) {
				if (GetAsyncKeyState(VK_DOWN))
				{
					p.isDownPressed();
				}
				else
				{
					p.isNotDownPressed();
				}
				if (GetAsyncKeyState(VK_SPACE))
				{
					if (game_state == 2) {
						p.isSpacePressed();
					}
					if (game_state == 1) {
						game_state = 2;
						PlaySound(TEXT("bgm.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
					}
				}
			}
		}

		void Respawn(string s) {
			if (s == "Bead") {
				
				for (int i = 0; i < 7; i++) {
					if (b[i].setActive) continue;
					b[i].Respawn();
					break;
				}
			}
			else if (s == "Enemy") {
				for (int i = 0; i < 7; i++) {
					if (e[i].setActive) continue;
					e[i].Respawn();
					break;
				}
			}
		}

		void UIset() {
			int num = p.score;
			int	thousands = num / 1000;
			int	hundreds = (num / 100) % 10;
			int	tens = (num / 10) % 10;
			int	ones = num % 10;

			ingameUI[1].ChangeVal(thousands);
			ingameUI[2].ChangeVal(hundreds);
			ingameUI[3].ChangeVal(tens);
			ingameUI[4].ChangeVal(ones);
		}

		void Update()
		{
			if (game_state == 2) {
				RespawnTimer_e = chrono::system_clock::now() - GenTimePoint_e;
				if (RespawnTimer_e.count() > respawnCool_e) {
					Respawn("Enemy");
					GenTimePoint_e = chrono::system_clock::now();
				}
				RespawnTimer_b = chrono::system_clock::now() - GenTimePoint_b;
				if (RespawnTimer_b.count() > respawnCool_b) {
					Respawn("Bead");
					GenTimePoint_b = chrono::system_clock::now();
				}
				for (int i = 0; i < 4; i++) {
					background[i].Update(p.speed); // 배경 프린트
				}
				for (int i = 2; i >= 0; i--) {
					backgroundObj[i].Update(p.speed); // 배경 오브젝트 프린트
				}
				for (int i = 0; i < 7; i++) {
					e[i].Update(p.speed); // 적 프린트
					p.isCollision(e[i]);
				}
				for (int i = 0; i < 7; i++) {
					b[i].Update(p.speed); // 구슬 프린트
					int a = p.isCollision(b[i]);
					if (a > 1) respawnCool_e -= a * 0.5;
					else if (a == 1) {
						b[i].setActive = false;
					} 
				}
				UIset();
				int result = p.Update();
				if (result == 0) {
					for (int i = 0; i < 5; i++) {
						ingameUI[i].SetActive(false);
					}
					game_state = 3;
					PlaySound(TEXT("win.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
				} 
			}
		}

		void RUIset(int a, int b) {
			int num = a;
			int	thousands = num / 1000;
			int	hundreds = (num / 100) % 10;
			int	tens = (num / 10) % 10;
			int	ones = num % 10;

			rankUI[b][1].ChangeVal(thousands);
			rankUI[b][2].ChangeVal(hundreds);
			rankUI[b][3].ChangeVal(tens);
			rankUI[b][4].ChangeVal(ones);
		}

		void Ranking() {
			int grade = p.score;
			for (int j = 0; j < 3; j++) {
				for (int i = 0; i < 5; i++) {
					rankUI[j][i].SetActive(true);
				}
			}
			str[1].SetActive(true);
			r.Draw(str[1]);
			int number1, number2, number3;
			ifstream file;
			file.open("data.txt");
			char data[50]; // 데이터 저장할 문자열
			file.getline(data, 50);
			number1 = atoi(data);
			RUIset(number1, 0);
			file.getline(data, 50);
			number2 = atoi(data);
			RUIset(number2, 1);
			file.getline(data, 50);
			number3 = atoi(data);
			RUIset(number3, 2);
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 5; j++) {
					rankUI[i][j].SetActive(true);
					r.Draw(rankUI[i][j]);
				}
			}
			file.close();
			if (is_rank == false) {
				if (grade > number3) {
					number3 = grade;
					if (grade > number2) {
						number3 = number2;
						if (grade > number1) {
							number2 = number1;
							number1 = grade;
						}
						else {
							number2 = grade;
						}
					}
					ofstream outFile;
					outFile.open("data.txt");
					outFile << number1 << "\n" << number2 << "\n" << number3;
					outFile.close();
				}
			}
			is_rank = true;
		}

		void Render()
		{
			r.Clear(); // 이전 화면 삭제
			r.ready(); // 렌더링 환경 준비
			if (game_state == 2) { // 만약 게임 진행중이라면
				for (int i = 0; i < 4; i++) {
					r.Draw(background[i]); // 배경 프린트
				}
				for (int i = 2 ; i >= 0; i--) {
					r.Draw(backgroundObj[i]); // 배경 오브젝트 프린트
				}
				for (int i = 0; i < 7; i++) {
					r.Draw(e[i]); // 적 프린트
				}
				for (int i = 0; i < 7; i++) {
					r.Draw(b[i]); // 구슬 프린트
				}
				for (int i = 0; i < 5; i++) {
					r.Draw(ingameUI[i]); // ui 프린트
				}
				r.Draw(p); // 플레이어 프린트
			}
			else {
				r.DrawScene(game_state); // 게임중이 아니라면 기타 화면 재생
				if (game_state == 3) {
					Ranking();
				}
			}
			if (is_pause) r.Draw(str[0]);
			r.Rendering();
			this_thread::sleep_for(chrono::milliseconds(10)); // 15초의 휴식을 주어 게임의 진행 속도 제어
		}
	};
}
