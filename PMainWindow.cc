#include <PDict.hh>
#include <PDictListModel.hh>
#include <PMainWindow.hh>
#include <PSearchDock.hh>
#include <PTextEdit.hh>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtWebKit/QWebView>
#include "ui_PMainWindow.h"
#include "ui_PSearchDock.h"

/********************************************* TX *********************************************/
/*                                        PMainWindow                                         */
/**********************************************************************************************/

PMainWindow :: PMainWindow(QWidget* parent):
  QMainWindow(parent),
  ui(new Ui::PMainWindow)
{
  QAction*      action;
  QVBoxLayout*  layout;

  ui->setupUi(this);

  m_dstTransEdit = new PTextEdit(this);
  ui->gridLayout->addWidget(m_dstTransEdit, 1, 0, 1, 2);

  m_model = new PDictListModel(this);
  m_model->openDict("Anglonas.dic");

  readPrefix("prefix.html");

  m_proxyModel = new QSortFilterProxyModel(this);
  m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  m_proxyModel->setSourceModel(m_model);

  m_searchDock = new PSearchDock(m_proxyModel, this);
  addDockWidget(Qt::LeftDockWidgetArea, m_searchDock);

  m_searchBox = new QLineEdit(this);
  m_searchBox->setMinimumHeight(25);
  m_searchBox->setVisible(false);

  m_webView = new QWebView(this);

  layout = new QVBoxLayout(this);

  ui->dictTab->setLayout(layout);
  layout->addWidget(m_webView);
  layout->addWidget(m_searchBox);

  action = m_webView->pageAction(QWebPage::Copy);
  action->setShortcut(QKeySequence("Ctrl+c"));
  ui->menuEdit->addAction(action);

  QLineEdit::connect(m_searchBox, SIGNAL(textChanged(QString)), this, SLOT(setFindText(QString)));
  QLineEdit::connect(m_searchBox, SIGNAL(returnPressed()), this, SLOT(findNext()));
  //QLineEdit::connect(m_searchBox, SIGNAL(), this, SLOT(findNext()));

  QLineEdit::connect(m_searchDock->getFilterEdit(), SIGNAL(textChanged(QString)), this, SLOT(setFilter(QString)));
  //QListView::connect(m_searchDock->getWordsView(), SIGNAL(activated(QModelIndex)), this, SLOT(setWord(QModelIndex)));
  QPushButton::connect(m_searchDock->ui->modeButton, SIGNAL(toggled(bool)), this, SLOT(setMode(bool)));
  QWebView::connect(m_webView, SIGNAL(linkClicked(QUrl)), this, SLOT(setLink(QUrl)));

  m_selectionModel = m_searchDock->getWordsView()->selectionModel();
  QItemSelectionModel::connect(m_selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                               this, SLOT(setSelection(QItemSelection,QItemSelection)));
  QPushButton::connect(ui->transButton, SIGNAL(clicked()), this, SLOT(transcribe()));
  QAction::connect(ui->actionFind, SIGNAL(triggered()), this, SLOT(findText()));

  setMode(true);
}

/**********************************************************************************************/

PMainWindow :: ~PMainWindow()
{
  delete ui;
}

/**********************************************************************************************/

void PMainWindow :: changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);

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

/*void PMainWindow :: findEnd()
{
  m_searchBox->setVisible(false);
}*/

/**********************************************************************************************/

void PMainWindow :: findNext()
{
  m_webView->findText(m_searchBox->text(), QWebPage::FindWrapsAroundDocument);
  m_webView->findText(m_searchBox->text(), QWebPage::HighlightAllOccurrences);
}

/**********************************************************************************************/

void PMainWindow :: findText()
{
  m_searchBox->setVisible(true);
  m_searchBox->setFocus(Qt::ShortcutFocusReason);
}

/**********************************************************************************************/

void PMainWindow :: readPrefix(const QString& fileName)
{
  QFile file(fileName);

  if (file.open(QIODevice::ReadOnly))
  {
    QTextStream stream(&file);

    m_prefix = stream.readAll();
  }
}

/**********************************************************************************************/

void PMainWindow :: setFilter(const QString& filter)
{
  if (filter.isEmpty())
  {
    m_proxyModel->setFilterFixedString("");
  }
  else
  {
    QModelIndex index0 = m_proxyModel->index(0, 0);

    m_proxyModel->setFilterRegExp("^" + filter);
    m_selectionModel->select(QItemSelection(index0, index0), QItemSelectionModel::ClearAndSelect);

  }
}

/**********************************************************************************************/

void PMainWindow ::	setFindText(const QString& text)
{
  m_webView->findText(QString(), QWebPage::HighlightAllOccurrences);
  m_webView->findText(text, QWebPage::HighlightAllOccurrences);
}

/**********************************************************************************************/

void PMainWindow :: setLink(const QUrl& url)
{
  QString word0 = url.toString();

  for (int i = 0; i < m_model->rowCount(); i++)
  {
    QString word = m_model->getWord(i);

    if (word == word0)
    {
      setWord(i);
      break;
    }
  }
}

/**********************************************************************************************/

void PMainWindow :: setMode(bool mode)
{
  if (mode)
    m_searchDock->ui->modeButton->setText("EN -> LT");
  else
    m_searchDock->ui->modeButton->setText("LT -> EN");

  m_model->setEnglish(mode);
  m_proxyModel->setFilterFixedString("");
  m_proxyModel->invalidate();
}

/**********************************************************************************************/

void PMainWindow :: setSelection(const QItemSelection& sel1, const QItemSelection& sel0)
{
  QModelIndexList indexes = sel1.indexes();

  if (indexes.size() == 1)
    setWord(indexes.at(0));
}

/**********************************************************************************************/

void PMainWindow :: setWord(int index)
{
  QString translation = m_prefix;

  translation += m_model->getTranslation(index);
  translation += "</BODY></HTML>";

  m_webView->setHtml(translation);
  m_webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
}

/**********************************************************************************************/

void PMainWindow :: setWord(const QModelIndex& index)
{
  QModelIndex index1 = m_proxyModel->mapToSource(index);

  setWord(index1.row());
}

/**********************************************************************************************/

void PMainWindow :: transcribe()
{
  QString     src   = ui->srcTransEdit->document()->toPlainText();
  QStringList words = src.split(QRegExp("\\W"));
  QString     dst;

  for (int i = 0; i < words.size(); i++)
  {
    QString word    = words.at(i);

    if (word.isEmpty())
      continue;

    int     wordId  = m_model->m_dict->findEnWord(word);
    QString trans   = m_model->m_dict->transcription(wordId);

    if (!trans.isNull())
    {
      dst += m_model->m_dict->transcription(wordId);
      dst += " ";
    }
    else
    {
      dst += QString("<?%1?> ").arg(word);
    }
  }

  m_dstTransEdit->document()->setPlainText(dst);
}

/**********************************************************************************************/
