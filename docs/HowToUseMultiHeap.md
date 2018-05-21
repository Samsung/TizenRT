# How to use(add) Multi-heap Regions

There are two cases to use multi-heap regions.  
1. continuous physical RAM but want to split regions  
2. separated physical RAMs but want to use for heap  

To use multi-heap regions, find below two steps.

## Set number of Heap regions

Set **CONFIG_MM_REGIONS** value which shows how many regions are.
```
Memory Management -> Number of memory regions -> change a number over 2
```
When CONFIG_MM_REGIONS has over 2, RAMx_START and RAMx_SIZE configs come out.

## Set RAM start address and size

Set start addresses, **CONFIG_RAMx_START** with hexa values and sizes, **CONFIG_RAMx_SIZE** with decimal values of new heap regions.
```
Hardware Configuration -> Chip Selection -> List of start address for additional heap region -> set values
Hardware Configuration -> Chip Selection -> List of size for additional heap region -> set values
```
Each region is separated by ```','``` as shown below example:
```
0x02000000,0x4000000
100,200
```

Based on above configurations, *up_addregion* function sets new regions automatically.
