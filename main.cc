#include <PDictCodec.hh>
#include <PMainWindow.hh>

#include <QtGui/QApplication>
#include <QtGui/QFontDatabase>

/********************************************* TX *********************************************/
/*                                       PDictListModel                                       */
/**********************************************************************************************/

int main(int argc, char *argv[])
{
  QApplication  app(argc, argv);
  PDictCodec    codec;
  PMainWindow   win;

  win.show();

  return app.exec();
}

/**********************************************************************************************/

