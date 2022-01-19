#pragma once

#include <iostream>
#include <string>
namespace {
    using namespace std;
    class IFieldValidator
    {
    public:
        virtual bool IsValid(string value) = 0;
    };
}