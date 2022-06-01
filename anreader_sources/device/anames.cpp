#include "anames.h"


//----------------------------------------------------------------------------------------------------------------------
const std::vector<QString> &ANames::Names() const
{
    if(m_names->size() == 0)
    {
        for(uint i = 0; i < count(); ++i)
        {
            if(byIndex(i).trimmed() != "----") //paralog or neptune feature
                m_names->push_back(byIndex(i).trimmed());
        }
    }
    return *m_names;
}
