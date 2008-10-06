/*
 * author: Daniel (dmilith) Dettlaff
 * email: dmilith at drakor.eu
 * released under GPLv2 license
 * © 2oo8
 */

#ifndef _QT4_CONFIG_WINDOW_H
#define _QT4_CONFIG_WINDOW_H

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
#include "ui_ekg2_qt4_config.h"

// comment this out to get code without verbose debug.

/* User Interface namespace is providing main UI inherited from automaticaly generated qt-designer templates */
namespace Ui {

  class Qt4Config : public QWidget, public Ui::ConfigWidget {
    // qt4 ui macro (for actions)
    Q_OBJECT

    public:
      Qt4Config( const QString& title );
     ~Qt4Config();
     
      // init qt4 actions (ui slots and signals)
      void init_actions();
      
    private:
      
    // qt4 action slots
    public slots:
      
    // qt4 action signals
    signals:
      void clicked();
  };

} // of namespace

#endif

