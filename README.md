# Embedded-real-time-control-system-for-a-parts-sorting-facility-on-a-factory-assembly-line.
An embedded real-time control system for a parts sorting facility on a factory assembly line was developed. The sorting facility consists of two conveyors that operate concurrently. The computing platform used for the controller is an embedded PC104 single board computer, running the VxWorks real-time operating system. The controller software should be implemented using the WindRiver Workbench software development tools and be written in the C or C++ programming language 

The handling system consists of 2 conveyor belts, each allowing for the concurrent sorting of objects (parts). The system interfaces with the controller using the PC’s parallel port.


   ![Conveyor](https://user-images.githubusercontent.com/46611428/104955277-ddd1b380-59c1-11eb-80b6-cb1dbcd4f227.png)
   
   
   Each conveyor features two sets of sensors and a sorting gate. The first sensor set allows for the detection of the object size. There are two types of parts presented to the conveyor:

•	Small parts can only trigger one of the size sensors at a time.
•	Large parts are long enough to trigger both size sensors simultaneously.

The second sensor set features just one object detection sensor, and is used to count the parts collected at the end of each conveyor for quality control purposes.

Basic Controller Requirements:

The basic requirements for operation of the controller software are as follows:
1.	The controller should be able to detect and distinguish between small and large objects present on each conveyor belt.
2.	When a large object is detected on a conveyor, the respective gate should remain open to allow the large object to pass to the end of the conveyor.
3.	When a small object is detected on a conveyor, the respective gate should close to route the small object off the conveyor.
4.	The sorting gate should be opened once a small object has been routed off to allow for the correct routing of subsequent objects on the conveyor.
5.	The controller should maintain a separate count of the number of small and large objects
detected on each conveyor.
6.	The controller should maintain a count of large objects collected at the end of each conveyor.
7.	The controller should provide a simple text based user interface that allows the user to perform the following actions:

i)	Display the total number of small objects detected for each conveyor.
ii)	Display the total number of large objects detected for each conveyor.
 
iii)	Display the total number of large objects collected for each conveyor.
iv)	Reset the counter values for each conveyor.
v)	Shutdown the controller application and free up any memory used by it.
8.	When shutting down the controller following a user prompt, the sorting gates should be opened. The application should also delete all created data structures (including semaphores, queues, etc.) and OS tasks.

Conveyor timing parameters:

To fulfil successfully the requirements of Section 3, you will need to know the timing parameters of the conveyor unit and the objects used on the conveyor:

Conveyor parameter	Symbol
Minimum duration for which a long object triggers both size detection sensors	T(detect)
Minimum duration for which a long object triggers the count sensor	T(count)
Time it takes for an object to reach the sorting gate from the second size sensor	T(gate)
Minimum duration for which a sorting gate needs to remain closed to route an object
off the conveyor	T(close)
Time required to read size sensors (size sensor reading delay)	D(size)
Time required to read count sensor (count sensor reading delay)	D(count)
Time required to change the gate states (gate control delay)	D(gate)
Time required to reset size sensors (size sensor reset time)	R(size)
Time required to reset count sensor (count sensor reset time)	R(count)


![Conveyor2](https://user-images.githubusercontent.com/46611428/104955624-56d10b00-59c2-11eb-8d52-ca64a5225371.png)


