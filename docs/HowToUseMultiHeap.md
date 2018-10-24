# How to use(add) Multi-heap

There are two cases to use multi-heap.
1. continuous physical RAM but want to split regions
2. separated physical RAMs but want to use for heap  

To use multi-heap, please find three steps as shown below:
- [Set number of Regions](#1-set-number-of-regions)
- [Set number of Heaps](#2-set-number-of-heaps)
- [Set Region start address, size and heap index](#3-set-region-start-address-size-and-heap-index)

## 1. Set number of Regions

Set **CONFIG_MM_REGIONS** value which shows how many regions are.
```
Memory Management -> Number of memory regions -> change a number over 2
```

## 2. Set number of Heaps

Set **CONFIG_MM_NHEAPS** value which indicates how many heaps are.
```
Memory Management -> Number of heaps -> change a number over 2
```

<U>For a detailed description of **REGION** and **HEAP**, please see the link below.</U>  
[Terms for REGION and HEAP](#example-of-multi-heap-usage)  

## 3. Set Region start address, size and heap index

Set start addresses, **CONFIG_RAM_REGIONx_START** with hexa values,  set sizes, **CONFIG_RAM_REGIONx_SIZE** with decimal values(in bytes) of new heap, and set indexes, **CONFIG_RAM_REGIONx_HEAP_INDEX** with decimal values of new heap.  
**CONFIG_RAM_REGIONx_HEAP_INDEX** can be start from 0.
```
Hardware Configuration -> Chip Selection -> List of start address for RAM region -> set values
Hardware Configuration -> Chip Selection -> List of size for RAM region
Hardware Configuration -> Chip Selection -> List of heap index for RAM region
```
Each region is separated by ```','``` as shown below example:
```
0x02000000,0x04000000,0x07000000
100,400,200
0,1,0
```

Based on above configurations, *up_addregion()* function sets new regions automatically.

## Example of Multi-heap Usage
If there are 3 physical RAMs, and want to use them as 4-heaps like below.  
![MultiHeap1](./media/multiheap_1.png)  
In this case, **CONFIG_MM_REGIONS** should be set to 6, and **CONFIG_MM_NHEAPS** should be set to 4.  
![MultiHeap2](./media/multiheap_2.png)
```
CONFIG_MM_REGIONS=6
CONFIG_MM_NHEAPS=4
CONFIG_RAM_REGIONx_START="0x1000,0x1400,0x3000,0x3800,0x7000,0x7100"
CONFIG_RAM_REGIONx_SIZE="1024,512,2048,512,256,4096"
CONFIG_RAM_REGIONx_HEAP_INDEX="0,1,1,2,2,3"
```

