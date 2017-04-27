# gr-correctiq
Finally, a GNURadio block to remove that IQ DC spike just like some software and drivers do!  Just drop it in your flowgraph after your
radio source and rock on.  No more offset tuning to get rid of that spike, now you can use the whole bandwidth.

Update:

Added 2 new blocks.  One that starts with auto-learning then switches to standard DC offset with the results to avoid any potential filtering.  This block takes frequency and upstream (radio-side) gain as parameters and if they change auto-recalibrates.  The second block is just a manual offset block that can be used for manual tuning.

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
