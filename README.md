# gr-correctiq
Finally, a GNURadio block to remove that IQ DC spike just like some software and drivers do!  Just drop it in your flowgraph after your
radio source and rock on.  No more offset tuning to get rid of that spike, now you can use the whole bandwidth.


## Building
Build is pretty standard:

mkdir build

cd build

cmake ..

make

make install

ldconfig

## Parameters
None.  Complex in, complex out.
