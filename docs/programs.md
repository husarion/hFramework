# Beginnings

When you start using new technology, you always need to setup your work environment first.
Fortunately, RoboCORE has a Web IDE -- you just need to go to [cloud.husarion.com/#ide](https://cloud.husarion.com/#ide) and create a new project.

You can also program RoboCORE without using the Web IDE, entirely on your computer. In this case you need to install the [Husarion SDK](https://wiki.husarion.com/howto:installation).

# Writing programs

--example c++ basic

The `hMain` function is called once RoboCORE starts. Put your own code there.
You need to include hFramework library - `#include <hFramework.h>` does it for you.

{% if port(stm32) %}
> This very simple program blinks LED1 every 500 milliseconds.

--example c++ basic_led

--seeAlso hLEDClass hSystem
{% elif port(linux) %}
> This very simple program outputs some data every 500 milliseconds.

--example c++ basic_time

--seeAlso hSystem
{% endif %}
