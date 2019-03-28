# Accessing Abseil SwissTables from C

This is a tiny wrapper library allowing C code to use SwissTables, Google's
state-of-the-art hash table implementation.

There is no pure C implementation of SwissTables, so it can't be used without a
wrapper library. I think the majority of C programmers use POSIX `hcreate(3)`
when they need a hash table, which is portable across UNIX systems.

I had a question, are SwissTables so good that I should start using them in C
code instead of `hcreate()` or even GLib's `GHashTable`?

Here is a quick test to find out.

# Building

```bash
$ git clone https://github.com/abseil/abseil-cpp.git
$ cmake .
$ make
```

Now you have `libswisstable.a` and `swisstable.h` that you can use from C,
remember to link against `libstdc++` and `libm`, and use `-pthread`

```bash
$ gcc -pthread yourcode.c libswisstable.a -lstdc++ -lm
```

To compile the benchmark use:

```bash
$ gcc -pthread -o benchmark benchmark.c libswisstable.a $(pkg-config --cflags --libs glib-2.0) -lc -lm -lstdc++
```
