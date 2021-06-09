# Contributing
This file provides some guidelines for contributing to this project.

## Style
* Function names and member variables shall be written in `camelCase`.
* Class names shall be written in `PascalCase`.
* Static variables and constants shall be written in `SNAKE_CAPS`.
* `const` always comes *after* the type.
* `&` is bound to the type on return values and to the variable name otherwise.
* Opening braces are on a new line in function definitions and on the same line on branch conditions and exceptions.
* One-liner blocks of branch conditions shall not be wrapped in braces.

```cpp
Foo& getFoo(Bar const &bar, SpammBox &spamm)
{
    if (bar.hasFoo())
        return bar.getFoo();
    
    try {
        return spamm.getFoo();   
    } catch (SomeException const &) {
        return GLOBAL_FOO;
    }
}
```