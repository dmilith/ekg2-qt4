/*
 * author: Daniel (dmilith) Dettlaff
 * email: dmilith at drakor.eu
 * released under GPL2 & LGPL license
 * © 2oo8
 */

#ifndef _QT4_WINDOW_H
#define _QT4_WINDOW_H


#include <cstdio>
#include <csignal>
#include <cassert>

#include <qt4/QtCore/QCoreApplication>
#include <qt4/QtGui/QApplication>
#include <qt4/QtCore/QLibrary>
#include <qt4/QtCore/QTimer>
#include <qt4/QtCore/Qt>
#include <qt4/QtCore/QFile>
#include <qt4/QtGui/QDialog>
#include <qt4/QtCore/QTextCodec>
#include <qt4/QtGui/QMessageBox>
#include <qt4/QtCore/QQueue>
#include <qt4/QtGui/QSlider>

#include "ui_ekg2_qt4.h"

const std::string MAIN_WINDOW_TITLE = "EKG2";

/* User Interface namespace is providing main UI inherited from automaticaly generated qt-designer templates */
namespace Ui {
  
  class Qt4Plugin : public QMainWindow, public Ui::MainWindow {
    // qt4 ui macro (for actions)
    Q_OBJECT

    public:
      Qt4Plugin( const QString& title = "dSipCom" );
     ~Qt4Plugin();
     
      // init qt4 actions (ui slots and signals)
      void init_actions();
      
    private:
      
    // qt4 action slots
    public slots:
	 	void quit();
      
    // qt4 action signals
    signals:
      void clicked();
  };

} // of namespace


#endif

