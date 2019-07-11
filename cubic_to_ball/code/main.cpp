#define GLUT_DISABLE_ATEXIT_HACK
#include <GL\glut.h>  //引用相关包
#include <stdlib.h>
#include <cmath>
#include <math.h>
using namespace std;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//参数设置
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//正方体的相关参数
GLfloat x = 0, y = 0, z = 0;	//中心位置
GLfloat half_length = 2;	//正方体边长的一半

//旋转显示的相关参数
GLfloat spin = 0;	//旋转角
float delta = 0.1;	//每次增加的旋转角度

//正方体三角网格化的相关参数
const int N = 8;	//正方体的一条边分成N条线段
float dl = half_length * 2.0 / N;	//每条线段的长度
const int vertex_num = (N + 1)*(N + 1);		//相应的一个面的生成的点的个数
GLfloat vertexes[6][vertex_num][3];		//三角网格化需要的生成的点集

//形变成球的相关参数
GLfloat ball_points[6][vertex_num][3];	//点的球坐标集
const int Times = 2000;	//形变的帧数
int number=0;	//当前显示第几帧
GLfloat dr[6][vertex_num];	//每个点每一帧增加的r
GLfloat r=(GLfloat)pow((pow(half_length, 2) + pow(half_length, 2) + pow(half_length, 2)), 0.5);	//最终得到的球体的半径

bool flag = 0;	//是否按下鼠标左键的标志，决定现在是否显示渐变过程
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//生成点集
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void vertex_comp(void)
{
	//第0面和第1面的点集
	for (int i = 0; i <= N; i++)
	{
		for (int j = 0; j <= N; j++)
		{
			vertexes[0][i*(N + 1) + j][0] = { x - half_length + j*dl };	//x的位置
			vertexes[0][i*(N + 1) + j][1] = { y - half_length + i*dl };	//y的位置
			vertexes[0][i*(N + 1) + j][2] = { z - half_length };	//z的位置

			vertexes[1][i*(N + 1) + j][0] = { x - half_length + j*dl };	//x的位置
			vertexes[1][i*(N + 1) + j][1] = { y - half_length + i*dl };	//y的位置
			vertexes[1][i*(N + 1) + j][2] = { z + half_length };	//z的位置
		}
	}

	//第2面和第3面的点集
	for (int i = 0; i <= N; i++)
	{
		for (int j = 0; j <= N; j++)
		{
			vertexes[2][i*(N + 1) + j][0] = { x - half_length };	//x的位置
			vertexes[2][i*(N + 1) + j][1] = { y - half_length + i*dl };	//y的位置
			vertexes[2][i*(N + 1) + j][2] = { z - half_length + j*dl };	//z的位置

			vertexes[3][i*(N + 1) + j][0] = { x + half_length };	//x的位置
			vertexes[3][i*(N + 1) + j][1] = { y - half_length + i*dl };	//y的位置
			vertexes[3][i*(N + 1) + j][2] = { z - half_length + j*dl };	//z的位置
		}
	}

	//第4面和第5面的点集
	for (int i = 0; i <= N; i++)
	{
		for (int j = 0; j <= N; j++)
		{
			vertexes[4][i*(N + 1) + j][0] = { x - half_length + j*dl };	//x的位置
			vertexes[4][i*(N + 1) + j][1] = { y - half_length };	//y的位置
			vertexes[4][i*(N + 1) + j][2] = { z - half_length + i*dl };	//z的位置

			vertexes[5][i*(N + 1) + j][0] = { x - half_length + j*dl };	//x的位置
			vertexes[5][i*(N + 1) + j][1] = { y + half_length };	//y的位置
			vertexes[5][i*(N + 1) + j][2] = { z - half_length + i*dl };	//z的位置
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//计算生成点集的球坐标
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
			ball_points[k][t][0] = (GLfloat)pow((pow(tempX, 2) + pow(tempY, 2) + pow(tempZ, 2)), 0.5);	//球坐标中的r
			ball_points[k][t][1] = (GLfloat)acos(tempZ / ball_points[k][t][0]);		//球坐标中的theta
			ball_points[k][t][2] = (GLfloat)atan2(tempY, tempX);	//球坐标中的phi

			dr[k][t] = (r - ball_points[k][t][0]) / Times;	////每个点每一帧增加的r
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//三角网格化
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void triangle_display(void)
{
	
	//根据生成的点生成每个面的三角网格
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
//生成三角网格形变的中间帧
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void middle_display(void)
{

	//根据生成的点生成每个面的三角网格
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
//生成正方体的边框
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cubic_display(void)
{
	//第0面，平行于z轴的面的四条边
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 0, 0);
	glVertex3f(x + half_length, y + half_length, z - half_length);
	glVertex3f(x + half_length, y - half_length, z - half_length);
	glVertex3f(x - half_length, y - half_length, z - half_length);
	glVertex3f(x - half_length, y + half_length, z - half_length);
	glVertex3f(x + half_length, y + half_length, z - half_length);
	glEnd();
	//第1面，垂直于z轴的面的四条边
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 1, 0);
	glVertex3f(x + half_length, y + half_length, z + half_length);
	glVertex3f(x + half_length, y - half_length, z + half_length);
	glVertex3f(x - half_length, y - half_length, z + half_length);
	glVertex3f(x - half_length, y + half_length, z + half_length);
	glVertex3f(x + half_length, y + half_length, z + half_length);
	glEnd();
	//第2面，垂直于x轴的面的四条边
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 1, 0);
	glVertex3f(x - half_length, y + half_length, z - half_length);
	glVertex3f(x - half_length, y - half_length, z - half_length);
	glVertex3f(x - half_length, y - half_length, z + half_length);
	glVertex3f(x - half_length, y + half_length, z + half_length);
	glVertex3f(x - half_length, y + half_length, z - half_length);
	glEnd();
	//第3面，垂直于x轴的面的四条边
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 1, 1);
	glVertex3f(x + half_length, y + half_length, z - half_length);
	glVertex3f(x + half_length, y - half_length, z - half_length);
	glVertex3f(x + half_length, y - half_length, z + half_length);
	glVertex3f(x + half_length, y + half_length, z + half_length);
	glVertex3f(x + half_length, y + half_length, z - half_length);
	glEnd();
	//第4面，垂直于y轴的面的四条边
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 1);
	glVertex3f(x + half_length, y - half_length, z - half_length);
	glVertex3f(x - half_length, y - half_length, z - half_length);
	glVertex3f(x - half_length, y - half_length, z + half_length);
	glVertex3f(x + half_length, y - half_length, z + half_length);
	glVertex3f(x + half_length, y - half_length, z - half_length);
	glEnd();
	//第5面，垂直于y轴的面的四条边
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
//主框架
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //清空颜色缓冲区
	glColor3f(0, 0, 0);   //重置颜色
	glLoadIdentity();   //清空矩阵
	glTranslatef(0, 0, -10); //将场景中的物体沿z轴负方向移动5个单位长

	glRotatef(spin, 1, 1, 0);	//使显示的立方体旋转
	
	cubic_display();	//显示正方体边框

	spin += delta;	//旋转角度的增加
	if (spin > 360)
		spin = 0;

	if (flag)
		middle_display();	//显示正方体逐渐形变为球体
	else
		triangle_display();	//显示正方体的三角网格化结果

	glutSwapBuffers();	//缓存

}


void init(void)
{
	glClearColor(0, 0, 0, 0);
	glClearDepth(1);
	glShadeModel(GL_FLAT); //选择平面明暗模式或光滑明暗模式
	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);   //设置机口
	glMatrixMode(GL_PROJECTION);  //指定哪一个矩阵是当前矩阵
	glLoadIdentity();
	gluPerspective(70, 1, 1.5, 20);   //创建透视投影矩阵(fovy,aspect,zNear,zFar);
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
	glutInit(&argc, argv); //固定格式
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);   //缓存模式
	glutInitWindowSize(600, 600);    //显示框的大小
	glutInitWindowPosition(500, 200); //确定显示框左上角的位置
	glutCreateWindow("正方体三角网格形变为球体");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutMouseFunc(Mouse);
	glutMainLoop(); //进人GLUT事件处理循环
	return 0;
}