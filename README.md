# Fibonacci Heap
**Fibonacci heap** is a priority queue data structure. It has a better amortized running time than, for example Binary heap, but due to a very high overhaed it is not used in pratice.

## ⚡ Performance
| Operation      | find-min  | delete-min                                | decrease-key                          | insert                                |
| -------------- | --------- | ----------------------------------------- | ------------------------------------- | ------------------------------------- |
| Binary Heap    | Θ(1)      | Θ(log n)                                  | Θ(log n)                              | Θ(log n)                              |
| Fibonacci Heap | Θ(1)      | O(log n) [am.](## "amortized complexity") | Θ(1) [am.](## "amortized complexity") | Θ(1) [am.](## "amortized complexity") |

##
Thank you [SithDev](https://www.youtube.com/watch?v=6JxvKfSV9Ns) for inspiring me to implement a Fibonacci heap!
