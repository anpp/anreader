#include "csvparser.h"
#include <QDebug>

//----------------------------------------------------------------------------------------------------------------------
CSVParser::CSVParser()
{

}

//----------------------------------------------------------------------------------------------------------------------
std::unique_ptr<QStringList> CSVParser::csvToken(const QString &line, const QString& delimiter)
{
    std::unique_ptr<QStringList> tokens = std::make_unique<QStringList>();
    QString token = "";
    QChar quote = '"';
    CSVState state = CSVState::outQuote;
    QChar c;

    auto outQuote { [&] () -> void
        {
            if(c == delimiter)
            {
                *tokens << token;
                token = "";
            }
            else
            {
                if(c == quote)
                    state = CSVState::inQuote;
                else
                    token += c;
            }
        }};

    foreach(c, line){
        switch(state)
        {
        case CSVState::outQuote:
            outQuote();
            break;
        case CSVState::inQuote:
            if(c == quote)
                state = CSVState::mayBeOutQuote;
            else
                token += c;
            break;
        case CSVState::mayBeOutQuote:
            if(c == quote)
            {
                state = CSVState::inQuote;
                token += c;
            }
            else
            {
                state = CSVState::outQuote;
                outQuote();
            }
            break;
        default:
            break;
        }
    }
    *tokens << token;
    return tokens;
}

