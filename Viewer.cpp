#include "Viewer.h"

Viewer *Viewer::instance = NULL;

// OpenGL Variables
GLuint textureId;
GLubyte rgbStream[WINDOW_WIDTH*WINDOW_HEIGHT*4];

Viewer::Viewer(int argc, char** argv) {

	instance = this;

	//kinect = Kinect();

	camera = Camera(
		Vector3D(CAM_POS_X, CAM_POS_Y ,CAM_POS_Z),
		Vector3D(CAM_VIEW_X, CAM_VIEW_Y, CAM_VIEW_Z),
		Vector3D(CAM_UP_X, CAM_UP_Y, CAM_UP_Z));


	mousePros = false;

	stringColor[0] = .5;
	stringColor[1] = .5;
	stringColor[2] = .5;
	stringColor[3] = 1.0;

	glutInit(&argc, argv);

	//scene window
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);

	glutInitWindowPosition (WINDOW_X_POS, WINDOW_Y_POS);

	sceneWindow = glutCreateWindow (WINDOW_NAME);

	glutDisplayFunc(setSceneDisplayFunction);
	glutIdleFunc(setIdeleFunction);
	glutReshapeFunc(setReshapeFunction);

	glutKeyboardFunc(setKeybordFunction);

	glutMouseFunc(setMouseFunction);
	glutMotionFunc(setMouseMoveFunction);
	glutPassiveMotionFunc(setMouseMoveFunction);


	//rgb stream
	glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);

	glutInitWindowPosition (WINDOW_X_POS+WINDOW_WIDTH, WINDOW_Y_POS);

	rgbWindow = glutCreateWindow ("RGB");

	glutDisplayFunc(setStreamDisplayFunction);

	//init
	init ();
}

Viewer::~Viewer() {
	
	delete instance;
}

void Viewer::addToRender(Renderable *rend){
	renderables.push_back(rend);
}

void Viewer::start(){
	glutMainLoop();
}

void Viewer::startTimer(){
	//timer
	INIReader ini(INI_FILE_PATH);
	if(ini.GetBoolean("timer", "TIMER_ON", false)){
		glutTimerFunc(ini.GetInteger("timer", "TIME", 5000), timerDispacher, 0);
	}
}

void Viewer::init(){

	//shader
	if(SMOOTH){
		glShadeModel (GL_SMOOTH);
	}else{
		glShadeModel(GL_FLAT);
	}

	//z-buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// color
	if(COLOR){
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE || GL_SPECULAR);
	}
	
  
	//light source
	if(LIGHT){
		GLfloat light_position[] = {L_POS_X, L_POS_Y, L_POS_Z, L_POS_W};
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		GLfloat ambient[] = {.1, .1, .1, 1.0};
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

		GLfloat diffuse[] = {.3, .3, .3, 1.0};
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

		GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	
	if(WIRED){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	}else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Initialize textures for rgb stream
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WINDOW_WIDTH, WINDOW_HEIGHT,
		0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*) rgbStream);
    glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_TEXTURE_2D);

	//projection
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	GLfloat ratio = (GLfloat) WINDOW_WIDTH/(GLfloat) WINDOW_HEIGHT;
	gluPerspective(40.0, ratio, 1.0f, 200.0f);
	glMatrixMode (GL_MODELVIEW);


	//timer
	startTimer();
	
}

void Viewer::renderScene(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	showInfo();

	camera.draw();

	for(unsigned i = 0;i<renderables.size();i++){
		renderables[i]->draw();
	}

	kinect.draw();

	glutSwapBuffers();
}

void Viewer::renderStream(){
	glPushMatrix();

	 // Camera setup
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, textureId);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
		WINDOW_WIDTH, WINDOW_HEIGHT, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*)rgbStream);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(WINDOW_WIDTH, 0, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, WINDOW_HEIGHT, 0.0f);
    glEnd();

	glPopMatrix();

	glutSwapBuffers();
}

void Viewer::update(){

	for(unsigned i = 0;i<renderables.size();i++){
		renderables[i]->update();
	}

	kinect.update();

	//render scene
	glutSetWindow(sceneWindow);
	glutPostRedisplay();

	//render rgb stream
	glutSetWindow(rgbWindow);
	glutPostRedisplay();
}



void Viewer::reshape (int w, int h){
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	GLfloat ratio = (GLfloat) w/(GLfloat) h;
	gluPerspective(40.0, ratio, 1.0f, 200.0f);
	glMatrixMode (GL_MODELVIEW);
}

void Viewer::keyboard (unsigned char key, int x, int y){
	switch (key) {
	case 'w':
		camera.move(CAMERA_MOVE_SPEED);
		break;
	case 's':
		camera.move(-CAMERA_MOVE_SPEED);
		break;
	case 'd':
		camera.strafe(CAMERA_MOVE_SPEED);
		break;
	case 'a':
		camera.strafe(-CAMERA_MOVE_SPEED);
		break;
	case 'q':
		camera.rotateY(CAMERA_ROTATE_SPEED);
		break;
	case 'e':
		camera.rotateY(-CAMERA_ROTATE_SPEED);
		break;

	case 'r':
		kinect.skeleton.saveVectorToBinary();
		std::cout<<"Skeleton sequence saved to binary\n";
		break;
	case 'l':
		kinect.skeleton.loadVectorFromBinary();
		std::cout << "Skeleton sequence loaded from binary\n";
		break;
	case 'm':
		kinect.skeleton.saveToMatlab();
		std::cout << "Skeleton sequence saved to matlab dat\n";
		break;
	case 'o':
		kinect.skeleton.saveMotionToOpenSim(kinect.getSkeletonFrameRate());
		std::cout << "Trajectory file saved to OpenSim format\n"; 
		break;
	case 'c':
		if(kinect.skeleton.animate == true) return;
		kinect.skeleton.clearData();
		std::cout << "Data cleared\n";
		break;
	case 'p':
		kinect.skeleton.animate = !kinect.skeleton.animate;
		std::cout << "Replay\n";
		break;
	case 'b':
		instance->kinect.skeleton.saveSegmentLengths();
		std::cout << "Segment lengths saved\n";
		break;
	default:
		break;
	}
}

void Viewer::mouseButton(int button, int state, int x, int y){
	if(button==GLUT_LEFT_BUTTON){
		
		if (state == GLUT_DOWN) {
			mousePros = true;
		}else  {
			mousePros = false;
		}
	}
}

void Viewer::mouseMove(int x, int y){
	static bool just_warped = false;
	if(just_warped) {
        just_warped = false;
        return;
    }
	if(mousePros){
		int difX = x-WINDOW_WIDTH/2;
		int difY = y-WINDOW_HEIGHT/2;

		if(abs(difX)>abs(difY)){
			camera.rotateY(-difX*CAMERA_ROTATE_SPEED);
		}else{
			camera.rotateX(-difY*CAMERA_ROTATE_SPEED);
		}
		
		glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		just_warped = true;
		
	}
	
}

void Viewer::drawString(const char *str, int x, int y, float color[4], void *font){
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glColor4fv(color);
    glRasterPos2i(x, y);

    // loop all characters in the string
    while(*str){
        glutBitmapCharacter(font, *str);
        ++str;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}

void Viewer::showInfo(){
    // backup current model-view matrix
    glPushMatrix();                     // save current modelview matrix
    glLoadIdentity();                   // reset modelview matrix

    // set to 2D orthogonal projection
    glMatrixMode(GL_PROJECTION);        // switch to projection matrix
    glPushMatrix();                     // save current projection matrix
    glLoadIdentity();                   // reset projection matrix
	gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT); // set to orthogonal projection

    float color[4] = {1, 1, 1, 0.5};

	std::ostringstream fps;
    fps << "FPS: " << kinect.getSkeletonFrameRate();
	drawString(&fps.str()[0], 0, WINDOW_HEIGHT-1*TEXT_HEIGHT, color, GLUT_BITMAP_HELVETICA_12);

	drawString("Key r: Save Skel", 0, WINDOW_HEIGHT-2*TEXT_HEIGHT, color, GLUT_BITMAP_HELVETICA_12);
	drawString("Key l: Load Skel", 0, WINDOW_HEIGHT-3*TEXT_HEIGHT, color, GLUT_BITMAP_HELVETICA_12);
	drawString("Key m: Save Matlab", 0, WINDOW_HEIGHT-4*TEXT_HEIGHT, color, GLUT_BITMAP_HELVETICA_12);
	drawString("Key o: Save Trc", 0, WINDOW_HEIGHT-5*TEXT_HEIGHT, color, GLUT_BITMAP_HELVETICA_12);
	drawString("Key c: Clear Data", 0, WINDOW_HEIGHT-6*TEXT_HEIGHT, color, GLUT_BITMAP_HELVETICA_12);
	drawString("Key p: Replay data", 0, WINDOW_HEIGHT-7*TEXT_HEIGHT, color, GLUT_BITMAP_HELVETICA_12);
	drawString("Key b: Segment lengths", 0, WINDOW_HEIGHT-8*TEXT_HEIGHT, color, GLUT_BITMAP_HELVETICA_12);

	drawString("Movements: w, s, a, d", 0, 0, color, GLUT_BITMAP_HELVETICA_12);
	drawString("Camera rotation: mouse",  0, TEXT_HEIGHT, color, GLUT_BITMAP_HELVETICA_12);

	
    // restore projection matrix
    glPopMatrix();                   // restore to previous projection matrix

    // restore modelview matrix
    glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
    glPopMatrix();                   // restore to previous modelview matrix
}


void Viewer::setSceneDisplayFunction(){
	instance->renderScene();
}

void Viewer::setStreamDisplayFunction(){
	instance->renderStream();
}

void Viewer::setIdeleFunction(){
	instance->update();
}

void Viewer::setReshapeFunction(int w, int h){
	instance->reshape(w, h);
}

void Viewer::setKeybordFunction(unsigned char key, int x, int y){
	instance->keyboard(key, x, y);
}

void Viewer::setMouseFunction(int button, int state, int x, int y){
	instance->mouseButton(button, state, x, y);
}

void Viewer::setMouseMoveFunction(int x, int y){
	instance->mouseMove(x, y);
}

void Viewer::timerDispacher(int i){
	INIReader ini(INI_FILE_PATH);

	switch(ini.GetInteger("timer", "TIMER_FUNCTION", 2)){

		case 1:
			instance->kinect.skeleton.saveMotionToOpenSim(instance->kinect.getSkeletonFrameRate());
			break;
		case 2:
			instance->kinect.skeleton.saveToMatlab();
			break;
		case 3:
			instance->kinect.skeleton.saveSegmentLengths();
			break;
		default:
			instance->kinect.skeleton.saveMotionToOpenSim(instance->kinect.getSkeletonFrameRate());
			instance->kinect.skeleton.saveToMatlab();
			instance->kinect.skeleton.saveSegmentLengths();
			break;
	}
	
	printf("Timer finished\n");
}
