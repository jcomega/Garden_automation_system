
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
int VCC_DHT22 = D3;
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
float Battery_Voltage = 0;
int redLEDetat = OFF;
int LED_flash_value = 00;
float temperature_val = 47.8;
float battery_val = 12.0;
float h;      // humidity
float t;      // temperature
int f = 0;    // failed?

char temperatureValue[43];
char AM2302_TempValue[30];
char AM2302_HumidValue[30];
char battery_VoltValue[35];
char Water_LevelValue[30];

#define  UPDATE_INTERVAL  100 // in milliseconds
#define  UPDATE_INTERVAL_TEMP  500 // in milliseconds
#define  FIFTEEN_MINUTES 60*15*1000 // in milliseconds
#define  TEN_MINUTES 60*10*1000 // in milliseconds
#define  FIVE_MINUTES 60*5*1000 // in milliseconds
#define  TREE_MINUTES 60*3*1000 // in milliseconds
#define  ONE_MINUTES 60*1*1000 // in milliseconds
#define  UPDATE_WIFI_ON 60*1000 // in milliseconds

uint32_t lastTime = 0; // in milliseconds
uint32_t lastTimeTemp = 0; // in milliseconds
uint32_t lastTimeLED = 0; // in milliseconds
uint32_t lastTimemoisture = 0; // in milliseconds
uint32_t lastTimeLong = 0; // in milliseconds
uint32_t lastTimeWifi = 0; // in milliseconds

uint32_t startTimepump1 = 0; // in milliseconds
uint32_t startTimepump2 = 0; // in milliseconds

void setup() {

    pinMode(BUTTONPIN,INPUT);
    pinMode(RELAY1,OUTPUT);
    pinMode(RELAY2,OUTPUT);
	pinMode(LIGHTPIN, INPUT);
	pinMode(TEMPPIN, INPUT);
    pinMode(VCC_DHT22,OUTPUT);
    
    pinMode(REDLED,OUTPUT);
    
    pinMode(MOISTURE_VCC,OUTPUT);
	pinMode(MOISTURE1_ANA, INPUT);
	pinMode(MOISTURE2_ANA, INPUT);
	pinMode(WATER_ANA, INPUT);
	pinMode(TEST12V_ANA, INPUT);

    digitalWrite(VCC_DHT22,HIGH);
    digitalWrite(REDLED,LOW);

    Spark.function("cmdpump1", cmdpump1Function);
    //Spark.function("readbutton", buttonFunction);
    Spark.function("cmdpump2", cmdpump2Function);
    //Spark.function("unlock", unlockFunction);
    
    Spark.variable("pump1state", &cmdpump1State, INT);
    Spark.variable("buttonstate", &buttonState, INT);
    Spark.variable("pump2state", &cmdpump2State, INT);
    Spark.variable("lightvalue", &lightValue, INT);
    
    // water temperature MCP9700A + water level
	Spark.variable("tempvaluew", &temperatureValue, STRING);
	
	//resistor sensor
    Spark.variable("humidsol1", &Humid_Sol1, INT);
    Spark.variable("humidsol2", &Humid_Sol2, INT);
    
    // 12V battery voltage
    Spark.variable("batteryvol", &battery_VoltValue, STRING);
	
	// am3202 SENSOR :
	Spark.variable("temperature", &AM2302_TempValue, STRING);
	Spark.variable("humidity", &AM2302_HumidValue, STRING);
	dht.begin();
    lastTimeWifi = millis();
}

void loop() {
    
    if(millis() - lastTimeWifi > TEN_MINUTES) {
    //digitalWrite(REDLED,HIGH);   
    Spark.sleep(300); //Wifi sleep for 5mn
    //digitalWrite(REDLED,LOW);
    
    while(WiFi.status() == 0)
        {
             //code stuff
             delay(10000);
        }
      
    lastTimeWifi = millis();
    }
    
    dht.poll();
    
    // Update every 10ms (default)
    if(millis() - lastTime > UPDATE_INTERVAL) {
        // Set a new last time
        lastTime = millis();

		//disable pump1 if this pump is activated since 5 minutes
        if(cmdpump1State == ON) {
            if(millis() - startTimepump1 > TREE_MINUTES) {
                cmdpump1State = OFF;
                digitalWrite(RELAY1, LOW);
            }
        }    
        
        //disable pump2 if this pump is activated since 5 minutes
        if(cmdpump2State == ON) {
            if(millis() - startTimepump2 > TREE_MINUTES) {
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
        
        //water temperature
		temperature_val = analogRead(TEMPPIN) * 0.800 ;//(Vana_temp*3300)/4086  // en mV
		temperature_val = ((temperature_val-500)/10); // Calcul température
		if(Water_Level>1000)
		sprintf(temperatureValue, "Reserve eau: %.1f°C || Niveau ok", temperature_val);
		else
		sprintf(temperatureValue, "Reserve eau: %.1f°C || Plus d'eau !", temperature_val);

		//sprintf(temperatureValue, "Temperature: %.1f°C (brute=%d)",temperature_val, analogRead(TEMPPIN));
		
		//light mesure
        lightValue =  analogRead(LIGHTPIN);
    
        // read, calcul and dispplay battery voltage
        Battery_Voltage = analogRead(TEST12V_ANA) * 0.005371; //((Vana*3300)/4096)*6.666   // 6.66 ration diviseur de tension avec 68K et 12K
		//temperature_val = analogRead(TEMPPIN) * 0.800 ;//(Vana_temp*3300)/4086  // en mV
		//temperature_val = ((temperature_val-500)/10); // Calcul température
		sprintf(battery_VoltValue, "Tension batterie: %.1fV", Battery_Voltage);
	
    }
    
    
	
    if(millis() - lastTimeTemp > UPDATE_INTERVAL_TEMP) {
        // Set a new last time
        lastTimeTemp = millis();
		// temperature measure

		f = 0;
        h = dht.readHumidity();
        t = dht.readTemperature();
    
        if (t==NAN || h==NAN || h==0 || t==0){
            f = 1; // not a number, fail.
        }
        else {
            f = 0; // both numbers! not failed.
		sprintf(AM2302_TempValue, "Température: %.1f°C", t);
		sprintf(AM2302_HumidValue, "Humidité: %.1f %%", h);
        }
    }
    
    if(millis() - lastTimeLong > FIFTEEN_MINUTES) {
        // Set a new last time
        lastTimeLong = millis();

        //turn off vcc DHT11 pin
        digitalWrite(VCC_DHT22, LOW);
        delay(500);
        digitalWrite(VCC_DHT22, HIGH);
        
	    dht.begin();
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

