#pragma once
#include "i_field.h"
#include <iostream>
#include <map>
#include <memory>

namespace 
{
    using namespace std;
    class IISO8583Defs
    {
    public:
        virtual map<int,shared_ptr<IField>> GetISO8583FieldTemplate() = 0;
    };
}