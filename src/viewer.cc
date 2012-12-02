/**
 *  @file
 *  @brief 
 *  @date 26/11/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glut.h>

#include <voronoi/voronoi.hh>
#include <viewer.hh>

#include <macros.hh>

extern "C" {

static void display_callback()
{
	voronoi::DiagramViewer::getInstance().DisplayCallback();
}

static void reshape_callback(int width, int height)
{
	voronoi::DiagramViewer::getInstance().ReshapeCallBack(width, height);
}

static void timer_callback(int milliseconds)
{
	voronoi::DiagramViewer::getInstance().TimerCallback(milliseconds);
}

static void keyboard_callback(unsigned char key, int x, int y)
{
	voronoi::DiagramViewer::getInstance().KeyboardCallback(key, x, y);
}

static void mouse_callback(int button, int state, int x, int y)
{
	voronoi::DiagramViewer::getInstance().MouseCallback(button, state, x, y);
}

static void mouse_motion_callback(int x, int y)
{
	voronoi::DiagramViewer::getInstance().MouseMotionCallback(x, y);
}

}

namespace voronoi {

const int DiagramViewer::kWindowPosY = 100;
const int DiagramViewer::kWindowPosX = 100;
const int DiagramViewer::kWindowWidth = 1200;
const int DiagramViewer::kWindowHeight = 800;
const int DiagramViewer::kTimerMilliseconds = 33;
const float DiagramViewer::kCameraDistance = 15.0f;
const float DiagramViewer::kCameraAngleY = 0.0f;
const float DiagramViewer::kCameraAngleX = 0.0f;
const void* DiagramViewer::kFont = GLUT_BITMAP_8_BY_13;

DiagramViewer& DiagramViewer::getInstance()
{
	static DiagramViewer instance;
	return instance;
}

DiagramViewer::DiagramViewer() :
		_mouse_x(false), _mouse_y(false), _mouse_left_button(false),
				_mouse_right_button(false), _diagram(NULL)
{
	Init();
}

DiagramViewer::~DiagramViewer()
{
}

void DiagramViewer::Init()
{
	InitDefaultValues();
	InitGLUT();
	RegisterCallbacks();
	InitGL();
	InitLights();
}

void DiagramViewer::InitDefaultValues()
{
	set_camera_angle_x(kCameraAngleX);
	set_camera_angle_y(kCameraAngleY);
	set_camera_distance(kCameraDistance);
}

void DiagramViewer::InitGLUT()
{
	int argc = 0;
	glutInit(&argc, NULL);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL); //GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	glutInitWindowPosition(kWindowPosX, kWindowPosY);
	glutInitWindowSize(kWindowWidth, kWindowHeight);
	glutCreateWindow("Diagrama de Voronoi");
}

void DiagramViewer::RegisterCallbacks()
{
	// Register GLUT callback functions
	glutDisplayFunc(display_callback);
	glutTimerFunc(kTimerMilliseconds, timer_callback, kTimerMilliseconds);
	glutReshapeFunc(reshape_callback);
	glutKeyboardFunc(keyboard_callback);
	glutMouseFunc(mouse_callback);
	glutMotionFunc(mouse_motion_callback);
}

void DiagramViewer::InitGL()
{
	glShadeModel(GL_SMOOTH);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0, 0, 0, 0); // background color
	glClearStencil(0); // clear stencil buffer
	glClearDepth(1.0f); // 0 is near, 1 is far
	glDepthFunc(GL_LEQUAL);
}

void DiagramViewer::InitLights()
{
	// set up light colors (ambient, diffuse, specular)
	GLfloat lightKa[] = { .2f, .2f, .5f, 1.0f }; // ambient light
	GLfloat lightKd[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // diffuse light
	GLfloat lightKs[] = { 0.5f, .5f, 1, 1 }; // specular light
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);
	// position the light
	float lightPos[4] = { 0, 0, 30, 1 }; // positional light
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
}

void DiagramViewer::DisplayCallback()
{
	if (diagram() == NULL)
		return;

	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClearColor(.5f, .5f, .5f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glLineWidth(100);

	glPushMatrix();
	glTranslatef(-3, -3, -camera_distance());

	glRotatef(camera_angle_x(), 1, 0, 0); // pitch
	glRotatef(camera_angle_y(), 0, 1, 0); // heading

	using voronoi::VoronoiDCEL;
	const VoronoiDCEL* d = diagram();

	// Prepare string drawing vectors
	std::vector<float> pos(3, 0.0f), color(4, 1.0f);
	pos[0] = -0.2f;
	pos[1] = -0.42f;
	int i = 1;

	// Draw all vertices as spheres
	glColor3f(0.5f, 0.0f, 0.1f);
	const std::vector<VoronoiDCEL::Vertex>& vertices = d->getVertices();
	for (std::vector<VoronoiDCEL::Vertex>::const_iterator it = vertices.begin();
			it != vertices.end(); it++) {
		int x = it->getData()->x, y = it->getData()->y;

		glTranslatef(x, y, 0);
		GLUquadricObj* cyl = gluNewQuadric();
		gluQuadricDrawStyle(cyl, GLU_FILL);
		gluSphere(cyl, 0.2, 50, 100);

		std::stringstream name("v");
		name << i++;
		Draw3DString(name.str(), pos, color, kFont);
		glTranslatef(-x, -y, 0);
	}

	// Draw edges and write vertices index
	glColor3f(.3f, .3f, .7f);
	const std::vector<VoronoiDCEL::HalfEdge>& edges = d->getHalfEdges();
	for (std::vector<VoronoiDCEL::HalfEdge>::const_iterator it = edges.begin();
			it != edges.end(); it++, it++) {
		int x = it->getOrigin()->getData()->x;
		int y = it->getOrigin()->getData()->y;
		int dstx = it->getTwin()->getOrigin()->getData()->x;
		int dsty = it->getTwin()->getOrigin()->getData()->y;

		glTranslatef(x, y, 0);
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(dstx - x, dsty - y, 0.0f);
		glEnd();
		glTranslatef(-x, -y, 0);
	}

	// Draw faces indexes
	glColor3f(.3f, .3f, .7f);
	const std::vector<VoronoiDCEL::Face>& faces = d->getFaces();
	for (std::vector<VoronoiDCEL::Face>::const_iterator it = faces.begin();
			it != faces.end(); it++) {
		float xsum = 0.0f;
		float ysum = 0.0f;
		VoronoiDCEL::HalfEdge* start = it->getBoundary();
		VoronoiDCEL::HalfEdge* current = start->getNext();
		xsum += start->getOrigin()->getData()->x;
		ysum += start->getOrigin()->getData()->y;

		int count = 1;
		while (current != start) {
			xsum += current->getOrigin()->getData()->x;
			ysum += current->getOrigin()->getData()->y;
			current = current->getNext();
			count++;
		}
		float x = xsum / count;
		float y = ysum / count;

		glTranslatef(x, y, 0);
		pos[0] = pos[1] = 0.0f;
		std::stringstream name;
		name << "f" << it->getData().id;
		Draw3DString(name.str(), pos, color, kFont);
		glTranslatef(-x, -y, 0);
	}

	glPopMatrix();
	glutSwapBuffers();
}

void DiagramViewer::ReshapeCallBack(int width, int height)
{
	UpdatePerspective(width, height);
}

void DiagramViewer::TimerCallback(int milliseconds)
{
	glutTimerFunc(milliseconds, timer_callback, milliseconds);
	glutPostRedisplay();
}

void DiagramViewer::KeyboardCallback(unsigned char key, int x, int y)
{
	UNUSED(key);
	UNUSED(x);
	UNUSED(y);
}

void DiagramViewer::MouseCallback(int button, int state, int x, int y)
{
	set_mouse_x(x);
	set_mouse_y(y);
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN)
			set_mouse_left_button(true);
		else if (state == GLUT_UP)
			set_mouse_left_button(false);
	} else if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN)
			set_mouse_right_button(true);
		else if (state == GLUT_UP)
			set_mouse_right_button(false);
	}
}

void DiagramViewer::MouseMotionCallback(int x, int y)
{
	if (mouse_left_button()) {
		set_camera_angle_y(camera_angle_y() + (y - mouse_y()) * 0.002f);
		set_camera_angle_x(camera_angle_x() + (x - mouse_x()) * 0.002f);
		set_mouse_x(x);
		set_mouse_y(y);
	}
	if (mouse_right_button()) {
		set_camera_distance(camera_distance() - (y - mouse_y()) * 0.0002f);
		set_mouse_y(y);
	}
}

void DiagramViewer::Show()
{
	glutMainLoop();
}

void DiagramViewer::UpdatePerspective(int width, int height)
{
	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, static_cast<float>(width) / height, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void DiagramViewer::set_diagram(VoronoiDCEL* diagram)
{
	_diagram = diagram;
}

const VoronoiDCEL* DiagramViewer::diagram() const
{
	return _diagram;
}

bool DiagramViewer::mouse_x() const
{
	return _mouse_x;
}

bool DiagramViewer::mouse_y() const
{
	return _mouse_y;
}

bool DiagramViewer::mouse_left_button() const
{
	return _mouse_left_button;
}

bool DiagramViewer::mouse_right_button() const
{
	return _mouse_right_button;
}

void DiagramViewer::set_mouse_x(bool value)
{
	_mouse_x = value;
}

void DiagramViewer::set_mouse_y(bool value)
{
	_mouse_y = value;
}

void DiagramViewer::set_mouse_left_button(bool value)
{
	_mouse_left_button = value;
}

void DiagramViewer::set_mouse_right_button(bool value)
{
	_mouse_right_button = value;
}

int DiagramViewer::camera_angle_y() const
{
	return _camera_angle_y;
}

int DiagramViewer::camera_angle_x() const
{
	return _camera_angle_x;
}

float DiagramViewer::camera_distance() const
{
	return _camera_distance;
}

void DiagramViewer::set_camera_angle_y(int value)
{
	_camera_angle_y = value;
}

void DiagramViewer::set_camera_angle_x(int value)
{
	_camera_angle_x = value;
}

void DiagramViewer::set_camera_distance(float value)
{
	_camera_distance = value;
}

void DiagramViewer::Draw3DString(const std::string str,
		const std::vector<float>& pos, const std::vector<float>& color,
		const void* font)
{
	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor4fv(&color[0]); // set text color
	glRasterPos3fv(&pos[0]); // place text position
	// loop all characters in the string
	char* ptr = const_cast<char*>(str.c_str());
	while (*ptr)
		glutBitmapCharacter(const_cast<void*>(font), *ptr++);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopAttrib();
}

}
