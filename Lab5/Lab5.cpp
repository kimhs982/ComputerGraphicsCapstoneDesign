#include <gl/glut.h>
#include <stdio.h>
#include <windows.h>
//#include <GL/freeglut.h>

bool spin_state = true;		// ȸ������ ����
bool spin_ccw = true;		// ȸ�� ����
double spin = 0;		// ȸ������ ����
int draw_mode = 0;		// ���� ����
int x_trans = 0, y_trans = 0;		// x, y �������� ���� �̵���

// ����� �ʱ�ȭ �Լ�
void init(void) {
	// ȭ���� �⺻������ Black����
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// ȭ�� ��ǥ ���� ����
	glMatrixMode(GL_PROJECTION);		// 3������ 2�������� ����
	glLoadIdentity();
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}

// Idle ó�� �Լ�
void idle(void) {
	if (spin_state && spin_ccw) {
		spin += 0.1;		// ȸ������ 0.1 ����
		if (spin > 360)
			spin -= 360;
	}
	else if (spin_state) {
		spin -= 0.1;
		if (spin < 0)
			spin += 360;
	}
	glutPostRedisplay();
}

// �ﰢ�� �׸��� �Լ�
void draw_triangle(void) {
	// ���� ������ ���������
	glColor4f(1.0f, 1.0f, 0.0f, 0.75f);
	glBegin(GL_TRIANGLES);

	glVertex2i(100, 300);
	glVertex2i(100, 100);
	glVertex2i(300, 200);

	glEnd();
}

// ������ �׸��� �Լ�
void draw_polygon(void) {
	// ���� ������ ���������
	glColor4f(1.0f, 1.0f, 0.0f, 0.75f);
	glBegin(GL_POLYGON);

	glVertex2i(100, 300);
	glVertex2i(100, 100);
	glVertex2i(300, 100);
	glVertex2i(300, 200);

	glEnd();
}

// Display callback �Լ�
void draw(void) {
	// ȭ���� �����ϰ� �����
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(250 + x_trans, 250 + y_trans, 0);		// �����̵�
	glRotatef(spin, 0, 0, 1);		// ȸ��
	glTranslatef(-250 - x_trans, -250 - y_trans, 0);		// �����̵�

	switch (draw_mode) {
	case 1:
		draw_triangle();		// �ﰢ�� �׸���
		break;
	case 2:
		draw_polygon();
		break;
	default:
		break;
	}

	glFlush();
	glutSwapBuffers();
}

// Menu ó�� �Լ� - �޴� ���ý� �����
void sub_menu_function(int option) {
	// Triangle ����޴� ���� ��
	if (option == 1) {
		printf("Submenu Triangle has been selected\n");
		draw_mode = 1;
	}
	// Polygon ����޴� ���� ��
	else if (option == 2) {
		printf("Submenu Polygon has been selected\n");
		draw_mode = 2;
	}
}

void main_menu_function(int option) {
	// Spin �޴� ���� ��
	if (option == 1) {
		printf("You selected Spin menu\n");
		if (spin_state)
			spin_state = false;
		else
			spin_state = true;
	}
	// Quit �޴� ���� ��
	else if (option == 999) {
		printf("You selected Quit menu\n");
		exit(0);
	}
	glutPostRedisplay();		// ������ �ٽ� �׸���
}

// Ű���� �Է� �ݹ� �Լ�
void keyboard(unsigned char key, int x, int y) {
	printf("You pressed %c\n", key);
	if (key == 's') {
		if (spin_state)
			spin_state = false;
		else
			spin_state = true;
	}
	else if (key == 'r')
		if (spin_ccw)
			spin_ccw = false;
		else
			spin_ccw = true;
	glutPostRedisplay();
}

// ����� �Է� �ݹ� �Լ�
void keyboard_special(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT)
		x_trans -= 5;
	else if (key == GLUT_KEY_RIGHT)
		x_trans += 5;
	else if (key == GLUT_KEY_UP)
		y_trans += 5;
	else if (key == GLUT_KEY_DOWN)
		y_trans -= 5;
}

int main(int argc, char** argv) {
	int submenu;

	// ������ �ʱ�ȭ
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("Lab5(12181763 ������)");
	init();		// ����� �ʱ�ȭ �Լ�

	// ���� �޴� ���� �� �߰�
	submenu = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("Triangle", 1);
	glutAddMenuEntry("Polygon", 2);

	// ���� �޴� ����
	glutCreateMenu(main_menu_function);
	glutAddSubMenu("Shape", submenu);
	glutAddMenuEntry("Spin", 1);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Idle �ݹ� �Լ�
	glutIdleFunc(idle);		// ��ǻ���� ���޽ð��� ȣ��

	// ���÷��� �ݹ� �Լ�
	glutDisplayFunc(draw);		// draw() : ���� �׸��� �Լ�

	// Ű���� �ݹ� �Լ�
	glutKeyboardFunc(keyboard);

	// ����� �ݹ� �Լ�
	glutSpecialFunc(keyboard_special);

	// Looping�� ����
	glutMainLoop();

	return 0;
}