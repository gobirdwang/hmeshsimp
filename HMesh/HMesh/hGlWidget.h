/***********************************************
 author : houtao
 spec   : qt opengl widget for displaying meshes
***********************************************/

#ifndef H_GL_WIDGET
#define H_GL_WIDGET

#include <QtGui>
#include <QGLWidget>
#include <QString>
#include "common.h"
#include "tri_soup.h"
#include "common_types.h"

using namespace icesop;

enum PrimitiveMode { SMOOTH, FLAT, FLAT_LINES, WIREFRAME };
enum ColorMode { VERT_COLOR, FACE_COLOR };

class hGlWidget : public QGLWidget
{
	Q_OBJECT

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void setGlState();

	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void wheelEvent(QWheelEvent * event);

public:
	hGlWidget();
	void setDrawQSlim();
	void setDrawPly();
	void setDrawTris();
	void openFile(QString _file_name);
	void computeNormals();
	void calcBoundingBox();

	void primitiveMode(PrimitiveMode m) { _primitive_mode = m; }
	void colorMode(ColorMode m) { _color_mode = m; }

private:
	// variables concerning drawing objects
	bool _draw_qslim;
	bool _draw_ply;
	bool _draw_tris;

	PrimitiveMode _primitive_mode;
	ColorMode _color_mode;

	// info about bounding box
	float _max_x, _min_x;
	float _max_y, _min_y;
	float _max_z, _min_z;
	float _center_x, _center_y, _center_z;
	float _range;

	double _glmat[16];		// translation matrix
	QPoint _lButtonPressPos;
	QPoint _rButtonPressPos;
	double _scale;
	int _operateMode;
	Vector3D _trans_point;
	double _rotate_degree;

	HNormal *vnormals;
	HNormal *fnormals;

	TriangleSoupContainer _tris_container;
};

#endif