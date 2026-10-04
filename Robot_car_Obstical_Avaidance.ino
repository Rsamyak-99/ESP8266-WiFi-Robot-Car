/******************* WiFi Robot Remote Control + HC-SR04 ********************
 * ESP8266 + L298N + 4 BO Motors
 *
 * HC-SR04:
 * TRIG -> D0
 * ECHO -> D8 through voltage divider
 *
 * Buzzer -> D7
 *
 * Reverse obstacle protection:
 * If object <= 25 cm while reversing:
 *      Motors STOP
 *      Buzzer BEEPS
 *      Reverse remains blocked
 ****************************************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>


// ==========================================================================
// MOTOR CONNECTIONS
// ==========================================================================

// Right side motors (FR + BR)
int enA = D1;
int in1 = D2;
int in2 = D3;

// Left side motors (FL + BL)
int in3 = D4;
int in4 = D5;
int enB = D6;


// ==========================================================================
// OTHER CONNECTIONS
// ==========================================================================

const int buzPin = D7;


// ==========================================================================
// HC-SR04 ULTRASONIC SENSOR
// ==========================================================================

const int ultrasonicTrig = D0;
const int ultrasonicEcho = D8;

// Obstacle stopping distance
const int STOP_DISTANCE = 25;     // cm


// ==========================================================================
// ROBOT SETTINGS
// ==========================================================================

String command = "";

int SPEED = 1023;

int speed_Coeff = 3;


// ==========================================================================
// WIFI SETTINGS
// ==========================================================================

const char* AP_SSID = "Robot Car";
const char* AP_PASSWORD = "Robot123";

ESP8266WebServer server(80);


// ==========================================================================
// REVERSE SAFETY VARIABLES
// ==========================================================================

// True when robot is currently reversing
bool reversing = false;

// Prevent continuous buzzer spam
unsigned long lastObstacleBeep = 0;

const unsigned long BEEP_INTERVAL = 500;


// ==========================================================================
// SETUP
// ==========================================================================

void setup()
{
  Serial.begin(115200);

  delay(500);

  Serial.println();
  Serial.println("======================================");
  Serial.println("      ESP8266 WIFI ROBOT CAR");
  Serial.println("      HC-SR04 SAFETY SYSTEM");
  Serial.println("======================================");


  // ------------------------------------------------------------------------
  // Buzzer
  // ------------------------------------------------------------------------

  pinMode(buzPin, OUTPUT);

  digitalWrite(buzPin, LOW);


  // ------------------------------------------------------------------------
  // HC-SR04
  // ------------------------------------------------------------------------

  pinMode(ultrasonicTrig, OUTPUT);

  pinMode(ultrasonicEcho, INPUT);

  digitalWrite(ultrasonicTrig, LOW);


  // ------------------------------------------------------------------------
  // Motor pins
  // ------------------------------------------------------------------------

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);


  // ------------------------------------------------------------------------
  // STOP MOTORS AT START
  // ------------------------------------------------------------------------

  analogWrite(enA, 0);
  analogWrite(enB, 0);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);


  // ------------------------------------------------------------------------
  // WIFI ACCESS POINT
  // ------------------------------------------------------------------------

  WiFi.mode(WIFI_AP);

  WiFi.softAP(AP_SSID, AP_PASSWORD);

  delay(1000);

  IPAddress myIP = WiFi.softAPIP();


  Serial.println();
  Serial.println("WiFi Access Point Started");

  Serial.print("WiFi Name: ");
  Serial.println(AP_SSID);

  Serial.print("WiFi Password: ");
  Serial.println(AP_PASSWORD);

  Serial.print("Robot IP Address: ");
  Serial.println(myIP);


  Serial.println();
  Serial.println("Connect your phone to:");

  Serial.println("SSID: Robot Car");

  Serial.println("Password: Robot123");

  Serial.println("IP: 192.168.4.1");


  // ------------------------------------------------------------------------
  // WEB SERVER
  // ------------------------------------------------------------------------

  server.on("/", HTTP_handleRoot);

  server.onNotFound(HTTP_handleRoot);

  server.begin();


  Serial.println();
  Serial.println("Web Server Started");


  // ------------------------------------------------------------------------
  // OTA
  // ------------------------------------------------------------------------

  ArduinoOTA.setHostname("Robot-Car");

  ArduinoOTA.begin();

  Serial.println("OTA Ready");

  Serial.println("======================================");
}


// ==========================================================================
// MAIN LOOP
// ==========================================================================

void loop()
{
  // ------------------------------------------------------------------------
  // OTA
  // ------------------------------------------------------------------------

  ArduinoOTA.handle();


  // ------------------------------------------------------------------------
  // WIFI REQUESTS
  // ------------------------------------------------------------------------

  server.handleClient();


  // ------------------------------------------------------------------------
  // GET COMMAND
  // ------------------------------------------------------------------------

  if (server.hasArg("State"))
  {
    command = server.arg("State");
  }


  // ========================================================================
  // FORWARD
  // ========================================================================

  if (command == "F")
  {
    reversing = false;

    Forward();
  }


  // ========================================================================
  // BACKWARD
  // ========================================================================

  else if (command == "B")
  {
    reversing = true;

    Backward();
  }


  // ========================================================================
  // RIGHT
  // ========================================================================

  else if (command == "R")
  {
    reversing = false;

    TurnRight();
  }


  // ========================================================================
  // LEFT
  // ========================================================================

  else if (command == "L")
  {
    reversing = false;

    TurnLeft();
  }


  // ========================================================================
  // FORWARD LEFT
  // ========================================================================

  else if (command == "G")
  {
    reversing = false;

    ForwardLeft();
  }


  // ========================================================================
  // BACKWARD LEFT
  // ========================================================================

  else if (command == "H")
  {
    reversing = true;

    BackwardLeft();
  }


  // ========================================================================
  // FORWARD RIGHT
  // ========================================================================

  else if (command == "I")
  {
    reversing = false;

    ForwardRight();
  }


  // ========================================================================
  // BACKWARD RIGHT
  // ========================================================================

  else if (command == "J")
  {
    reversing = true;

    BackwardRight();
  }


  // ========================================================================
  // STOP
  // ========================================================================

  else if (command == "S")
  {
    reversing = false;

    Stop();
  }


  // ========================================================================
  // BUZZER
  // ========================================================================

  else if (command == "V")
  {
    reversing = false;

    BeepHorn();
  }


  // ========================================================================
  // SPEED SETTINGS
  // ========================================================================

  else if (command == "0")
  {
    SPEED = 10;
  }

  else if (command == "1")
  {
    SPEED = 60;
  }

  else if (command == "2")
  {
    SPEED = 81;
  }

  else if (command == "3")
  {
    SPEED = 95;
  }

  else if (command == "4")
  {
    SPEED = 105;
  }

  else if (command == "5")
  {
    SPEED = 122;
  }

  else if (command == "6")
  {
    SPEED = 155;
  }

  else if (command == "7")
  {
    SPEED = 196;
  }

  else if (command == "8")
  {
    SPEED = 272;
  }

  else if (command == "9")
  {
    SPEED = 400;
  }

  else if (command == "q")
  {
    SPEED = 1023;
  }


  // ========================================================================
  // CONTINUOUS REVERSE SAFETY
  // ========================================================================

  if (reversing)
  {
    CheckReverseSafety();
  }
}


// ==========================================================================
// HTTP HANDLER
// ==========================================================================

void HTTP_handleRoot()
{
  server.send(200, "text/html", "");

  if (server.hasArg("State"))
  {
    Serial.print("Command: ");
    Serial.println(server.arg("State"));
  }
}


// ==========================================================================
// HC-SR04 DISTANCE MEASUREMENT
// ==========================================================================

long getDistance()
{
  // Make sure trigger is LOW
  digitalWrite(ultrasonicTrig, LOW);

  delayMicroseconds(2);


  // 10 microsecond trigger pulse
  digitalWrite(ultrasonicTrig, HIGH);

  delayMicroseconds(10);

  digitalWrite(ultrasonicTrig, LOW);


  // Read echo
  long duration = pulseIn(
    ultrasonicEcho,
    HIGH,
    25000
  );


  // No echo received
  if (duration == 0)
  {
    return 999;
  }


  // Convert time to distance
  long distance = duration * 0.0343 / 2;


  return distance;
}


// ==========================================================================
// REVERSE SAFETY CHECK
// ==========================================================================

void CheckReverseSafety()
{
  static unsigned long lastSensorCheck = 0;

  // Check sensor every 80 ms
  if (millis() - lastSensorCheck < 80)
  {
    return;
  }

  lastSensorCheck = millis();


  long distance = getDistance();


  Serial.print("Reverse Distance: ");

  Serial.print(distance);

  Serial.println(" cm");


  // ========================================================================
  // OBJECT DETECTED
  // ========================================================================

  if (distance <= STOP_DISTANCE)
  {
    // Stop motors immediately
    Stop();


    Serial.println();
    Serial.println("**************************************");
    Serial.println("      !!! OBSTACLE DETECTED !!!");
    Serial.println("      REVERSE BLOCKED");
    Serial.println("**************************************");
    Serial.println();


    // Buzzer beep
    if (millis() - lastObstacleBeep >= BEEP_INTERVAL)
    {
      ObstacleBeep();

      lastObstacleBeep = millis();
    }
  }
}


// ==========================================================================
// OBSTACLE BUZZER
// ==========================================================================

void ObstacleBeep()
{
  digitalWrite(buzPin, HIGH);

  delay(100);

  digitalWrite(buzPin, LOW);

  delay(100);

  digitalWrite(buzPin, HIGH);

  delay(100);

  digitalWrite(buzPin, LOW);
}


// ==========================================================================
// FORWARD
// ==========================================================================

void Forward()
{
  analogWrite(enA, SPEED);

  analogWrite(enB, SPEED);


  digitalWrite(in1, HIGH);

  digitalWrite(in2, LOW);


  digitalWrite(in3, HIGH);

  digitalWrite(in4, LOW);
}


// ==========================================================================
// BACKWARD
// ==========================================================================

void Backward()
{
  // Check obstacle before starting reverse
  long distance = getDistance();


  Serial.print("Starting Reverse Distance: ");

  Serial.print(distance);

  Serial.println(" cm");


  // Object too close
  if (distance <= STOP_DISTANCE)
  {
    Stop();

    Serial.println("!!! OBSTACLE DETECTED !!!");

    Serial.println("Reverse movement BLOCKED");


    ObstacleBeep();

    return;
  }


  // Safe to reverse
  analogWrite(enA, SPEED);

  analogWrite(enB, SPEED);


  digitalWrite(in1, LOW);

  digitalWrite(in2, HIGH);


  digitalWrite(in3, LOW);

  digitalWrite(in4, HIGH);
}


// ==========================================================================
// RIGHT
// ==========================================================================

void TurnRight()
{
  analogWrite(enA, SPEED);

  analogWrite(enB, SPEED);


  digitalWrite(in1, LOW);

  digitalWrite(in2, HIGH);


  digitalWrite(in3, HIGH);

  digitalWrite(in4, LOW);
}


// ==========================================================================
// LEFT
// ==========================================================================

void TurnLeft()
{
  analogWrite(enA, SPEED);

  analogWrite(enB, SPEED);


  digitalWrite(in1, HIGH);

  digitalWrite(in2, LOW);


  digitalWrite(in3, LOW);

  digitalWrite(in4, HIGH);
}


// ==========================================================================
// FORWARD LEFT
// ==========================================================================

void ForwardLeft()
{
  analogWrite(enA, SPEED);

  analogWrite(enB, SPEED / speed_Coeff);


  digitalWrite(in1, HIGH);

  digitalWrite(in2, LOW);


  digitalWrite(in3, HIGH);

  digitalWrite(in4, LOW);
}


// ==========================================================================
// BACKWARD LEFT
// ==========================================================================

void BackwardLeft()
{
  long distance = getDistance();


  if (distance <= STOP_DISTANCE)
  {
    Stop();

    Serial.println("!!! OBSTACLE DETECTED !!!");

    Serial.println("Backward Left BLOCKED");


    ObstacleBeep();

    return;
  }


  analogWrite(enA, SPEED);

  analogWrite(enB, SPEED / speed_Coeff);


  digitalWrite(in1, LOW);

  digitalWrite(in2, HIGH);


  digitalWrite(in3, LOW);

  digitalWrite(in4, HIGH);
}


// ==========================================================================
// FORWARD RIGHT
// ==========================================================================

void ForwardRight()
{
  analogWrite(enA, SPEED / speed_Coeff);

  analogWrite(enB, SPEED);


  digitalWrite(in1, HIGH);

  digitalWrite(in2, LOW);


  digitalWrite(in3, HIGH);

  digitalWrite(in4, LOW);
}


// ==========================================================================
// BACKWARD RIGHT
// ==========================================================================

void BackwardRight()
{
  long distance = getDistance();


  if (distance <= STOP_DISTANCE)
  {
    Stop();

    Serial.println("!!! OBSTACLE DETECTED !!!");

    Serial.println("Backward Right BLOCKED");


    ObstacleBeep();

    return;
  }


  analogWrite(enA, SPEED / speed_Coeff);

  analogWrite(enB, SPEED);


  digitalWrite(in1, LOW);

  digitalWrite(in2, HIGH);


  digitalWrite(in3, LOW);

  digitalWrite(in4, HIGH);
}


// ==========================================================================
// STOP
// ==========================================================================

void Stop()
{
  analogWrite(enA, 0);

  analogWrite(enB, 0);


  digitalWrite(in1, LOW);

  digitalWrite(in2, LOW);


  digitalWrite(in3, LOW);

  digitalWrite(in4, LOW);
}


// ==========================================================================
// MANUAL BUZZER / HORN
// ==========================================================================

void BeepHorn()
{
  digitalWrite(buzPin, HIGH);

  delay(150);

  digitalWrite(buzPin, LOW);

  delay(80);
}
