# README

Please note: This language pack is in an early stage of development.

## AspectC++ AOP

There is initial code using aspectC++ for instrumentation. We have incorporated
that code in the source tree. You may find the aspect definitions under
`libkieker/include/aspectc++`. However, we have not yet tested them.

## Before Compiling

After checkout, you may have to perform the following four operations before you
can compile the library and the sample programs.

- `libtoolize`
- `aclocal`
- `automake --add-missing`
- `autoconf`

## Compile

`./configure ; make ; make install`

You ma want to set a different installation location. This can be done when
invoking `./configure`. See `./configure --help` for details.

Installation in a separate directory can be done with --prefix=/my/directory

## Examples

Example instrumented programs can be found in `examples`.

- `basic-example` shows a manual instrumentation.

## Using the Language Pack

You can instrument C, C++ and Fortran application (or any other language
supported by GCC) with the kieker-lang-pack-c. To enable instrumentation you
have to specify -finstrument-functions as compiler option to instrument all
functions, add libkieker to the library path and add -lkieker to the linker
options.

To limit instrumentation you may use 

-finstrument-functions-exclude-file-list
-finstrument-functions-exclude-function-list

More details can be found here: 
https://gcc.gnu.org/onlinedocs/gcc-4.4.4/gcc/Code-Gen-Options.html

Depending on the configure and make setup used in your C, C++, Fortran project
your setup could look like this:

export LIBS=-lkieker
export LDFLAGS=-L/my/directory/lib/
export CFLAGS=-finstrument-functions

Kieker supports two environment variables:
- KIEKER_HOSTNAME
- KIEKER_PORT
which must be set to a port (default 5678) and a hostname (e.g. localhost)
specifying the computer receiving the monitoring data.

Use the Kieker collector to receive the data.
https://kieker-monitoring.readthedocs.io/en/latest/kieker-tools/Collector---Kieker-Data-Bridge.html

