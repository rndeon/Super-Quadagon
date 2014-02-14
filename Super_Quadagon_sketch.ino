#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

int pinCS = 10; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 1;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
int holex=0;
int holey=0;
int playerPosition=0;
int playerx=2;
int playery=2;
int buttonleft=7;
int buttonright=6;
int buttonReset=5;
long wallInterval = 500;
long wallIntervalLevelMod = 20;
long previousWallTime = 0;
long playerInterval = 100;
long playerIntervalLevelMod = 5;
long previousPlayerTime = 0;
int shell_num = 0;
boolean previousGameOver =false;
long gameOverTime = 0;
long gameOverFlashInterval = 100;
boolean gameOverFlash=HIGH;
uint8_t level = 0;
void randomize_hole(){
  holex=random(0,2) * 4;
  holey=random(0,2) * 4;
}
void setup() {
  matrix.setIntensity(0);

// Adjust to your own needs
  matrix.setPosition(0, 0, 0); // The first display is at <0, 0>
//  matrix.setPosition(1, 1, 0); // The second display is at <1, 0>
//  matrix.setPosition(2, 2, 0); // The third display is at <2, 0>
//  matrix.setPosition(3, 3, 0); // And the last display is at <3, 0>
//  ...
  matrix.setRotation(0, 0);    // The first display is position upside down
//  matrix.setRotation(3, 2);    // The same hold for the last display
  
  randomize_hole();
  pinMode(buttonleft, INPUT);
  pinMode(buttonright, INPUT);    
  pinMode(buttonReset, INPUT);
}

void movePlayer()
{
  int leftState = digitalRead(buttonleft) == HIGH? 1 : 0; //int myInt = (someBool ? 1 : 0)
  int rightState = digitalRead(buttonright) == HIGH? 1 : 0;
   if( leftState == 1 ^ rightState == 1){ //XOR
     playerPosition=playerPosition + rightState - leftState;
     if (playerPosition < 0) { //modular 0 - 11
       playerPosition = 11;
     } else if (playerPosition > 11 ){
      playerPosition = 0;     
     }
//     Serial.println(playerPosition);
    if(playerPosition ==0 || playerPosition >= 9) {
      playerx = 2;
    } else if (playerPosition == 1 || playerPosition == 8){
      playerx = 3;
    }else if (playerPosition == 2 || playerPosition == 7){
      playerx = 4;
    }else if (playerPosition >=3 && playerPosition <=6)  {
      playerx=5;
    }
    if(playerPosition >=0 && playerPosition <= 3) {
        playery = 2;
      } else if (playerPosition == 11 || playerPosition == 4){
        playery = 3;
      }else if (playerPosition == 10 || playerPosition == 5){
        playery = 4;
      }else if (playerPosition >=6 && playerPosition <=9)  {
        playery=5;
     }  
   }
}
void drawScore(){
  if (level <= 7) {
      matrix.drawFastHLine(0,0,level+1, !gameOverFlash);
    } else {      
      matrix.drawFastHLine(0,0,8, !gameOverFlash);
    }
    if (level > 7 && level <=14){
      matrix.drawFastVLine(7,1,level-7, !gameOverFlash);
    } else if ( level > 7){
      matrix.drawFastVLine(7,1,7, !gameOverFlash);
    }
    if (level > 14 && level <= 21){
      matrix.drawLine(21-level,7,6,7,!gameOverFlash);
    } else if (level > 14) {
      matrix.drawFastHLine(0,7,7, !gameOverFlash);
    }
    if (level > 21){
      matrix.drawLine(0,28-level,0,6,!gameOverFlash);
    }
}
void drawToDisplay(boolean gameOverScreen){
  matrix.fillScreen(LOW);
  if(gameOverScreen && !previousGameOver){
 //  Serial.println("game over") ;
   gameOverTime = millis();
   previousGameOver = true;
  }
  if (previousGameOver){
    if (millis() >= gameOverTime + gameOverFlashInterval){
       gameOverFlash = ! gameOverFlash;
       gameOverTime = millis();
    }
    drawScore();
    matrix.drawRect(shell_num, shell_num,matrix.width() - 2*shell_num, matrix.height() - 2*shell_num , gameOverFlash);
  } else{
    //walls
    matrix.drawRect(shell_num, shell_num,matrix.width() - 2*shell_num, matrix.height() - 2*shell_num , HIGH);
    //hole
    matrix.fillRect(holex,holey,matrix.width()/2,matrix.height()/2,LOW);  
  }
    //player
    matrix.drawPixel(playerx,playery,HIGH);
  matrix.write(); // Send bitmap to display 
  
}
boolean playerIsInHole(){
  return (playerx >=holex && playerx < holex + matrix.width()/2) 
                 && (playery >=holey && playery < holey + matrix.height()/2) ;
}

void loop() {
  long currentTime = millis();
  boolean gameOver = false;
  if (!previousGameOver){
    //Update walls position
    if ( currentTime - previousWallTime >= wallInterval - wallIntervalLevelMod*level)
    {
      previousWallTime = currentTime;
      if (shell_num <3 ){
        shell_num++;
      } 
      else if (level >= 24){
        gameOver = true;//you win!
      }else {
        shell_num = 0;
        randomize_hole();
        level++;
        //Serial.println(level);
      }
    }
    //update player position
    if (currentTime - previousPlayerTime >=playerInterval - playerIntervalLevelMod*level)
    {
       movePlayer();
       previousPlayerTime = currentTime;
    }
    //check collision
    
    if(shell_num == 2 && !gameOver){
       gameOver = ! playerIsInHole();
    }
  
  }
  if (digitalRead(buttonReset) == HIGH){
    shell_num = 0;
    randomize_hole();
    previousGameOver = false;
    level=0;
  }
  drawToDisplay(gameOver);
}
