# README

Please note: THis language pack is in an early stage of development. It is based on a master thesis and reuses code from there.
However, it also needs to conform to the current protocol version of binary Kieker data transfer. There are some issues with it.

## Before Compiling

After checkout, you may have to perform the following four operations before you can compile the library and the sample programs.

- `aclocal`
- `libtoolize`
- `automake`
- `autoconf`

## Compile

`./configure ; make ; make install`

You ma want to set a different installation location. This can be done when invoking `./configure`. See `./configure --help` for details.

## Examples

Example instrumented programs can be found in `examples`.

- `basic-example` shows a manual instrumentation.



