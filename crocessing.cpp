// Crocessing is a C library that enables Processing-style
// C-Programming for Creative Coding.
// Written by Jusub Kim (showyourmind@gmail.com), 2013

#include "crocessing.h"
#include "SOIL.h"

int mouseX=0;
int mouseY=0;
int mouseButton;
char key;
_KEYCODE keycode = NONE;
int width = 600;
int height = 600;
int frameCount=0;
int MousePressed = 0;

int _frameRate=30;
int _sizeSet = 0;

float _maxColor1 = 255;
float _maxColor2 = 255;
float _maxColor3 = 255;
float _maxColor4 = 255;

float _strokeCol1=1, _strokeCol2=0, _strokeCol3=0, _strokeCol4=1;
float _fillCol1=1, _fillCol2=1, _fillCol3=1, _fillCol4 = 1; 
float _bgInit1 = 255, _bgInit2 = 255, _bgInit3= 255, _bgInit4 = 255;

int _started = 0;
int _fill = 1;
int _stroke = 1;
int _texturing = 0;

int _firstVertex_type, _firstVertex_called, _vertTotal=0;
float *_vertex_x, *_vertex_y, *_vertex_z;
float *_uv_x, *_uv_y;//## for texture u,v
_SHAPE_MODE _shapeMode;
_ALIGN_MODE _alignMode = CORNER;

int _strokeWeight = 1;
int _textSize = 20;
_cImage* _gTextures = NULL;

#define VERT_SZ 1000000

_COLOR_MODE _colorMode = RGB;

// audio related
FMOD_SYSTEM		*_audioSystem;

CAudio loadAudio(char *fileName)
{
    // play it using FMOD library
    FMOD_SOUND		*cSound = NULL;
    FMOD_System_CreateSound(_audioSystem, fileName, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESAMPLE, 0, &cSound);
    if (cSound ==  NULL) printf("Can't find the audio file %s. \n", fileName);
    return cSound;
}

void playAudio( CAudio in, float volume)
{
    FMOD_CHANNEL	*soundChannel = 0;
	FMOD_System_Update(_audioSystem);
	FMOD_System_PlaySound(_audioSystem, FMOD_CHANNEL_FREE, in, 1, &soundChannel);
	FMOD_Channel_SetVolume(soundChannel, volume);
	FMOD_Channel_SetPaused(soundChannel, false);
}

int millis()
{
#ifdef __APPLE__
    struct timeval tv; 
    gettimeofday(&tv, NULL);
	return (tv.tv_sec*1000000 + tv.tv_usec)/1000;
#else
	return GetTickCount();
#endif
}

#ifdef __APPLE__
float min( float a, float b)
{
    return a<b? a: b;
}

float max( float a, float b)
{
    return a>b? a: b;
}
#endif

float random(float s, float e)
{
    float r = (float)(rand())/RAND_MAX;
    return s + r * (e-s) ;
}
void beginShape(_SHAPE_MODE mode)
{
    _firstVertex_called = 0;
    _shapeMode = mode;
}

void callVertices() 
{
    for (int i=0;i<_vertTotal;i++) {
        if(_firstVertex_type==2){
			glTexCoord2f(_uv_x[i], _uv_y[i]);//##added
            glVertex2f(_vertex_x[i], _vertex_y[i]);
		}
        else{
			glTexCoord2f(_uv_x[i], _uv_y[i]);//##added
            glVertex3f(_vertex_x[i], _vertex_y[i], _vertex_z[i]);
		}
    }
}

void endShape(_SHAPE_MODE mode)
{
    if (mode == CLOSE) _shapeMode = POLYGON;
    
    switch(_shapeMode) {
        case POLYGON:
            if (_fill) {
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                glColor4f( _fillCol1, _fillCol2, _fillCol3, _fillCol4);
                glBegin(GL_POLYGON);
                callVertices();
                glEnd();
            }
            if (_stroke) {
                glColor4f(_strokeCol1, _strokeCol2, _strokeCol3, _strokeCol4);
                glBegin(GL_LINE_LOOP);
                callVertices();
                glEnd();
            }
            break;
        case LINE_STRIP:
            glBegin(GL_LINE_STRIP);
            if (_stroke) {
                glColor4f(_strokeCol1, _strokeCol2, _strokeCol3, _strokeCol4);
                callVertices();
            }
            glEnd();
            break;
        case LINES:
            glBegin(GL_LINES);
            if (_stroke) {
                glColor4f(_strokeCol1, _strokeCol2, _strokeCol3, _strokeCol4);
                callVertices();
            }
            glEnd();
            break;
        case TRIANGLES:
            if (_fill) {
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                glColor4f( _fillCol1, _fillCol2, _fillCol3, _fillCol4);
                glBegin(GL_TRIANGLES);
                callVertices();
                glEnd();
            }
            if (_stroke) {
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                glColor4f(_strokeCol1, _strokeCol2, _strokeCol3, _strokeCol4);
                glBegin(GL_TRIANGLES);
                callVertices();
                glEnd();
            }
            break;
        case TRIANGLE_STRIP:
            if (_fill) {
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                glColor4f( _fillCol1, _fillCol2, _fillCol3, _fillCol4);
                glBegin(GL_TRIANGLE_STRIP);
                callVertices();
                glEnd();
            }
            if (_stroke) {
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                glColor4f(_strokeCol1, _strokeCol2, _strokeCol3, _strokeCol4);
                glBegin(GL_TRIANGLE_STRIP);
                callVertices();
                glEnd();
            }
            break;
        case TRIANGLE_FAN:
            if (_fill) {
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                glColor4f( _fillCol1, _fillCol2, _fillCol3, _fillCol4);
                glBegin(GL_TRIANGLE_FAN);
                callVertices();
                glEnd();
            }
            if (_stroke) {
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                glColor4f(_strokeCol1, _strokeCol2, _strokeCol3, _strokeCol4);
                glBegin(GL_TRIANGLE_FAN);
                callVertices();
                glEnd();
            }
            break;
        case QUADS:
            if (_fill) {
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                glColor4f( _fillCol1, _fillCol2, _fillCol3, _fillCol4);
                glBegin(GL_QUADS);
                callVertices();
                glEnd();
            }
            if (_stroke) {
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                glColor4f(_strokeCol1, _strokeCol2, _strokeCol3, _strokeCol4);
                glBegin(GL_QUADS);
                callVertices();
                glEnd();
            }
			if(_texturing){ //##
				glBindTexture(GL_TEXTURE_2D, 0);
				_texturing=false;
				_shapeMode=LINE_STRIP;
			}
            break;
        case QUAD_STRIP:
            if (_fill) {
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                glColor4f( _fillCol1, _fillCol2, _fillCol3, _fillCol4);
                glBegin(GL_QUAD_STRIP);
                callVertices();
                glEnd();
            }
            if (_stroke) {
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                glColor4f(_strokeCol1, _strokeCol2, _strokeCol3, _strokeCol4);
                glBegin(GL_QUAD_STRIP);
                callVertices();
                glEnd();
            }
            break;
        case POINTS:
            glColor4f(_strokeCol1, _strokeCol2, _strokeCol3, _strokeCol4);
            glBegin(GL_POINTS);
            callVertices();
            glEnd();
            break;
        default: break;
            
    }
    
    if (mode == CLOSE) {
        if (_firstVertex_type ==2) {
            glVertex2f(_vertex_x[0], _vertex_y[0]);
        } else {
            glVertex3f(_vertex_x[0], _vertex_y[0], _vertex_z[0]);
        }
        glEnd();
        
        if (!_fill) return;
        
        glBegin(GL_POLYGON);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glColor4f( _fillCol1, _fillCol2, _fillCol3, _fillCol4);
        if (_firstVertex_type == 2) {
            for (int i=0;i<_vertTotal;i++)
                glVertex2f(_vertex_x[i], _vertex_y[i]);
        } else {
            for (int i=0;i<_vertTotal;i++)
                glVertex3f(_vertex_x[i], _vertex_y[i],_vertex_z[i]);
        }
            
        glEnd();
        return;
        
    }
    glEnd();
}

void vertex(float x, float y)
{
    if (_firstVertex_called == 0) {
        _firstVertex_called = 1;
        _firstVertex_type = 2;
        _vertTotal = 0;
    }
    
    _vertex_x[ _vertTotal ] = x;
    _vertex_y[ _vertTotal ] = height-y;
    _vertTotal++;
}

void vertex(float x, float y, float z)
{
    static int ind = 0;
    if (_firstVertex_called == 0) {
        _firstVertex_called = 1;
        _firstVertex_type = 3;
        ind = 0;
    }
    
    _vertex_x[ _vertTotal ] = x;
    _vertex_y[ _vertTotal ] = height-y;
    _vertex_z[ _vertTotal ] = z;
    _vertTotal++;
}

/*## u,v added for texture mapping*/
void vertex(float x, float y, float u, float v)
{
    if (_firstVertex_called == 0) {
        _firstVertex_called = 1;
        _firstVertex_type = 2;
        _vertTotal = 0;
    }
    
    _vertex_x[ _vertTotal ] = x;
    _vertex_y[ _vertTotal ] = height-y;

	_uv_x[_vertTotal]=u;
	_uv_y[_vertTotal]=v;

    _vertTotal++;
}

/*## u,v added for texture mapping*/
void vertex(float x, float y, float z, float u, float v)
{
    static int ind = 0;
    if (_firstVertex_called == 0) {
        _firstVertex_called = 1;
        _firstVertex_type = 3;
        ind = 0;
    }
    
    _vertex_x[ _vertTotal ] = x;
    _vertex_y[ _vertTotal ] = height-y;
    _vertex_z[ _vertTotal ] = z;

	_uv_x[_vertTotal]=u;
	_uv_y[_vertTotal]=v;

    _vertTotal++;
}

void HSB2RGB(float *p1, float *p2, float *p3)
{
    *p1 = constrain(*p1,0,360);
    *p2 = constrain(*p2,0,100);
    *p3 = constrain(*p3,0,100);
    
    float hue = *p1/360;
    float sat = *p2/100;
    float val = *p3/100;
    
    float r, g, b;
    
    int i = floor(hue * 6);
    float f = hue * 6 - i;
    float p = val * (1 - sat);
    float q = val * (1 - f * sat);
    float t = val * (1 - (1 - f) * sat);
    
    switch(i % 6){
        case 0: r = val, g = t, b = p; break;
        case 1: r = q, g = val, b = p; break;
        case 2: r = p, g = val, b = t; break;
        case 3: r = p, g = q, b = val; break;
        case 4: r = t, g = p, b = val; break;
        case 5: r = val, g = p, b = q; break;
    }
    
    *p1 = r*255;
    *p2 = g*255;
    *p3 = b*255;
}

void colorMode(_COLOR_MODE mode)
{
    _colorMode = mode;
}

float dist(float x1, float y1, float x2, float y2)
{
    return sqrt( (x2-x1)*(x2-x1) + ( y2-y1)*(y2-y1) );
}

float map(float in, float in_min, float in_max, float out_min, float out_max)
{
	float m = (in - in_min) / (in_max-in_min);
	return (1-m) * out_min + m * out_max;
}

void translate(float x, float y)
{
	glTranslatef(x, -y,0);
}

void rotate(float th)
{
    glTranslatef(0, +height,0);
    glRotatef(-th*180/PI, 0, 0, 1);
    glTranslatef(0, -height,0);
}

void strokeWeight(float in)
{
	glPointSize( in );
	glLineWidth( in );
}

void line(int x1, int y1, int x2, int y2)
{
     glColor4f(_strokeCol1, _strokeCol2, _strokeCol3, _strokeCol4);
     glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	 glBegin(GL_LINES); 
	 glVertex2f(x1,height-y1);
	 glVertex2f(x2,height-y2);
	 glEnd();
}

void point(int x, int y)
{
    glColor4f(_strokeCol1, _strokeCol2, _strokeCol3, _strokeCol4);
    glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
    glBegin(GL_POINTS);
    glVertex2f(x,height-y);
    glEnd();
}

void rect(int x, int y, int w, int h)
{
    if (_fill) {
        glColor4f( _fillCol1, _fillCol2, _fillCol3, _fillCol4);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glRecti(x, height-y, x+w, height-y-h);
    }
    if (_stroke) {
        glColor4f(_strokeCol1, _strokeCol2, _strokeCol3, _strokeCol4);
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glRecti(x, height-y, x+w, height-y-h);
    }
}

float radians(float in)
{
    return in*PI/180;
}

void noFill()
{
    _fill = false;
}

void noStroke()
{
    _stroke = false;
}

void ellipse(int cx, int cy, int w, int h)
{
    if (_fill) {
        glColor4f( _fillCol1, _fillCol2, _fillCol3, _fillCol4);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, height-cy);
        for(int t = 0; t <= 64; t++) {
            float th = TWO_PI*(t/64.f);
            float x = w/2*cos(th);
            float y = h/2*sin(th);
            glVertex2f(cx + x, height - cy + y);
        }
        glEnd();
    }
    
    if (_stroke) {
        glColor4f(_strokeCol1, _strokeCol2, _strokeCol3, _strokeCol4);
        glBegin(GL_LINE_LOOP);
        for(int t = 0; t < 64; t++) {
            float th = TWO_PI*(t/64.f);
            float x = w/2*cos(th);
            float y = h/2*sin(th);
            glVertex2f(cx + x, height - cy + y);
        }
        glEnd();
    }
}

void stroke(float p1, float p2, float p3, float p4)
{
    if (_colorMode == HSB) {
        HSB2RGB(&p1, &p2, &p3);
    }
    _strokeCol1 = p1/_maxColor1;
    _strokeCol2 = p2/_maxColor2;
    _strokeCol3 = p3/_maxColor3;
    _strokeCol4 = p4/_maxColor4;
    _stroke = true;
}

void stroke(float p1, float p2, float p3)
{
    stroke(p1,p2,p3,_maxColor4);
    _stroke = true;
}

void stroke(float p1) 
{
    if (_colorMode == HSB)
        stroke(0,0,p1,_maxColor4);
    else
        stroke(p1,p1,p1,_maxColor4);
    _stroke = true;
}

void fill(float p1, float p2, float p3, float p4)
{
    if (_colorMode == HSB) {
        HSB2RGB(&p1, &p2, &p3);
    }
    _fillCol1 = p1/_maxColor1;
    _fillCol2 = p2/_maxColor2;
    _fillCol3 = p3/_maxColor3;
    _fillCol4 = p4/_maxColor4;
    _fill = true;
}

void fill(float p1, float p2, float p3)
{
    fill(p1,p2,p3,_maxColor4);
    _fill = true;
}


void fill(float a)
{
    if (_colorMode == HSB)
        fill(0,0,a,_maxColor4);
    else
        fill(a,a,a,_maxColor4);
    _fill = true;
}

void background(float p1, float p2, float p3, float p4)
{
	if (_colorMode == HSB) {
        HSB2RGB(&p1, &p2, &p3);
    }

    if (!_started) {
        _bgInit1 = p1, _bgInit2 = p2, _bgInit3 = p3, _bgInit4 = p4;
        return;
    }
    
	glClearColor(p1/_maxColor1, p2/_maxColor2, p3/_maxColor3, p4/_maxColor4);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void background(float p1, float p2, float p3)
{
	background(p1,p2,p3,_maxColor4);
}

void background(float b)
{
    if (_colorMode == HSB)
        background(0,0,b,_maxColor4);
    else
        background(b,b,b,_maxColor4);
}

void size(int w, int h)
{
	width =w;
	height =h;
	glutInitWindowSize(w, h);
	_sizeSet = true;
}

void size(int w, int h, int mode)
{
	size(w,h);
}

float constrain(float in, float min, float max)
{
    if (in<min) return min;
    else if (in>max) return max;
    else return in;
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) mouseButton = LEFT;
    else if (button == GLUT_RIGHT_BUTTON) mouseButton = RIGHT;
        
	if (state == GLUT_DOWN) { MousePressed = true; mousePressed();}
	else if (state == GLUT_UP) { MousePressed = false; mouseReleased();}
	glutPostRedisplay();
}

void dragging(int x, int y)
{
    mouseX = constrain(x, 0, width);
    mouseY = constrain(y, 0, height);
    
    mouseDragged();
    
	glutPostRedisplay();
}

void motion(int x, int y)
{
    mouseX = constrain(x, 0, width);
    mouseY = constrain(y, 0, height);
    
    mouseMoved();

	glutPostRedisplay();
}


void keyFunc(unsigned char k, int x, int y)
{
	key = k;
	keyPressed();
	glutPostRedisplay();
}

void specialKeyFunc(int k, int x, int y)
{
	switch( k ) {
        case GLUT_KEY_UP: keycode = UP;
            break;
        case GLUT_KEY_DOWN: keycode = DOWN;
            break;
        case GLUT_KEY_LEFT: keycode = LEFT;
            break;
        case GLUT_KEY_RIGHT: keycode = RIGHT;
            break;            
    }
	keyPressed();
	glutPostRedisplay();
}

void frameRate(int r) 
{
	_frameRate = r;
}

void drawSketch()
{
	frameCount++;
	glLoadIdentity();
    if (!_started) {
        _started = true;
        background(_bgInit1, _bgInit2, _bgInit3, _bgInit4);
    }
	draw();
	//glutSwapBuffers(); // for double buffer
    glFlush();
}

void idle()
{
    //  Get the number of milliseconds since glutInit called
    //  (or first call to glutGet(GLUT ELAPSED TIME)).
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
	static int previousTime = 0;
 
    //  Calculate time passed
    int timeInterval = currentTime - previousTime;
 
	if (timeInterval > 1000/_frameRate) {
        //  Set time
        previousTime = currentTime;
		drawSketch();
    }
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	width =w, height =h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,w,0.0,h); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init() {	
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH); 
	glEnable(GL_LINE_SMOOTH);
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_TEXTURE_2D);
    
    _vertex_x = new float[VERT_SZ];
    _vertex_y = new float[VERT_SZ];
    _vertex_z = new float[VERT_SZ];

    // texture related
	_texturing=false;
    _uv_x = new float[VERT_SZ];
    _uv_y = new float[VERT_SZ];
    
    _cImage *img = _gTextures;
    while (img) {
        glGenTextures(1, &img->textureID);
        glBindTexture(GL_TEXTURE_2D, img->textureID);
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        if (img->channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->width, img->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixel);
        else if (img->channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixel);
        
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        
        glBindTexture(GL_TEXTURE_2D, 0);
        img = img->next;
    }
}

void addToTextures(_cImage *img)
{
    img->next = _gTextures;
    _gTextures = img;
}

_cImage* loadImage(const char *filename)
{
	_cImage* img = (_cImage*) malloc(sizeof(_cImage));
	img->pixel=SOIL_load_image(filename,&img->width,&img->height,&img->channels,SOIL_LOAD_AUTO);
    if (img->pixel==NULL) {
        printf("Can't find the image %s.\n", filename);
    } else {
        addToTextures(img);
    }
	return img;
}

void imageMode(_ALIGN_MODE mode) {
    _alignMode = mode;
}

void image(_cImage* img, int a, int b)
{
	int strokeSave = _stroke;
	_stroke = false;
	noStroke();
	beginShape();
    texture(img);
    if (_alignMode == CORNER) {
        vertex(a, b, 0, 0); 
        vertex(a+img->width, b, 1, 0);
        vertex(a+img->width, b+img->height, 1, 1);
        vertex(a, b+img->height, 0, 1);
    } else if (_alignMode == CENTER) {
        vertex(a-img->width/2, b-img->height/2, 0, 0);
        vertex(a+img->width/2, b-img->height/2, 1, 0);
        vertex(a+img->width/2, b+img->height/2, 1, 1);
        vertex(a-img->width/2, b+img->height/2, 0, 1);
    }
    endShape();
	_stroke = strokeSave;
}

void image(_cImage* image, int a, int b, int c, int d)
{
	int strokeSave = _stroke;
	_stroke = false;
	beginShape();
	texture(image);
    if (_alignMode == CORNER) {
        vertex(a, b, 0, 0);
        vertex(a+c, b, 1, 0);
        vertex(a+c, b+d, 1, 1);
        vertex(a, b+d, 0, 1);
    } else if (_alignMode == CENTER) {
        vertex(a-c/2, b-d/2, 0, 0);
        vertex(a+c/2, b-d/2, 1, 0);
        vertex(a+c/2, b+d/2, 1, 1);
        vertex(a-c/2, b+d/2, 0, 1);
    }

	endShape(); 
	_stroke = strokeSave;
}

void texture(_cImage* img)
{
    if (img == NULL || img->pixel == NULL) return;
    
	_shapeMode = QUADS;
	_texturing=true;

    glBindTexture(GL_TEXTURE_2D, img->textureID);
}

void text(const char *text, int x, int y)
{
	int strokeWeightSave = _strokeWeight;
    strokeWeight(2);
    
	float h = 152.38;//by glutStrokeHeight(GLUT_STROKE_ROMAN);
	//float w = glutStrokeLength(GLUT_STROKE_ROMAN,(unsigned char*)"Z"); //width of 'Z'
    float factor = _textSize / h;
    
    glPushMatrix();
	glTranslatef(x,height-y-_textSize,0); //1. position  (should be modified later)
	glScalef(factor, factor, factor); //2. size
    
	glColor4f(_fillCol1,_fillCol2,_fillCol3,_fillCol4); //3. color
	unsigned long len=strlen(text);
	for(int i=0;i<len;i++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
	}
    glPopMatrix();
    
	strokeWeight(strokeWeightSave);
}

void textSize(int size)
{
	_textSize=size;
}

void startCrocessing()
{
	int argc = 1;
	char **argv = new char*[1];
	argv[0] = new char[256];
	strcpy(argv[0], "Crocessing");

	glutInit(&argc, (char**)argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH); // single buffer
    //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //double buffer
    
    // audio setup
    FMOD_System_Create(&_audioSystem);
    FMOD_System_Init(_audioSystem, 100, FMOD_INIT_NORMAL, NULL);
	
    // call user setup function
    setup();
	
    if (!_sizeSet) size(width, height);
	glutCreateWindow((const char*)argv[0]);

	init();

	glutMouseFunc(mouse);
	glutMotionFunc(dragging);
	glutPassiveMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutDisplayFunc(drawSketch);
	glutKeyboardFunc(keyFunc);
    glutSpecialFunc(specialKeyFunc);
	glutIdleFunc(idle);
	glutMainLoop();
}
