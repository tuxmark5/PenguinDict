#ifndef PSEARCHDOCK_HH
#define PSEARCHDOCK_HH

/**********************************************************************************************/
#include <Penguin.hh>
#include <QtWidgets/QDockWidget>
/********************************************* TX *********************************************/
/*                                        PSearchDock                                         */
/**********************************************************************************************/

namespace Ui
{
  class PSearchDock;
}

/**********************************************************************************************/

class PSearchDock: public QDockWidget
{
  Q_OBJECT

  public:
    Ui::PSearchDock*  ui;

  public:
    explicit    PSearchDock(QAbstractItemModel* model, QWidget* parent = 0);
    pnone       ~PSearchDock();
    QLineEdit*  getFilterEdit();
    QListView*  getWordsView();

  protected:
    void        changeEvent(QEvent *e);
};

/**********************************************************************************************/

#endif // PSEARCHDOCK_HH
