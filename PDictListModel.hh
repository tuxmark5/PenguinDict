#ifndef PDICTLISTMODEL_HH
#define PDICTLISTMODEL_HH

/**********************************************************************************************/
#include <Penguin.hh>
#include <QtCore/QAbstractListModel>
#include <QtCore/QFile>
/********************************************* TX *********************************************/
/*                                       PDictListModel                                       */
/**********************************************************************************************/

class PDictListModel: public QAbstractListModel
{
  Q_OBJECT

  enum Lang
  {
    English,
    Lithuanian
  };

  public:
    PDict*    m_dict;
    Lang      m_language;

  public:
    pnone       PDictListModel(QObject* parent = 0);
    QVariant    data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QString     getTranslation(int index);
    QString     getTranslation(const QModelIndex& index);
    QString     getWord(int index) const;
    bool        isEnglish() const { return m_language == English; }
    bool        isLithuanian() const { return m_language == Lithuanian; }
    bool        openDict(const QString& fileName);
    int         rowCount(const QModelIndex& parent = QModelIndex()) const;

  public slots:
    void        setEnglish(bool en);
};

/**********************************************************************************************/

#endif // PDICTLISTMODEL_HH
