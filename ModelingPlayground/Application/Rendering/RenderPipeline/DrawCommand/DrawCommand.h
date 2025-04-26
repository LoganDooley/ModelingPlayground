#pragma once

class DrawCommand
{
public:
    virtual ~DrawCommand() = 0;

    virtual void Execute() = 0;
};
