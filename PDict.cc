#include <PDict.hh>
#include <QtCore/QFile>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

/********************************************* TX *********************************************/
/*                                           PDict                                            */
/**********************************************************************************************/

pnone PDict :: PDict(QObject* parent):
  QObject(parent),
  m_enTableSz(0),
  m_ltTableSz(0),
  m_trTableSz(0)
{
  m_file = new QFile();
}

/**********************************************************************************************/

QByteArray PDict :: decodeByteArray(u4_t srcOffset, int len, u4_t seed)
{
  QByteArray dst(pointer(srcOffset), len);

  u1_t* dst0 = (u1_t*) dst.data();
  u1_t* dst1 = dst0 + dst.size();

  m_wseed = seed;
  for (; dst0 < dst1; dst0++)
    *dst0 = (*dst0 ^ wrand()) & 0xFF;

  return dst;
}

/**********************************************************************************************/

QString PDict :: decodeString(u4_t srcOffset, int len, u4_t seed)
{
  QByteArray  dst(pointer(srcOffset), len);

  u1_t* dst0 = (u1_t*) dst.data();
  u1_t* dst1 = dst0 + dst.size();

  m_wseed = seed;
  for (; dst0 < dst1; dst0++)
    *dst0 = (*dst0 ^ wrand()) & 0xFF;

  QTextStream stream(dst);

  stream.setCodec("penguin-5");
  return stream.readAll();
}

/**********************************************************************************************/

void PDict :: init()
{
  m_enTablePtr  = read4(0x08);
  m_enTableSz   = read4(0x0C);
  m_enLibPtr    = m_enTablePtr + m_enTableSz;

  m_ltTablePtr  = 0x241b3b9;
  m_ltTableSz   = read4(0x44);
  m_ltLibPtr    = m_ltTablePtr + m_ltTableSz;

  m_trTablePtr  = 0x2F8991;
  m_trTableSz   = read4(0x3C);
  m_trLibPtr    = m_trTablePtr + m_trTableSz;

  int enLength = getEnWordCount();
  int ltLength = getLtWordCount();

  m_enWords.reserve(enLength);
  m_enWords.reserve(ltLength);

  for (int i = 0; i < enLength; i++)
    m_enWords.append(getEnWord(i));
  for (int i = 0; i < ltLength; i++)
    m_ltWords.append(getLtWord(i));
}

/**********************************************************************************************/

QString PDict :: getEnWord(int index)
{
  u4_t  entry0  = read4(m_enTablePtr + index * 4);
  u4_t  entry1  = read4(m_enTablePtr + index * 4 + 4);
  u4_t  entrySz = (entry1 - entry0 - 4);
  u4_t  seed    = read4(m_enLibPtr + entry0);

  return decodeString(m_enLibPtr + entry0 + 4, entrySz, seed);
}

/**********************************************************************************************/

int PDict :: getEnWordCount()
{
  return (m_enTableSz / 4) - 1;
}

/**********************************************************************************************/

QString PDict :: getLtWord(int index)
{
  u4_t  entry0  = read4(m_ltTablePtr + index * 4);
  u4_t  entry1  = read4(m_ltTablePtr + index * 4 + 4);
  u4_t  entrySz = (entry1 - entry0);

  u4_t  count   = read4(m_ltLibPtr + entry0);
  u4_t  wordPtr = entry0  + (count << 2);
  u4_t  wordSz  = entrySz - (count << 2);

  return decodeString(m_ltLibPtr + wordPtr, wordSz, wordPtr);
}

/**********************************************************************************************/

int PDict :: getLtWordCount()
{
  return (m_ltTableSz / 4) - 1;
}

/**********************************************************************************************/

int PDict :: findEnWord(QString word)
{
  for (int i = 0; i < m_enWords.size(); i++)
    if (m_enWords.at(i).compare(word, Qt::CaseInsensitive) == 0)
      return i;

  return -1;
}

/**********************************************************************************************/

QList<int> PDict :: mapToEn(int index)
{
  u4_t        entry0  = read4(m_ltTablePtr + index * 4);
  u4_t        count   = read4(m_ltLibPtr + entry0);
  QList<int>  list;

  for (int i = 1; i < count; i++)
    list.append(read4(m_ltLibPtr + entry0 + i * 4));

  return list;
}

/**********************************************************************************************/

bool PDict :: readDict(QString fileName)
{
  m_file->setFileName(fileName);

  if (m_file->open(QIODevice::ReadOnly))
  {
    m_fileMap = m_file->map(0, m_file->size());
    init();

    return true;
  }
  return false;
}

/**********************************************************************************************/

int PDict :: wrand()
{
  m_wseed *= 0x343FD;
  m_wseed += 0x269EC3;

  return (m_wseed >> 0x10) & 0x7FFF;
}

/**********************************************************************************************/

QString PDict :: transcription(int id)
{
  if (id >= 0)
  {
    QString trans = translation(id);
    int     off0  = trans.indexOf("<SPAN class=PH>[");
    int     off1;

    if (off0 > 0)
    {
      off0 += 15;
      off1  = trans.indexOf(']', off0) + 1;
      trans = trans.mid(off0, off1 - off0);
      trans.remove(QRegExp("<SPAN.*/SPAN>"));

      return trans;
    }
  }

  return QString();
}

/**********************************************************************************************/

QString PDict :: translation(int index)
{
  u4_t entry0   = read4(m_trTablePtr + index * 4);
  u4_t entry1   = read4(m_trTablePtr + index * 4 + 4);
  u4_t entrySz  = (entry1 - entry0 - 4);

  return decodeString(m_trLibPtr + entry0 + 4, entrySz, index);
}

/**********************************************************************************************/
