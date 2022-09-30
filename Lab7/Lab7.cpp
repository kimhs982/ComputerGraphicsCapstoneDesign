#include <gl/glut.h>
#include <stdio.h>
#include <Windows.h>

#define PLANET_NUMBER 6		// 행성(위성)의 수

float sun_rotation_angle;		// 태양 자전 각도
float rotation_angle[PLANET_NUMBER];		// 행성(위성) 자전 각도
float rot_angle_change[PLANET_NUMBER] = { 0.002, 0.005, 0.45, 0.13, 0.001, 0.05 };		// 행성 자전 각도 변화
float revolution_angle[PLANET_NUMBER];		// 행성(위성) 공전 각도
float rev_angle_change[PLANET_NUMBER] = { 0.4, 0.15, 0.45, 0.13, 0.3, 0.05 };		// 행성 공전 각도 변화


// 초기화 함수
void init(void) {
	// 화면의 기본색으로 검정색 설정
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// set blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// set antialiasing
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_FASTEST);
	glEnable(GL_LINE_SMOOTH);

	printf("init 함수 호출\n");
}

// 윈도우 생성 및 크기 변화 함수
void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);		// projection transform
	glMatrixMode(GL_MODELVIEW);

	printf("resize 함수 호출\n");
}

void idle(void) {
	// 태양의 자전
	sun_rotation_angle += 0.2;
	if (sun_rotation_angle > 360)
		sun_rotation_angle -= 360;

	// 행성의 자전
	for (int i = 0; i < PLANET_NUMBER; i++) {
		rotation_angle[i] += rot_angle_change[i];
		if (rotation_angle[i] > 360) rotation_angle[i] -= 360;
	}

	// 행성의 공전
	for (int i = 0; i < PLANET_NUMBER; i++) {
		revolution_angle[i] += rev_angle_change[i];
		if (revolution_angle[i] > 360) revolution_angle[i] -= 360;
	}

	// 화면을 다시 그리도록 요청
	glutPostRedisplay();
}

void draw_axis(void) {
	glLineWidth(3);		// 좌표축의 두께
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);		// X축은 red
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0);		// Y축은 green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);		// z축은 blue
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4);
	glEnd();
	glLineWidth(1);		// 좌표축의 두께 다시 환원
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(15, 15, 15, 0, 0, 0, 0, 1, 0);

	draw_axis();
	// 태양(sun)
	glRotatef(sun_rotation_angle, 0, 1, 0);		// 자전

	glColor3f(1, 0, 0);		// red
	glutWireSphere(3, 60, 60);

	glPushMatrix();		// 현재의 행렬을 스택에 저장
	glPushMatrix();		// 현재의 행렬을 스택에 저장

	// 금성(venus)
	glRotatef(revolution_angle[0], 0, 1, 0);		// 공전
	glTranslatef(3, 0, 0);		// 평행이동
	glRotatef(rotation_angle[0], 0, 1, 0);		// 자전

	glColor3f(0.8, 0.8, 0.8);		// light gray
	glutWireSphere(0.9, 30, 30);

	// 화성(mars)
	glPopMatrix();		// 태양만 그렸을 때의 상태로 행렬 복귀

	glRotatef(revolution_angle[1], 0, 1, 0);		// 공전
	glTranslatef(8, 0, 0);		// 평행이동
	glRotatef(rotation_angle[1], 0, 1, 0);		// 자전

	glColor3f(0, 1, 1);		// cyan
	glutWireSphere(0.5, 25, 25);

	glPushMatrix();		// 현재의 행렬을 스택에 저장

	// 포보스(phobos)
	glRotatef(revolution_angle[2], 0, 1, 0);		// 공전
	glTranslatef(1, 0, 0);		// 평행이동
	glRotatef(rotation_angle[2], 0, 1, 0);		// 공전

	glColor3f(0.7, 0.7, 0.7);		// light gray
	glutWireSphere(0.2, 20, 20);

	// 데이모스(deimos)
	glPopMatrix();		// 화성을 그렸을 때의 상태로 행렬 복귀

	glRotatef(revolution_angle[3], 0, 1, 0);		// 공전
	glTranslatef(2, 0, 0);		// 평행이동
	glRotatef(rotation_angle[3], 0, 1, 0);		// 공전

	glColor3f(0.6, 0.6, 0.6);		// light gray
	glutWireSphere(0.1, 15, 15);

	// 지구(earth)
	glPopMatrix();		// 태양만 그렸을 때의 상태로 행렬 복귀

	glRotatef(revolution_angle[4], 0, 1, 0);		// 공전
	glTranslatef(5, 0, 0);		// 평행이동
	glRotatef(rotation_angle[4], 0, 1, 0);		// 자전

	glColor3f(0, 0, 1);		// blue
	glutWireSphere(1, 40, 40);

	// 달(moon)
	glRotatef(revolution_angle[5], 0, 1, 0);		// 공전
	glTranslatef(2, 0, 0);		// 평행이동
	glRotatef(rotation_angle[5], 0, 1, 0);		// 자전

	glColor3f(0, 0.5, 1);		// light blue
	glutWireSphere(0.25, 20, 20);

	glFlush();
	glutSwapBuffers();
}

void main_menu_function(int option) {
	if (option == 999) {
		printf("You selected Quit menu");
		exit(0);
	}
}

int main(int argc, char** argv) {
	// Window 초기화
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("Lab7(12181763 김형석)");
	init();

	// main menu 생성
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Idle callback 함수
	glutIdleFunc(idle);

	// Display callback 함수
	glutDisplayFunc(draw);

	// resize callback 함수
	glutReshapeFunc(resize);

	// Looping의 시작
	glutMainLoop();

	return 0;
}