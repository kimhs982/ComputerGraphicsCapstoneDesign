#include <gl/glut.h>
#include <stdio.h>
#include <Windows.h>

double m_RotateAngle;

// 초기화 함수
void init(void) {
	// 화면의 기본색으로 Black설정
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// set blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// set antialiasing
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_FASTEST);
	glEnable(GL_LINE_SMOOTH);

	// Buffer clearing시 이용할 값을 등록(초기화 함수)
	glClearDepth(1.0f);

	// Light 설정 -> 일단 그냥 이용
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat specularMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMateriali(GL_FRONT, GL_SHININESS, 20);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glFrontFace(GL_CW);		// Teapot은 CW로 정의됨

	// Depth-Test Enable
	glEnable(GL_DEPTH_TEST);		// glEnable(GL_DEPTH_TEST)가 있을 때와 없을 때의 결과를 비교

	// Polygon mode Enable
	glPolygonMode(GL_BACK, GL_LINE);

	// back-face culling EnableS
	glEnable(GL_CULL_FACE);
}

void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);

	glMatrixMode(GL_MODELVIEW);
}

void draw(void) {
	// 화면을 깨끗하게 지우기
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);		// Viewing Transform

	glPushMatrix();
		glRotatef(m_RotateAngle, 0, 1, 0);
		GLdouble eq[4] = { 1.0, 0.0, 0.0, 0.0 };
		glClipPlane(GL_CLIP_PLANE0, eq);
		glEnable(GL_CLIP_PLANE0);
	glPopMatrix();

	glColor3f(1, 0, 0);		// Red
	glutSolidTeapot(4);

	glFlush();
	glutSwapBuffers();
}

// 유휴시간 처리 함수
void idle(void) {
	m_RotateAngle += 0.1;

	// 화면을 다시 그리도록 요청
	glutPostRedisplay();
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("Lab8(12181763 김형석)");
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