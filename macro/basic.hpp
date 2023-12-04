#pragma once

#define TO_STRING_AUX(x) #x
#define TO_STRING(x) TO_STRING_AUX(x)

#define CONCAT_AUX(x, y) x##y
#define CONCAT(x, y) CONCAT_AUX(x, y)

#define UNPAREN_AUX(...) __VA_ARGS__
#define UNPAREN(...) __VA_ARGS__
