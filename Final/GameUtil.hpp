#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <chrono>
#pragma comment(lib,"OpenGL32")

using namespace std;

namespace Game_Engine
{
	class Object {
	public:
		float x = 0, y = 0, width = 0, height = 0, level = 0; // 오브젝트 사이즈
		string name; // 해당 오브젝트의 이름
		bool setActive; // 해당 오브젝트의 활성화 여부
		float speed, frameCount, nowFrameCount; // 이동하는 속도, 프레임 카운터
		int frame;
		bool is_sliding;

		void Update(int s) {
			x -= (speed + s);
			if (setActive) {
				x -= speed;
				if (x < -1 - width) setActive = false;
				nowFrameCount -= 1;
				if (nowFrameCount < 0) {
					nowFrameCount = frameCount;
					frame += 1;
					if (frame == 4) frame = 0;
				}
			}
		}
	};

	class Background : public Object {
	public:
		Background() {
			x = 0;
			y = 0;
			height = 0;
			width = 0;
			speed = 0;
			name = "name";
			setActive = true;
		}

		void Set(float px, float py, float h, float w, float s, string n) {
			x = px;
			y = py;
			height = h;
			width = w;
			speed = s;
			name = n;
		}

		void Update(int s) {
			x -= (speed + s);
			if (x < -3) x = 1;
		}

		void Respawn() {
			x = 1;
			setActive = true;
		}
	};

	class Bead : public Object { // 여의주
	public:
		Bead() {
			name = "Bead";
			x = 1;
			width = 0.25f;
			height = 0.25f;
			setActive = false;
			speed = 0.025f;
		}

		void Respawn() {
			srand((unsigned int)time(NULL)); //seed값으로 현재시간 부여
			x = 1;
			float a = rand() % 25;
			int b = rand() % 2;
			a = a / 50;
			if (b == 0) a *= -1;
			y = a;
			setActive = true;
		}
	};

	class Enemy : public Object { // 적
	public:
		Enemy() {
			name = "Enemy";
			width = 0.2f;
			height = 0.2f;
			x = -1.6f;
			speed = 0.022f;
			setActive = false;
			frame = 0;
			frameCount = 2;
			nowFrameCount = frameCount;
		}

		void Respawn() {
			srand((unsigned int)time(NULL)); //seed값으로 현재시간 부여
			x = 1;
			float a = rand() % 50;
			int b = rand() % 2;
			a = a / 100;
			if (b == 0) a *= -1;
			y = a;
			frame = 0;
			nowFrameCount = frameCount;
			setActive = true;
		}
	};
	
	class UI : public Object {
	public :
		void Set(float px, float py, float h, float w, string n) {
			x = px;
			y = py;
			height = h;
			width = w;
			name = n;
			setActive = true;
		}

		void ChangeVal(int i) {
			switch (i) {
			case 0:
				name = "Zero";
				break;
			case 1:
				name = "One";
				break;
			case 2:
				name = "Two";
				break;
			case 3:
				name = "Three";
				break;
			case 4:
				name = "Four";
				break;
			case 5:
				name = "Five";
				break;
			case 6:
				name = "Six";
				break;
			case 7:
				name = "Seven";
				break;
			case 8:
				name = "Eight";
				break;
			case 9:
				name = "Nine";
				break;
			default :
				name = "Zero";
				break;
			}
		}

		void SetActive(bool b) {
			setActive = b;
		}
	};

	class Player : public Object {
	private:
		bool is_jump, is_jumpable, doublejump;
		float jumpPower, jumpHeight, gravity, Collisionheight, Collisionwidth, CollisionX, CollisionY;
	public:
		float score;
		Player() {
			name = "Player";
			x = -0.75f;
			y = -1;
			width = 0.5f;
			height = 0.5f;
			Collisionwidth = 0.35f;
			Collisionheight = 0.35f;
			is_jump = false;
			doublejump = false;
			jumpPower = 0;
			gravity = 0.03f;
			jumpHeight = 0;
			is_jumpable = true;
			is_sliding = false;
			level = 1;
			score = 0;
			speed = 0;
			setActive = true;
			frameCount = 12;
			nowFrameCount = frameCount;
			frame = 0;
			CollisionX = x + 0.075f;
		}

		void isNotDownPressed() { // 아래 버튼이 안눌린동안 슬라이딩 취소
			if (is_sliding) { // 슬라이딩중이라면 슬라이딩 취소
				is_sliding = false;
				frame = 0;
				nowFrameCount = frameCount;
			}
		}

		void isDownPressed() { // 아래 버튼 눌리면 중력 증가 및 슬라이딩
			if (!is_jumpable) { // 점프중이라면 중력 증가
				gravity += 0.003f;
			}
			else if(!is_sliding){
				is_sliding = true;
				frame = 0;
				nowFrameCount = frameCount;
			}
		}

		void isSpacePressed() { // 스페이스바 눌리면 점프명령
			if (is_jumpable) {
				is_jumpable = false;
				is_jump = true;
				jumpHeight = y + 0.8f;
				jumpPower = 0.15f;
			}
			else if (doublejump) {
				doublejump = false;
				is_jump = true;
				jumpHeight = y + 0.8f;
				jumpPower = 0.15f;
			}
		}

		int Update() { // 프레임당 처리
			if (setActive == true) {
				CollisionX = x + 0.075f;
				CollisionY = y + 0.075f;
				if (is_sliding) { // 슬라이딩중이라면 키 작게
					Collisionheight = 0.25f;
				}
				else { // 슬라이딩중이 아니라면 키 크게
					Collisionheight = 0.35f;
				}
				if (is_jump) { // is_jump일때 점프
					if (y < jumpHeight) { // 점프 목표 높이에 도달할때까지 점프
						y += jumpPower;
						if (jumpPower > 0.05f) jumpPower -= 0.003f;
					}
					else { // 점프 목표 높이에 도달시 하강
						gravity = 0.01f;
						is_jump = false;
						if (level == 2) {
							doublejump = true;
							level = 3;
						}
					}
				}
				else if (!(is_jumpable) && !(is_jump)) {
					y -= gravity;
					gravity += 0.002f;
					if (y < -1) {
						y = -1;
						is_jumpable = true;
						if (level == 3) level = 2;
					}
				}
				if(speed < 0.02f)speed += 0.0005f;
				if (nowFrameCount > 0) {
					nowFrameCount -= 1;
				}
				else {
					nowFrameCount = frameCount;
					frame += 1;
					if (frame == 4) frame = 0;
					return 1;
				}
			}
			else {
				return 0;
			}
		}

		int isCollision(Object o) { // 오브젝트와 충돌했는지 체크
			if (o.setActive) {
				if (CollisionX + Collisionwidth > o.x && CollisionX < o.x + o.width && y + Collisionheight > o.y && y < o.y + o.height) {
					if (o.name == "Enemy") {
						setActive = false;
						return 0;
					}
					else if (o.name == "Bead") {
						score += 1;
						if (score == 3) {
							level = 2;
							doublejump = true;
							return 2;
						}
						return 1;
					}
				}
			}
			return 0;
		}
	};

	typedef struct tagBITMAPHEADER {
		BITMAPFILEHEADER bf;
		BITMAPINFOHEADER bi;
	}BITMAPHEADER;

	class Renderer
	{
	public :
		BYTE* snake_walk[4];
		BYTE* snake_slide[4];
		BYTE* leemoogi_walk[4];
		BYTE* leemoogi_slide[4];
		BYTE* Enemy[4];
		BYTE* Background[4];
		BYTE* Bead;
		BYTE* Scene[2];
		BYTE* Num[10];
		BYTE* Score;
		BYTE* Str[2];
		BYTE* Rank[3];
	private:
		GLFWwindow* window;
		BITMAPHEADER originalHeader;	//비트맵의 헤더부분을 파일에서 읽어 저장할 구조체
		int imgSize;			//이미지의 크기를 저장할 변수
		GLuint texName;

		BYTE* LoadBitmapFile(BITMAPHEADER* bitmapHeader, int* imgSize, const char* filename)
		{
			FILE* fp = fopen(filename, "rb");	//파일을 이진읽기모드로 열기
			if (fp == NULL)
			{
				printf("파일로딩에 실패했습니다.\n");	//fopen에 실패하면 NULL값을 리턴
				return NULL;
			}
			else
			{
				fread(&bitmapHeader->bf, sizeof(BITMAPFILEHEADER), 1, fp);	//비트맵파일헤더 읽기
				fread(&bitmapHeader->bi, sizeof(BITMAPINFOHEADER), 1, fp);	//비트맵인포헤더 읽기

				int imgSizeTemp = (bitmapHeader->bi.biWidth * bitmapHeader->bi.biHeight) * 4;	//이미지 사이즈 계산
				*imgSize = imgSizeTemp;	// 이미지 사이즈를 상위 변수에 할당

				BYTE* image = (BYTE*)malloc(sizeof(BYTE) * imgSizeTemp);	//이미지크기만큼 메모리할당
				fread(image, sizeof(BYTE), imgSizeTemp, fp);//이미지 크기만큼 파일에서 읽어오기

				BYTE B, R;

				for (int i = 0; i < *imgSize; i += 4) {

					B = image[i];
					R = image[i + 2];

					image[i] = R;
					image[i + 2] = B;
				}
				fclose(fp);

				return image;
			}
		}

		static void error_callback(int error, const char* description)
		{
			fputs(description, stderr);
		}
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GL_TRUE);
		}

	public:
		Renderer() {
			glfwSetErrorCallback(error_callback);
			if (!glfwInit())
				exit(EXIT_FAILURE);
			window = glfwCreateWindow(640, 480, "Dodge_Elflee", NULL, NULL);
			if (!window)
			{
				glfwTerminate();
				exit(EXIT_FAILURE);
			}
			glfwMakeContextCurrent(window);
			glfwSetKeyCallback(window, key_callback);
			setTextureByte();
		}
		~Renderer() {
			glfwDestroyWindow(window);
			glfwTerminate();
			exit(EXIT_SUCCESS);
		}

		void setTextureByte() {
			snake_walk[0] = LoadBitmapFile(&originalHeader, &imgSize, "snake_walk1.bmp");
			snake_walk[1] = LoadBitmapFile(&originalHeader, &imgSize, "snake_walk2.bmp");
			snake_walk[2] = LoadBitmapFile(&originalHeader, &imgSize, "snake_walk3.bmp");
			snake_walk[3] = LoadBitmapFile(&originalHeader, &imgSize, "snake_walk4.bmp");
			snake_slide[0] = LoadBitmapFile(&originalHeader, &imgSize, "snake_slide1.bmp");
			snake_slide[1] = LoadBitmapFile(&originalHeader, &imgSize, "snake_slide2.bmp");
			snake_slide[2] = LoadBitmapFile(&originalHeader, &imgSize, "snake_slide3.bmp");
			snake_slide[3] = LoadBitmapFile(&originalHeader, &imgSize, "snake_slide4.bmp");
			leemoogi_walk[0] = LoadBitmapFile(&originalHeader, &imgSize, "leemoogi_walk1.bmp");
			leemoogi_walk[1] = LoadBitmapFile(&originalHeader, &imgSize, "leemoogi_walk2.bmp");
			leemoogi_walk[2] = LoadBitmapFile(&originalHeader, &imgSize, "leemoogi_walk3.bmp");
			leemoogi_walk[3] = LoadBitmapFile(&originalHeader, &imgSize, "leemoogi_walk4.bmp");
			leemoogi_slide[0] = LoadBitmapFile(&originalHeader, &imgSize, "leemoogi_slide1.bmp");
			leemoogi_slide[1] = LoadBitmapFile(&originalHeader, &imgSize, "leemoogi_slide2.bmp");
			leemoogi_slide[2] = LoadBitmapFile(&originalHeader, &imgSize, "leemoogi_slide3.bmp");
			leemoogi_slide[3] = LoadBitmapFile(&originalHeader, &imgSize, "leemoogi_slide4.bmp");
			Enemy[0] = LoadBitmapFile(&originalHeader, &imgSize, "Enemy1.bmp");
			Enemy[1] = LoadBitmapFile(&originalHeader, &imgSize, "Enemy2.bmp");
			Enemy[2] = LoadBitmapFile(&originalHeader, &imgSize, "Enemy3.bmp");
			Enemy[3] = LoadBitmapFile(&originalHeader, &imgSize, "Enemy4.bmp");
			Background[0] = LoadBitmapFile(&originalHeader, &imgSize, "Ground.bmp");
			Background[1] = LoadBitmapFile(&originalHeader, &imgSize, "Sky.bmp");
			Background[2] = LoadBitmapFile(&originalHeader, &imgSize, "Mountain.bmp");
			Background[3] = LoadBitmapFile(&originalHeader, &imgSize, "Cloud.bmp");
			Bead = LoadBitmapFile(&originalHeader, &imgSize, "Bead.bmp");
			Scene[0] = LoadBitmapFile(&originalHeader, &imgSize, "Title.bmp");
			Scene[1] = LoadBitmapFile(&originalHeader, &imgSize, "End.bmp");
			Num[0] = LoadBitmapFile(&originalHeader, &imgSize, "Zero.bmp");
			Num[1] = LoadBitmapFile(&originalHeader, &imgSize, "One.bmp");
			Num[2] = LoadBitmapFile(&originalHeader, &imgSize, "Two.bmp");
			Num[3] = LoadBitmapFile(&originalHeader, &imgSize, "Three.bmp");
			Num[4] = LoadBitmapFile(&originalHeader, &imgSize, "Four.bmp");
			Num[5] = LoadBitmapFile(&originalHeader, &imgSize, "Five.bmp");
			Num[6] = LoadBitmapFile(&originalHeader, &imgSize, "Six.bmp");
			Num[7] = LoadBitmapFile(&originalHeader, &imgSize, "Seven.bmp");
			Num[8] = LoadBitmapFile(&originalHeader, &imgSize, "Eight.bmp");
			Num[9] = LoadBitmapFile(&originalHeader, &imgSize, "Nine.bmp");
			Score = LoadBitmapFile(&originalHeader, &imgSize, "Score.bmp");
			Str[0] = LoadBitmapFile(&originalHeader, &imgSize, "Pause.bmp");
			Str[1] = LoadBitmapFile(&originalHeader, &imgSize, "Rank.bmp");
			Rank[0] = LoadBitmapFile(&originalHeader, &imgSize, "1st.bmp");
			Rank[1] = LoadBitmapFile(&originalHeader, &imgSize, "2nd.bmp");
			Rank[2] = LoadBitmapFile(&originalHeader, &imgSize, "3rd.bmp");
		}

		void ready() {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
			glGenTextures(1, &texName);
			glBindTexture(GL_TEXTURE_2D, texName);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		void setTexture(BYTE* image) { // 지정한 FileName으로 Bitmap파일 불러와서 텍스쳐 설정
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		}

		void Clear() { // 화면 청소
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			glClearColor(1, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void DrawScene(int A) // 시작 / 결과화면 출력
		{
			if (!glfwWindowShouldClose(window))
			{
				if (A == 1) {
					setTexture(Scene[0]); // Title
				}
				if (A == 3) {
					setTexture(Scene[1]); // End
				}
				glEnable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
				glColor3f(1, 1, 1);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
				glTexCoord2f(0.0f, 1.0f); glVertex2f(-1, 1);
				glTexCoord2f(1.0f, 1.0f); glVertex2f(1, 1);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(1, -1);
				glTexCoord2f(0.0f, 0.0f); glVertex2f(-1, -1);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			else {
				return;
			}
		}

		int Draw(Object obj) // 오브젝트 출력
		{
			if (!glfwWindowShouldClose(window))
			{
				if (obj.setActive == true) {
					if (obj.name == "Player") {
						if (obj.level == 1) {
							if (obj.is_sliding == false) {
								setTexture(snake_walk[obj.frame]);
							}
							else {
								setTexture(snake_slide[obj.frame]);
							}
						}
						else{
							if (obj.is_sliding == false) {
								setTexture(leemoogi_walk[obj.frame]);
							}
							else {
								setTexture(leemoogi_slide[obj.frame]);
							}
						}
					}
					else if (obj.name == "Enemy") {
						setTexture(Enemy[obj.frame]);
					}
					else if (obj.name == "Bead") {
						setTexture(Bead);
					}
					else if (obj.name == "Ground")
					{
						setTexture(Background[0]); // Ground
					}
					else if (obj.name == "Sky")
					{
						setTexture(Background[1]); // Sky
					}
					else if (obj.name == "Mountain")
					{
						setTexture(Background[2]); // Mountain
					}
					else if (obj.name == "Cloud")
					{
						setTexture(Background[3]); // Cloud
					}
					else if (obj.name == "Zero")
					{
						setTexture(Num[0]); // 0
					}
					else if (obj.name == "One")
					{
						setTexture(Num[1]); // 1
					}
					else if (obj.name == "Two")
					{
						setTexture(Num[2]); // 2
					}
					else if (obj.name == "Three")
					{
						setTexture(Num[3]); // 3
					}
					else if (obj.name == "Four")
					{
						setTexture(Num[4]); // 4
					}
					else if (obj.name == "Five")
					{
						setTexture(Num[5]); // 5
					}
					else if (obj.name == "Six")
					{
						setTexture(Num[6]); // 6
					}
					else if (obj.name == "Seven")
					{
						setTexture(Num[7]); // 7
					}
					else if (obj.name == "Eight")
					{
						setTexture(Num[8]); // 8
					}
					else if (obj.name == "Nine")
					{
						setTexture(Num[9]); // 9
					}
					else if (obj.name == "Score")
					{
						setTexture(Score); // Score
					}
					else if (obj.name == "Pause")
					{
						setTexture(Str[0]); // Pause
					}
					else if (obj.name == "Rank")
					{
						setTexture(Str[1]); // Rank
					}
					else if (obj.name == "1st")
					{
						setTexture(Rank[0]); // Rank
					}
					else if (obj.name == "2nd")
					{
						setTexture(Rank[1]); // Rank
					}
					else if (obj.name == "3rd")
					{
						setTexture(Rank[2]); // Rank
					}
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glEnable(GL_TEXTURE_2D);
					glBegin(GL_QUADS);
					glColor4f(1, 1, 1, 1);
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
					glTexCoord2f(0.0f, 1.0f); glVertex2f(obj.x, (obj.y + obj.height));
					glTexCoord2f(1.0f, 1.0f); glVertex2f((obj.x + obj.width), (obj.y + obj.height));
					glTexCoord2f(1.0f, 0.0f); glVertex2f((obj.x + obj.width), obj.y);
					glTexCoord2f(0.0f, 0.0f); glVertex2f(obj.x, obj.y);
					glEnd();
					glDisable(GL_TEXTURE_2D);
					return 1;
				}
			}
			else {
				return 0;
			}
		}

		void Rendering() {
			if (!glfwWindowShouldClose(window))
			{
				glfwSwapBuffers(window);
				glfwPollEvents();
			}
		}

	};
};
