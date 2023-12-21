# MemoryAllocators

A minimalistic memory allocator implementing malloc, calloc, realloc, and free.
Unlike other malloc projects that rely solely on sbrk or introduce high complexities, this project balances simplicity and efficiency by utilizing both sbrk and mmap. 

# References

 - https://www.ibm.com/developerworks/jp/linux/library/l-memory/index.html (Japanese)
 - https://arjunsreedharan.org/post/148675821737/write-a-simple-memory-allocator (English)
 - http://www.valinux.co.jp/technologylibrary/document/linux/malloc0001/ (Japanese)

# License

Copyright (c) 2023 s0sasaki

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
