hPyFramework
==========

micropython connector to hFramework

Building
==========

Requirements
----------

Make sure you have mpy-gen submodule fetched.

Download and extract hPython repository.

Build hPython.

Building
----------

Build with:
```
cmake -DROBOCORE_VERSION=0.9.7 -HPYTHON_PATH=<PATH_TO_HPYTHON> .
make
```

Usage
----------

Add the following lines to your CMakeLists.txt file:

```
add_component_lib(hPython "<PATH_TO_HPYTHON>")
add_component_lib(hPyFramework "<PATH_TO_HFRAMEWORK>")
```

In code, use the following statements to register hPyFramework:

before `int main()` insert:
```
void register_hPyFramework();
```

after `hPython::init()` insert:
```
register_hPyFramework();
```
