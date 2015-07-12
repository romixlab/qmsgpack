Installation
------------

.. contents::
   :depth:  4

qmsgpack is a pure Qt library (Qt4 and Qt5 supported), so you can build it for almost any platform Qt supports.
Also there are several build options:

 - CMake
 - qmake

And two ways of using it: build separately and include to your project, or build with your project (qmake subdirs)

Build
=====

CMake
^^^^^

Get the latest qmsgpack version by grabbing the source code from GitHub:

.. code-block:: bash

   $ git clone https://github.com/romixlab/qmsgpack.git

Now build and install it:

.. code-block:: bash

    cd qmsgpack
    mkdir build && cd build
    cmake ..
    make
    sudo make install

There are several useful cmake options available:

.. cmdoption:: -DBUILD_TESTS=True

    Build all the tests, run with ``make tests``

.. cmdoption:: -DCMAKE_INSTALL_PREFIX=/usr

    Change install location to `/usr`

.. cmdoption:: -DCMAKE_BUILD_TYPE=Debug
    
    Change build type to debug mode (default is `Release`), could be very useful if something goes wrong

Add options before ``..`` as follow:

.. code-block:: bash

    cmake -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_TESTS=True ..

qmake
^^^^^

Get the latest qmsgpack version by grabbing the source code from GitHub:

.. code-block:: bash

   $ git clone https://github.com/romixlab/qmsgpack.git

Now build and install it:

.. code-block:: bash

    cd qmsgpack
    qmake
    make
    sudo make install

Also you can just open ``qmsgpack.pro`` in Qt Creator and build it from there.

Use it
======

Separate build
^^^^^^^^^^^^^^

In case of CMake and qmake build, just add following lines to your .pro file:

.. code-block:: makefile

    LIBS += -lqmsgpack

On Windows you may also set the ``INCLUDEPATH`` variable to appropriate location

qmake subproject
^^^^^^^^^^^^^^^^

If you are working on a bigger Qt project, building qmsgpack with your project automatically can be very neat feature.
Below are the list of variables that allows you to do it:

.. cmdoption:: QMSGPACK_SUBPROJECT
    
    Settings this variable to ``true`` changes build and destination folder

.. cmdoption:: QMSGPACK_BUILD_DIR

    Build directory, default value: ``../../build/3rdparty``

.. cmdoption:: QMSGPACK_BIN_DIR

    Where to put dynamic library file, default value: ``../../bin``

.. tip::
    You can use GitHub submodules to automatically clone qmsgpack repository