# Embedded-real-time-control-system-for-a-parts-sorting-facility-on-a-factory-assembly-line.
An embedded real-time control system for a parts sorting facility on a factory assembly line was developed. The sorting facility consists of two conveyors that operate concurrently. The computing platform used for the controller is an embedded PC104 single board computer, running the VxWorks real-time operating system. The controller software should be implemented using the WindRiver Workbench software development tools and be written in the C or C++ programming language 

The handling system consists of 2 conveyor belts, each allowing for the concurrent sorting of objects (parts). The system interfaces with the controller using the PC’s parallel port.


   ![Conveyor](https://user-images.githubusercontent.com/46611428/104955277-ddd1b380-59c1-11eb-80b6-cb1dbcd4f227.png)
   
   
   Each conveyor features two sets of sensors and a sorting gate. The first sensor set allows for the detection of the object size. There are two types of parts presented to the conveyor:

•	Small parts can only trigger one of the size sensors at a time.
•	Large parts are long enough to trigger both size sensors simultaneously.

The second sensor set features just one object detection sensor, and is used to count the parts collected at the end of each conveyor for quality control purposes.


