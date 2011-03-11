#ifndef PDICT_HH
#define PDICT_HH

/**********************************************************************************************/
#include <Penguin.hh>
#include <QtCore/QObject>
/********************************************* TX *********************************************/
/*                                           PDict                                            */
/**********************************************************************************************/

class PDict: public QObject
{
  Q_OBJECT

  public:
    QFile*            m_file;
    u1_t*             m_fileMap;
    int               m_wseed;
    QVector<QString>  m_enWords;
    QVector<QString>  m_ltWords;

    u4_t        m_enTablePtr, m_enTableSz, m_enLibPtr;
    u4_t        m_ltTablePtr, m_ltTableSz, m_ltLibPtr;
    u4_t        m_trTablePtr, m_trTableSz, m_trLibPtr;

  public:
    pnone       PDict(QObject* parent = 0);
    QByteArray  decodeByteArray(u4_t srcOffset, int len, u4_t seed);
    QString     decodeString(u4_t srcOffset, int len, u4_t seed);
    void        init();
    QString     getEnWord(int index);
    int         getEnWordCount();
    QString     getLtWord(int index);
    int         getLtWordCount();
    int         findEnWord(QString word);
    QList<int>  mapToEn(int entry);
    const char* pointer(int offset) { return (const char*) &m_fileMap[offset]; }
    bool        readDict(QString fileName);
    int         wrand();
    u4_t        read4(u4_t ptr) { return *((u4_t*) &m_fileMap[ptr]); }
    QString     transcription(int id);
    QString     translation(int id);
};

/**********************************************************************************************/

#endif // PDICT_HH
