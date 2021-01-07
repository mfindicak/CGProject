#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include <Camera.h>
#include <fstream>
#include <string>
#include <list>
#include <sstream>

GLsizei myWidth = 640;
GLsizei myHeight = 480;
std::list<std::string> names;
std::list<GLdouble> numbers;
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
				GLdouble number;
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

void draw3D(std::list<std::string> listNames, std::list<GLdouble> listNumbers) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	glLoadIdentity();                 // Reset the model-view matrix
	gluLookAt(camera.x, camera.y, camera.z, 0, camera.centerY, 0, 0, 1, 0);

	int listSize = listNumbers.size();
	int counter = 0;
	for (GLdouble x : listNumbers) {
		if (counter == 0) {
			glTranslatef(0.f, 0.0f, (listSize - 1) * 2.5f);
			drawCube((GLdouble)x);
		}
		else {
			glTranslatef(0.0f, 0.0f, -5.0f);
			drawCube((GLdouble)x);
		}
		counter++;
	}
}

void draw2D(std::list<std::string> listNames, std::list<GLdouble> listNumbers) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(0, 640, 0, 480);

	GLsizei startX = 120;
	GLsizei startY = 50;

	//For Draw X-Y Axis
	glLineWidth(2.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	glVertex2i(520, startY);
	glVertex2i(startX, startY);
	glVertex2i(startX, startY);
	glVertex2i(startX, 450);
	glEnd();

	GLint counter = 0;
	GLdouble eachPixel = (GLdouble)390 / listNames.size();
	GLdouble* nameCenterX = new GLdouble[listNames.size()];
	GLdouble nameStartX = 0;
	GLdouble nameStartY = 0;

	for (std::string x : listNames) {

		GLint textWidth = x.size() * 9;
		nameStartX = startX + counter * eachPixel + (eachPixel - textWidth) / 2;
		*(nameCenterX + counter) = nameStartX + (GLdouble)(textWidth / 2);
		glRasterPos2i(nameStartX, startY - 15);
		for (char y : x) {
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, y);
		}
		counter++;
	}



	GLdouble maxNumber = 0;
	for (GLdouble x : listNumbers) {
		if (x > maxNumber) maxNumber = x;
	}

	GLdouble logResult = log10(maxNumber);
	GLint logNumber = int(logResult) - 1;
	GLint interval = (GLint(maxNumber) / 10) + 1;
	counter = 1;
	for (GLint i = 1;i <= 10;i++) {
		glRasterPos2i((GLdouble)startX - 4.5f, startY + i * 36.36f);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '-');
		glRasterPos2i(startX - logNumber * 9 - 23, startY + i * 36.36f);
		std::string s = std::to_string(interval * counter);
		for (char x : s) {
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, x);
		}
		counter++;
	}

	counter = 0;
	for (GLdouble x : listNumbers) {
		glRasterPos2i(*(nameCenterX + counter) - 4.5f, startY + (GLdouble)x * 36.36f / interval);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '*');
		counter++;
	}
}

void init2D() {
	glLoadIdentity();
	glClearColor(1.0, 1.0, 1.0, 0.0);		// beyaz renkte gösterim penceresi
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
	GLdouble total = 0;
	for (GLdouble x : numbers) total += x;
	camera.changeCenter((GLdouble)total / numbers.size());
	glutInit(&argc, argv);            // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
	glutInitWindowSize(myWidth, myHeight);   // Set the window's initial width & height
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