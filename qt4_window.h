/*
 * author: Daniel (dmilith) Dettlaff
 * email: dmilith at drakor.eu
 * released under GPL2 license
 * © 2oo8
 */

#ifndef _QT4_WINDOW_H
#define _QT4_WINDOW_H

#include <QProcess>
#include <QCoreApplication>
#include <QApplication>
#include <QTimer>
#include <Qt>
#include <QFile>
#include <QDialog>
#include <QTextCodec>
#include <QMessageBox>
#include <QQueue>
#include <QSlider>
#include <QKeyEvent>
#include <QAbstractItemView>

#include "ekg2_includes.h"
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
      Qt4Plugin( const QString& title = "Ekg2" );
     ~Qt4Plugin();

	  	// used when window is resized
	  	virtual void resizeEvent( QResizeEvent * event );
		// used when key event
		virtual void keyPressEvent( QKeyEvent * event );

		// init qt4 actions (ui slots and signals)
	  	void set_current_window( int value );
		int get_current_window();
		const char* get_current_window_name();
      void init_actions();
		bool is_alive();
		void auto_resize();
		
		// variables
		QList<QString> command_buffer;
      
    private:
	 	Qt4Config *config_window;
	 	int current_window;
		int current_command;
      
    // qt4 action slots
    public slots:
	 	void open_config_window();
		void enable_debug_window();
		void clear_current_window();
		void qt_entry_command_exec();
		void new_window();
		void kill_window();
		void next_window();
		void previous_window();
//		void qt_entry_command_previous();
      
    // qt4 action signals
    signals:
      void clicked();
		void returnPressed();
		void currentChanged();
  };

} // of namespace


#endif

