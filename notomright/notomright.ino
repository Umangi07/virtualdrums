
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include "I2Cdev.h"3+3
#include "MPU6050_6Axis_MotionApps20.h"



int i,j=0;
/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

MPU6050 mpu;
#define LED_PIN 13


// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
float roll1=0,roll2=0,roll3=0,roll4=0,roll5=0,pitch1=0,pitch2=0,count=1;
int counter=0;
// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };


// ================================================================
// ===          INTERRUPT DETECTION ROUTINE(IMU)                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}




void setup(void) 
{
   // join I2C bus (I2Cdev library doesn't do this automatically)
   #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
       Wire.begin();
       TWBR = 12; // 400kHz I2C clock (200kHz if CPU is 8MHz)
   #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
   Fastwire::setup(400, true);
   #endif
   Serial.begin(115200);
   //while (!Serial);
   //Serial.println(F("Initializing I2C devices..."));
   mpu.initialize();

   // verify connection
   //Serial.println(F("Testing device connections..."));
   //Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

   // wait for ready
   //Serial.println(F("\nSend any character to begin DMP programming and demo: "));
   //while (Serial.available() && Serial.read()); // empty buffer
   //while (!Serial.available());                 // wait for data
   //while (Serial.available() && Serial.read()); // empty buffer again
   delay(1000);
   // load and configure the DMP
   //Serial.println(F("Initializing DMP..."));
   devStatus = mpu.dmpInitialize();
   
   // supply your own gyro offsets here, scaled for min sensitivity
   mpu.setXGyroOffset(220);
   mpu.setYGyroOffset(76);
   mpu.setZGyroOffset(-85);
   mpu.setZAccelOffset(1788); // 1688 factory
   if (devStatus == 0) {
   // turn on the DMP, now that it's ready
   //Serial.println(F("Enabling DMP..."));
   mpu.setDMPEnabled(true);

   // enable Arduino interrupt detection
   //Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
   attachInterrupt(0, dmpDataReady, RISING);
   mpuIntStatus = mpu.getIntStatus();

   // set our DMP Ready flag so the main loop() function knows it's okay to use it
   //Serial.println(F("DMP ready! Waiting for first interrupt..."));
   dmpReady = true;

   // get expected DMP packet size for later comparison
   packetSize = mpu.dmpGetFIFOPacketSize();
   } 
   else {
   // ERROR!
   // 1 = initial memory load failed
   // 2 = DMP configuration updates failed
   // (if it's going to break, usually the code will be 1)
   /*Serial.print(F("DMP Initialization failed (code "));
   Serial.print(devStatus);
   Serial.println(F(")"));*/
   }

   // configure LED for output
   pinMode(LED_PIN, OUTPUT);
   if(!mag.begin()){
   /* There was a problem detecting the HMC5883 ... check your connections */
   Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
   //while(1);
   }

}
void loop(void) 
{
 // ================================================================
 // ===                   PRESSURE SENSOR                        ===
 // ================================================================ 
 int pdata=analogRead(A0);
 bool leg=false;
 if(pdata<300)
 {
  leg=true;
 }
 
 // ================================================================
 // ===                          IMU                             ===
 // ================================================================ 

   // if programming failed, don't try to do anything
   if (!dmpReady) return;

   // wait for MPU interrupt or extra packet(s) available
   while (!mpuInterrupt && fifoCount < packetSize) {  }

   // reset interrupt flag and get INT_STATUS byte
   mpuInterrupt = false;
   mpuIntStatus = mpu.getIntStatus();

   // get current FIFO count
   fifoCount = mpu.getFIFOCount();

   // check for overflow (this should never happen unless our code is too inefficient)
   if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
   // reset so we can continue cleanly
   mpu.resetFIFO();
   //Serial.println(F("FIFO overflow!"));

   // otherwise, check for DMP data ready interrupt (this should happen frequently)
   } 
   
   else if (mpuIntStatus & 0x02) {
        
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

            
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
 
 // ================================================================
 // ===                       MAGNETOMETER                       ===
 // ================================================================
  j++;
  
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);
 
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  // Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  //Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  //Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");

  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  float heading2 = atan2(event.magnetic.z, event.magnetic.x);
  float heading3 = atan2(event.magnetic.z, event.magnetic.y);
  

  float declinationAngle = 0.22;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  /* if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;*/
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  float headingDegrees2 = heading2 * 180/M_PI; 
  float headingDegrees3 = heading3 * 180/M_PI; 
  //Serial.print("j");
       
  boolean play=false;
  roll1=ypr[1] * 180/M_PI;
  if((roll1-roll3)>2 &&(roll3<=roll2) && (roll3<=roll4) && (roll5-roll3)>2)
       play=true;           
       roll5=roll4;
       roll4=roll3;
       roll3=roll2;
       roll2=roll1;
         
 
  //Serial.print(roll1);Serial.print("\n");
  if(j<=30)
  {
        i +=headingDegrees;
        //Serial.print("hello");
        //Serial.print(j);
  }
  if(j==31)
        i=i/30;
  if(j>31)
        headingDegrees=headingDegrees-i;
  if(headingDegrees < 0)
        headingDegrees += 360;
    
  // Check for wrap due to addition of declination.
  if(headingDegrees > 360)
        headingDegrees -= 360;
  
  //Serial.print("Heading (degrees): "); Serial.print(headingDegrees); Serial.print("\n");
  //Serial.println(analogRead(A2));//200
  //Serial.print("flex: "); Serial.print(fdata); Serial.print("\n");
  //Serial.print("Heading (degrees2): "); Serial.print(headingDegrees2); Serial.print("\t");
  //Serial.print("Heading (degrees3): "); Serial.print(headingDegrees3); Serial.print("\n");
  //Serial.print("world");

  if(leg && headingDegrees<=140 && headingDegrees>0 &&play)
  Serial.print("a");
  else if(!leg && headingDegrees<=140 && headingDegrees>0 && play)
  Serial.print("b");
 
  else if(headingDegrees>=200 && headingDegrees<360 && play) //&& p n                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                lay)
  Serial.print("e");
  else
  Serial.print("z");
  
  //Serial.print(roll1);Serial.print("\t");
  //if(roll1<0)
//  value=1;
//  else
  
 
   }
   //Wire.flush();
   //Serial.flush();
 }
