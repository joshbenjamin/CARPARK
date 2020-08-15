/*

    This program aims to test the Detection Algorithm with the correct weights.
    It displays whethere a car is deteced or not.

 */

#include <Arduino.h>
#include <math.h>

// utlrasonic pinout
#define ULTRASONIC_TRIG_PIN     12   // pin TRIG to D1
#define ULTRASONIC_ECHO_PIN     13   // pin ECHO to D2

#define NUMBER_OF_INPUTS        4
#define NUMBER_OF_HIDDEN        5
#define NUMBER_OF_OUTPUT        1


// The predefinded weights from the input layer to the hidden layer
const float w1[NUMBER_OF_INPUTS][NUMBER_OF_HIDDEN] = {{3.047807891045997,-4.166844731655434,4.056922526966152,2.9766438944893094,-0.7823756995034258},
      {3.423860564910007,-6.290039506729565,-1.555990907011296,-1.154927857485252,0.9856876254458832},
      {0.22023579525386586,-1.8513661719121164,-5.867928992902801,-4.607212517035493,2.3689496302435753},
      {-2.001695438582874,4.5740839806367255,-4.08229114112469,-3.1527437311495574,1.206871937397001}};

// The predefinded weights from the hidden layer to the output layer
const float w2[NUMBER_OF_HIDDEN] = {-5.88593656622018,
      11.654997713149923,
      10.035937809335099,
      6.938568069584963,
      -3.6576900648344797};

// Initial irrelivent values for the 4 latest readings
float vals[NUMBER_OF_INPUTS] = {2242, 2242, 2242, 2241};

// Booleans to determine whethere the car is present or not
boolean carPresent = false;
boolean temp = carPresent;

void setup() {
    // Sets up the Serial communication and the Ultrasonic based on the pins
    Serial.begin(9600);
    pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
    pinMode(ULTRASONIC_ECHO_PIN, INPUT);

    // Scales all the values in the array by the maximum distance observed of 2261
    for(int i=0; i<NUMBER_OF_INPUTS; i++){
      vals[i] = vals[i]/2261.0;
    }
}

void loop() {

    Serial.println();

    long duration, distance;
    
    // The next block of code lets the Ultrasonic sensor take a reading
    // ********************************************
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);  
    delayMicroseconds(2); 
    
    digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
    delayMicroseconds(10); 
    
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
    duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
    distance = (duration/2) / 29.1;
    // ********************************************

    changeValues(vals, distance);

    // Displays the values at each iteration
    for(int i=0; i<NUMBER_OF_INPUTS; i++){
        Serial.print("Value #");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(vals[i], 15);
    }
    
    carPresent = detect(vals);

    // Displays whether a car was identified in the way or not
    Serial.print("Is it a car though?" );
    Serial.println(carPresent, 15);

    Serial.println();

    // Every 3000 miliseconds, another reading is taken 
    delay(3000);
}

// The detection function that is called each loop
boolean detect(float *vals){
    
    // The nect block of code takes the 4 latest values observed and passes them to do a feedforward to establish the output value
    // *****************************************
    float hidden[NUMBER_OF_HIDDEN];
    for (int i=0; i<NUMBER_OF_INPUTS; i++)
    {
        for (int j=0; j<NUMBER_OF_HIDDEN; j++)
        {
            hidden[j] += vals[i] * w1[i][j];
        }      
    }

    float outtie = 0;
    for (int o=0; o<NUMBER_OF_HIDDEN; o++)
    {
        hidden[o] = sigmoid(hidden[o]);
        outtie += hidden[o] * w2[o];     
    }

    outtie = sigmoid(outtie);

    // *****************************************

    Serial.print("Output from algo: ");
    Serial.println(outtie, 15);

    // If the output is greater than a chosen cut off, it is rendered to be either an object present or not
    if(outtie > 0.95)
    {
        return true;
    }
    else
    {
        return false;
    }

}

// A mathematical function, the Sigmoid
float sigmoid(float val){
    return 1/(1+exp(-val));
}

// Method used to update the values in the array. They all become shifted down by one and the new value becomes the 4th one.
void changeValues(float *values, float newVal){
    values[0] = values[1];
    values[1] = values[2];
    values[2] = values[3];
    values[3] = newVal/2261.0;
    Serial.print("New added value: ");
    Serial.println(newVal);
}

