#ifndef PMAINWINDOW_HH
#define PMAINWINDOW_HH

/**********************************************************************************************/
#include <Penguin.hh>
#include <QtCore/QModelIndex>
#include <QtGui/QMainWindow>
#include <QtGui/QSortFilterProxyModel>
/********************************************* TX *********************************************/
/*                                        PMainWindow                                         */
/**********************************************************************************************/

namespace Ui
{
  class PMainWindow;
}

/**********************************************************************************************/

class PMainWindow: public QMainWindow
{
  Q_OBJECT

  private:
    PDictListModel*         m_model;
    QString                 m_prefix;
    QSortFilterProxyModel*  m_proxyModel;
    PSearchDock*            m_searchDock;
    QItemSelectionModel*    m_selectionModel;
    QWebView*               m_webView;
    QTextEdit*              m_dstTransEdit;
    QLineEdit*              m_searchBox;
    Ui::PMainWindow*        ui;

  public:
    explicit  PMainWindow(QWidget *parent = 0);
    pnone     ~PMainWindow();
    void      readPrefix(const QString& fileName);

  public slots:
    //void      findEnd();
    void      findNext();
    void      findText();
    void    	setFilter(const QString& filter);
    void    	setFindText(const QString& text);
    void      setLink(const QUrl& url);
    void    	setMode(bool mode);
    void      setSelection(const QItemSelection& sel1, const QItemSelection& sel0);
    void      setWord(int index);
    void    	setWord(const QModelIndex& index);
    void      transcribe();

  protected:
    void      changeEvent(QEvent *e);
};

/**********************************************************************************************/

#endif // PMAINWINDOW_HH
