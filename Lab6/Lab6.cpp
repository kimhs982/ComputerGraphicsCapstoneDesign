#define _USE_MATH_DEFINES

#include <gl/glut.h>
#include <stdio.h>
#include <math.h>

double radius = 20;
double theta = 45, phi = 45;
double cam[3] = { 10, 10 * sqrt(2), 10 };		// ī�޶�(��)�� ��ġ
double center[3] = { 0,0,0 };		// �ٶ󺸴� ���� ��ġ
double up[3] = { 0,1,0 };		// ī�޶��� up ����

// ����� �ʱ�ȭ �Լ�
void init(void) {
	// ȭ���� �⺻������ Black����
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// set blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// set antialiasing
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_FASTEST);
	glEnable(GL_LINE_SMOOTH);
}

void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);

	glMatrixMode(GL_MODELVIEW);
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
	// ȭ���� �����ϰ� �����
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cam[0], cam[1], cam[2], center[0], center[1], center[2], up[0], up[1], up[2]);		// Viewing Transform

	glColor3f(1, 1, 0);		// Yellow
	glutWireTeapot(4);
	draw_axis();

	glFlush();
	glutSwapBuffers();
}

void cal_coordinates(double change_theta, double change_phi) {
	theta += change_theta, phi += change_phi;
	if (theta <= 0) {
		theta = 360 + theta;
		up[1] = -up[1];
	}
	else if (theta == (double)180)
		up[1] = -up[1];
	else if (theta > 360) {
		theta = theta - 360;
		up[1] = -up[1];
	}

	if (phi < 0) phi = 360 + phi;
	else if (phi > 360) phi -= 360;

	cam[0] = radius * sin(theta * (M_PI / 180)) * sin(phi * (M_PI / 180));
	cam[1] = radius * cos(theta * (M_PI / 180));
	cam[2] = radius * sin(theta * (M_PI / 180)) * cos(phi * (M_PI / 180));
}

void special_keyboard(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) {
		cal_coordinates(0, -5);
		printf("theta : %.0lf, phi : %.0lf\n", theta, phi);
		draw();
	}
	else if (key == GLUT_KEY_RIGHT) {
		cal_coordinates(0, 5);
		printf("theta : %.0lf, phi : %.0lf\n", theta, phi);
		draw();
	}
	else if (key == GLUT_KEY_UP) {
		cal_coordinates(-5, 0);
		printf("theta : %.0lf, phi : %.0lf\n", theta, phi);
		draw();
	}
	else if (key == GLUT_KEY_DOWN) {
		cal_coordinates(5, 0);
		printf("theta : %.0lf, phi : %.0lf\n", theta, phi);
		draw();
	}
}

void main_menu_function(int option) {
	if (option == 1) {
		printf("You selected Init menu\n");
		theta = 45, phi = 45, up[1] = 1;
		cal_coordinates(0, 0);
		draw();
	}
	else if (option == 999) {
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
	glutCreateWindow("Lab6(12181763 ������)");
	init();
	
	// main menu ����
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Init", 1);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	// Display callback �Լ�
	glutDisplayFunc(draw);

	// Special keyboard callback �Լ�
	glutSpecialFunc(special_keyboard);
	
	// resize callback �Լ�
	glutReshapeFunc(resize);

	// Looping�� ����
	glutMainLoop();

	return 0;
}