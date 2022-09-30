#include <gl/glut.h>
#include <stdio.h>
#include <Windows.h>

#define PLANET_NUMBER 6		// �༺(����)�� ��

float sun_rotation_angle;		// �¾� ���� ����
float rotation_angle[PLANET_NUMBER];		// �༺(����) ���� ����
float rot_angle_change[PLANET_NUMBER] = { 0.002, 0.005, 0.45, 0.13, 0.001, 0.05 };		// �༺ ���� ���� ��ȭ
float revolution_angle[PLANET_NUMBER];		// �༺(����) ���� ����
float rev_angle_change[PLANET_NUMBER] = { 0.4, 0.15, 0.45, 0.13, 0.3, 0.05 };		// �༺ ���� ���� ��ȭ


// �ʱ�ȭ �Լ�
void init(void) {
	// ȭ���� �⺻������ ������ ����
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// set blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// set antialiasing
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_FASTEST);
	glEnable(GL_LINE_SMOOTH);

	printf("init �Լ� ȣ��\n");
}

// ������ ���� �� ũ�� ��ȭ �Լ�
void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);		// projection transform
	glMatrixMode(GL_MODELVIEW);

	printf("resize �Լ� ȣ��\n");
}

void idle(void) {
	// �¾��� ����
	sun_rotation_angle += 0.2;
	if (sun_rotation_angle > 360)
		sun_rotation_angle -= 360;

	// �༺�� ����
	for (int i = 0; i < PLANET_NUMBER; i++) {
		rotation_angle[i] += rot_angle_change[i];
		if (rotation_angle[i] > 360) rotation_angle[i] -= 360;
	}

	// �༺�� ����
	for (int i = 0; i < PLANET_NUMBER; i++) {
		revolution_angle[i] += rev_angle_change[i];
		if (revolution_angle[i] > 360) revolution_angle[i] -= 360;
	}

	// ȭ���� �ٽ� �׸����� ��û
	glutPostRedisplay();
}

void draw_axis(void) {
	glLineWidth(3);		// ��ǥ���� �β�
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);		// X���� red
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0);		// Y���� green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);		// z���� blue
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4);
	glEnd();
	glLineWidth(1);		// ��ǥ���� �β� �ٽ� ȯ��
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(15, 15, 15, 0, 0, 0, 0, 1, 0);

	draw_axis();
	// �¾�(sun)
	glRotatef(sun_rotation_angle, 0, 1, 0);		// ����

	glColor3f(1, 0, 0);		// red
	glutWireSphere(3, 60, 60);

	glPushMatrix();		// ������ ����� ���ÿ� ����
	glPushMatrix();		// ������ ����� ���ÿ� ����

	// �ݼ�(venus)
	glRotatef(revolution_angle[0], 0, 1, 0);		// ����
	glTranslatef(3, 0, 0);		// �����̵�
	glRotatef(rotation_angle[0], 0, 1, 0);		// ����

	glColor3f(0.8, 0.8, 0.8);		// light gray
	glutWireSphere(0.9, 30, 30);

	// ȭ��(mars)
	glPopMatrix();		// �¾縸 �׷��� ���� ���·� ��� ����

	glRotatef(revolution_angle[1], 0, 1, 0);		// ����
	glTranslatef(8, 0, 0);		// �����̵�
	glRotatef(rotation_angle[1], 0, 1, 0);		// ����

	glColor3f(0, 1, 1);		// cyan
	glutWireSphere(0.5, 25, 25);

	glPushMatrix();		// ������ ����� ���ÿ� ����

	// ������(phobos)
	glRotatef(revolution_angle[2], 0, 1, 0);		// ����
	glTranslatef(1, 0, 0);		// �����̵�
	glRotatef(rotation_angle[2], 0, 1, 0);		// ����

	glColor3f(0.7, 0.7, 0.7);		// light gray
	glutWireSphere(0.2, 20, 20);

	// ���̸�(deimos)
	glPopMatrix();		// ȭ���� �׷��� ���� ���·� ��� ����

	glRotatef(revolution_angle[3], 0, 1, 0);		// ����
	glTranslatef(2, 0, 0);		// �����̵�
	glRotatef(rotation_angle[3], 0, 1, 0);		// ����

	glColor3f(0.6, 0.6, 0.6);		// light gray
	glutWireSphere(0.1, 15, 15);

	// ����(earth)
	glPopMatrix();		// �¾縸 �׷��� ���� ���·� ��� ����

	glRotatef(revolution_angle[4], 0, 1, 0);		// ����
	glTranslatef(5, 0, 0);		// �����̵�
	glRotatef(rotation_angle[4], 0, 1, 0);		// ����

	glColor3f(0, 0, 1);		// blue
	glutWireSphere(1, 40, 40);

	// ��(moon)
	glRotatef(revolution_angle[5], 0, 1, 0);		// ����
	glTranslatef(2, 0, 0);		// �����̵�
	glRotatef(rotation_angle[5], 0, 1, 0);		// ����

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
	// Window �ʱ�ȭ
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("Lab7(12181763 ������)");
	init();

	// main menu ����
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Idle callback �Լ�
	glutIdleFunc(idle);

	// Display callback �Լ�
	glutDisplayFunc(draw);

	// resize callback �Լ�
	glutReshapeFunc(resize);

	// Looping�� ����
	glutMainLoop();

	return 0;
}