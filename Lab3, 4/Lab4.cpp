#include <gl/glut.h>
#include <stdio.h>
#include <string.h>

bool antialias_on = false;		// Antialiasing on/off ���� ����
bool alphablending_on = false;		// Alpha-Blending on/off ���� ����
int mode = 0;		// mode ���� ����, 0: None, 1: Point, 2: Line, 3: Triangle
int point_x[100], point_y[100];		// ���� x��ǥ & y��ǥ, �ʱⰪ (0, 0)
int line_x[200], line_y[200];		// ���� x��ǥ & y��ǥ, �ʱⰪ (0, 0)
int triangle_x[300], triangle_y[300];		// ��(�ﰢ��)�� x��ǥ & y��ǥ, �ʱⰪ (0, 0)
int point_index = 0;		// ���� x��ǥ & y��ǥ index, �ʱⰪ 0
int line_index = 0;		// ���� x��ǥ & y��ǥ index, �ʱⰪ 0
int triangle_index = 0;		// ��(�ﰢ��)�� x��ǥ & y��ǥ index, �ʱⰪ 0
bool isChanged = false;		// Antialiasing �Ǵ� Alpha-Blending ������ ����� ���

// ����� �ʱ�ȭ �Լ�
void init(void) {
	// ȭ���� �⺻������ Black����
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// ȭ�� ��ǥ ���� ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 500, 500, 0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);		// Alpha-Blending ��� ����
	// Antialiasing ǰ�� ����
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

// �� �׸��� �Լ�
void draw_point(void)
{
	// ���� ������ ���� ������
	glColor3f(1.0f, 0.0f, 0.0f);
	// ���� ũ�� (�ʱⰪ�� 1.0)
	glPointSize(4.0f);
	glBegin(GL_POINTS);

	glVertex2i(point_x[point_index - 1], point_y[point_index - 1]);

	glEnd();
}

// �ٽ� �� �׸��� �Լ�
void redraw_point(void)
{
	if (!alphablending_on)
		// ���� ������ ���� ������
		glColor3f(1.0f, 0.0f, 0.0f);
	else
		glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
	// ���� ũ�� (�ʱⰪ�� 1.0)
	glPointSize(4.0f);
	glBegin(GL_POINTS);

	for (unsigned i = 0; i < point_index; i++)
		glVertex2i(point_x[i], point_y[i]);

	glEnd();
}


// �� �׸��� �Լ�
void draw_line(void)
{
	// ���� ������ ���������
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);

	glVertex2i(line_x[line_index - 2], line_y[line_index - 2]);
	glVertex2i(line_x[line_index - 1], line_y[line_index - 1]);

	glEnd();
}

// �ٽ� �� �׸��� �Լ�
void redraw_line(void)
{
	if (!alphablending_on)
		// ���� ������ ���������
		glColor3f(1.0f, 1.0f, 0.0f);
	else
		glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
	glBegin(GL_LINES);

	for (unsigned i = 0; i < line_index; i += 2) {
		glVertex2i(line_x[i], line_y[i]);
		glVertex2i(line_x[i + 1], line_y[i + 1]);
	}

	glEnd();
}

// ��(Triangle) �׸��� �Լ�
void draw_triangle(void)
{
	// ���� ������ �ʷϻ�����
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);

	glVertex2i(triangle_x[triangle_index - 3], triangle_y[triangle_index - 3]);
	glVertex2i(triangle_x[triangle_index - 2], triangle_y[triangle_index - 2]);
	glVertex2i(triangle_x[triangle_index - 1], triangle_y[triangle_index - 1]);

	glEnd();
}

// �ٽ� ��(Triangle) �׸��� �Լ�
void redraw_triangle(void)
{
	if (!alphablending_on)
		// ���� ������ �ʷϻ�����
		glColor3f(0.0f, 1.0f, 0.0f);
	else
		glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	glBegin(GL_TRIANGLES);

	for (unsigned i = 0; i < triangle_index; i += 3) {
		glVertex2i(triangle_x[i], triangle_y[i]);
		glVertex2i(triangle_x[i + 1], triangle_y[i + 1]);
		glVertex2i(triangle_x[i + 2], triangle_y[i + 2]);
	}

	glEnd();
}

// ���ڿ� ��� �Լ�
void draw_string(void* font, const char* str, int x, int y) {
	glRasterPos2i(x, y);		// ���ڿ��� ���� ��� ��ġ ����
	for (unsigned i = 0; i < strlen(str); i++)
		glutBitmapCharacter(font, str[i]);
}

// Display callback �Լ�
void draw(void)
{
	glColor3f(1.0f, 1.0f, 1.0f);		// ���
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "12181763 HyungSuk Kim", 20, 20);

	if (!isChanged) {
		if (mode == 1)
			draw_point();
		else if (mode == 2 && (line_index % 2 == 0))
			draw_line();
		else if (mode == 3 && (triangle_index % 3 == 0))
			draw_triangle();
	}
	else if (!((point_index == 0) && (line_index == 0) && (triangle_index == 0))) {
		redraw_point();
		redraw_line();
		redraw_triangle();
		isChanged = false;
	}

	glFlush();
}

// Antialiasing ó�� �Լ�
void antiAliasing(void) {
	if (!antialias_on) {
		glEnable(GL_BLEND);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		antialias_on = true;
	}
	else {
		glDisable(GL_BLEND);
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
		antialias_on = false;
	}
}

// Ű���� �Է�ó�� �Լ�
void keyboard_ANTI_BLEND(unsigned char key, int x, int y)
{
	if (key == 'a' || key == 'A')
		if (!antialias_on) {
			printf("Anti-aliasing on\n");
			antiAliasing();
			isChanged = true;
		}
		else {
			printf("Anti-aliasing off\n");
			antiAliasing();
			isChanged = true;
		}
	else if (key == 'b' || key == 'B')
		if (!alphablending_on) {
			printf("Alpha-Blending on\n");
			glEnable(GL_BLEND);
			alphablending_on = true;
			isChanged = true;
		}
		else {
			printf("Alpha-Blending off\n");
			if (!antialias_on)
				glDisable(GL_BLEND);
			alphablending_on = false;
			isChanged = true;
		}

	draw();
	glFlush();
}

// ���콺 �Է�ó�� �Լ�
void mouse_draw(int button, int state, int x, int y)
{
	if (mode == 0 || button != 0 || state == 1) return;
	switch (mode) {
	case 1:
		point_x[point_index] = x;
		point_y[point_index] = y;
		point_index++;
		break;
	case 2:
		line_x[line_index] = x;
		line_y[line_index] = y;
		line_index++;
		break;
	case 3:
		triangle_x[triangle_index] = x;
		triangle_y[triangle_index] = y;
		triangle_index++;
		break;
	}

	printf("button(%d), state(%d), x(%d), y(%d)\n", button, state, x, y);
}

// MENU ó�� �Լ� - �޴� ���ý� �����
void main_menu_function(int option) {
	if (option == 1) {
		printf("You selected CLEAR\n");
		point_index = line_index = triangle_index = 0;
		glClear(GL_COLOR_BUFFER_BIT);
		draw();
	}
	else if (option == 999) {
		printf("You selected QUIT\n");
		exit(0);
	}
}


void sub_menu_function(int option) {
	// �� �׸���
	if (option == 1)
		mode = 1;
	// �� �׸���
	else if (option == 2)
		mode = 2;
	// �ﰢ�� �׸���
	else if (option == 3)
		mode = 3;

	printf("Submenu %d has been selected\n", option);
}

int main(int argc, char** argv) {
	int mode_shape;
	// Window �ʱ�ȭ
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("Lab4");
	init();

	mode_shape = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Triangle", 3);

	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Clear", 1);
	glutAddMenuEntry("Quit", 999);
	glutAddSubMenu("mode", mode_shape);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMouseFunc(mouse_draw);

	// Ű���� Callback �Լ�, Anti-Aliasing�� Alpha-Blending ON/OFF
	glutKeyboardFunc(keyboard_ANTI_BLEND);

	// ���÷��� Callback �Լ�
	glutDisplayFunc(draw);		// draw() -> ���� �׸��� �Լ�

	// Looping ����
	glutMainLoop();

	return 0;
}