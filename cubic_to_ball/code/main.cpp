#define GLUT_DISABLE_ATEXIT_HACK
#include <GL\glut.h>  //������ذ�
#include <stdlib.h>
#include <cmath>
#include <math.h>
using namespace std;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//���������ز���
GLfloat x = 0, y = 0, z = 0;	//����λ��
GLfloat half_length = 2;	//������߳���һ��

//��ת��ʾ����ز���
GLfloat spin = 0;	//��ת��
float delta = 0.1;	//ÿ�����ӵ���ת�Ƕ�

//�������������񻯵���ز���
const int N = 8;	//�������һ���߷ֳ�N���߶�
float dl = half_length * 2.0 / N;	//ÿ���߶εĳ���
const int vertex_num = (N + 1)*(N + 1);		//��Ӧ��һ��������ɵĵ�ĸ���
GLfloat vertexes[6][vertex_num][3];		//����������Ҫ�����ɵĵ㼯

//�α�������ز���
GLfloat ball_points[6][vertex_num][3];	//��������꼯
const int Times = 2000;	//�α��֡��
int number=0;	//��ǰ��ʾ�ڼ�֡
GLfloat dr[6][vertex_num];	//ÿ����ÿһ֡���ӵ�r
GLfloat r=(GLfloat)pow((pow(half_length, 2) + pow(half_length, 2) + pow(half_length, 2)), 0.5);	//���յõ�������İ뾶

bool flag = 0;	//�Ƿ����������ı�־�����������Ƿ���ʾ�������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���ɵ㼯
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void vertex_comp(void)
{
	//��0��͵�1��ĵ㼯
	for (int i = 0; i <= N; i++)
	{
		for (int j = 0; j <= N; j++)
		{
			vertexes[0][i*(N + 1) + j][0] = { x - half_length + j*dl };	//x��λ��
			vertexes[0][i*(N + 1) + j][1] = { y - half_length + i*dl };	//y��λ��
			vertexes[0][i*(N + 1) + j][2] = { z - half_length };	//z��λ��

			vertexes[1][i*(N + 1) + j][0] = { x - half_length + j*dl };	//x��λ��
			vertexes[1][i*(N + 1) + j][1] = { y - half_length + i*dl };	//y��λ��
			vertexes[1][i*(N + 1) + j][2] = { z + half_length };	//z��λ��
		}
	}

	//��2��͵�3��ĵ㼯
	for (int i = 0; i <= N; i++)
	{
		for (int j = 0; j <= N; j++)
		{
			vertexes[2][i*(N + 1) + j][0] = { x - half_length };	//x��λ��
			vertexes[2][i*(N + 1) + j][1] = { y - half_length + i*dl };	//y��λ��
			vertexes[2][i*(N + 1) + j][2] = { z - half_length + j*dl };	//z��λ��

			vertexes[3][i*(N + 1) + j][0] = { x + half_length };	//x��λ��
			vertexes[3][i*(N + 1) + j][1] = { y - half_length + i*dl };	//y��λ��
			vertexes[3][i*(N + 1) + j][2] = { z - half_length + j*dl };	//z��λ��
		}
	}

	//��4��͵�5��ĵ㼯
	for (int i = 0; i <= N; i++)
	{
		for (int j = 0; j <= N; j++)
		{
			vertexes[4][i*(N + 1) + j][0] = { x - half_length + j*dl };	//x��λ��
			vertexes[4][i*(N + 1) + j][1] = { y - half_length };	//y��λ��
			vertexes[4][i*(N + 1) + j][2] = { z - half_length + i*dl };	//z��λ��

			vertexes[5][i*(N + 1) + j][0] = { x - half_length + j*dl };	//x��λ��
			vertexes[5][i*(N + 1) + j][1] = { y + half_length };	//y��λ��
			vertexes[5][i*(N + 1) + j][2] = { z - half_length + i*dl };	//z��λ��
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�������ɵ㼯��������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ball_points_comp(void)
{
	for (int k = 0; k < 6; k++)
	{
		for (int t = 0; t < vertex_num; t++)
		{
			float tempX = vertexes[k][t][0];
			float tempY = vertexes[k][t][1];
			float tempZ = vertexes[k][t][2];
			ball_points[k][t][0] = (GLfloat)pow((pow(tempX, 2) + pow(tempY, 2) + pow(tempZ, 2)), 0.5);	//�������е�r
			ball_points[k][t][1] = (GLfloat)acos(tempZ / ball_points[k][t][0]);		//�������е�theta
			ball_points[k][t][2] = (GLfloat)atan2(tempY, tempX);	//�������е�phi

			dr[k][t] = (r - ball_points[k][t][0]) / Times;	////ÿ����ÿһ֡���ӵ�r
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void triangle_display(void)
{
	
	//�������ɵĵ�����ÿ�������������
	for (int k = 0; k < 6; k++)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				glBegin(GL_TRIANGLE_STRIP);
				switch (k)
				{
				case 0:glColor3d(1, 0.8, 0.8); break;
				case 1:glColor3d(1, 1, 0.8); break;
				case 2:glColor3d(0.8, 1, 0.8); break;
				case 3:glColor3d(0.8, 1, 1); break;
				case 4:glColor3d(0.1, 0.8, 1); break;
				default:glColor3d(1, 0.8, 1); break;
				}
				glVertex3f(vertexes[k][i*(N + 1) + j][0], vertexes[k][i*(N + 1) + j][1], vertexes[k][i*(N + 1) + j][2]);
				glVertex3f(vertexes[k][i*(N + 1) + j + 1][0], vertexes[k][i*(N + 1) + j + 1][1], vertexes[k][i*(N + 1) + j + 1][2]);
				glVertex3f(vertexes[k][i*(N + 1) + j + N + 1][0], vertexes[k][i*(N + 1) + j + N + 1][1], vertexes[k][i*(N + 1) + j + N + 1][2]);

				switch (k)
				{
				case 0:glColor3d(1, 0, 0); break;
				case 1:glColor3d(1, 1, 0); break;
				case 2:glColor3d(0, 1, 0); break;
				case 3:glColor3d(0, 1, 1); break;
				case 4:glColor3d(0, 0, 1); break;
				default:glColor3d(1, 0, 1); break;
				}
				glVertex3f(vertexes[k][i*(N + 1) + j + 1][0], vertexes[k][i*(N + 1) + j + 1][1], vertexes[k][i*(N + 1) + j + 1][2]);
				glVertex3f(vertexes[k][i*(N + 1) + j + N + 1][0], vertexes[k][i*(N + 1) + j + N + 1][1], vertexes[k][i*(N + 1) + j + N + 1][2]);
				glVertex3f(vertexes[k][i*(N + 1) + j + N + 2][0], vertexes[k][i*(N + 1) + j + N + 2][1], vertexes[k][i*(N + 1) + j + N + 2][2]);
				glEnd();
			}
		}
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�������������α���м�֡
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void middle_display(void)
{

	//�������ɵĵ�����ÿ�������������
	for (int k = 0; k < 6; k++)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{

				GLfloat R1 = ball_points[k][i*(N + 1) + j][0] + number*dr[k][i*(N + 1) + j];
				GLfloat Theta1 = ball_points[k][i*(N + 1) + j][1];
				GLfloat Phi1 = ball_points[k][i*(N + 1) + j][2];

				GLfloat R2 = ball_points[k][i*(N + 1) + j + 1][0] + number*dr[k][i*(N + 1) + j + 1];
				GLfloat Theta2 = ball_points[k][i*(N + 1) + j + 1][1];
				GLfloat Phi2 = ball_points[k][i*(N + 1) + j + 1][2];

				GLfloat R3 = ball_points[k][i*(N + 1) + j + N + 1][0] + number*dr[k][i*(N + 1) + j + N + 1];
				GLfloat Theta3 = ball_points[k][i*(N + 1) + j + N + 1][1];
				GLfloat Phi3 = ball_points[k][i*(N + 1) + j + N + 1][2];

				GLfloat R4 = ball_points[k][i*(N + 1) + j + N + 2][0] + number*dr[k][i*(N + 1) + j + N + 2];
				GLfloat Theta4 = ball_points[k][i*(N + 1) + j + N + 2][1];
				GLfloat Phi4 = ball_points[k][i*(N + 1) + j + N + 2][2];

				glBegin(GL_TRIANGLE_STRIP);
				switch (k)
				{
				case 0:glColor3d(1, 0.8, 0.8); break;
				case 1:glColor3d(1, 1, 0.8); break;
				case 2:glColor3d(0.8, 1, 0.8); break;
				case 3:glColor3d(0.8, 1, 1); break;
				case 4:glColor3d(0.1, 0.8, 1); break;
				default:glColor3d(1, 0.8, 1); break;
				}
				glVertex3f(R1*sin(Theta1)*cos(Phi1), R1*sin(Theta1)*sin(Phi1), R1*cos(Theta1));
				glVertex3f(R2*sin(Theta2)*cos(Phi2), R2*sin(Theta2)*sin(Phi2), R2*cos(Theta2));
				glVertex3f(R3*sin(Theta3)*cos(Phi3), R3*sin(Theta3)*sin(Phi3), R3*cos(Theta3));

				switch (k)
				{
				case 0:glColor3d(1, 0, 0); break;
				case 1:glColor3d(1, 1, 0); break;
				case 2:glColor3d(0, 1, 0); break;
				case 3:glColor3d(0, 1, 1); break;
				case 4:glColor3d(0, 0, 1); break;
				default:glColor3d(1, 0, 1); break;
				}
				glVertex3f(R2*sin(Theta2)*cos(Phi2), R2*sin(Theta2)*sin(Phi2), R2*cos(Theta2));
				glVertex3f(R3*sin(Theta3)*cos(Phi3), R3*sin(Theta3)*sin(Phi3), R3*cos(Theta3));
				glVertex3f(R4*sin(Theta4)*cos(Phi4), R4*sin(Theta4)*sin(Phi4), R4*cos(Theta4));
				glEnd();
			}
		}
	}
	number += 1;
	if (number > Times)
		number = Times;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����������ı߿�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cubic_display(void)
{
	//��0�棬ƽ����z������������
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 0, 0);
	glVertex3f(x + half_length, y + half_length, z - half_length);
	glVertex3f(x + half_length, y - half_length, z - half_length);
	glVertex3f(x - half_length, y - half_length, z - half_length);
	glVertex3f(x - half_length, y + half_length, z - half_length);
	glVertex3f(x + half_length, y + half_length, z - half_length);
	glEnd();
	//��1�棬��ֱ��z������������
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 1, 0);
	glVertex3f(x + half_length, y + half_length, z + half_length);
	glVertex3f(x + half_length, y - half_length, z + half_length);
	glVertex3f(x - half_length, y - half_length, z + half_length);
	glVertex3f(x - half_length, y + half_length, z + half_length);
	glVertex3f(x + half_length, y + half_length, z + half_length);
	glEnd();
	//��2�棬��ֱ��x������������
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 1, 0);
	glVertex3f(x - half_length, y + half_length, z - half_length);
	glVertex3f(x - half_length, y - half_length, z - half_length);
	glVertex3f(x - half_length, y - half_length, z + half_length);
	glVertex3f(x - half_length, y + half_length, z + half_length);
	glVertex3f(x - half_length, y + half_length, z - half_length);
	glEnd();
	//��3�棬��ֱ��x������������
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 1, 1);
	glVertex3f(x + half_length, y + half_length, z - half_length);
	glVertex3f(x + half_length, y - half_length, z - half_length);
	glVertex3f(x + half_length, y - half_length, z + half_length);
	glVertex3f(x + half_length, y + half_length, z + half_length);
	glVertex3f(x + half_length, y + half_length, z - half_length);
	glEnd();
	//��4�棬��ֱ��y������������
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 1);
	glVertex3f(x + half_length, y - half_length, z - half_length);
	glVertex3f(x - half_length, y - half_length, z - half_length);
	glVertex3f(x - half_length, y - half_length, z + half_length);
	glVertex3f(x + half_length, y - half_length, z + half_length);
	glVertex3f(x + half_length, y - half_length, z - half_length);
	glEnd();
	//��5�棬��ֱ��y������������
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 0, 1);
	glVertex3f(x + half_length, y + half_length, z - half_length);
	glVertex3f(x - half_length, y + half_length, z - half_length);
	glVertex3f(x - half_length, y + half_length, z + half_length);
	glVertex3f(x + half_length, y + half_length, z + half_length);
	glVertex3f(x + half_length, y + half_length, z - half_length);
	glEnd();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //�����ɫ������
	glColor3f(0, 0, 0);   //������ɫ
	glLoadIdentity();   //��վ���
	glTranslatef(0, 0, -10); //�������е�������z�Ḻ�����ƶ�5����λ��

	glRotatef(spin, 1, 1, 0);	//ʹ��ʾ����������ת
	
	cubic_display();	//��ʾ������߿�

	spin += delta;	//��ת�Ƕȵ�����
	if (spin > 360)
		spin = 0;

	if (flag)
		middle_display();	//��ʾ���������α�Ϊ����
	else
		triangle_display();	//��ʾ��������������񻯽��

	glutSwapBuffers();	//����

}


void init(void)
{
	glClearColor(0, 0, 0, 0);
	glClearDepth(1);
	glShadeModel(GL_FLAT); //ѡ��ƽ������ģʽ��⻬����ģʽ
	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);   //���û���
	glMatrixMode(GL_PROJECTION);  //ָ����һ�������ǵ�ǰ����
	glLoadIdentity();
	gluPerspective(70, 1, 1.5, 20);   //����͸��ͶӰ����(fovy,aspect,zNear,zFar);
	glMatrixMode(GL_MODELVIEW);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
		flag = 1;
}

int main(int argc, char *argv[])
{
	vertex_comp();
	ball_points_comp();
	glutInit(&argc, argv); //�̶���ʽ
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);   //����ģʽ
	glutInitWindowSize(600, 600);    //��ʾ��Ĵ�С
	glutInitWindowPosition(500, 200); //ȷ����ʾ�����Ͻǵ�λ��
	glutCreateWindow("���������������α�Ϊ����");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutMouseFunc(Mouse);
	glutMainLoop(); //����GLUT�¼�����ѭ��
	return 0;
}