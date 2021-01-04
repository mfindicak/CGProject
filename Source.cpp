#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include <Camera.h>
#include <fstream>
#include <string>
#include <list>
#include <sstream>

std::list<std::string> names;
std::list<int> numbers;
char title[] = "Data Visualization 031890002";
int refreshMills = 15;
int graph = 2;

Camera camera;

void readData(const char fileName[]) {
	std::ifstream file(fileName);
	int counter = 0;
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			if (counter % 2 == 0) {
				names.push_back(line.c_str());
			}
			else {
				std::string text = line.c_str();
				std::stringstream geek(text);
				int number;
				geek >> number;
				numbers.push_back(number);
			}
			counter++;
		}
		file.close();
	}
}

void drawCube(GLdouble height) {
	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex3f(1.0f, height, -1.0f);
	glVertex3f(-1.0f, height, -1.0f);
	glVertex3f(-1.0f, height, 1.0f);
	glVertex3f(1.0f, height, 1.0f);

	// Bottom face (y = -1.0f)
	glColor3f(1.0f, 0.5f, 0.0f);     // Orange
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Front face  (z = 1.0f)
	glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glVertex3f(1.0f, height, 1.0f);
	glVertex3f(-1.0f, height, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face (z = -1.0f)
	glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, height, -1.0f);
	glVertex3f(1.0f, height, -1.0f);

	// Left face (x = -1.0f)
	glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glVertex3f(-1.0f, height, 1.0f);
	glVertex3f(-1.0f, height, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face (x = 1.0f)
	glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
	glVertex3f(1.0f, height, -1.0f);
	glVertex3f(1.0f, height, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();  // End of drawing color-cube
}

void draw3D(std::list<std::string> listNames, std::list<int> listNumbers) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	glLoadIdentity();                 // Reset the model-view matrix
	gluLookAt(camera.x, camera.y + 2, camera.z, 0, 2, 0, 0, 1, 0);

	int listSize = listNumbers.size();
	int counter = 0;
	for (int x : listNumbers) {
		if (counter == 0) {
			glTranslatef(0.f, 0.0f, -(listSize - 1) * 2.5f);
			drawCube((GLdouble)x);
		}
		else {
			glTranslatef(0.0f, 0.0f, 5.0f);
			drawCube((GLdouble)x);
		}
		counter++;
	}
}

void draw2D(std::list<std::string> listNames, std::list<int> listNumbers) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(0, 640, 0, 480);
	GLsizei xRaster = 25, yRaster = 150;
	GLubyte etiket[36] = { 'O','c','a',	'S','u','b',	'M','a','r',
						'N','i','s',	'M','a','y',	'H','a','z',
						'T','e','m',	'A','g','u',	'E','y','l',
						'E','k','m',	'K','a','s',	'A','r','a', };
	GLint dataValue[12] = { 420, 342, 324, 310, 262, 185,
							190, 196, 217, 240, 312, 438 };

	GLint ay, k;
	GLint x = 30; //grafik i�in x konumunu ilklendir

	glClear(GL_COLOR_BUFFER_BIT); //G�sterim penceresini temizle
	glLineWidth(3.0);			//�izgi kal�nl���n� ayarla
	glEnable(GL_LINE_STIPPLE);	//�izgi �zellik de�i�imini aktifle�tir
	glColor3f(0.0f, 0.0f, 1.0f);			//�izgi rengini maviye ata
	//Veriyi k�r�kl� hat �eklinde �izdir
	glBegin(GL_LINE_STRIP);
	for (k = 0; k < 12; k++)
		glVertex2i(x + k * 50, dataValue[k]);
	glEnd();
	glDisable(GL_LINE_STIPPLE);		//�izgi �zellik de�i�imini inaktifle�tir

	//Veri noktalar�n� y�ld�z �eklinde �izdir
	glColor3f(1.0f, 0.0f, 0.0f);			//i�aret�i rengini k�rm�z�ya ata
	for (k = 0; k < 12; k++) {
		glRasterPos2i(xRaster + k * 50, dataValue[k] - 4);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '*');
	}

	//Grafik etiketlerini �izdir
	glColor3f(0.0f, 0.0f, 0.0f);			//yaz� rengini siyaha ata
	xRaster = 20;
	for (ay = 0; ay < 12; ay++) {
		glRasterPos2i(xRaster, yRaster);
		for (k = 3 * ay; k < 3 * ay + 3; k++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, etiket[k]);
		xRaster += 50;
	}
}

void init2D() {
	glLoadIdentity();
	glClearColor(1.0, 1.0, 1.0, 0.0);		// beyaz renkte g�sterim penceresi
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
}

void initGL() {
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	gluPerspective(45.0f, (double)640 / 480, 0.1f, 100.0f);
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
	if (graph == 1) draw3D(names, numbers);
	else if (graph == 2) draw2D(names, numbers);

	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

/* Called back when timer expired [NEW] */
void timer(int value) {
	glutPostRedisplay();      // Post re-paint request to activate display()
	glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'a' or key == 'A') {
		camera.turnLeft(3);
	}
	else if (key == 'd' or key == 'D') {
		camera.turnRight(3);
	}
	else if (key == 'w' or key == 'W') {
		camera.changeDistance(-0.2);
	}
	else if (key == 's' or key == 'S') {
		camera.changeDistance(0.2);
	}
	else if (key == 'e' or key == 'E') {
		camera.y += 0.7f;
	}
	else if (key == 'f' or key == 'F') {
		camera.y -= 0.7f;
	}
	else if (key == '1') {
		initGL();
		graph = 1;
	}
	else if (key == '2') {
		init2D();
		graph = 2;
	}
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	readData("data.txt");
	glutInit(&argc, argv);            // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
	glutInitWindowSize(640, 480);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow(title);          // Create window with the given title
	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event
	if (graph == 1) initGL();		// Our own OpenGL initialization
	else if (graph == 2) init2D();
	glutKeyboardFunc(keyboard);
	glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}