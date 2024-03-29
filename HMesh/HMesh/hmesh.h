/*
 *  Qt Main Window For HMesh Gui Executable
 *
 *  Author: Ht
 *  Email : waytofall916 at gmail dot com
 *
 *  Copyright (C) Ht. All rights reserved.
 */

#ifndef HMESH_H
#define HMESH_H

#include <QtGui/QMainWindow>
#include "ui_hmesh.h"
#include "hGLWidget.h"
#include "dialog/qslimdialog.h"

// Qt Main Window For HMesh Gui Executable
class HMesh : public QMainWindow
{
	Q_OBJECT

public:
	HMesh(QWidget *parent = 0, Qt::WFlags flags = 0);
	~HMesh();

private:
	void initMenus();

private:
	Ui::HMeshClass ui;

	hGlWidget *_hglwidget;
	QSlimDialog *_qslim_dialog;
	QSlimDialog *_psimp_dialog;

	// variables for open files
	QString _prev_path;
	QString _file_name;
	QString _file_ext;

	// menus
	QMenu* _menu_file;			// file menu
		QAction* _action_open;
	QMenu* _menu_simp;			// simplification menu
		QAction* _action_qslim;
		QAction* _action_psimp;
	QMenu* _menu_surf_xtract;	// surface extraction menu
		QAction* _action_mc;
		QAction* _action_mc_simp;
	QMenu* _menu_render;		// render menu
		QMenu* _menu_primitive;
			QActionGroup* _primitive_group;
				QAction* _action_wireframe;
				QAction* _action_flat_lines;
				QAction* _action_flat;
				QAction* _action_smooth;
		QMenu* _menu_color_mode;
			QActionGroup* _color_group;
				QAction* _action_vert_color;
				QAction* _action_face_color;
		QAction* _action_draw_tri_index;
		QAction* _action_light_on;

public slots:
	void on_open_file();
	void on_qslim();
	void on_psimp();
	void on_mc();
	void on_mcsimp();

	void on_wireframe() { _hglwidget->primitiveMode(WIREFRAME); _hglwidget->update(); }
	void on_flat_lines() { _hglwidget->primitiveMode(FLAT_LINES); _hglwidget->update(); }
	void on_flat() { _hglwidget->primitiveMode(FLAT); _hglwidget->update(); }
	void on_smooth() { _hglwidget->primitiveMode(SMOOTH); _hglwidget->update(); }
	
	void on_vert_color() { _hglwidget->colorMode(VERT_COLOR); _hglwidget->update(); }
	void on_face_color() { _hglwidget->colorMode(FACE_COLOR); _hglwidget->update(); }

	void on_draw_tri_index() { _hglwidget->setDrawTriIndex(); _hglwidget->update(); }
	void on_light_on_off() {  _hglwidget->setLightOnOff(); _hglwidget->update(); }
};

#endif // HMESH_H
