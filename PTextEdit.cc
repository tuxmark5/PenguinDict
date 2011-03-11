#include <PTextEdit.hh>
#include <QtCore/QMimeData>

/********************************************* TX *********************************************/
/*                                         QTextEdit                                          */
/**********************************************************************************************/

PTextEdit :: PTextEdit(QWidget* parent):
  QTextEdit(parent)
{
}

/**********************************************************************************************/

QMimeData* PTextEdit :: createMimeDataFromSelection() const
{
  QMimeData* mimeData = new QMimeData();

  mimeData->setText(textCursor().selectedText());
  return mimeData;
}

/**********************************************************************************************/
