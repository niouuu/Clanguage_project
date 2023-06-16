Pizza Ordering and Delivery System
This project is a pizza ordering and delivery system implemented using the POSIX threads (pthreads) package. The system allows customers to place electronic orders, which are then prepared, baked, and delivered to the customers. To handle a large number of orders with a limited number of service points, the system incorporates mutual exclusion using mutexes and synchronization using condition variables.

Features
Electronic Order Placement: Customers can place pizza orders electronically.
Order Preparation: The system prepares the orders based on the received requests.
Baking Process: The prepared orders are baked to perfection.
Delivery: The system ensures efficient delivery of the baked orders to the customers.
Mutual Exclusion: Mutexes are used to ensure that only one thread can access a shared resource at a time, preventing conflicts.
Synchronization: Condition variables are employed to synchronize the threads, allowing them to wait or signal each other when necessary.
