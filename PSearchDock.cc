#include <PSearchDock.hh>
#include "ui_PSearchDock.h"

/********************************************* TX *********************************************/
/*                                        PSearchDock                                         */
/**********************************************************************************************/

PSearchDock :: PSearchDock(QAbstractItemModel* model, QWidget* parent):
  QDockWidget(parent),
  ui(new Ui::PSearchDock)
{
  ui->setupUi(this);
  ui->searchField->setFocus(Qt::OtherFocusReason);
  ui->wordView->setModel(model);
}

/**********************************************************************************************/

PSearchDock :: ~PSearchDock()
{
  delete ui;
}

/**********************************************************************************************/

void PSearchDock :: changeEvent(QEvent* e)
{
  QDockWidget::changeEvent(e);
  switch (e->type())
  {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;

    default:
      break;
  }
}

/**********************************************************************************************/

QLineEdit* PSearchDock :: getFilterEdit()
{
  return ui->searchField;
}

/**********************************************************************************************/

QListView* PSearchDock :: getWordsView()
{
  return ui->wordView;
}

/**********************************************************************************************/
