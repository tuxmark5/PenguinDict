#ifndef PDICTCODEC_HH
#define PDICTCODEC_HH

/**********************************************************************************************/
#include <Penguin.hh>
#include <QtCore/QTextCodec>
/********************************************* TX *********************************************/
/*                                         PDictCodec                                         */
/**********************************************************************************************/

class PDictCodec: public QTextCodec
{
  public:
    QList<QByteArray> m_aliases;
    QChar             m_unicodeTable[256];

  public:
    pnone             PDictCodec();
    QList<QByteArray> aliases() const;
    QByteArray        convertFromUnicode(const QChar* input, int number, ConverterState* state) const;
    QString           convertToUnicode(const char* chars, int len, ConverterState* state) const;
    int               mibEnum() const;
    QByteArray        name() const;
};

/**********************************************************************************************/

#endif // PDICTCODEC_HH
