/*
Slave
Connect +5v of master to Vin
Connect GND to GND
A4 -> A4, A5 -> A5
*/

#include <Wire.h>
#include <LiquidCrystal.h>
#include <Servo.h>

/* 12 11 5 4 3 2 */
#define RS 12
#define ENABLE 11
#define D4 10
#define D5 9
#define D6 8
#define D7 7
/* LCD pins end */
/*
R/W to GND
VSS to GND
VCC to +5V

*/
#define SERVOPIN 6

#define SUCCESSLED 3
#define FAILLED 2

void setup()
{	
	LiquidCrystal LCD(RS, ENABLE, D4, D5, D6, D7);
  LCD.begin(16, 2);
  LCD.noCursor();
  Servo doormech;
  doormech.attach(SERVOPIN);
	Wire.begin(0);
	Wire.onReceive(performAction);
	Serial.begin(9600);
}

void performAction(int numBytesRead)
{
	int actionID;
	while(1 < Wire.available())
	{
		actionID = Wire.read();
		if(actionID == 1)
		{
			/* 1. LCD Message  */
			LCD.print("Authenticated.");
			LCD.setCursor(0, 1);
			LCD.print("Door unlocked.");
			/* 2. LED Green    */
			digitalWrite(SUCCESSLED, HIGH);
			/* 3. Servo rotate */
			for(int i = 0; i <= 90; i++)
			{
				doormech.write(i);
				delay(50);
			}
			delay(5000);
			doormech.write(0);
		}
		else
		{
			/* Reset all possible variables set by any previous success */
		  	doormech.write(0);
		  	digitalWrite(SUCCESSLED, LOW);
		  	LCD.clear();
		  	/* 1. LED Red     */
		  	digitalWrite(FAILLED, HIGH);
				/* 2. LCD Message */
				LCD.setCursor(0, 0);
				LCD.print("Not authenticated");
				LCD.setCursor(0, 1);
				LCD.print("Please try again");
		}
	}
}

void loop()
{
	delay(10);
}
