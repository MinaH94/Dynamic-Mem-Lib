# Dynamic Mem Lib
A library for dynamic memory allocation, couldn't find a more generic name!

## Credits
All credits to Phillip Johnston for his amazing explanation at: [Implementing Malloc: First-fit Free List](https://embeddedartistry.com/blog/2017/02/15/implementing-malloc-first-fit-free-list/)

## Target
This is written to be a target-independant library, hence it could be used anywhere! \
But it was mainly written for STM32.

## How to use
Open the file `DynamicMemAlloc_cfg.h` and set the macro `DYNAMIC_MEMORY_POOL_SIZE_BYTES` to the required size of the heap.

The size should be a multiple of `sizeof(void*)`, else it'll be rounded down, for further reading about this requirement/restriction see this: [The Lost Art of Structure Packing: 3. Alignment requirements](http://www.catb.org/esr/structure-packing/)

## TODO
Implement `realloc()`
