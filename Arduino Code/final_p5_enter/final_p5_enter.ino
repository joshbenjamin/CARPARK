/*

  This program is the final code that is deployed to the Arduino unit.
  It uses the detection algortithm as well as the Wi-Fi capabilities.
  When a car interacts with this unit, it makes its necessary updates to the server, based on the status and parking lot.

*/

// ************** Included all libraries needed *************** //
#include <NewPingESP8266.h>
#include <math.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
// ************************************************************ //


// ************* Defined all the constant numbers ************* //
#define ULTRASONIC_TRIG_PIN     12   // pin TRIG to D1
#define ULTRASONIC_ECHO_PIN     13   // pin ECHO to D2

#define NUMBER_OF_INPUTS        4
#define NUMBER_OF_HIDDEN        5
#define NUMBER_OF_OUTPUT        1

#define ACCEPTANCE_VALUE        0.8
// ************************************************************ //


// ***** All the constants required for Wi-Fi connection ****** //
const char *ssid = "GetYourOwnWiFi";  
const char *password = "dataaintcheap";
// ************************************************************ //


// ******** Constants related to specific parking lot ********* //
const String URL = "http://uctcarpark.herokuapp.com/api/Parkings/";

// These will be predefined, based on where the unit is placed, at which parking lot and what status
const String postStatus = "ENTER";
const String postParking = "P6";

// This then becomes the post data that is sent to the server
const String postData = "status=" + postStatus + "&code=" + postParking;
// ************************************************************ //


// **************** Weights for the ANN *********************** //
const float weights1[NUMBER_OF_INPUTS][NUMBER_OF_HIDDEN] = {{3.047807891045997,-4.166844731655434,4.056922526966152,2.9766438944893094,-0.7823756995034258},
      {3.423860564910007,-6.290039506729565,-1.555990907011296,-1.154927857485252,0.9856876254458832},
      {0.22023579525386586,-1.8513661719121164,-5.867928992902801,-4.607212517035493,2.3689496302435753},
      {-2.001695438582874,4.5740839806367255,-4.08229114112469,-3.1527437311495574,1.206871937397001}};

const float weights2[NUMBER_OF_HIDDEN] = {-5.88593656622018,
      11.654997713149923,
      10.035937809335099,
      6.938568069584963,
      -3.6576900648344797};
// ************************************************************ //

// Fake values that are values that would register as no car present
float inputs[NUMBER_OF_INPUTS] = {2242, 2242, 2242, 2241};
float outputANN;
long distance, duration;

boolean carPresent = false;
boolean tempCarPresent = carPresent;

NewPingESP8266 ultrasonic(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN);

void setup() {

  // Defines the functionality of the Ultrasonic sensor
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);

  // Begins Wi-Fi connection to a central hotspot. The credentals as above are supplied to connect.
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful and shows the IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 
  
  // Scales the random values by the maximum observed value 
  for(int i=0; i<NUMBER_OF_INPUTS; i++){
      inputs[i] = inputs[i]/2261.0;
  }
  
}

void loop() {

  // The next block of code lets the Ultrasonic sensor take a reading
  // ********************************************
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  distance = (duration/2) / 29.1;
  //Serial.print("Current dist: ");
  //Serial.println(distance);
  // ********************************************

  // Updates the values of the detection, taking the distance that was just measured
  changeValues(inputs, distance);

  // The detection algorithm is then run
  outputANN = detection(inputs);

  // Temporarily assign the old car presence to a variable
  tempCarPresent = carPresent;
  
  // Determines whether there is a car present based on whether the output from the ANN is greater or lower than the acceptance value
  if(outputANN > ACCEPTANCE_VALUE){
    carPresent = true;
  }
  else{
    carPresent = false;
  }

  // A block of code that prevents the server from updating at every stage but rather only when an object is detected as present
    // but is then no longer present
  if(!tempCarPresent && carPresent){
    
    // If this all goes through, we make the HTTP POST
    HTTPPost();
    // Print that there is a car present
    PrintThere(inputs);
  }

  // This entire loop is run every 300 miliseconds
  delay(300);
}

// A function which is able to make an HTTP POST Request to the server
void HTTPPost(){

  HTTPClient http;

  // Begins the HTTP connection via the supplied and predetermined URL
  http.begin(URL);
  // Determines that the content type will be as follows
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Gets the code from the server and makes the POST
  int returnCode = http.POST(postData);
  Serial.print("Code: ");
  Serial.println(returnCode);

  // Ends the HTTP connections
  http.end();
}

// A detection algorithm that is used
float detection(float *values){
  
  // The nect block of code takes the 4 latest values observed and passes them to do a feedforward to establish the output value
  // *****************************************
  float hidden[NUMBER_OF_HIDDEN];
  for (int i=0; i<NUMBER_OF_INPUTS; i++)
  {
    for (int j=0; j<NUMBER_OF_HIDDEN; j++)
    {
      hidden[j] += values[i] * weights1[i][j];
    }      
  }

  float outtie = 0;
  for (int o=0; o<NUMBER_OF_HIDDEN; o++)
  {
    hidden[o] = sigmoid(hidden[o]);
    outtie += hidden[o] * weights2[o];     
  }

  outtie = sigmoid(outtie);
  // *****************************************

  // Returns the calculated value to the main loop above
  return outtie;
}

// A mathematical function, called the Sigmoid function. Used to limit the output to between -1 and 1.
float sigmoid(float value){
  return 1/(1+exp(-value));
}

// A function which updates the values of the 4 latest sensor readings.
// The new reading becomes the 4th element in the array, and is scaled by the maximum observed value of 2261.
void changeValues(float *inputs, float newInput){
  inputs[0] = inputs[1];
  inputs[1] = inputs[2];
  inputs[2] = inputs[3];
  inputs[3] = newInput/2261.0;
  Serial.print(";");
}

// A function which prints to the Serial that there is a car present, with the distance at which it is detected
void PrintThere(float *vals){
  Serial.println();
  Serial.println("******************");
  for(int i=0; i<NUMBER_OF_INPUTS; i++){
    Serial.print("Distance ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(vals[i]*2261);  
  }
  Serial.println();
  Serial.println("----- There is a car there -----");
  Serial.println();
  Serial.println("******************");
}
