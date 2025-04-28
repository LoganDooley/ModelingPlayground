#pragma once

class DrawCommand
{
public:
    virtual ~DrawCommand() = default;

    virtual void Execute() const = 0;
};
