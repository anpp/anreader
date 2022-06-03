#include "anames.h"


//----------------------------------------------------------------------------------------------------------------------
const std::vector<QString> &ANames::Names() const
{
    if(m_names->size() == 0)
    {
        for(uint i = 0; i < filled(); ++i)
        {
            QString name = byIndex(i).trimmed();
            if(name != "----") //paralog or neptune feature
                m_names->push_back(name);
        }
    }
    return *m_names;
}
