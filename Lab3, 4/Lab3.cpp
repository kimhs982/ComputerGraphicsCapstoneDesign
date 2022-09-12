#include <gl/glut.h>
#include <stdio.h>
#include <windows.h>

// ����� �ʱ�ȭ �Լ�
void init(void)
{
	// ȭ���� �⺻������ Blue ���� 
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

	// ȭ�� ��ǥ ���� ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}

// �� �׸��� �Լ�
void draw_point(void)
{
	// ���� ������ ���� ������
	glColor3f(1.0f, 0.0f, 0.0f);
	// ���� ũ�� (�ʱⰪ�� 1.0)
	glPointSize(4.0f);
	glBegin(GL_POINTS);

	glVertex2i(5, 5);
	glVertex2i(200, 200);
	glVertex2i(450, 300);

	glEnd();
}

// �� �׸��� �Լ�, mode(1: LIne, 2: Stripple Line, 3: Loop Line)
void draw_line(int mode = 1)
{ 
	// ���� ������ ���������
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);

	glVertex2i(50, 50);
	glVertex2i(250, 450);
	glVertex2i(300, 450);
	glVertex2i(400, 100);

	glEnd();
}

// ��(Triangle) �׸��� �Լ�
void draw_triangle(void)
{
	// ���� ������ ���������
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	
	glVertex2i(50, 50);
	glVertex2i(250, 450);
	glVertex2i(400, 100);

	glEnd();
}

// �ﰢ�� ���� �׸��� �Լ�
void draw_triangle_strip(void)
{
	// ���� ������ �ʷϻ�����
	glColor3f(0.3f, 0.5f, 0.0f);
	glBegin(GL_TRIANGLE_STRIP);

	glVertex2i(50, 50);
	glVertex2i(50, 480);
	glVertex2i(200, 40);
	glVertex2i(90, 470);
	glVertex2i(300, 30);
	glVertex2i(250, 450);

	glEnd();
}

// �� �ﰢ�� �׸��� �Լ�
void draw_triangle_fan(void)
{
	// �ȿ� ������ �ʷϻ�����
	glColor3f(0.3f, 0.5f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLE_FAN);

	glVertex2i(200, 200);
	glVertex2i(200, 450);
	glVertex2i(300, 400);
	glVertex2i(400, 350);
	glVertex2i(420, 180);

	glEnd();
}

// ���� �׸��� �Լ�
void draw_quads(void)
{
	// ���� ������ ȸ������
	glColor3f(0.2f, 0.3f, 0.3f);
	glBegin(GL_QUADS);

	glVertex2i(80, 50);
	glVertex2i(200, 40);
	glVertex2i(180, 200);
	glVertex2i(100, 150);

	glVertex2i(250, 30);
	glVertex2i(370, 40);
	glVertex2i(430, 400);
	glVertex2i(330, 300);

	glEnd();
}

// ���� ���� �׸��� �Լ�
void draw_quads_strip(void)
{
	// ���� ������ ȸ������
	glColor3f(0.2f, 0.3f, 0.3f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);		// �ո鸸 �����ֱ�
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUAD_STRIP);

	glVertex2i(50, 80);
	glVertex2i(40, 300);
	glVertex2i(80, 70);
	glVertex2i(80, 280);
	glVertex2i(150, 80);
	glVertex2i(130, 300);
	glVertex2i(200, 100);
	glVertex2i(230, 350);

	glEnd();
}

// ������ �׸��� �Լ�
void draw_polygon(void)
{
	// ���� ������ ȸ������
	glColor3f(0.2f, 0.3f, 0.3f);
	glFrontFace(GL_CW);		// �޼����� �������� ���� �� ���� �հ��� ������ �ո�
	glPolygonMode(GL_BACK, GL_LINE);		// �޸��� ������ ǥ��
	glBegin(GL_POLYGON);		// ������ �׸���

	// �ð�������� Vertex ����
	glVertex2i(150, 300);
	glVertex2i(200, 200);
	glVertex2i(330, 250);
	glVertex2i(400, 350);
	glVertex2i(250, 400);

	glEnd();
}

// MENU ó�� �Լ� - �޴� ���ý� �����
void main_menu_function(int option)
{
	if (option == 1)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		draw_point();
		glFlush();
	}
	else if (option == 999)
	{
		printf("You selected QUIT\n");
		exit(0);
	}
	printf("Main menu %d has been selected\n", option);
}

void sub_menu_function1(int option)
{
	glClear(GL_COLOR_BUFFER_BIT);
	// �� �׸���
	if (option == 1)
	{
		glDisable(GL_LINE_STIPPLE);
		draw_line();
		glFlush();
	}
	// ���� �׸���
	else if (option == 2)
	{
		glLineWidth(2.0f);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(3, 0xAAAA);
		draw_line();
	}
	// ���� �� �׸���
	else if (option == 3)
	{
		// ���� ���� ���������
		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_LINE_LOOP);

		glVertex2i(5, 5);
		glVertex2i(250, 450);
		glVertex2i(300, 250);
		glVertex2i(400, 100);

		glEnd();
	}
	glFlush();

	printf("Submenu %d has been selected\n", option);
}

void sub_menu_function2(int option)
{
	glClear(GL_COLOR_BUFFER_BIT);
	// �ﰢ�� �׸���
	if (option == 1)
		draw_triangle();
	// �ﰢ�� ���� �׸���
	else if (option == 2)
		draw_triangle_strip();
	// �ﰢ�� �� �׸���
	else if (option == 3)
		draw_triangle_fan();
	// ���� �׸���
	else if (option == 4)
		draw_quads();
	// ���� ���� �׸���
	else if (option == 5)
		draw_quads_strip();
	// ������ �׸���
	else if (option == 6)
		draw_polygon();
	glFlush();

	printf("Submenu %d has been selected\n", option);
}

// Display callback �Լ�
void draw(void)
{
	// ȭ���� �����ϰ� �����
	glClear(GL_COLOR_BUFFER_BIT);

	/*draw_point();
	draw_line();
	draw_triangle();*/

	glFlush();
}

int main(int argc, char** argv)
{
	int line, polygon;

	// Window �ʱ�ȭ
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My First GL Program(12181763_������)");
	init();		// ����� �ʱ�ȭ �Լ�

	// Popup menu ���� �� �߰�
	line = glutCreateMenu(sub_menu_function1);
	glutAddMenuEntry("Line", 1);
	glutAddMenuEntry("Stipple Line", 2);
	glutAddMenuEntry("Loop Line", 3);		// 3���� �׸��� �ִ� �޴� ����

	polygon = glutCreateMenu(sub_menu_function2);
	glutAddMenuEntry("Triangle", 1);
	glutAddMenuEntry("Triangle Strip", 2);
	glutAddMenuEntry("Triangle Fan", 3);
	glutAddMenuEntry("Quads", 4);
	glutAddMenuEntry("Quad Strip", 5);
	glutAddMenuEntry("Polygon", 6);

	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Point", 1);
	glutAddSubMenu("Line", line);
	glutAddSubMenu("Polygon", polygon);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// ���÷��� Callback �Լ�
	glutDisplayFunc(draw);		// draw() -> ���� �׸��� �Լ�

	// Looping ����
	glutMainLoop();

	return 0;
}