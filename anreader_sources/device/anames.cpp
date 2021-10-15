#include "anames.h"


//----------------------------------------------------------------------------------------------------------------------
const std::vector<QString> &ANames::Names() const
{
    if(m_names->size() == 0)
    {
        for(uint i = 0; i < count(); ++i)
            m_names->push_back(byIndex(i));
    }
    return *m_names;
}
