FROM joshwyant/gcc-cross

ENV num_processors 8
ENV TARGET i686-elf
ENV PREFIX "/usr/local/cross"

# Install gdb
RUN curl -s https://ftp.gnu.org/gnu/gdb/gdb-14.2.tar.gz --output gdb-14.2.tar.gz > /dev/null \
    && tar xvzf gdb-14.2.tar.gz \
    && mkdir build-gdb \
    && cd build-gdb \
    && ../gdb-14.2/configure --target=${TARGET} --prefix=${PREFIX} --disable-werror\
    && make -j ${num_processors} all-gdb\
    && make install-gdb\
    && cd ..\
    && rm -rf build-gdb \
    && rm -rf gdb-14.2 \
    && rm gdb-14.2.tar.gz

RUN DEBIAN_FRONTEND=noninteractive apt-get --quiet --yes update \
    && apt-get --quiet --yes install grub-pc-bin qemu-system xorriso libvncserver-dev libgcrypt-dev curl libsdl2-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists

RUN mkdir bochs \
    && cd bochs \
    && curl -sL https://sourceforge.net/projects/bochs/files/bochs/2.8/bochs-2.8.tar.gz/download | tar xz --strip-components=1 \
    && ./configure --with-x11 --with-sdl2 --disable-sdl2-opengl \
    && make \
    && make install

ENV LIBGL_ALWAYS_INDIRECT 1

ENTRYPOINT [ "/bin/bash" ]
