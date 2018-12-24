#pragma once

class Input
{
private:
    Input() = default;

public:
    static Input& get()
    {
        static Input instance;
        return instance;
    }

public:
    void handleInput();

public:
    bool MoveLeft = false;
    bool MoveRight = false;
};
