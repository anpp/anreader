#include "anames.h"


//----------------------------------------------------------------------------------------------------------------------
const std::vector<std::unique_ptr<QString>> &ANames::Names() const
{
    if(m_names->size() == 0)
    {
        for(uint i = 0; i < filled(); ++i)
        {
            std::unique_ptr<QString> name = std::make_unique<QString>(byIndex(i).trimmed());            
                m_names->push_back(std::move(name));
        }
    }
    return *m_names;
}
