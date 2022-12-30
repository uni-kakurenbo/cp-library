#pragma once

struct uncopyable {
    uncopyable(){}
    uncopyable(const uncopyable&) = delete;
    uncopyable& operator=(const uncopyable&) = delete;
};
