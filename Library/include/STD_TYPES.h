#ifndef STD_TYPES_H_
#define STD_TYPES_H_

/* unsigned types */
/**
 * Unsigned integer type of size 8-bits (1 byte)\n
 * Minimum = 0\n
 * Maximum = 255
 */
typedef unsigned char      u8;

/**
 * Unsigned integer type of size 16-bits (2 bytes)\n
 * Minimum = 0\n
 * Maximum = 65,535
 */
typedef unsigned short int u16;

/**
 * Unsigned integer type of size 32-bits (4 bytes)\n
 * Minimum = 0\n
 * Maximum = 4,294,967,295
 */
typedef unsigned long int  u32;

/**
 * Unsigned integer type of size 64-bits (8 bytes)\n
 * Minimum = 0\n
 * Maximum = 18,446,744,073,709,551,615
 */
typedef unsigned long long u64;


/* signed types */
/**
 * Signed integer type of size 8-bits (1 bytes)\n
 * Minimum = -128\n
 * Maximum = 127
 */
typedef signed char        s8;

/**
 * Signed integer type of size 16-bits (2 bytes)\n
 * Minimum = -32,768\n
 * Maximum = 32,767
 */
typedef signed short int   s16;

/**
 * Signed integer type of size 32-bits (4 bytes)\n
 * Minimum = -2,147,483,648\n
 * Maximum = 2,147,483,647
 */
typedef signed long int    s32;

/**
 * Signed integer type of size 64-bits (8 bytes)\n
 * Minimum = -9,223,372,036,854,775,808\n
 * Maximum = 9,223,372,036,854,775,807
 */
typedef signed long long   s64;


/* floating (decimal point) types */
/**
 * Signed decimal-point type of size 32-bits (4 bytes)\n
 * Minimum = 1.175494351 x10^(-38)\n
 * Maximum = 3.402823466 x10^(+38)
 */
typedef float              f32;

/**
 * Signed decimal-point type of size 64-bits (8 bytes)\n
 * Minimum = 2.2250738585072014 x10^(-308)\n
 * Maximum = 1.7976931348623158 x10^(+308)
 */
typedef double             f64;

#endif /* STD_TYPES_H_ */
