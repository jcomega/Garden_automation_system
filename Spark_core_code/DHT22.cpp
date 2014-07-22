#include "DHT22.h"
 
DHT::DHT(uint8_t pin, uint8_t type, uint8_t count) {
    _pin = pin;
    _type = type;
    _count = count;
    firstreading = true;
    _next_read_millis = 0;
    _next_read_step = 1;
}
 
 
void DHT::begin(void) {
    // set up the pins!
    pinMode(_pin, INPUT);
    digitalWrite(_pin, HIGH);
    _lastreadtime = 0;
    _next_read_millis = 0;
    _next_read_step = 1;
}
 
 
void DHT::poll(void) {
    if(_next_read_millis<=millis()) {
        switch(_next_read_step) {
            case 1:
                DHT::read1();
                break;
            
            case 2:
                DHT::read2();
                break;
            
            case 3:
                DHT::read3();
                break;
        }
    }
}
 
 
//boolean S == Scale.  True == Farenheit; False == Celcius
float DHT::readTemperature(bool S) {
    float _f;
    
    switch (_type) {
        case DHT11:
            _f = data[2];
            
            if(S)
                _f = convertCtoF(_f);
            
            return _f;
            
            
        case DHT22:
        case DHT21:
            _f = data[2] & 0x7F;
            _f *= 256;
            _f += data[3];
            _f /= 10;
            
            if (data[2] & 0x80)
                _f *= -1;
                
            if(S)
                _f = convertCtoF(_f);
            
            return _f;
    }
    
    return NAN;
}
 
 
float DHT::convertCtoF(float c) {
    return c * 9 / 5 + 32;
}
 
 
float DHT::readHumidity(void) {
    float _f;
    switch (_type) {
        case DHT11:
            _f = data[0];
            return _f;
            
            
        case DHT22:
        case DHT21:
            _f = data[0];
            _f *= 256;
            _f += data[1];
            _f /= 10;
            return _f;
    }
 
    return NAN;
}
 
 
void DHT::read1(void) {
    // pull the pin high and wait 250 milliseconds
    digitalWrite(_pin, HIGH);
    
    _next_read_millis = millis()+250;
    _next_read_step = 2;
}
 
 
void DHT::read2(void) {
    unsigned long currenttime;
 
 
    currenttime = millis();
    if (currenttime < _lastreadtime) {
        // ie there was a rollover
        _lastreadtime = 0;
    }
    
    if (!firstreading && ((currenttime - _lastreadtime) < 2000)) {
        //delay(2000 - (currenttime - _lastreadtime));
        //return true; // return last correct measurement
        return;
    }
    
    firstreading = false;
    _lastreadtime = millis();
    
    data[0] = data[1] = data[2] = data[3] = data[4] = 0;
    
    // now pull it low for ~20 milliseconds
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    
    _next_read_millis = millis()+20;
    _next_read_step = 3;
}
 
 
void DHT::read3(void) {
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    
    cli();
    
    digitalWrite(_pin, HIGH);
    delayMicroseconds(40);
    pinMode(_pin, INPUT);
    
    // read in timings
    for ( i=0; i< MAXTIMINGS; i++) {
        counter = 0;
        
        while (digitalRead(_pin) == laststate) {
            counter++;
            delayMicroseconds(1);
            
            if (counter == 255)
                break;
        }
        
        laststate = digitalRead(_pin);
    
        if (counter == 255)
            break;
    
        // ignore first 3 transitions
        if ((i >= 4) && (i%2 == 0)) {
            // shove each bit into the storage bytes
            data[j/8] <<= 1;
            
            if (counter > _count)
                data[j/8] |= 1;
                
            j++;
        }
    }
    
    sei();
 
    
    // check we read 40 bits and that the checksum matches
    //if ((j >= 40) &&  (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)))
    //    return true;
    
    
    //return false;
    
    _next_read_step = 1;
    _next_read_millis = millis()+2000;
}