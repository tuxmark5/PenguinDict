#include <PDict.hh>
#include <PDictListModel.hh>

/********************************************* TX *********************************************/
/*                                       PDictListModel                                       */
/**********************************************************************************************/

PDictListModel :: PDictListModel(QObject* parent):
  QAbstractListModel(parent),
  m_dict(new PDict(this)),
  m_language(English)
{
}

/**********************************************************************************************/

QVariant PDictListModel :: data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  int id = index.row();

  switch (role)
  {
    case Qt::DisplayRole:
      return getWord(id);

    case Qt::BackgroundRole:
      break;
  }

  return QVariant();
}

/**********************************************************************************************/

QString PDictListModel :: getTranslation(int index)
{
  if (m_language == English)
  {
    return m_dict->translation(index);
  }

  if (m_language == Lithuanian)
  {
    QList<int> map = m_dict->mapToEn(index);
    QString    str;

    for (int i = 0; i < map.length(); i++)
    {
      str += m_dict->translation(map.at(i));
      qDebug() << i << str;
    }

    return str;
  }

  return "<bad language>";
}

/**********************************************************************************************/

QString PDictListModel :: getTranslation(const QModelIndex& index)
{
  return getTranslation(index.row());
}

/**********************************************************************************************/

QString PDictListModel :: getWord(int index) const
{
  switch (m_language)
  {
    case English:
      return m_dict->m_enWords.at(index);

    case Lithuanian:
      return m_dict->m_ltWords.at(index);
  }

  return "<bad language>";
}

/**********************************************************************************************/

bool PDictListModel :: openDict(const QString& fileName)
{
  return m_dict->readDict(fileName);
}

/**********************************************************************************************/

int	PDictListModel :: rowCount(const QModelIndex& parent) const
{
  switch (m_language)
  {
    case English:
      return m_dict->getEnWordCount();

    case Lithuanian:
      return m_dict->getLtWordCount();
  }

  return 0;
}

/**********************************************************************************************/

void PDictListModel :: setEnglish(bool en)
{
  if (en)
    m_language = English;
  else
    m_language = Lithuanian;

  emit dataChanged(index(1), index(rowCount()));
  qDebug() << "EMIT" << rowCount();
}

/**********************************************************************************************/
