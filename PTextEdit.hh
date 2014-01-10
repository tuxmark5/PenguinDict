#ifndef PTEXTEDIT_HH
#define PTEXTEDIT_HH

/**********************************************************************************************/
#include <Penguin.hh>
#include <QtWidgets/QTextEdit>
/********************************************* TX *********************************************/
/*                                         QTextEdit                                          */
/**********************************************************************************************/

class PTextEdit: public QTextEdit
{
  Q_OBJECT

  public:
    pnone       PTextEdit(QWidget* parent = 0);
    QMimeData*  createMimeDataFromSelection() const;
};

/**********************************************************************************************/

#endif // PTEXTEDIT_HH
