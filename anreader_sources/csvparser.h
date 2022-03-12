#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <memory>

class QString;
class QStringList;

enum class CSVState {outQuote, inQuote, mayBeOutQuote};

class CSVParser
{
public:
    CSVParser();

    static std::unique_ptr<QStringList> csvToken(const QString& line, const QString& delimiter);
};

#endif // CSVPARSER_H
