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

ENTRYPOINT [ "/bin/bash" ]