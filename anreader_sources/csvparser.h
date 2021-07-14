#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <QStringList>
#include <memory>

enum class CSVState {outQuote, inQuote, mayBeOutQuote};

class CSVParser
{
public:
    CSVParser();

    static std::unique_ptr<QStringList> csvToken(const QString& line, const QString& delimiter);
};

#endif // CSVPARSER_H
