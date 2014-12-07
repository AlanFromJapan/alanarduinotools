/*
 * BalconyMeter.c
 *
 * Created: 2012/04/08 18:33:34
 *  Author: Alan
 */ 

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/wdt.h>

 uint8_t mLetters[] = {
	0b11101011,//0 positive
	0b10001000,//1 positive
	0b10110011,//2 positive
	0b10111010,//3 positive
	0b11011000,//4 positive
	0b01111010,//5 positive
	0b01111011,//6 positive
	0b10101000,//7 positive
	0b11111011,//8 positive
	0b11111010//9 positive
};

#define DOT_MASK 0b00000100

#define AVGBUFFER_LEN 5
uint16_t mTempBufferAverage[AVGBUFFER_LEN];
uint8_t mTempBufferHead = 0;

#define DIGIT_POV_US 1000
#define PINBIT_DECIMAL 2
#define PINBIT_UNITS 3
#define PINBIT_TENS 4


//watchdog interrupt : just resume activity
ISR(WDT_vect) {
	cli();
	wdt_disable();
	sei();
}


//We consider that all digit given as parameter are x10 compared to the real value (for showing 1 decimal)
// 3 means 0.3, 99 means 9.9, 123  means 12.3
void showDigit(uint16_t pValue){
	//decimals
	PORTD =  mLetters[pValue % 10];
	PORTC =  (~(1 << PINBIT_DECIMAL)) & 0xFC;
	//pov	
	_delay_us(DIGIT_POV_US);
	
	//units + the decimal dot
	PORTD =  mLetters[(pValue / 10) % 10] | DOT_MASK;
	PORTC =  (~(1 << PINBIT_UNITS)) & 0xFC;
	//pov	
	_delay_us(DIGIT_POV_US);
	
	//tens if != 0
	if ((pValue/ 100) % 10 != 0) {
		PORTD =  mLetters[(pValue/ 100) % 10];
		PORTC =   (~(1 << PINBIT_TENS)) & 0xFC;
		//pov	
		_delay_us(DIGIT_POV_US);
	}	
	
	//and turn everybody off
	PORTD = 0x00;
}

void numbers99dot9(){
	
	uint16_t i = 0;
    while(1)
    {
		//just some silly temporisation
		for (uint32_t j = 0; j < 1000; j++){
			showDigit(i);
		}
		
		i= (i + 1) % 1000;
    }	
}

void TestAllSegments() 
{
	while(1)
    {
		//j selects the number
        for (int j = 4; j >= 2; --j){
			//start with all segments off
			PORTC = 0x00;
			PORTC = ~(1 << j);
			
			for (int i=0; i < 8; i++){
				PORTD = (1 << i);
				
				_delay_ms(8);
			}
		}
    }
}


/*
 *  ShutOffADC      shut down the ADC and prepare for power reduction
 * http://www.seanet.com/~karllunt/atmegapowerdown.html
 */
void  ShutOffADC(void)
{
    ACSR = (1<<ACD);                        // disable A/D comparator
    ADCSRA = (0<<ADEN);                     // disable A/D converter
    DIDR0 = 0x3f;                           // disable all A/D inputs (ADC0-ADC5)
    DIDR1 = 0x03;                           // disable AIN0 and AIN1
}

//Read ADC value into a 16 bits uint16
uint16_t ReadADCx16() 
{
	//goto ADC noise reduction mode, startup the ADC
	PRR &= ~(1<<PRADC);
    ADCSRA = (1<<ADEN);   
	//stop all digit display (to avoid noise)
	PORTD = 0x00; _delay_ms(2);     
	
	
	
	ADCSRA |= (1 << ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // wait until conversion is done

	uint16_t v = ADCL;
	v = (ADCH << 8) + v;
	
	
	
	//stop all the ADC stuffs, disable ADC
	PRR = 0xFF;
    ADCSRA = (0<<ADEN);                     // disable A/D converter
	ShutOffADC();
	
	return v;
}

//Watchdog setup to be 8sec sleep(consumption goes like 0.4 mA
//http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1258212686
void myWatchdogEnable() {  // turn on watchdog timer; interrupt mode every n sec
  cli();
  MCUSR = 0;
  WDTCSR |= 0b00011000;
  //WDTCSR = 0b01000111; //sleep 2sec
  WDTCSR = 0b01100001; //sleep 8sec
  sei();
} 

//Main body in fact, an inifinite loop
void ReadTemp(){
	
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	
    while(1)
    {
		uint32_t v = ReadADCx16();
		//convert to centigrad and *10 (because we are working with INT and want to display 1 decimal)
		//v = (v * 5 * 100 * 10) / 1024; <=> v * 5 * 1000 / 1024 <=> v *5 but we loose a bit of precision
		//v = v * 5; 
		v = (v * 5 * 100 * 10) >> 10; //>> 10 <=> /1024
		
		//ad'hoc calibration
		//apparently my system reads a bit too high temp, about 5% too high
		//I don't have the will to make extensive test, calculate formula etc... 
		//v = v - v/20; //finally readings are ok like that after comparison, just keep the code for memory
		
		//average the temp with latest readings
		mTempBufferAverage[mTempBufferHead] = v;
		mTempBufferHead = (mTempBufferHead +1) % AVGBUFFER_LEN;
		
		//calculate an average with a circle buffer
		v= 0;
		for (uint16_t j = 0; j < AVGBUFFER_LEN; j++){
			v += mTempBufferAverage[j];
		}
		v = v / AVGBUFFER_LEN;
		
		//just some silly temporisation
		for (uint16_t j = 0; j < 10000; j++){
			showDigit(v);
			//_delay_us(1000);
			//_delay_ms(1); // save about 0.5mA per ms of sleep. 1 does not change the brightness, after it does but saves more power... depends if you want to read the temperature by daylight too.
		}
		
		
		//and now the sleep n time 8 sec
		for (int i =0; i < 2; i++){
			wdt_reset();
			myWatchdogEnable();
			sleep_mode();
		}		
    }	
}




void InitADC(){
	/*********************************************************************/
	//ADC setup
	//ADC0, right adjust, using aref
	//ADMUX = 0x00;
	ADMUX = 0x00 | (1 << REFS0) | (0 << ADLAR);
	
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
	
/*********************************************************************/	
	
}

int main(void)
{
	//uses a 12MHz crystal because I messed the fuses ... so let's go slowly and run 1/256th of that speed to save electricity
	//EDIT: I managed to rechange the fuses to internal 8MHz oscillator, save 3 mA power
	CLKPR = (1 << CLKPCE) | (1 << CLKPS3);
	
	//PORTD is the 7segment control, everybody goes output
	DDRD = 0xFF;
	
	//PORTC bits [5-2] control which 7segment is selected
	//bits [1-0] are for input of temperature
	DDRC = 0b11111100;
	
	//start with all segments off
	PORTC = 0x00;
	
	//init the ADC
	InitADC();

	//SimpleTestDigit();
	
	//numbers99dot9();
	
    //TestAllSegments();

/*********************************************************************/	
	cli();                                    // quiet for just a moment
	ShutOffADC();                             // prepare ADC for sleep
	PRR = (1<<PRTWI) | (1<<PRTIM0) | (1<<PRTIM1) | (1<<PRTIM2) | (1<<PRSPI) | (1<<PRADC) | (1<<PRUSART0);
/*********************************************************************/	

	ReadTemp();
}