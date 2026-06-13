// Author: T TECH
// Project: Gravity Matrix
// Lisence: MIT License 
// Date Created: 08/03/2026
// Date Modified: 13/06/2026
/* _______              _______      _______      _______      __   __
/\__  __\            /\__  __\    /\    __\    /\   ___\    /\ \ /\ \
\/_/\ \_/            \/_/\ \_/    \ \   \_/_   \ \  \__/    \ \ \\_\ \
   \ \ \                \ \ \      \ \    __\   \ \  \       \ \  ___ \
    \ \ \                \ \ \      \ \   \_/_   \ \  \____   \ \ \_/\ \
     \ \_\                \ \_\      \ \______\   \ \______\   \ \_\\ \_\
      \/_/                 \/_/       \/______/    \/______/    \/_/ \/_/
*/

#include <Wire.h>
#include "LedControl.h"

// I2C address of the MPU-6050
const int MPU_addr=0x68;

// Create 16 bit variables for acceleration, temperature and gyro (x,y,z)
int16_t x_accel, y_accel, z_accel, temp, x_gyro, y_gyro, z_gyro;

// Create float for the gyro offset error (to subtract to increase acuracy)
float x_gyro_error = 0;
float y_gyro_error = 0;
float z_gyro_error = 0;

// Create float orientation (x,y,z)
float x_orient = 0; // Roll
float y_orient = 0; // Pitch 
float z_orient = 0; // Yaw

// Time between last data read from the gyro
unsigned long previous_time = 0;

// Gyro Sensitivty, divisor neede to convert raw gyro data into degress per second (from datasheet)
float gyro_scale = 131.0;

// Initialize the dot matrix
LedControl dot_matrix_1 = LedControl(12,10,11,1);

// Assign time between updates of the display
unsigned long delaytime1=500;
unsigned long delaytime2=50;

// Create a 8x8 matrix for the dot matrix grid to display
int matrix_grid_1 [8][8] = 
{
  {0,0,0,0,0,0,0,0}, 
  {0,0,0,0,0,0,0,0}, 
  {0,0,0,0,0,0,0,0}, 
  {0,0,0,0,0,0,0,0}, 
  {0,0,0,0,0,0,0,0}, 
  {0,0,0,0,0,0,0,0}, 
  {0,0,0,0,0,0,0,0}, 
  {0,0,0,0,0,0,0,0}, 
};

// User choice of simulation algorithms
int simulation_choice = -1;

// Setup
void setup()
{
  // Set up seiral monitor
  Serial.begin(9600);
  Serial.println(" ");

  // Setup pin 2 for the button
  pinMode(2, INPUT_PULLUP);

  // Set up wire library 
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  bool answered = false;

  // Ask the user which Simulation Algorithms they want to use until a valid response is given
  while (answered == false)
  {
    // Display the question
    Serial.println("Please Select a Simulation Algorithm:");
    Serial.println("[1] Particle Matrix Simulation");
    Serial.println("[2] Line Matrix Simulation");

    // Wait until data is received in the serial buffer
    while (Serial.available() == 0) 
    {

    }

    // Read integer value from serial stream
    simulation_choice = Serial.parseInt();

    // Clear any remaining bytes from the serial buffer
    while (Serial.available() > 0) 
    {
    Serial.read(); 
    }
    // Exit the loop if valid answer is given if not display an error message
    if (simulation_choice == 1 or simulation_choice == 2)
    {
      answered = true;
    }
    else
    {
      Serial.println("Error: Please enter a valid input");
    }
  }

  // Wait 1000ms before calibration
  delay(1000);

  // Calibrate the gyro
  gyro_calibrate();

  //Record the current time (ms) for the gyro time interval
  previous_time = millis();

  //The MAX72XX is in power-saving mode on startup, do a wakeup call
  dot_matrix_1.shutdown(0,false);
  //Set the brightness to a medium values
  dot_matrix_1.setIntensity(0,8);
  // and clear the display
  dot_matrix_1.clearDisplay(0);

  // Inialize the matrix with 32 dots 
  matrix_inialize(32);

  return;
}

// To calibrate the gyro, determine the error/drift over a period of time for which the gryo is staying still (gives gyro error), this can then be subtracted from the gyro when moving to reduce the drift error 
void gyro_calibrate()
{
  // Temp long to add up x,y,z axis readings
  long x_total_reading = 0;
  long y_total_reading = 0;
  long z_total_reading = 0;

  // Number of reading samples to take for the average drift/error calculation
  int samples = 1000;

  // Tell the user to hold the gyro still/flat
  Serial.println("IMU Sensor: Calibrating, Keep Sensor Still");

  // Run the loop for the amount of samples being taken
  for (int i = 0; i < samples; i++)
  {
    // Get the sensor data
    gyro_read(); 

    // Adds the current x,y,z reading to the sum
    x_total_reading = x_total_reading + x_gyro;
    y_total_reading = y_total_reading + y_gyro;
    z_total_reading = z_total_reading + z_gyro;

    // Delay (ms) so the same data doesn't get read twice
    delay(7.5);
  }

  // Calculate the average drift/error by divding the total sum by the number of samples
  x_gyro_error = x_total_reading / (float)samples; 
  y_gyro_error = y_total_reading / (float)samples; 
  z_gyro_error = z_total_reading / (float)samples; 

  Serial.println("IMU Sensor: Finished Calibrating");

  return;
}

// Read the data from the MPU-6050 GY-521 Module
void gyro_read()
{
  // Point to register 0x3B and keep connection active
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers

  // Gather all data from sensor
  x_accel = Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  y_accel = Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  z_accel = Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  temp = Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  x_gyro = Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  y_gyro = Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  z_gyro = Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    
  return;
}

// Inialize the matrix with dots from the bottom up
void matrix_inialize(int dots)
{
  // Loop through rows and columns to fill the grid
  for (int j = 0; j < 8; j++)
  {
    for (int i = 0; i < 8; i ++)
    {
      // If remaining spaces are less than or equal to required dots, set index to 1
      if ((64 - (j * 8) - i) <= dots)
      {
        matrix_grid_1[j][i] = 1;
      }
      else
      {
        matrix_grid_1[j][i] = 0;
      }
    }
  }

  return;
}

// Display a array matrix onto the LED Matrix
void matrix_display(LedControl dot_matrix) 
{
    int grid_rotate[8][8];

    // Rotate the matrix by 90 degrees counter clockwise
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            grid_rotate[i][j] = matrix_grid_1 [j][7 - i];
        }
    }

  // Create an array of byte for the dot matrix to display
  byte grid_display [8] = {B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111};

  // Go though the grid and set the grid to 0 or 1 (byte)
  for (int j = 0; j < 8; j++)
  {
    grid_display [j] = (grid_rotate[j][0] << 7) | (grid_rotate[j][1] << 6) | (grid_rotate[j][2] << 5) | (grid_rotate[j][3] << 4) | (grid_rotate[j][4] << 3) | (grid_rotate[j][5] << 2) | (grid_rotate[j][6] << 1) | (grid_rotate[j][7] << 0);
  }

  // Send the grid_display to the dot matrix
  dot_matrix.setRow(0,0,grid_display[0]);
  dot_matrix.setRow(0,1,grid_display[1]);
  dot_matrix.setRow(0,2,grid_display[2]);
  dot_matrix.setRow(0,3,grid_display[3]);
  dot_matrix.setRow(0,4,grid_display[4]);
  dot_matrix.setRow(0,5,grid_display[5]);
  dot_matrix.setRow(0,6,grid_display[6]);
  dot_matrix.setRow(0,7,grid_display[7]);

  return;
}

// Simulate the matrix based on a angle with particles
void particle_matrix_simulate(float angle)
{
  // Invert angle for correct visual rotation
  angle = angle * -1;

  int new_spot_x = 0;
  int new_spot_y = 0;
  float new_spot_score = 0;

  // Calculate X and Y gravity vector components using radians
  float gravity_x = sin(angle * 3.14159 / 180.0);
  float gravity_y = cos(angle * 3.14159 / 180.0);

  // Cycle thought each dot on the matrix
  for (int j = 0; j < 8; j++)
  {
    for (int i = 0; i < 8; i++)
    {
      if (matrix_grid_1[j][i] == 1)
      {
        new_spot_x = 0;
        new_spot_y = 0;
        new_spot_score = 0;

        // Check the 8 surrounding neighbor cells
        for (int k = -1; k < 2; k++)
        {
          for (int l = -1; l < 2; l++)
          {
            // Verify neighbor indices are within the 8x8 matrix bounds
            if (i+l >= 0 and i+l < 8 and j+k >= 0 and j+k < 8)
            {
              // Check if the target neighbor cell is empty
              if (matrix_grid_1[j+k][i+l] == 0)
              {
                // Calculate directional score based on gravity components
                float score = l * gravity_x + k * gravity_y;

                // Update the best target spot if score is higher
                if (score > new_spot_score)
                {
                  new_spot_score = score;
                  new_spot_x = l;
                  new_spot_y = k;
                }
              }
            }
          }
        }

        // Move the particle to the new neighbor spot if a valid higher score was found
        if ((new_spot_x != 0 or new_spot_y != 0) and new_spot_score > 0)
        {
          matrix_grid_1[j][i] = 0;
          matrix_grid_1[j + new_spot_y][i + new_spot_x] = 1;
        }

      }
    }
  }

  return;
}

// Simulate the matrix based on a angle with a line
void line_matrix_simulate(float angle_deg)
{
  // Clear the matrix before rendering the line
  matrix_inialize(0);

  // Convert angle from degrees to radians and find the slope
  float angle_rad = angle_deg * (3.14159256/180.0);
  float tangent_slope = tan(angle_rad);

  // Loop through grid to calculate line threshold positions
  for (int j = 0; j < 8; j++)
  {
    for (int i = 0; i < 8; i++)
    {
      // Calculate Y coordinate of the line relative to matrix center
      float line_y = tangent_slope * (i - 4) + 3.5;
    
      // Fill the matrix grid above or below the line depending on angle
      if (angle_deg >= -90.0 and angle_deg <= 90.0)
      {
        if (j >= line_y)
        {
          matrix_grid_1[j][i] = 1;
        }
      }
      else
      {
        if (j <= line_y)
        {
          matrix_grid_1[j][i] = 1;
        }
      }
    }
  }
  return;
}

// Loop
void loop()
{
  // Get the current time (ms) since the program started
  unsigned long current_time = millis();

  // Calculate the time interval and convert (ms to s)
  float interval_time = (current_time - previous_time) / 1000.0;

  // Update the previous time to the current time for the next interation
  previous_time = current_time;

  // Read the gyro
  gyro_read();
  
  // Remove the error/drift from each axis to get the true value
  float x_gyro_true = (float)x_gyro - x_gyro_error;
  float y_gyro_true = (float)y_gyro - y_gyro_error;
  float z_gyro_true = (float)z_gyro - z_gyro_error;

  // Divde the true values by the scale to get the spin speed in degrees per second
  x_gyro_true = x_gyro_true / gyro_scale;
  y_gyro_true = y_gyro_true / gyro_scale;
  z_gyro_true = z_gyro_true / gyro_scale;

  // Integrate (multiple by time) to get the change in angle which will be added to the previous angle
  x_orient = x_orient + x_gyro_true * interval_time;
  y_orient = y_orient + y_gyro_true * interval_time;
  z_orient = z_orient + z_gyro_true * interval_time;

  // Output the orientation to the serial monitor
  Serial.print("X Orientation: ");
  Serial.print(x_orient);
  Serial.print(" | Y Orientation: ");
  Serial.print(y_orient);
  Serial.print(" | Z Orientation: ");
  Serial.println(z_orient);

  // Check the selected simulation mode and process current orientation data
  if (simulation_choice == 1)
  {
    // Simulate the gravity using particle physics on the dot matrix with the coresponding angle
    particle_matrix_simulate(x_orient);
  }
  else if (simulation_choice == 2)
  {
    // Simulate the gravity using line physics on the dot matrix with the coresponding angle
    line_matrix_simulate(x_orient);
  }

  // Display the dot matrix
  matrix_display(dot_matrix_1);

  // Check if button on pin 2 is pressed to trigger a sensor reset
  if (digitalRead(2) == 0)
  {
    Serial.println("IMU Sensor: Sensor Reset");
    x_orient = 0;
    y_orient = 0;
    z_orient = 0;
    delay(50);
  }

  // Delay for 50ms to keep serial monitor readable and to avoid reading the same data twice
  delay(10);

  return;
}