# Virtual Memory Management and Page Replacement Algorithms

This project involves designing and implementing a simulated virtual memory management system and several page replacement algorithms in C/C++. Below is a brief overview of the project's components and functionality:

## Overview

### Page Table Structure

The project defines classes for virtual memory, physical memory, page tables, and page table entries to manage memory mapping efficiently.

### Initialization

Upon initialization, physical memory, virtual memory, and a disk file are configured with customizable parameters such as frame size, frame count, and the chosen page replacement algorithm.

### Reading/Writing Values

Functions have been implemented to read and write values from/to physical memory. They automatically handle page replacement when necessary, ensuring efficient memory usage.

### Reading/Writing Frames

Specialized functions are responsible for loading frames from and saving frames to the disk file whenever a page fault occurs.

### Page Replacement Algorithms

The project includes three page replacement algorithms:
- Second Chance
- Least Recently Used (LRU)
- WSClock

These algorithms manage page replacement strategies to optimize memory usage.

### Memory Operations

The project offers various memory operations, including:
- Linear search
- Array summation
- Matrix-vector multiplication
- Binary search

These operations are performed on the virtual memory, showcasing the system's functionality.

### Test Results

Extensive testing has been carried out on different memory operations, comparing the efficiency of the Second Chance and Least Recently Used (LRU) algorithms. Performance results have been collected for various memory sizes and sorting algorithms to evaluate system behavior.