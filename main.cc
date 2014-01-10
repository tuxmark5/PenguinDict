#include <PDictCodec.hh>
#include <PMainWindow.hh>

#include <QtGui/QFontDatabase>
#include <QtWidgets/QApplication>

/********************************************* TX *********************************************/
/*                                       PDictListModel                                       */
/**********************************************************************************************/

int main(int argc, char *argv[])
{
  QApplication  app(argc, argv);
  PDictCodec*   codec = new PDictCodec();
  PMainWindow   win;

  win.show();

  return app.exec();
}

/**********************************************************************************************/

