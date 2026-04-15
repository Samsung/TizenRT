--------------------------------------------------
## Full C++ Support using LLVM + libc++
--------------------------------------------------
Current libc++ version used in TizenRT:
    libc++ 4.0

When LLVM is enabled:
- It uses basic functionality from lib/libxx as well to provide the LLVM support on tizenrt
- Full STL becomes available
- C++14 and partial C++17 library support available

Some C++17 features are partially supported due to libc++ 4.0 limitations.

C++ feature reference:
https://libcxx.llvm.org/
https://en.cppreference.com/

--------------------------------------------------
## How to Enable C++ with LLVM in TizenRT (CONFIG_LIBCXX)
--------------------------------------------------

Run:
    ./dbuild.sh menuconfig

Enable:

To enable full C++ support:
    Built-in Libraries -->
        LLVM C++ Library (libcxx) -->
                Build LLVM libcxx

## libc++ 4.0 

libc++ 4.0, released in March 2017 before the final C++17 standard ratification, is stable for c++11/14 
but has several limitations in C++17 feature support. 

### Major Missing C++17 Features in libc++ 4.0:

#### 1. **Filesystem Library**
- `std::filesystem` is present but incomplete/unstable

#### 2. **Parallel Algorithms**
- Parallel execution policies were incomplete or missing
- `std::execution` namespace features were not fully available

#### 3. **Charconv**
- std::from_chars/to_chars
- Missing or incomplete 

#### 4. **Type Traits Enhancements**
- Several new type traits like `std::is_invocable` (which we implemented as a polyfill) were missing
- `std::conjunction`, `std::disjunction`, `std::negation` might have been incomplete

#### 5. **STL Container Improvements**
- `insert()` and `erase()` node-handle return values for associative containers
- Node handles (`extract()` functionality) were not implemented

#### 6. **Mathematical Special Functions**
- The new mathematical functions (beta, Legendre polynomials, hermite etc.) were missing

#### 7. **Hardware Interference Size**
- `std::hardware_destructive_interference_size` and `std::hardware_constructive_interference_size` were not available

