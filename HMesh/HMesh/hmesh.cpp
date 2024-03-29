#include "hmesh.h"

#include <string.h>
#include <string>
#include <iostream>
#include <stdlib.h>

#include <QDir>
#include <QFileDialog>
#include <QString>
#include <QtGui/QLineEdit>

#include "ply/ply_inc.h"
#include "mc.h"
#include "dialog/mcsimpdialog.h"
#include "dialog/mcdialog.h"

using std::cout;
using std::endl;

HMesh::HMesh(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	_hglwidget = new hGlWidget();
	this->setCentralWidget(_hglwidget);

	_qslim_dialog = new QSlimDialog(this);
	_qslim_dialog->getLineEdit()->setText(tr("-t 500"));
	_psimp_dialog = new QSlimDialog(this);
	_psimp_dialog->getLineEdit()->setText(tr("-t 2000"));

	_file_name = "";
	//F:/bunny/reconstruction/bun_zipper.ply
	//C:/Users/Ardin/Desktop/bunny/reconstruction/bun_zipper.ply
	_file_name = "F:/bunny/reconstruction/bun_zipper.ply";
	_file_ext = "ply";
	_prev_path = QDir::currentPath();

	initMenus();
}

HMesh::~HMesh()
{

}

void HMesh::initMenus() {

	// initialize menus
	QMenuBar *menu_bar = this->menuBar();

	// file menu
	_menu_file = menu_bar->addMenu("Files");
		_action_open = _menu_file->addAction("Open");
		connect(_action_open, SIGNAL(triggered()), this, SLOT(on_open_file()));
	
	// simplification menu
	_menu_simp = menu_bar->addMenu("Simplification");
		_action_qslim = _menu_simp->addAction("qSlim");
		connect(_action_qslim, SIGNAL(triggered()), this, SLOT(on_qslim()));
		_action_psimp = _menu_simp->addAction("PatchingSimp");
		connect(_action_psimp, SIGNAL(triggered()), this, SLOT(on_psimp()));

	// surface extraction menu
	_menu_surf_xtract = menu_bar->addMenu("Surface Xtract");	
		_action_mc = _menu_surf_xtract->addAction("Marching Cubes");
		connect(_action_mc, SIGNAL(triggered()), this, SLOT(on_mc()));
		_action_mc_simp = _menu_surf_xtract->addAction("MC Simplification");
		connect(_action_mc_simp, SIGNAL(triggered()), this, SLOT(on_mcsimp()));

	// render menu
	_menu_render = menu_bar->addMenu("Render");
		_menu_primitive = _menu_render->addMenu("Primitive");
			_primitive_group = new QActionGroup(this);
				_action_wireframe = _menu_primitive->addAction("Wireframe");
				_action_wireframe->setCheckable(true);
				_primitive_group->addAction(_action_wireframe);
				connect(_action_wireframe, SIGNAL(triggered()), this, SLOT(on_wireframe()));

				_action_flat = _menu_primitive->addAction("Flat");
				_action_flat->setCheckable(true);
				_primitive_group->addAction(_action_flat);
				connect(_action_flat, SIGNAL(triggered()), this, SLOT(on_flat()));

				_action_flat_lines = _menu_primitive->addAction("Flat Lines");
				_action_flat_lines->setCheckable(true);
				_primitive_group->addAction(_action_flat_lines);
				connect(_action_flat_lines, SIGNAL(triggered()), this, SLOT(on_flat_lines()));
				_action_flat_lines->setChecked(true);

				_action_smooth = _menu_primitive->addAction("Smooth");
				_action_smooth->setCheckable(true);
				_primitive_group->addAction(_action_smooth);
				connect(_action_smooth, SIGNAL(triggered()), this, SLOT(on_smooth()));
		_menu_color_mode = _menu_render->addMenu("Color Mode");
			_color_group = new QActionGroup(this);
				_action_vert_color = _menu_color_mode->addAction("Vertex Color");
				_action_vert_color->setCheckable(true);
				_color_group->addAction(_action_vert_color);
				connect(_action_vert_color, SIGNAL(triggered()), this, SLOT(on_vert_color()));

				_action_face_color = _menu_color_mode->addAction("Face Color");
				_action_face_color->setCheckable(true);
				_color_group->addAction(_action_face_color);
				connect(_action_face_color, SIGNAL(triggered()), this, SLOT(on_face_color()));
				_action_face_color->setChecked(true);

		_action_draw_tri_index = _menu_render->addAction("Draw Tri Index");
		_action_draw_tri_index->setCheckable(true);
		_action_draw_tri_index->setChecked(false);
		connect(_action_draw_tri_index, SIGNAL(changed()), this, SLOT(on_draw_tri_index()));

		_action_light_on = _menu_render->addAction("Light On");
		_action_light_on->setCheckable(true);
		_action_light_on->setChecked(false);
		connect(_action_light_on, SIGNAL(changed()), this, SLOT(on_light_on_off()));
}

void HMesh::on_open_file()
{
	_file_name = QFileDialog::getOpenFileName(this,
		tr("Open File"), this->_prev_path,
		tr("Mesh Files(*.ply *.tris *._tris);"
		"All Files(*.*)"));
	_prev_path = _file_name;
	_file_ext = _file_name.mid(_file_name.lastIndexOf(".") + 1);

	_hglwidget->openFile(_file_name);

	update();

	return;
}

void HMesh::on_qslim()
{
	return;

	if(_file_name == "")
		return;

	int return_code = _qslim_dialog->exec();
	if(return_code == QDialog::Rejected)
	{
		return;
	}

	std::cout << std::endl;

	QString smf_name = _file_name;
	QString ply_name = _file_name.mid(_file_name.lastIndexOf("/") + 1);
	QString pure_name = ply_name.mid(0, ply_name.lastIndexOf("."));

	// generate the cmd line input and split
	//extern int qslim_entry(int argc, char **argv);
	int decimate_num = 500;
	QString arg = "execname -o " + pure_name + "_simp.sm " + _qslim_dialog->getLineEdit()->text() + " " + smf_name;
	QStringList arg_list = arg.split(" ", QString::SkipEmptyParts);
	char* argv[35];
	for(int i = 0; i < arg_list.count(); i ++)
	{
		argv[i] = new char[strlen(arg_list.at(i).toStdString().c_str()) + 1];
		memcpy(argv[i], arg_list[i].toStdString().c_str(), strlen(arg_list[i].toStdString().c_str()) + 1);
	}
	//slim_cleanup();
	//qslim_entry(arg_list.count(), argv);

	for(int i = 0; i < arg_list.count(); i ++)
	{
		delete argv[i];
	}

	_hglwidget->setDrawQSlim();
}

void HMesh::on_psimp() {

	if(_file_name == "")
		return;

	int return_code = _psimp_dialog->exec();
	if(return_code == QDialog::Rejected) {
		return;
	}

	QString arg = "execname " + _psimp_dialog->getLineEdit()->text() + " " + _file_name;
	QStringList arg_list = arg.split(" ", QString::SkipEmptyParts);
	char* argv[35];
	for(int i = 0; i < arg_list.count(); i ++)
	{
		argv[i] = new char[strlen(arg_list[i].toLocal8Bit().data()) + 1];
		memcpy(argv[i], arg_list[i].toLocal8Bit().data(), strlen(arg_list[i].toLocal8Bit().data()) + 1);
	}
	//extern int psimp_entry(int, char**, bool);
	cout << endl;
	//psimp_entry(arg_list.count(), argv, false);

	QString pure_name = _file_name.mid(_file_name.lastIndexOf("/") + 1);
	pure_name = pure_name.mid(0, pure_name.lastIndexOf("."));
	QString out_name = pure_name + "_patches/" + pure_name + "_psimp_txt.ply";

	_hglwidget->openFile(out_name);
	update();
}

void HMesh::on_mc() {
	//"F:/raw/raw_sets/neghip/neghip.dat"
	//"F:/raw/raw_sets/foot/foot.dat"
	//"F:/raw/raw_sets/head/head.dat"
	//"D:/volsets/CT_128x128x53_char/CT_128x128x53_char.dat"

	QMCDialog mcDialog(this);
	connect(&mcDialog, SIGNAL(mcParams(string, double, int*)), 
			_hglwidget, SLOT(setDrawMC(string, double, int*)));
	mcDialog.exec();

	//_hglwidget->setDrawMC("D:/volsets/CT_128x128x53_char/CT_128x128x53_char.dat", 100.5);
}

void HMesh::on_mcsimp() {
	//"F:/raw/raw_sets/neghip/neghip.dat"
	//"F:/raw/raw_sets/foot/foot.dat"
	//"F:/raw/raw_sets/head/head.dat"
	//"D:/volsets/CT_128x128x53_char/CT_128x128x53_char.dat"

	QMCSimpDialog mcsimpDialog(this);
	connect(&mcsimpDialog, SIGNAL(mcsimpParams(string, double, int*, double, unsigned int)), 
			_hglwidget, SLOT(setDrawMCSimp(string, double, int*, double, unsigned int)));
	int return_code = mcsimpDialog.exec();
	if(return_code == QDialog::Rejected) {
		return;
	}
}