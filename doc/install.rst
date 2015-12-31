Installation
------------

.. contents::
   :depth:  4

qmsgpack is a pure Qt library (Qt4 and Qt5 supported), so you can build it for almost any platform Qt supports.
There are two build methods:

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

.. cmdoption:: -DWITH_GUI_TYPES=True

    Build with support for QtGui types (QColor)

.. cmdoption:: -DWITH_LOCATION_TYPES=True

    Build with support for QtLocation types(QGeoCoordinate). Might not work, because CMake seems to be failing to find QtLocation, in this case you can try qmake instead.

Add options before ``..`` as follow:

.. code-block:: bash

    cmake -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_TESTS=True ..

Custom Qt installation
""""""""""""""""""""""

If you installed Qt with online installer, cmake will most likely not find it, in this case try adding following lines to CMakeLists.txt:

.. code-block:: cmake

    set(Qt5Core_DIR "/opt/Qt5.6.0/5.6/gcc_64/lib/cmake/Qt5Core")
    set(Qt5Test_DIR "/opt/Qt5.6.0/5.6/gcc_64/lib/cmake/Qt5Test")
    set(Qt5_DIR "/opt/Qt5.6.0/5.6/gcc_64/lib/cmake/Qt5Core")
    set(QT_QMAKE_EXECUTABLE "/opt/Qt5.6.0/5.6/gcc_64/bin/qmake")

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

Just add following lines to your .pro file:

.. code-block:: makefile

    LIBS += -lqmsgpack

On Windows you may also set the ``INCLUDEPATH`` variable to appropriate location
