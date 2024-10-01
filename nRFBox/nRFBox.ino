/* ____________________________
   This software is licensed under the MIT License:
   https://github.com/cifertech/nrfbox
   ________________________________________ */

#include <Arduino.h>
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include "icon.h"
#include "scanner.h"
#include "analyzer.h"
#include "jammer.h"
#include "blejammer.h"
#include "spoofer.h"
#include "sourapple.h"

//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0); // [full framebuffer, size = 1024 bytes]
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

Adafruit_NeoPixel pixels(1, 14, NEO_GRB + NEO_KHZ800);


const unsigned char* bitmap_icons[8] = {
  bitmap_icon_scanner,
  bitmap_icon_analyzer,
  bitmap_icon_jammer,
  bitmap_icon_ble_jammer,
  bitmap_icon_spoofer,
  bitmap_icon_apple,
  bitmap_icon_about
};

const int MENU_INITIAL = 0;

const int NUM_ITEMS = 7; 
const int MAX_ITEM_LENGTH = 20; 

char menu_items [NUM_ITEMS] [MAX_ITEM_LENGTH] = {  
  { "Scanner" }, 
  { "Analyzer" },
  { "Jammer" },
  { "BLE Jammer" },
  { "BLE Spoofer" }, 
  { "Sour Apple" },
  { "About" }
 };
 

#define BUTTON_UP_PIN 26 
#define BUTTON_SELECT_PIN 32
#define BUTTON_DOWN_PIN 33 

bool callAbout = true;

int button_up_clicked = 0; 
int button_select_clicked = 0; 
int button_down_clicked = 0; 

int item_selected = 0; 

int item_sel_previous; 
int item_sel_next; 

int current_screen = 0;  


void about() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(7, 15, "CiferTech@gmail.com");
  u8g2.drawStr(12, 35, "GitHub/cifertech");
  u8g2.drawStr(7, 55, "instagram/cifertech");
  u8g2.sendBuffer();
}


void setup() {

  u8g2.begin();
  u8g2.setBitmapMode(1);

  pixels.begin();

  u8g2.clearBuffer();

  //u8g2.drawXBMP(0, 0, 128, 64, logo_cifer);

  u8g2.setFont(u8g2_font_ncenB14_tr); 
  u8g2.setCursor(15, 35); 
  u8g2.print("nRF-BOX");
  
  u8g2.sendBuffer(); 
  delay(3000);

  pinMode(BUTTON_UP_PIN, INPUT_PULLUP); 
  pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP); 
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP); 

}


void checkAndCallMenu(int index,void(*function_setup)(),void(*function_call)()){
  if (current_screen == 0 && item_selected == index) {
      function_setup();
      while (item_selected == index) {
          if (digitalRead(BUTTON_SELECT_PIN) == HIGH) {
              function_call();
             if (callAbout) {                
                  callAbout = false;  // Toggle the state to not call about()
              } else {
                  break;  // Toggle the state to break the loop
                  callAbout = true;  // Reset the state for the next cycle
              }

              while (digitalRead(BUTTON_SELECT_PIN) == HIGH) {
                  // Wait for the button to be released
                  
                  if (callAbout = true){
                    break;
                  }
            }
          }
      }
  }
}


void checkState(){
 if (current_screen == 0) { // MENU SCREEN
    
      if ((digitalRead(BUTTON_UP_PIN) == LOW) && (button_up_clicked == 0)) { 
        item_selected = item_selected - 1; 
        button_up_clicked = 1; 
        if (item_selected < 0) { 
          item_selected = NUM_ITEMS-1;
        }
      }
      else if ((digitalRead(BUTTON_DOWN_PIN) == LOW) && (button_down_clicked == 0)) { 
        item_selected = item_selected + 1; 
        button_down_clicked = 1; 
        if (item_selected >= NUM_ITEMS) { 
          item_selected = 0;
          }
      } 

      if ((digitalRead(BUTTON_UP_PIN) == HIGH) && (button_up_clicked == 1)) { 
        button_up_clicked = 0;
      }
      if ((digitalRead(BUTTON_DOWN_PIN) == HIGH) && (button_down_clicked == 1)) { 
        button_down_clicked = 0;
      }
  }

}

void btnSelect(int number,int check){

  if ((digitalRead(BUTTON_SELECT_PIN) == LOW) && (button_select_clicked == check)) { 
     button_select_clicked = number; 
     }
}


void paginateMenu(){
  
  item_sel_previous = item_selected - 1;
  if (item_sel_previous < 0) {item_sel_previous = NUM_ITEMS - 1;} 
  item_sel_next = item_selected + 1;  
  if (item_sel_next >= NUM_ITEMS) {item_sel_next = 0;} 
}


void callAboutIfSelect(){
  if (current_screen == MENU_INITIAL && item_selected == 6) {
    while (item_selected == 6) {
        if (digitalRead(BUTTON_SELECT_PIN) == HIGH) {
            if (callAbout) {
                about();
                callAbout = false;  // Toggle the state to not call about()
            } else {
                break;  // Toggle the state to break the loop
                callAbout = true;  // Reset the state for the next cycle
            }

            while (digitalRead(BUTTON_SELECT_PIN) == HIGH) {
                // Wait for the button to be released
                if (callAbout = true){
                  break;
                }
            }
        }
    }
  }

}


void loop() {

 
  checkState();

  btnSelect(1,0);

  callAboutIfSelect();

  checkAndCallMenu(5,sourappleSetup,sourappleLoop);
  checkAndCallMenu(4,spooferSetup,spooferLoop);
  checkAndCallMenu(3,blejammerSetup,blejammerLoop);
  checkAndCallMenu(2,jammerSetup,jammerLoop);
  checkAndCallMenu(1,analyzerSetup,analyzerLoop);
  checkAndCallMenu(1,scannerSetup,scannerLoop);


  btnSelect(0,1);

  paginateMenu();


  u8g2.clearBuffer();  

    if (current_screen == MENU_INITIAL) { 
      
      u8g2.drawXBMP(0, 22, 128, 21, bitmap_item_sel_outline);

      u8g2.setFont(u8g_font_7x14);
      u8g2.drawStr(25, 15, menu_items[item_sel_previous]); 
      u8g2.drawXBMP( 4, 2, 16, 16, bitmap_icons[item_sel_previous]);          

      u8g2.setFont(u8g_font_7x14B);    
      u8g2.drawStr(25, 15+20+2, menu_items[item_selected]);   
      u8g2.drawXBMP( 4, 24, 16, 16, bitmap_icons[item_selected]);     

      u8g2.setFont(u8g_font_7x14);     
      u8g2.drawStr(25, 15+20+20+2+2, menu_items[item_sel_next]);   
      u8g2.drawXBMP( 4, 46, 16, 16, bitmap_icons[item_sel_next]);  

      u8g2.drawXBMP(128-8, 0, 8, 64, bitmap_scrollbar_background);

      u8g2.drawBox(125, 64/NUM_ITEMS * item_selected, 3, 64/NUM_ITEMS);             
    } 
    
  u8g2.sendBuffer(); 

}
