---

# CARPARK

CARPARK is a low-cost, easily implementable Smart Parking solution for University of Cape Town campsuses and similar environments.

### Abstract
There is currently a recurring problem on UCT campuses whereby students and staff struggle to find parking daily. In an attempt to provide more information than ever before for people to make better parking decisions, a system has been developed which tracks the availability of parking spots in the parking lots across each campus. The system has been designed with cheap, yet reliable technology in mind, making it exceedingly more costly effective than similar technologies in the field that exist. By doing so, in conjunction with its ease of implementation in any existing infrastructure, it increases its likeliness to be adopted by any system with similar requirements. The testing of the system involved the real-time analysis of a parking lot and the interaction the system had with it. Judging from the tests that have been conducted, the system is only a few modifications away from being completely flawless and accurate as it currently shows promise. As early signs would indicate, the adoption of this system, at its conclusion, will give potential parkers invaluable parking information that they have not had before. The necessity for a system such as this cannot be understated and with CARPARK, there is evidently a revolutionary Smart Parking solution ready to be expanded on and ultimately adopted.

---

# Code Breakdown

### Arduino Code
This code was written in the Arduino IDE in the C language and deployed onto the NodeMCU module at different points. The files are:
 - **demo.ino** - The first demo to display the counting of cars and the interaction with the Ultrasonic sensor.
 - **server_get_count.ino** - This program was used to test that the unit was able to retrieve data from the server and display it on an LCD display.
 - **test_detection_algo.ino** - Once the Artificial Neural Network was created, this program was used to test the effectiveness of the detection algorithm.
 - **final_p5_enter.ino** - This program was the last to be developed and included the final detection algorithm with the Wi-Fi connection to the server.

In order to test these pieces of code, they need to be deployed to the completed unit. 

### Server Code
This code uses the Django Framework and was written predominantly in the Python language, using the Sublime Text Editor.. A server was created for which API points were established for connection.

The most significant files in the server are:
 - models.py - Defines the structure of the database as these are the classes, or models of objects, that are used.
 - urls.py - The URL extensions that may be reached to obtain data
 - views.py - Retrieve the needed information at each URL extension

In order to run the server, dependencies that can be found in the *requirements.txt* file need to be installed.
The server is then started via the command line:

```sh
$ cd '.\Server Code\'
$ python manage.py runserver
```

This will then run the server on the machine at *localhost:8000*.

### Artificial Neural Network Code
The code that is found here pertain to the usage of an Artificial Neural Network. These files were written in the Python language. The values that are obtained for the weights of the ANN are then transferred to the Arduino Code.

The files here are:
 - DrawANN.py - Used to draw an Artificial Neural Network with a given structure.
 An example of running this file to produce an ANN with 4 input nodes, 5 hidden layer nodes and 1 output layer node looks as follows:
  ```sh
  $ cd './Arduino Code/'
  $ python DrawANN.py 4,5,1
  ```
 - 451ANN-Train.py - This program uses a set of training data to train an Artificial Neural Network to produce a set of weights that can be used by the Arduino Code.
 An example of running this file to produce the weights after 10,000 iterations looks as follows:
  ```sh
  $ cd './Arduino Code/'
  $ python 451ANN-Train.py 10000
  ```

---
 
# Links to Software

 - Arduino IDE - <https://www.arduino.cc/en/main/software>
 - Django Framework - <https://www.djangoproject.com/download/>
 - Sublime Text - <https://www.sublimetext.com/3>

---

# Author Information
This code was written by Joshua Benjamin, student number of BNJJOS003, for his Computer Science Honours Project, CARPARK.

---