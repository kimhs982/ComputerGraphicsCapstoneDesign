#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define INITIAL_SPIN_SPEED 2.0
#define MAX_DUST_REGION 20
#define DUST_NUM 20
#define _USE_MATH_DEFINES

#include  <GL/glut.h>
#include <GL/freeglut.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "bmpfuncs.h"
#include "ObjParser.h"

// 전역변수
volatile double radius = 15;
volatile double theta = 45.0;
volatile double phi = 45.0;
volatile double upVector = cos(phi * M_PI / 180);
volatile double eye_x, eye_y, eye_z;		// 눈(카메라)의 위치
volatile double center[3] = { 0, 0, 0 };		// 눈(카메라)이 바라보는 중심
int selector = 0;		// 0: all, 1: brush bar, 2: end cap, 3: head body, 4: cleaning bar, 5: engine body
double brush_rotate_angle;		// brush bar 회전 각도
double brush_rotate_speed = INITIAL_SPIN_SPEED;		// brush bar 회전 속도
double trans_x[6], trans_y[6], trans_z[6];		// 평행이동, 5: 먼지통
double vacuum_rotate_angle;		// 청소기 회전 각도(y축 기준)
int point_x[DUST_NUM], point_z[DUST_NUM];		// point dust 개수
int line_x1[DUST_NUM], line_x2[DUST_NUM], line_z1[DUST_NUM], line_z2[DUST_NUM];		// line dust 개수
//int triangle_x1[DUST_NUM], triangle_x2[DUST_NUM], triangle_x3[DUST_NUM], triangle_z1[DUST_NUM], triangle_z2[DUST_NUM], triangle_z3[DUST_NUM];			// triangle dust 개수
double cleaning_vacuum_pos_x, cleaning_vacuum_pos_z;		// cleaning mode에서 brush bar의 위치
double cleaning_range = INITIAL_SPIN_SPEED / 2.0;		// 먼지를 빨아들이는 범위
int cleaning_bar_color = 0;		// cleaning bar의 색

bool disassemble = false;		// 분해
bool antialiase_on = false;		// Antialiasing
bool display_axis = false;		// Axis
bool cleaning_mode = false;		// 청소
bool vacuum_switch = false;		// 청소기 스위치
bool brush_bar_sep = false;		// brush bar 분리
bool end_cap_sep = false;		// end cap 분리
bool head_body_sep = false;		// head body 분리
bool cleaning_bar_sep = false;		// cleaning bar 분리
bool engine_body_sep = false;		// engine body 분리
bool dust_container_sep = false;		// 먼지통 분리
bool cleaning_bar_color_changed = true;		// 청소봉의 색 변경 여부

// object variables
ObjParser* brushBar;
ObjParser* endCap;
ObjParser* headBody[8];		// 0: glass, 1: light, 2: button
ObjParser* cleaningBar[3];
ObjParser* engineBody[13];		// 0: glass, 1: start button, 2: discard button, 12: display button

// texture mapping set
GLuint brushBar_obj;
GLuint endCap_obj;
GLuint headBody_obj[8];
GLuint cleaningBar_obj[3];
GLuint engineBody_obj[13];

// 함수 선언
void init(void);
void resize(int, int);

void idle(void);

void draw(void);
void drawObject(void);		// obj 파일들을 그리는 함수
void draw_axis(double);
void draw_text(void);		// 화면에 text를 보여주는 함수
void draw_string(void*, const char*, float, float, float, float, float);

ObjParser* get_resource(const char*);		// ObjParser 클래스 객체 동적 할당
void load_resources(void);		// obj 파일을 ObjParser 클래스의 생성자로 넘겨주어 객체 생성
void setTextureMapping(const char*, GLuint&);
void draw_obj(ObjParser*);
void draw_obj_with_texture(ObjParser*, GLuint&);

void mouse(int, int, int, int);
void motion(int, int);
void passiveMotion(int, int);
void mouseWheel(int, int, int, int);

void keyboard(uchar, int, int);
void special_keyboard(int, int, int);

void popup_menu(void);
void main_menu_function(int);
void sub_menu_function1(int);

void light_default(void);
void set_default(void);

void print_instruction(void);
void dust_maker(void);		// 무작위로 먼지 좌표 생성
void draw_dust(void);		// 먼지 생성/제거

void cal_cleaning_vacuum_pos(void);

int main(int argc, char** argv) {
	// Window 초기화
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(-1, -1);		// 창의 왼쪽(Left), 위쪽(Top) 코너를 설정. 기본값(-1)
	glutCreateWindow("Computer Graphics mini project 12181763 김형석");
	init();

	// pop-up menu
	popup_menu();

	// resource road
	load_resources();

	// Draw callback
	glutDisplayFunc(draw);

	// Idle callback
	glutIdleFunc(idle);

	// Mouse wheel callback
	glutMouseWheelFunc(mouseWheel);

	// Keyboard callback
	glutKeyboardFunc(keyboard);

	// Special keyboard callbak
	glutSpecialFunc(special_keyboard);

	// Main loop start
	glutMainLoop();

	return 0;
}

// 함수 정의
void init(void) {
	// 화면의 기본색: White
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);		// Alpha: 0(불투명) ~ 1(투명)

	// Clear buffer
	glClearDepth(1.0);

	// set blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set antialiasing
	glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glEnable(GL_LINE_SMOOTH);

	light_default();

	// Resize callback
	glutReshapeFunc(resize);

	// Print instruction
	print_instruction();


	// texture mapping setting
	glEnable(GL_TEXTURE_2D);
	setTextureMapping("./bmp/BrushBar.bmp", brushBar_obj);
	setTextureMapping("./bmp/Black.bmp", endCap_obj);
	setTextureMapping("./bmp/Green.bmp", headBody_obj[1]);
	setTextureMapping("./bmp/Green.bmp", headBody_obj[2]);
	setTextureMapping("./bmp/Green.bmp", headBody_obj[3]);
	setTextureMapping("./bmp/Gray.bmp", headBody_obj[4]);
	setTextureMapping("./bmp/Blue.bmp", headBody_obj[5]);
	setTextureMapping("./bmp/Gray.bmp", headBody_obj[6]);
	setTextureMapping("./bmp/Red.bmp", headBody_obj[7]);
	setTextureMapping("./bmp/Gray.bmp", cleaningBar_obj[1]);
	setTextureMapping("./bmp/Red.bmp", cleaningBar_obj[2]);
	setTextureMapping("./bmp/Red.bmp", engineBody_obj[1]);
	setTextureMapping("./bmp/Red.bmp", engineBody_obj[2]);
	setTextureMapping("./bmp/Gray.bmp", engineBody_obj[3]);
	setTextureMapping("./bmp/Black.bmp", engineBody_obj[4]);
	setTextureMapping("./bmp/Metal.bmp", engineBody_obj[5]);
	setTextureMapping("./bmp/Red.bmp", engineBody_obj[6]);
	setTextureMapping("./bmp/EngineColor.bmp", engineBody_obj[7]);
	setTextureMapping("./bmp/Gray.bmp", engineBody_obj[8]);
	setTextureMapping("./bmp/Cyan.bmp", engineBody_obj[9]);
	setTextureMapping("./bmp/Black.bmp", engineBody_obj[10]);
	setTextureMapping("./bmp/InnerDisplay.bmp", engineBody_obj[11]);
	setTextureMapping("./bmp/Metal.bmp", engineBody_obj[12]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //GL_REPLACE : polygon의 원래 색상은 무시하고 texture로 덮음

	glEnable(GL_DEPTH_TEST);		// Depth test enable
}

void resize(int width, int height) {
	glViewport(0, 0, width, height);
	printf("WIDTH: %d HEIGHT: %d\n", width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*
	Perspective 설정 gluPerspective(fovy, aspect, zNear, zFar)
	첫 번째 인자 fovy: 수직 방향의 field of view --> 단위 각(degree), 자료형 double
	두 번째 인자 aspect: clipping window의 가로:세로 비율 --> 가로/세로, 자료형 double
	세 번째 인자 zNear: Near plane까지의 거리 ( > 0 ) --> 자료형 double
	네 번째 인자 zFar: Far plane까지의 거리 ( > 0 ) --> 자료형 double
	*/
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}
void idle(void) {
	if (vacuum_switch) {		// cleaning mode & turn on 상태인 경우
	// brush bar 회전
		brush_rotate_angle += brush_rotate_speed;
		if (brush_rotate_angle > 360)
			brush_rotate_angle -= 360;
	}

	glutPostRedisplay();
}

void draw(void) {
	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	eye_x = radius * sin(theta * M_PI / 180) * cos(phi * M_PI / 180);
	eye_y = radius * sin(phi * M_PI / 180);
	eye_z = radius * cos(theta * M_PI / 180) * cos(phi * M_PI / 180);
	upVector = cos(phi * M_PI / 180);

	gluLookAt(eye_x, eye_y, eye_z, center[0], center[1], center[2], 0, upVector, 0);

	//glDisable(GL_LIGHT1);
	GLfloat light_position[] = { eye_x, eye_y, eye_z, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);


	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	if (display_axis)
		draw_axis(4);
	draw_text();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	// cleaning bar texture mapping setting
	if (cleaning_bar_color == 0 && cleaning_bar_color_changed) {
		setTextureMapping("./bmp/CleaningBar0.bmp", cleaningBar_obj[0]);
		cleaning_bar_color_changed = false;
	}
	else if (cleaning_bar_color == 1 && cleaning_bar_color_changed) {
		setTextureMapping("./bmp/CleaningBar1.bmp", cleaningBar_obj[0]);
		cleaning_bar_color_changed = false;
	}

	drawObject();

	if (cleaning_mode)
		draw_dust();
	
	glFlush();
	glutSwapBuffers();
}

void drawObject(void) {
	// Brush bar
	glColor3f(1.f, 1.f, 1.f);
	if (selector == 0 || selector == 1) {
		glPushMatrix();
		glRotatef(vacuum_rotate_angle, 0, 1, 0);		// 회전
		glTranslatef(trans_x[0], trans_y[0], trans_z[0]);		// 평행이동
		glRotatef(brush_rotate_angle, 1, 0, 0);		// brush bar 회전
		draw_obj_with_texture(brushBar, brushBar_obj);
		glPopMatrix();
	}

	// End cap
	if (selector == 0 || selector == 2) {
		glPushMatrix();
		glRotatef(vacuum_rotate_angle, 0, 1, 0);		// 회전
		glTranslatef(trans_x[1], trans_y[1], trans_z[1]);		// 평행이동
		draw_obj_with_texture(endCap, endCap_obj);
		glPopMatrix();
	}

	// Head body
	if (selector == 0 || selector == 3) {
		glPushMatrix();
		glRotatef(vacuum_rotate_angle, 0, 1, 0);		// 회전
		glTranslatef(trans_x[2], trans_y[2], trans_z[2]);		// 평행이동
		draw_obj(headBody[0]);
		for (int i = 1; i < 8; i++)
			draw_obj_with_texture(headBody[i], headBody_obj[i]);
		glPopMatrix();
	}

	// Cleaning bar
	if (selector == 0 || selector == 4) {
		glPushMatrix();
		glRotatef(vacuum_rotate_angle, 0, 1, 0);		// 회전
		glTranslatef(trans_x[3], trans_y[3], trans_z[3]);		// 평행이동
		for (int i = 0; i < 3; i++)
			draw_obj_with_texture(cleaningBar[i], cleaningBar_obj[i]);
		glPopMatrix();
	}

	// Engine body
	if (selector == 0 || selector == 5) {
		glPushMatrix();
		glRotatef(vacuum_rotate_angle, 0, 1, 0);		// 회전
		glTranslatef(trans_x[4], trans_y[4], trans_z[4]);		// 평행이동
		glPushMatrix();
		glTranslatef(trans_x[5], trans_y[5], trans_z[5]);
		draw_obj(engineBody[0]);
		glPopMatrix();
		for (int i = 1; i < 13; i++)
			draw_obj_with_texture(engineBody[i], engineBody_obj[i]);
		glPopMatrix();
	}
}

void draw_axis(double length) {
	if (display_axis) {
		glLineWidth(2);

		glBegin(GL_LINES);
		glColor3f(1, 0, 0);		// x axis color: Red
		glVertex3f(0, 0, 0);
		glVertex3f(length, 0, 0);

		glColor3f(0, 1, 0);		// y axis color: Green
		glVertex3f(0, 0, 0);
		glVertex3f(0, length, 0);

		glColor3f(0, 0, 1);		// z axis color: Blue
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, length);
		glEnd();

		glLineWidth(1);
	}
	glutPostRedisplay();
}

void draw_text(void) {
	char* str_selector = (char*)"All";
	switch (selector) {
	case 0:
		str_selector = (char*)"All";
		break;
	case 1:
		str_selector = (char*)"Brush bar";
		break;
	case 2:
		str_selector = (char*)"End cap";
		break;
	case 3:
		str_selector = (char*)"Head body";
		break;
	case 4:
		str_selector = (char*)"Cleaning bar";
		break;
	case 5:
		str_selector = (char*)"Engine body";
		break;
	default:
		break;
	}
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, str_selector, -9, 9, 0.0, 1.0, 1.0);

	if (!cleaning_mode && selector == 0) {
		draw_string(GLUT_BITMAP_HELVETICA_18, "----------------------mouse & keyboard navigation----------------------", -8.5, -5.0, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "mouse wheel: zoom in/out", -8.5, -5.5, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "directional key: camera location", -8.5, -6.0, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "'a': antialiasing on/off", -8.5, -6.5, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "'s': disassemble/assemble(all)", -8.5, -7.0, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "'d': print/remove draw_axis", -8.5, -7.5, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "tab: change cleaning bar color", -8.5, -8.0, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "F5: restart", -8.5, -8.5, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "esc: exit",-8.5, -9.0, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "0/1/2/3/4: disassemble/assemble brush bar/end cap/head body/cleaning bar/engine body", -8.5, -9.5, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "------------------------------------------------------------------", -8.5, -10.0, 0.0, 0.0, 0.0);
	}
	else if (!cleaning_mode && selector != 0) {
		double gap = 0.0;
		if (selector == 5)
			gap = 0.5;
		draw_string(GLUT_BITMAP_HELVETICA_18, "----mouse & keyboard navigation----", 1.5, -6.0 + gap, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "mouse wheel: zoom in/out", 1.5, -6.5 + gap, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "directional key: camera location", 1.5, -7.0 + gap, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "'a': antialiasing on/off", 1.5, -7.5 + gap, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "'d': print/remove draw_axis", 1.5, -8.0 + gap, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "tab: change cleaning bar color", 1.5, -8.5 + gap, 0.0, 0.0, 0.0);
		if (selector == 5) {
			draw_string(GLUT_BITMAP_HELVETICA_18, "'p': separate dust container", 1.5, -8.5, 0.0, 0.0, 0.0);
			gap = 0.5;
		}
		draw_string(GLUT_BITMAP_HELVETICA_18, "F5: restart", 1.5, -9.0, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "esc: exit", 1.5, -9.5, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "------------------------------", 1.5, -10.0, 0.0, 0.0, 0.0);
	}
	else if (cleaning_mode && selector == 0) {
		draw_string(GLUT_BITMAP_HELVETICA_18, "------------mouse & keyboard navigation------------", -3.0, -5.5, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "mouse wheel: zoom in/out", -3.0, -6.0, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "directional key: camera location", -3.0, -6.5, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "tab: change cleaning bar color", -3.0, -7.0, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "'p': vacuum cleaner turn on/off", -3.0, -7.5, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "'-'/'+': brush bar spin speed up/down", -3.0, -8.0, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "'w'/'a'/'s'/'d': move vacuum cleaner forward/left/backward/right", -3.0, -8.5, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "F5: restart", -3.0, -9.0, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "esc: exit", -3.0, -9.5, 0.0, 0.0, 0.0);
		draw_string(GLUT_BITMAP_HELVETICA_18, "----------------------------------------------", -3.0, -10.0, 0.0, 0.0, 0.0);
	}
}

void draw_string(void* font, const char* str, float x_position, float y_position, float red, float green, float blue) {
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(red, green, blue);
	glRasterPos3f(x_position, y_position, 0);
	for (unsigned int i = 0; i < strlen(str); i++)
		glutBitmapCharacter(font, str[i]);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}

ObjParser* get_resource(const char* str) {
	return new ObjParser(str);
}

void load_resources(void) {
	brushBar = get_resource("./obj/BrushBar.obj");
	endCap = get_resource("./obj/EndCap.obj");
	headBody[0] = get_resource("./obj/HeadBody0.obj");
	headBody[1] = get_resource("./obj/HeadBody1.obj");
	headBody[2] = get_resource("./obj/HeadBody2.obj");
	headBody[3] = get_resource("./obj/HeadBody3.obj");
	headBody[4] = get_resource("./obj/HeadBody4.obj");
	headBody[5] = get_resource("./obj/HeadBody5.obj");
	headBody[6] = get_resource("./obj/HeadBody6.obj");
	headBody[7] = get_resource("./obj/HeadBody7.obj");
	cleaningBar[0] = get_resource("./obj/CleaningBar0.obj");
	cleaningBar[1] = get_resource("./obj/CleaningBar1.obj");
	cleaningBar[2] = get_resource("./obj/CleaningBar2.obj");
	engineBody[0] = get_resource("./obj/EngineBody0.obj");
	engineBody[1] = get_resource("./obj/EngineBody1.obj");
	engineBody[2] = get_resource("./obj/EngineBody2.obj");
	engineBody[3] = get_resource("./obj/EngineBody3.obj");
	engineBody[4] = get_resource("./obj/EngineBody4.obj");
	engineBody[5] = get_resource("./obj/EngineBody5.obj");
	engineBody[6] = get_resource("./obj/EngineBody6.obj");
	engineBody[7] = get_resource("./obj/EngineBody7.obj");
	engineBody[8] = get_resource("./obj/EngineBody8.obj");
	engineBody[9] = get_resource("./obj/EngineBody9.obj");
	engineBody[10] = get_resource("./obj/EngineBody10.obj");
	engineBody[11] = get_resource("./obj/EngineBody11.obj");
	engineBody[12] = get_resource("./obj/EngineBody12.obj");
}

void setTextureMapping(const char* bmp_file, GLuint& obj) {
	int imgWidth, imgHeight, channels;
	uchar* img = readImageData(bmp_file, &imgWidth, &imgHeight, &channels);

	int texNum = 1;
	glGenTextures(texNum, &obj);
	glBindTexture(GL_TEXTURE_2D, obj);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, img);
}

void draw_obj(ObjParser* objParser) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3) {
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x,
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x,
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}
	glEnd();
}

void draw_obj_with_texture(ObjParser* objParser, GLuint& obj_texture) {
	glDisable(GL_BLEND);
	// glEnable(GL_TEXTURE_2D);	// texture 색 보존을 위한 enable
	glBindTexture(GL_TEXTURE_2D, obj_texture);
	//glBindTexture(GL_TEXTURE_2D, textureObj);
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3) {
		glTexCoord2f(objParser->textures[objParser->textureIdx[n] - 1].x,
			objParser->textures[objParser->textureIdx[n] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x,
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x,
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

		glTexCoord2f(objParser->textures[objParser->textureIdx[n + 1] - 1].x,
			objParser->textures[objParser->textureIdx[n + 1] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		glTexCoord2f(objParser->textures[objParser->textureIdx[n + 2] - 1].x,
			objParser->textures[objParser->textureIdx[n + 2] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}
	glEnd();
	glEnable(GL_BLEND);
}

void mouse(int button, int state, int x, int y) {

	glutPostRedisplay();
}

void mouseWheel(int button, int dir, int x, int y) {
	if (dir > 0) {
		if (radius > 1) radius -= 0.5;
		printf("zoom in! [radius(%.1lf)]\n", radius);
	}
	else {
		if (radius < 100) radius += 0.5;
		printf("zoom out! [radius(%.1lf)]\n", radius);
	}
	glutPostRedisplay();
}

void keyboard(uchar key, int x, int y) {
	switch (key) {
	case 'w':
		if (cleaning_mode) {
			for (int i = 0; i < 5; i++)
				trans_z[i] += 0.1;
			printf("moving forward!\n");
		}
		break;
	case 's':
		if (!cleaning_mode) {
			if (disassemble) {
				disassemble = false;
				cleaning_bar_sep = end_cap_sep = head_body_sep = cleaning_bar_sep = engine_body_sep = false;
				for (int i = 0; i < 5; i++)
					trans_x[i] = trans_y[i] = trans_z[i] = 0;
				printf("assemble(all)!\n");
			}
			else {
				if (selector == 0) {
					disassemble = true;
					cleaning_bar_sep = end_cap_sep = head_body_sep = cleaning_bar_sep = engine_body_sep = true;
					trans_z[0] = 4.0;
					trans_x[1] = 3.0;
					trans_x[2] = -4.0;
					trans_y[3] = 2, trans_z[3] = 2;
					trans_y[4] = -4;
					printf("disassemble(all)!\n");
				}
				else
					printf("you can't disassemble object when view mode is \"all(0)\"!\n");
			}
		}
		else {
			for (int i = 0; i < 5; i++)
				trans_z[i] -= 0.1;
			printf("moving backward!\n");
		}
		break;
	case 'a':
		if (!cleaning_mode) {
			if (antialiase_on) {
				antialiase_on = false;
				glDisable(GL_BLEND);
				printf("antialiasing off!\n");
			}
			else {
				antialiase_on = true;
				glEnable(GL_BLEND);
				printf("antialiasing on!\n");
			}
		}
		else {
			vacuum_rotate_angle++;
			if (vacuum_rotate_angle <= -360) vacuum_rotate_angle = 0.0;
			printf("turn left!\n");
		}
		break;
	case 'd':
		if (!cleaning_mode) {
			if (display_axis) {
				display_axis = false;
				printf("remove axis!\n");
			}
			else {
				display_axis = true;
				printf("display axis!\n");
			}
		}
		else {
			vacuum_rotate_angle--;
			if (vacuum_rotate_angle >= 360) vacuum_rotate_angle = 0.0;
			printf("turn right!\n");
		}
		break;
	case 'p':
		if (cleaning_mode) {
			if (vacuum_switch) {
				vacuum_switch = false;
				PlaySound(NULL, 0, 0);
				PlaySound(TEXT("VacuumCleanerTurnOff.wav"), NULL, SND_FILENAME | SND_ASYNC);
				printf("turn off vacuum cleaner!\n");
			}
			else {
				vacuum_switch = true;
				PlaySound(TEXT("VacuumCleanerTurnOn.wav"), NULL, SND_FILENAME | SND_SYNC);
				PlaySound(TEXT("VacuumCleaner.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
				printf("turn on vacuum cleaner!\n");
			}
		}
		else {
			if (selector == 5)
				if (!dust_container_sep) {
					trans_y[5] = -1.0;
					trans_z[5] = 1.0;
					dust_container_sep = true;
				}
				else {
					trans_y[5] = 0.0;
					trans_z[5] = 0.0;
					dust_container_sep = false;
				}
		}
		break;
	case 45:		// '-'
		if (cleaning_mode) {
			if (brush_rotate_speed > 0.11) {
				brush_rotate_speed -= 0.1;
				cleaning_range = brush_rotate_speed / 2.0;
				printf("brush bar spin speed: %.1lf\n", brush_rotate_speed);
			}
		}
		break;
	case 61:		// '+'
		if (cleaning_mode) {
			if (brush_rotate_speed < 9.99) {
				brush_rotate_speed += 0.1;
				cleaning_range = brush_rotate_speed / 2.0;
				printf("brush bar spin speed: %.1lf\n", brush_rotate_speed);
			}
		}
		break;
	case 48:		// 0
		if (!cleaning_bar_sep) {
			trans_z[0] = 4.0;
			cleaning_bar_sep = true;
		}
		else {
			trans_z[0] = 0.0;
			cleaning_bar_sep = false;
		}
		break;
	case 49:		// 1
		if (selector == 0) {
			if (!end_cap_sep) {
				trans_x[1] = 3.0;
				end_cap_sep = true;
			}
			else {
				trans_x[1] = 0;
				end_cap_sep = false;
			}
		}
		break;
	case 50:		// 2
		if(selector == 0){
			if (!head_body_sep) {
				trans_x[2] = -4.0;
				head_body_sep = true;
			}
			else {
				trans_x[2] = 0;
				head_body_sep = false;
			}
		}
		break;
	case 51:		// 3
		if(selector==0){
			if (!cleaning_bar_sep) {
				trans_y[3] = 2;
				trans_z[3] = 2;
				cleaning_bar_sep = true;
			}
			else {
				trans_y[3] = 0;
				trans_z[3] = 0;
				cleaning_bar_sep = false;
			}
		}
		break;
	case 52:		// 4
		if(selector==0){
			if (!engine_body_sep) {
				trans_y[4] = -4;
				engine_body_sep = true;
			}
			else {	
				trans_y[4] = 0;
				engine_body_sep = false;
			}
		}
		break;
	case 9:		// tab
		if (cleaning_bar_color == 0) {
			cleaning_bar_color = 1;
			cleaning_bar_color_changed = true;
		}
		else if (cleaning_bar_color == 1) {
			cleaning_bar_color = 0;
			cleaning_bar_color_changed = true;
		}
		break;
	case 27:		// ESC
		exit(0);
		break;
	default:
		printf("(keyboard)you pressed %d\n", key);
		break;
	}
	glFlush();
	glutPostRedisplay();
}

void special_keyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		theta -= 1;
		if (theta <= -360) theta = 0.0;
		break;
	case GLUT_KEY_RIGHT:
		theta += 1;
		if (theta >= 360) theta = 0.0;
		break;
	case GLUT_KEY_DOWN:
		phi -= 1;
		if (abs((int)phi) % 360 == 0) phi = 0.0;
		break;
	case GLUT_KEY_UP:
		phi += 1;
		if (abs((int)phi) % 360 == 0) phi = 0.0;
		break;
	case 5:		// F5
		print_instruction();
		set_default();
		if (vacuum_switch) {
			PlaySound(NULL, 0, 0);
			PlaySound(TEXT("VacuumCleanerTurnOff.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		cleaning_mode = false;
		vacuum_switch = false;
		break;
	default:
		printf("(special keyboard)you pressed %d\n", key);
	}
	glFlush();
	glutPostRedisplay();
}

void popup_menu(void) {
	int submenu1 = glutCreateMenu(sub_menu_function1);
	glutAddMenuEntry("all", 0);
	glutAddMenuEntry("brush bar", 1);
	glutAddMenuEntry("end cap", 2);
	glutAddMenuEntry("head body", 3);
	glutAddMenuEntry("cleaning bar", 4);
	glutAddMenuEntry("engine body", 5);

	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("init", 0);
	glutAddMenuEntry("cleaning mode", 1);
	glutAddSubMenu("view mode", submenu1);
	glutAddMenuEntry("quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void main_menu_function(int option) {
	if (option == 0) {		// init
		set_default();
		print_instruction();
		if (cleaning_mode) {
			if (vacuum_switch) {
				PlaySound(NULL, 0, 0);
				PlaySound(TEXT("VacuumCleanerTurnOff.wav"), NULL, SND_FILENAME | SND_SYNC);
			}
			radius = 30;
			vacuum_switch = false;
			brush_rotate_speed = INITIAL_SPIN_SPEED;
			display_axis = true;
			dust_maker();
		}
	}
	else if (option == 1) {		// cleaning mode
		if (cleaning_mode) {
			cleaning_mode = false;
			vacuum_switch = false;
			brush_rotate_speed = INITIAL_SPIN_SPEED;
			set_default();
			printf("cleaning mode off!\n\n");
			print_instruction();
			for (int i = 0; i < DUST_NUM; i++) {
				point_x[i] = point_z[i] = 0;
				line_x1[i] = line_x2[i] = line_z1[i] = line_z2[i] = 0;
			}
			draw_dust();
		}
		else {
			if (!disassemble && (selector == 0)) {
				cleaning_mode = true;
				printf("cleaning mode on!\n\n");
				print_instruction();
				display_axis = true;
				dust_maker();
				radius = 30;
			}
			else
				printf("set \"assemble\" or view mode \"all(0)\" before cleaning mode!\n");
		}
	}
	else if (option == 999)		// exit
		exit(0);
	glFlush();
	glutPostRedisplay();
}

void sub_menu_function1(int option) {
	if (!cleaning_mode) {
		switch (option) {
		case 0:
			set_default();
			selector = 0;
			print_instruction();
			break;
		case 1:
			set_default();
			selector = 1;
			print_instruction();
			break;
		case 2:
			set_default();
			selector = 2;
			trans_x[1] = -1;
			print_instruction();
			break;
		case 3:
			set_default();
			selector = 3;
			print_instruction();
			break;
		case 4:
			set_default();
			selector = 4;
			trans_y[3] = -2;
			trans_z[3] = 2;
			print_instruction();
			break;
		case 5:
			set_default();
			selector = 5;
			trans_y[4] = -4;
			trans_z[4] = 4;
			print_instruction();
			break;
		}
		printf("object view mode: %d\n", option);
	}
	else
		printf("set \"cleaning mode\" off before object view mode!\n");

	glutPostRedisplay();
}

void light_default(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	/* Light0 조명 관련 설정 */
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat light_position[] = { eye_x, eye_y, eye_z, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	/********* light point position setting **********/
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	GLfloat specularMaterial[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuseMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat ambientMaterial[] = { 0.9f, 0.9f, 0.9f, 1.0f };

	/************* Material  setting *************/
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);


	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glDisable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// DEPTH TEST ENABLE
	glFrontFace(GL_CW);	// CW CCW바꿔보면서 front face 변경해보기!
}

void set_default(void) {
	radius = 20;
	theta = 45.0;
	phi = 45.0;
	for (int i = 0; i < 6; i++)
		trans_x[i] = trans_y[i] = trans_z[i] = 0;
	display_axis = false;
	antialiase_on = false;
	disassemble = false;
	selector = 0;
	vacuum_rotate_angle = 0;
	brush_rotate_speed = INITIAL_SPIN_SPEED;
	cleaning_range = INITIAL_SPIN_SPEED / 5.0;
	trans_y[5] = trans_z[5] = 0.0;
}

void print_instruction(void) {
	printf("\n-------- mouse & keyboard navigation --------\n");
	printf("mouse wheel: zoom in/out\n");
	printf("directional key: camera location\n");
	if (cleaning_mode) {
		printf("\'p\': vacuum cleaner turn on/off\n");
		printf("\'-\'/\'+\': brush bar spin speed up/down\n");
		printf("\'w\'/\'a\'/\'s\'/\'d\': move vacuum cleaner forward/left/backward/right");
	}
	else {
		printf("\'a\': antialiasing on/off\n");
		if (selector == 0) {
			printf("\'s\': disassemble/assemble(all)\n");
			printf("0/1/2/3/4: disassemble/assemble brush bar/end cap/head body/cleaning bar/engine body\n");
		}
		else if (selector == 5)
			printf("\'p\': separate dust container\n");
		printf("\'d\': print/remove draw_axis\n");
	}
	printf("tab: change cleaning bar color\n");
	printf("F5: 다시 시작\n");
	printf("esc: 종료\n");
	printf("---------------------------------------------\n");
}

void dust_maker(void) {
	srand(unsigned int(time(NULL)));

	int dust_point = rand() % DUST_NUM + 1;
	int dust_line = rand() % DUST_NUM + 1;
	//int dust_triangle = rand() % DUST_NUM + 1;

	// point dust
	for (int i = 0; i < dust_point; i++) {
		int pos_x = rand() % MAX_DUST_REGION - MAX_DUST_REGION / 2;
		int pos_z = rand() % MAX_DUST_REGION - MAX_DUST_REGION / 2;

		point_x[i] = pos_x;
		point_z[i] = pos_z;
	}

	// line dust
	for (int i = 0; i < dust_line; i++) {
		int pos_x1 = rand() % MAX_DUST_REGION - MAX_DUST_REGION / 2;
		int pos_x2 = rand() % MAX_DUST_REGION - MAX_DUST_REGION / 2;
		int pos_z1 = rand() % MAX_DUST_REGION - MAX_DUST_REGION / 2;
		int pos_z2 = rand() % MAX_DUST_REGION - MAX_DUST_REGION / 2;

		line_x1[i] = pos_x1;
		line_x2[i] = pos_x2;
		line_z1[i] = pos_z1;
		line_z2[i] = pos_z2;
	}

	// trianle dust
	/*for (int i = 0; i < dust_triangle; i++) {
		int pos_x1 = rand() % MAX_DUST_REGION - MAX_DUST_REGION / 2;
		int pos_x2 = rand() % MAX_DUST_REGION - MAX_DUST_REGION / 2;
		int pos_x3 = rand() % MAX_DUST_REGION - MAX_DUST_REGION / 2;
		int pos_z1 = rand() % MAX_DUST_REGION - MAX_DUST_REGION / 2;
		int pos_z2 = rand() % MAX_DUST_REGION - MAX_DUST_REGION / 2;
		int pos_z3 = rand() % MAX_DUST_REGION - MAX_DUST_REGION / 2;

		triangle_x1[i] = pos_x1;
		triangle_x2[i] = pos_x2;
		triangle_x3[i] = pos_x3;
		triangle_z1[i] = pos_z1;
		triangle_z2[i] = pos_z2;
		triangle_z3[i] = pos_z3;
	}*/
}

void draw_dust(void) {
	if (vacuum_switch) {
		// remove point dust
		for (int i = 0; i < DUST_NUM; i++) {
			if (point_x[i] == 0 && point_z[i] == 0)
				continue;

			cal_cleaning_vacuum_pos();
			if (sqrt(pow(point_x[i] - cleaning_vacuum_pos_x, 2) + pow(point_z[i] - cleaning_vacuum_pos_z, 2)) < cleaning_range)
				point_x[i] = point_z[i] = 0;
		}

		// remove line dust
		for (int i = 0; i < DUST_NUM; i++) {
			if ((line_x1[i] == 0 && line_z1[i] == 0) && (line_x2[i] == 0 && line_z2[i] == 0))
				continue;

			cal_cleaning_vacuum_pos();
			if (sqrt(pow((line_x1[i] + line_x2[i]) / 2.0 - cleaning_vacuum_pos_x, 2) + pow((line_z1[i] + line_z2[i]) / 2.0 - cleaning_vacuum_pos_z, 2)) < cleaning_range)
				line_x1[i] = line_x2[i] = line_z1[i] = line_z2[i] = 0;
		}

		// remove triangle dust
		/*for (int i = 0; i < DUST_NUM; i++) {
			if ((triangle_x1[i] == 0 && triangle_z1[i] == 0) && (triangle_x2[i] == 0 && triangle_z2[i] == 0) && (triangle_x2[i] == 0 && triangle_z2[i] == 0))
				continue;

			cal_cleaning_vacuum_pos();
			if (sqrt(pow((triangle_x1[i] + triangle_x2[i] + triangle_x3[i]) / 3.0 - cleaning_vacuum_pos_x, 2) + pow((triangle_z1[i] + triangle_z2[i] + triangle_z3[i]) / 2.0 - cleaning_vacuum_pos_z, 2)) < cleaning_range)
				triangle_x1[i] = triangle_x2[i] = triangle_x3[i] = triangle_z1[i] = triangle_z2[i] = triangle_z3[i] = 0;
		}*/
	}

	// point dust
	for (int i = 0; i < DUST_NUM; i++) {
		if (point_x[i] == 0 && point_z[i] == 0)
			continue;

		glPointSize(4.0);
		glBegin(GL_POINTS);
		glVertex3i(point_x[i], 0, point_z[i]);
		glEnd();
		glPointSize(1.0);
	}

	// line dust
	for (int i = 0; i < DUST_NUM; i++) {
		if ((line_x1[i] == 0 && line_z1[i] == 0) && (line_x2[i] == 0 && line_z2[i] == 0))
			continue;

		glLineWidth(2.0);
		glBegin(GL_LINES);
		glVertex3i(line_x1[i], 0, line_z1[i]);
		glVertex3i(line_x2[i], 0, line_z2[i]);
		glEnd();
		glLineWidth(1.0);
	}

	// triangle dust
	/*for (int i = 0; i < DUST_NUM; i++) {
		if ((triangle_x1[i] == 0 && triangle_z1[i] == 0) && (triangle_x2[i] == 0 && triangle_z2[i] == 0) && (triangle_x2[i] == 0 && triangle_z2[i] == 0))
			continue;

		glBegin(GL_TRIANGLES);
		glVertex3i(triangle_x1[i], 0, triangle_z1[i]);
		glVertex3i(triangle_x2[i], 0, triangle_z2[i]);
		glVertex3i(triangle_x3[i], 0, triangle_z3[i]);
		glEnd();
	}*/

	glutPostRedisplay();
}

void cal_cleaning_vacuum_pos(void) {
	cleaning_vacuum_pos_x = sin(vacuum_rotate_angle * M_PI / 180) * trans_z[0];
	cleaning_vacuum_pos_z = cos(vacuum_rotate_angle * M_PI / 180) * trans_z[0];
}