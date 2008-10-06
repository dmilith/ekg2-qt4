/*
 * author: Daniel (dmilith) Dettlaff
 * email: dmilith at drakor.eu
 * released under GPL2 license
 * © 2oo8
 */

#ifndef _QT4_WINDOW_H
#define _QT4_WINDOW_H

#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <QtGui/QApplication>
#include <QtCore/QLibrary>
#include <QtCore/QTimer>
#include <QtCore/Qt>
#include <QtCore/QFile>
#include <QtGui/QDialog>
#include <QtCore/QTextCodec>
#include <QtGui/QMessageBox>
#include <QtCore/QQueue>
#include <QtGui/QSlider>

#include "qtplugin_config.h"
#include "ui_ekg2_qt4.h"
#include "qt4_config_window.h"

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
		bool is_alive();
      
    private:
	 	Qt4Config *config_window;
      
    // qt4 action slots
    public slots:
	 	void open_config_window();
      
    // qt4 action signals
    signals:
      void clicked();
  };

} // of namespace


#endif

