#include <avr/io.h>
#include <Wire.h>
#include <SPI.h>
#include <digitalWriteFast.h>
#include <GraphicsLib.h>
#include <MI0283QT9.h>
#include <Arduino.h>
#include "nunchuck_funcs.h"
#include <util/delay.h>
#include <stdlib.h>
#include "Game.h"
#include "Menu.h"
#include "Jump.h"
#include "Opmaak.h"
  

  
void Game::game(MI0283QT9 lcd) {
Jump J;
Menu M;
Opmaak O;

  lcd.fillScreen(RGB(255, 255, 255)); // scherm leeg
  lcd.drawText(10, 210, "Score:", RGB(0, 0, 0), RGB(255, 255, 255), 2);
  lcd.drawInteger(105, 210, M.score, DEC, RGB(0, 0, 0), RGB(255, 255, 255), 2);
  O.tekenLijn(lcd);
  nunchuck_setpowerpins();
  nunchuck_init();
  O.speler(lcd);
  O.drawMoeilijkheid(lcd);

  J.positionY = 160;
  O.obstakelLocatie1 = 0;
  O.obstakelLocatie2 = 0;
  O.aantalObstakels = 0;
  //J.in_air = false;
  
  while (gameIsLive) {
    nunchuck_get_data();
    J.zbutton = nunchuck_zbutton();
    
    //Serial.print(J.zbutton);
    
    O.randomLevel();
    vormObstakel1 = O.obstakelVorm1;
    locatieObstakel1 = O.obstakelLocatie1;
    yPosition = J.positionY;
    Serial.print(O.obstakelLocatie1);
    Serial.print(" - ");
    Serial.println(locatieObstakel1);
    Serial.print(O.obstakelVorm1);
    Serial.print(" - ");
    Serial.print(vormObstakel1);
    hitbox();
    O.sidescroll(lcd);
    
    O.teken(lcd);
    J.checkJump();
    J.tekenJump(lcd);
    
//    Serial.println(death);
//    Serial.println(J.positionY);
//    Serial.println(O.obstakelLocatie1);
//    Serial.print('\n');
//    _delay_ms(100);
    
    
    if (death) {
      gameIsLive = false;
    }
  }

  
}
void Game::hitbox() {
  Jump J;
  Opmaak O;

  Serial.print(" - ");
  Serial.print(O.obstakelVorm1);
  Serial.print(" - ");
  Serial.println(vormObstakel1);
  if (vormObstakel1 == 2) { //als eerste figuur == 4kant
    Serial.println("vorm1 == 2");
    if (47 > locatieObstakel1) {
      Serial.println("47 > O.Obstakel1");
      if (47 > locatieObstakel1 && yPosition > 128) {
        death = true;
      }
      if (yPosition <= 128) {
        J.velocityY = 0.0;
        J.in_air = false;
        J.positionY = 128;
        geland = true;
      }
    }
    if (32 < locatieObstakel1 && geland) {
     J.in_air = true;
    }
    if(yPosition > 160){
      J.in_air = false;
    }
  }
  if (O.obstakelVorm1 == 1) {
    if (47 > O.obstakelLocatie1) {
      if (J.positionY > currentY) {
        death = true;
      }
      if (O.obstakelLocatie1 < 31) {
        currentY += 2;
      } else {
        currentY -= 2;
      }
    }
  }
  if (currentY == 128) {
    currentY = 160;
  }
}