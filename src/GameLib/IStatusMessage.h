#pragma once
#include <string>

class IStatusMessage
{
public:
    virtual ~IStatusMessage() = default;

    virtual void ShowStatusMessage(const std::string& msg) = 0;
};
