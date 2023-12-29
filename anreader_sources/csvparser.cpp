#include "csvparser.h"
#include <QStringList>
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

    auto outQuote { [&] (QChar ch) -> void
        {
            if(ch == delimiter)
            {
                *tokens << token;
                token = "";
            }
            else
            {
                if(ch == quote)
                    state = CSVState::inQuote;
                else
                    token += ch;
            }
        }};

    for(const auto& c: line){
        switch(state)
        {
        case CSVState::outQuote:
            outQuote(c);
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
                outQuote(c);
            }
            break;
        default:
            break;
        }
    }
    *tokens << token;
    return tokens;
}

