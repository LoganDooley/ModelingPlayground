#pragma once

class DrawCommand
{
public:
    virtual ~DrawCommand() = default;

    virtual void Execute() = 0;
};
