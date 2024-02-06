# Matrix chain multiplication

## Install & Build

        $ git clone https://github.com/XelerT/matrix.git
        $ git checkout lvl2
        $ cd matrix
        $ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build 

## Run
Run lvl1 mode:

        $ ./matrix --lvl1=true

Run lvl2 mode:

        $ ./matrix

Run chain multiplication comparison mode: 

        $ ./matrix --mul=true