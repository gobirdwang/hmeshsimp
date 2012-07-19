/*
 *  The patch class in a patching simplification
 *
 *  Author: Ht
 *  Email : waytofall916 at gmail dot com
 *
 *  Copyright (C) Ht-waytofall. All rights reserved.
 */


#ifndef __H_MESH_PATCH__
#define __H_MESH_PATCH__

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>

#include "util_common.h"
#include "trivial.h"

using std::ostream;
using std::ofstream;
using std::ifstream;
using std::fstream;
using std::ostringstream;
using std::list;


/*
 *  interior boundary triangle: the triangle which has some vertices belonging to 
 *	  different patches
 *
 *  interior boundary vertex: the vertex belonging to the patch while adjacent to 
 *    interior boundary triangle
 *
 *  exterior boundary vertex: the vertex not belonging to the patch while adjacent
 *    to some interior boundary triangles which has some vertices belonging to the 
 *    patch
 */

/* ================================ & DEFINITION & ============================= */

#define VERT_ITEM_SIZE sizeof(float)

/* helper data object */
class HIdVertex {
public:
	bool operator < (const HIdVertex &v) const {
		return id < v.id;
	}

	bool operator == (const HIdVertex &v) const {
		return id == v.id;
	}

	void write(ostream &out) {

		WRITE_UINT(out, id);
		WRITE_BLOCK(out, v.x, VERT_ITEM_SIZE);
		WRITE_BLOCK(out, v.y, VERT_ITEM_SIZE);
		WRITE_BLOCK(out, v.z, VERT_ITEM_SIZE);
	}

public:
	uint	id;
	HVertex	v;
};

/* a generic patch class */
class HMeshPatch {
public:
	HMeshPatch() { vert_count = 0; face_count = 0; interior_count = 0; exterior_count = 0; }

	bool openForWrite(const char* vert_name, const char* face_name);
	bool closeForWrite();
	inline bool addInteriorVertex(const uint &orig_id, const HVertex &v);
	inline void addInteriorBound(const uint &orig_id);
	inline void addExteriorBound(const uint &orig_id, const HVertex &v);
	inline bool addFace(const HTripleIndex<uint> &f);

	bool openForRead(const char* vert_name, const char* face_name);
	bool closeForRead();
	inline bool nextInteriorVertex(uint &orig_id, HVertex &v);
	inline bool nextInteriorBound(uint &orig_id);
	inline bool nextExteriorBound(uint &orig_id, HVertex &v);
	inline bool nextFace(HTripleIndex<uint> &f);

	uint interiors() const { return interior_count; }
	uint exteriors() const { return exterior_count; }
	uint verts() const { return vert_count; }
	uint faces() const { return face_count; }

public:
	uint vert_count;
	uint interior_count;
	uint exterior_count;
	uint face_count;

	list<uint> interior_bound;
	list<HIdVertex> exterior_bound;

private:
	ofstream vert_out;
	ofstream face_out;
	ifstream vert_in;
	ifstream face_in;
};

/* interior boundary triangles */
class HIBTriangles {
public:
	HIBTriangles() { face_count = 0; }

	bool openIBTFileForWrite(const char* dir_path);
	inline bool addIBTriangle(const HTripleIndex<uint> &f);
	bool closeIBTFileForWrite();

	bool openIBTFileForRead(const char* dir_path);
	inline bool nextIBTriangle(HTripleIndex<uint> &f);
	bool closeIBTFileForRead();

	uint faceCount() const { return face_count; }

public:
	ofstream ibt_out;
	ifstream ibt_in;
	uint face_count;
};

/* a patch generated by the uniform grid partitioning algorithm */
class HGridPatch: public HMeshPatch {
public:
	/*
	 * grid_index:	the tripled index of the uniform grid
	 *				concerning the patch when using a uniform
	 *				patch partitioning algorithm
	 */
	bool openForWrite(const char* dir_path, const HTripleIndex<uint> grid_index);
	bool openForRead(const char* dir_path, const HTripleIndex<uint> grid_index);

private:
	inline void getVertPatchName(const HTripleIndex<uint> &grid_index, char *buf);
	inline void getFacePatchName(const HTripleIndex<uint> &grid_index, char *buf);

};


/* ================================ & IMPLEMENTATION & ============================= */

/* -- HMeshPatch -- */

bool HMeshPatch::addInteriorVertex(const uint &orig_id, const HVertex &v) {

	WRITE_UINT(vert_out, orig_id);
	WRITE_BLOCK(vert_out, v.x, VERT_ITEM_SIZE);
	WRITE_BLOCK(vert_out, v.y, VERT_ITEM_SIZE);
	WRITE_BLOCK(vert_out, v.z, VERT_ITEM_SIZE);
	vert_count ++;

	if (vert_out.good())
		return true;

	cerr << "#ERROR: writing patch vertex " << orig_id << " failed" << endl;
	return false;
}

void HMeshPatch::addInteriorBound(const uint &orig_id) {

	interior_bound.push_back(orig_id);
}

void HMeshPatch::addExteriorBound(const uint &orig_id, const HVertex &v) {

	HIdVertex idv;
	idv.id = orig_id;
	idv.v = v;
	exterior_bound.push_back(idv);
}

bool HMeshPatch::addFace(const HTripleIndex<uint> &f) {

	WRITE_UINT(face_out, f.i);
	WRITE_UINT(face_out, f.j);
	WRITE_UINT(face_out, f.k);
	face_count ++;

	if (face_out.good())
		return true;
	cerr << "#ERROR: writing patch face <" << f.i << ", " << f.j << ", " << f.k << "> failed" << endl;
	return false;
}

bool HMeshPatch::nextInteriorVertex(uint &orig_id, HVertex &v) {

	READ_UINT(vert_in, orig_id);
	READ_BLOCK(vert_in, v.x, VERT_ITEM_SIZE);
	READ_BLOCK(vert_in, v.y, VERT_ITEM_SIZE);
	READ_BLOCK(vert_in, v.z, VERT_ITEM_SIZE);

	if (vert_in.good())
		return true;

	cerr << "#ERROR: reading patch vertex failed" << endl;
	return false;
}

bool HMeshPatch::nextInteriorBound(uint &orig_id) {

	READ_UINT(vert_in, orig_id);

	if (vert_in.good())
		return true;

	cerr << "#ERROR: reading interior boundary vertex failed" << endl;
	return false;
}

bool HMeshPatch::nextExteriorBound(uint &orig_id, HVertex &v) {

	READ_UINT(vert_in, orig_id);
	READ_BLOCK(vert_in, v.x, VERT_ITEM_SIZE);
	READ_BLOCK(vert_in, v.y, VERT_ITEM_SIZE);
	READ_BLOCK(vert_in, v.z, VERT_ITEM_SIZE);

	if (vert_in.good())
		return true;

	cerr << "#ERROR: reading exterior boundary vertex failed" << endl;
	return false;
}

bool HMeshPatch::nextFace(HTripleIndex<uint> &f) {

	READ_UINT(face_in, f.i);
	READ_UINT(face_in, f.j);
	READ_UINT(face_in, f.k);

	if (face_in.good())
		return true;
	cerr << "#ERROR: reading patch face failed" << endl;
	return false;
}


/* -- HIBTriangles -- */

bool HIBTriangles::addIBTriangle(const HTripleIndex<uint> &f) {
	
	WRITE_UINT(ibt_out, f.i);
	WRITE_UINT(ibt_out, f.j);
	WRITE_UINT(ibt_out, f.k);
	face_count ++;

	if (ibt_out.good())
		return true;
	cerr << "#ERROR: writing interior boundary triangle failed" << endl;
	return false;
}

bool HIBTriangles::nextIBTriangle(HTripleIndex<uint> &f) {

	READ_BLOCK(ibt_in, f.i, VERT_ITEM_SIZE);
	READ_BLOCK(ibt_in, f.i, VERT_ITEM_SIZE);
	READ_BLOCK(ibt_in, f.i, VERT_ITEM_SIZE);

	if (ibt_out.good())
		return true;
	cerr << "#ERROR: writing interior boundary triangle failed" << endl;
	return false;
}

/* -- HGridPatch -- */

void HGridPatch::getVertPatchName(const HTripleIndex<uint> &grid_index, char *buf) {

	ostringstream oss_name;
	oss_name << "vert_" << grid_index.i << "_" << grid_index.j << "_" << grid_index.k;
	stringToCstr(oss_name.str(), buf);
}

void HGridPatch::getFacePatchName(const HTripleIndex<uint> &grid_index, char *buf) {

	ostringstream oss_name;
	oss_name << "face_" << grid_index.i << "_" << grid_index.j << "_" << grid_index.k;
	stringToCstr(oss_name.str(), buf);
}

#endif //__H_MESH_PATCH__