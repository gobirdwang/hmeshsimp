/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2012, Institute of CG & CAD, Tsinghua University.
 * All Rights Reserved.
 * 
 * @file    QMain.cpp
 * @brief   QVisualizer entry point.
 * 
 * This file defines the entry point of QVisualizer, which initialize the UI and enter the message loop.
 * 
 * @version 1.0
 * @author  Jackie Pang
 * @e-mail: 15pengyi@gmail.com
 * @date    2012/02/06
 */

#include <QtGui/QApplication>
#include "QVisualizer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QVisualizer w;
    w.show();
    return a.exec();
}