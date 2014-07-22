/* ======================= Includes ================================= */

#include "DHT22.h"

#define ON 1
#define OFF 0

//DHT22 Setup
#define DHTPIN D2    // Digital pin D2
#define DHTTYPE AM2301  // sensor type.  Options: [DHT11, DHT22, DHT21, AM2301]
DHT dht(DHTPIN, DHTTYPE);


int STARTPIN = D7;
int BUTTONPIN = D0;
int RELAY1 = D5;
int RELAY2 = D6;
int LIGHTPIN = A0;  //LIGHT PIN, photoresistance    
int TEMPPIN = A1;  //MCP9700A temperature sensor
int REDLED = A6;
int MOISTURE_VCC = D4;//MOISTURE VSS POWER SUPPLY
int MOISTURE1_ANA = A2; //MOISTURE  analog in 1
int MOISTURE2_ANA = A3; //MOISTURE  analog in 2
int WATER_ANA = A4; //water  analog in 1
int TEST12V_ANA = A5; //mesure 12V


int cmdpump1State = 0;
int cmdpump2State = 0;
int buttonState = 0;
int redLEDState = 0;
int lightValue = 1000;
int Humid_Sol1 = 100;
int Humid_Sol2 = 200;
int Water_Level = 300;
int redLEDetat = OFF;
int LED_flash_value = 00;
float temperature_val = 47.8;
float h;      // humidity
float t;      // temperature
int f = 0;    // failed?

char temperatureValue[35];
char AM2302_TempValue[30];
char AM2302_HumidValue[30];

uint32_t UPDATE_INTERVAL = 10; // in milliseconds
uint32_t UPDATE_INTERVAL_TEMP = 500; // in milliseconds
uint32_t FLASH_LED_INTERVAL = 150; // in milliseconds
uint32_t FIFTEEN_MINUTES = 60*15*1000; // in milliseconds
uint32_t FIVE_MINUTES = 60*5*1000; // in milliseconds
uint32_t ONE_MINUTES = 60*1*1000; // in milliseconds

uint32_t lastTime = 0; // in milliseconds
uint32_t lastTimeTemp = 0; // in milliseconds
uint32_t lastTimeLED = 0; // in milliseconds
uint32_t lastTimemoisture = 0; // in milliseconds

uint32_t startTimepump1 = 0; // in milliseconds
uint32_t startTimepump2 = 0; // in milliseconds

void setup() {
    pinMode(STARTPIN,OUTPUT);
    pinMode(BUTTONPIN,INPUT);
    pinMode(RELAY1,OUTPUT);
    pinMode(RELAY2,OUTPUT);
	pinMode(LIGHTPIN, INPUT);
	pinMode(TEMPPIN, INPUT);
    pinMode(REDLED,OUTPUT);
    
    pinMode(MOISTURE_VCC,OUTPUT);
	pinMode(MOISTURE1_ANA, INPUT);
	pinMode(MOISTURE2_ANA, INPUT);
	pinMode(WATER_ANA, INPUT);

    digitalWrite(STARTPIN,LOW);
    //digitalWrite(TRUNKPIN,LOW);
    //digitalWrite(LOCKPIN,LOW);
    //digitalWrite(UNLOCKPIN,LOW);
    digitalWrite(REDLED,LOW);

    Spark.function("cmdpump1", cmdpump1Function);
    Spark.function("readbutton", buttonFunction);
    Spark.function("cmdpump2", cmdpump2Function);
    Spark.function("unlock", unlockFunction);
    
    Spark.variable("pump1state", &cmdpump1State, INT);
    Spark.variable("buttonstate", &buttonState, INT);
    Spark.variable("pump2state", &cmdpump2State, INT);
    Spark.variable("lightvalue", &lightValue, INT);
    
    // water temperature MCP9700A
	Spark.variable("tempvalue", &temperatureValue, STRING);
	
	//resistor sensor
    Spark.variable("humidsol1", &Humid_Sol1, INT);
    Spark.variable("humidsol2", &Humid_Sol2, INT);
	
	// am3202 SENSOR :
	Spark.variable("temperature", &AM2302_TempValue, STRING);
	Spark.variable("humidity", &AM2302_HumidValue, STRING);
	dht.begin();
}

void loop() {
    
    dht.poll();
    
    
    // Update every 10ms (default)
    if(millis() - lastTime > UPDATE_INTERVAL) {
        // Set a new last time
        lastTime = millis();
		
	
		//read button and change led D7
		if(digitalRead(BUTTONPIN) == ON) {
			//digitalWrite(STARTPIN, LOW); // Single press the start button to stop
			buttonState = ON; // change variable
		}
		else {
			//digitalWrite(STARTPIN, HIGH); // 
			buttonState = OFF; // change variable
		}
		
        lightValue =  analogRead(LIGHTPIN);
        
		//lightValue =  1500;
		LED_flash_value = lightValue/10;
		
		
		//disable pump1 if this pump is activated since 5 minutes
        if(cmdpump1State == ON) {
            if(millis() - startTimepump1 > FIVE_MINUTES) {
                cmdpump1State = OFF;
                digitalWrite(RELAY1, LOW);
            }
        }    
        
        //disable pump2 if this pump is activated since 5 minutes
        if(cmdpump2State == ON) {
            if(millis() - startTimepump2 > FIVE_MINUTES) {
                cmdpump2State = OFF;
                digitalWrite(RELAY2, LOW);
            }
        }
        
    }
    

// mesure moisture
    if(millis() - lastTimemoisture > UPDATE_INTERVAL_TEMP) {
        // Set a new last time
        lastTimemoisture = millis();

        //turn off vcc pin moisture
        digitalWrite(MOISTURE_VCC, HIGH);
        //digitalWrite(MOISTURE2_VCC, HIGH);
        delay(100);
        Humid_Sol1 = analogRead(MOISTURE1_ANA);
        Humid_Sol2 = analogRead(MOISTURE2_ANA);
        Water_Level = analogRead(WATER_ANA);
        delay(100);
        //turn off vcc pin moisture
        digitalWrite(MOISTURE_VCC, LOW);
        //digitalWrite(MOISTURE2_VCC, LOW);
        

    }
    
    
	
    if(millis() - lastTimeTemp > UPDATE_INTERVAL_TEMP) {
        // Set a new last time
        lastTimeTemp = millis();
		// temperature measure
		temperature_val = analogRead(TEMPPIN) * 0.800 ;//(Vana_temp*3300)/4086  // en mV
		temperature_val = ((temperature_val-500)/10); // Calcul température
		sprintf(temperatureValue, "Température de l'eau: %.1f°C", temperature_val);
		//sprintf(temperatureValue, "Temperature: %.1f°C (brute=%d)",temperature_val, analogRead(TEMPPIN));
		
		f = 0;
        h = dht.readHumidity();
        t = dht.readTemperature();
    
        if (t==NAN || h==NAN) {
            f = 1; // not a number, fail.
        }
        else {
            f = 0; // both numbers! not failed.
		sprintf(AM2302_TempValue, "Température: %.1f°C", t);
		sprintf(AM2302_HumidValue, "Humidité: %.1f %%", h);
        }
    }
	
	// clignioteur de led en fonction de la lumière
    if(millis() - lastTimeLED > LED_flash_value) {
        // Set a new last time
        lastTimeLED = millis();
		if(redLEDState == ON) {
			if (redLEDetat == ON ) 
			{
				digitalWrite(REDLED,LOW);
				redLEDetat = OFF;
			}	
			else 
			{
				digitalWrite(REDLED,HIGH);
				redLEDetat = ON;
			}	
        }
		else digitalWrite(REDLED,LOW);
    }
	
}

int cmdpump1Function(String args) {
    if(cmdpump1State == OFF) {
        cmdpump1State = ON; // engine started
        startTimepump1 = millis(); // start mesure timing

		digitalWrite(RELAY1, HIGH); // Double press the start button to start

        return 200; // This is checked in the web app controller for validation
    }
    else {
        cmdpump1State = OFF; // engine stopped

		digitalWrite(RELAY1, LOW);
	
        return 200; // This is checked in the web app controller for validation
    }
}

int cmdpump2Function(String args) {
    if(cmdpump2State == OFF) {
        cmdpump2State = ON; // engine started
        startTimepump2 = millis(); // start mesure timing

		digitalWrite(RELAY2, HIGH); // Double press the start button to start

        return 200; // This is checked in the web app controller for validation
    }
    else {
        cmdpump2State = OFF; // engine stopped

		digitalWrite(RELAY2, LOW);
	
        return 200; // This is checked in the web app controller for validation
    }
}

int buttonFunction(String args) {
    if(digitalRead(BUTTONPIN) == ON) {
        buttonState = ON; // change variable
		//digitalWrite(STARTPIN, LOW); // Single press the start button to stop
    }
    else {
        buttonState = OFF; // change variable
		//digitalWrite(STARTPIN, HIGH); // 
    }
    return 200; // This is checked in the web app controller for validation
}

int lockFunction(String args) {
    if(redLEDState == OFF) {
        redLEDState = ON; // locks locked
        //digitalWrite(LOCKPIN, HIGH); // Single press the lock button
        //delay(500);
        //digitalWrite(LOCKPIN, LOW);
    }
	else redLEDState = OFF;
    return 200; // This is checked in the web app controller for validation
}

int unlockFunction(String args) {
    if(redLEDState == ON) {
        redLEDState = OFF; // locks unlocked
        //digitalWrite(UNLOCKPIN, HIGH); // Single press the unlock button
        //delay(500);
        //digitalWrite(UNLOCKPIN, LOW);
    }
    return 200; // This is checked in the web app controller for validation
}