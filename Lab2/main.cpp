#include <gl/glut.h>
#include <stdio.h>
#include <windows.h>

// ����� �ʱ�ȭ �Լ�
void init(void)
{
	// ȭ���� �⺻������ dark blue ���� 
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
}

// ���콺 �Է�ó�� �Լ�
void mouse(int button, int state, int x, int y)
{
	// ���ڵ��� �ؼ��ؼ� ���ϴ� ����� ����
	printf("Mouse button is clicked! (%d, %d, %d, %d)\n", button, state, x, y);
}

// ���콺 ������ ó�� �Լ�
void motion(int x, int y)
{
	// ���ڵ��� �ؼ��ؼ� ���ϴ� ����� ����
	printf("Mouse is moving! (%d, %d)\n", x, y);
}

// Ű���� �Է�ó�� �Լ�
void keyboard(unsigned char key, int x, int y)
{
	// ���ڵ��� �ؼ��ؼ� ���ϴ� ����� ����
	printf("You pressed %c\n", key);
}

// Idle ó�� �Լ�
void idle(void)
{
	// ���� ��ü�� ������(animation) ������ �̿�� ����
	printf("%d second has elapsed since the system was started\n", GetTickCount() / 1000);
}

// MENU ó�� �Լ� - �޴� ���ý� �����
void sub_menu_function(int option)
{
	printf("Submenu %d has been selected\n", option);
}

void main_menu_function(int option)
{
	printf("Main menu %d has been selected\n", option);
	if (option == 999)
		exit(0);
}

// Display callback �Լ�
void draw(void)
{
	// Teapot 1���� �׸��� �ӽ� ��ƾ 
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);		// ���ڸ� �ٲٸ� ���� ��ȭ
	glutWireTeapot(0.5f);		// �����ڸ� �ϳ� �׸�
	glFlush();
}

int main(int argc, char** argv)
{
	int submenu1;

	// Window �ʱ�ȭ
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My First GL Program(12181763_������)");
	init();		// ����� �ʱ�ȭ �Լ�

	// Popup menu ���� �� �߰�
	submenu1 = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("Sub1", 1);
	glutAddMenuEntry("Sub2", 2);
	glutAddMenuEntry("Sub3", 3);		// 3���� �׸��� �ִ� �޴� ����

	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAddMenuEntry("Go!", 11);
	glutAddSubMenu("Sub Menu", submenu1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// ���콺 Callback �Լ�
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	// Ű���� Callback �Լ�
	glutKeyboardFunc(keyboard);

	/* // idle Callback �Լ�
	glutIdleFunc(idle); */

	// ���÷��� Callback �Լ�
	glutDisplayFunc(draw);		// draw() -> ���� �׸��� �Լ�

	// Looping ����
	glutMainLoop();

	return 0;
}