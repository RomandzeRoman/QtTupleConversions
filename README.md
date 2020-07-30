# QtTupleConversions
Qt module for conversion structs to std::tuple and vice versa

Module contains template fuctions, that allow you to convert
any plain data struct to std::tuple and std::tuple to struct

## Example
Make tuple from struct:
```
#include "tupleconversions.h"

// create needed struct
struct Data {
    int a;
};
Data d{1};
auto t = TupleConversions::makeTuple(d); // t is std::tuple<int>(1)
```
Make struct from tuple:
```
#include "structconversions.h"

// create needed struct
struct Data {
    int a;
};

// make struct from tuple
auto t = std::make_tuple(2);
auto d = StructConversions::makeFromTuple<Data>(t); // d is Data{2}     
```

## Usage
Clone in project root folder   
### Include in project
To include in project add code below in .pro file:
```
include(QtTupleConversions/QtTupleConversions.pri)
```