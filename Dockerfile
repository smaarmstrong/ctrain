# Everything ctrain needs: gcc (with ASan/UBSan), make, gdb, valgrind.
FROM gcc:14
RUN apt-get update && \
    apt-get install -y --no-install-recommends valgrind gdb && \
    rm -rf /var/lib/apt/lists/*
WORKDIR /ctrain
CMD ["bash"]
