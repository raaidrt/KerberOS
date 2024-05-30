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
    && apt-get --quiet --yes install grub-pc-bin qemu-system xorriso \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists

ENTRYPOINT [ "/bin/bash" ]