/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2012, Institute of CG & CAD, Tsinghua University.
 * All Rights Reserved.
 * 
 * @file    QIO.cpp
 * @brief   QIO class declaration.
 * 
 * This file declares the most commonly used methods defined in QIO.h.
 * 
 * @version 1.0
 * @author  Jackie Pang
 * @e-mail: 15pengyi@gmail.com
 * @date    2012/02/07
 */

//#include <gl/glew.h>

#include <iostream>
#include <fstream>

//#include "QUtility.h"
#include "QIO.h"

QIO::QIO()
{}

QIO::~QIO()
{}

bool QIO::getFileContent(std::string fileName, std::string &content)
{
    std::ifstream file(fileName.c_str());
    if (!file)
    {
        std::cerr << " > ERROR: unable to open input file: \"" << fileName << "\"." <<  std::endl;
        return false;
    }
    
    file.seekg(0, std::ios::end);
    int length = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // content.reserve(length);
    // content.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    content.resize(length);
    file.read((char*)content.data(), length);
    file.close();
    
    return true;
}

bool QIO::getFileData(std::string fileName, void *data, unsigned int size)
{
    std::ifstream file(fileName.c_str(), std::ios::binary);
    if (!file)
    {
        std::cerr << " > ERROR: unable to open input file: \"" << fileName << "\"." <<  std::endl;
        return false;
    }
    
    file.read((char *)data, size);
    if (file.gcount() != size)
    {
        std::cerr << " > ERROR: reading data failed." << std::endl;
        return false;
    }
    file.close();

    return true;
}

// read file from the offset, -ht
bool QIO::getFileDataOff(std::string fileName, void *data, unsigned int size, int offset)
{
	std::ifstream file(fileName.c_str(), std::ios::binary);
	if (!file)
	{
		std::cerr << " > ERROR: unable to open input file: \"" << fileName << "\"." <<  std::endl;
		return false;
	}

	file.seekg(offset);

	file.read((char *)data, size);
	if (file.gcount() != size)
	{
		std::cerr << " > ERROR: reading data failed." << std::endl;
		return false;
	}
	file.close();

	return true;
}

bool saveFileData(std::string fileName, void *data, unsigned int size)
{
    std::ofstream file(fileName.c_str(), std::ios::binary);
    if (!file)
    {
        std::cerr << " > ERROR: unable to open output file: \"" << fileName << "\"." <<  std::endl;
        return false;
    }

    file.write((char *)data, size);
    if (file.fail())
    {
        std::cerr << " > ERROR: writing data failed." << std::endl;
        return false;
    }
    file.close();

    return true; 
}