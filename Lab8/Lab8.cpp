#include <gl/glut.h>
#include <stdio.h>
#include <Windows.h>

double m_RotateAngle;

// �ʱ�ȭ �Լ�
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

	// Buffer clearing�� �̿��� ���� ���(�ʱ�ȭ �Լ�)
	glClearDepth(1.0f);

	// Light ���� -> �ϴ� �׳� �̿�
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

	glFrontFace(GL_CW);		// Teapot�� CW�� ���ǵ�

	// Depth-Test Enable
	glEnable(GL_DEPTH_TEST);		// glEnable(GL_DEPTH_TEST)�� ���� ���� ���� ���� ����� ��

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
	// ȭ���� �����ϰ� �����
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

// ���޽ð� ó�� �Լ�
void idle(void) {
	m_RotateAngle += 0.1;

	// ȭ���� �ٽ� �׸����� ��û
	glutPostRedisplay();
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("Lab8(12181763 ������)");
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