# README

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



