/*******************************************************************************
   main.c: Stopky
   Copyright (C) 2021 Brno University of Technology,
                      Faculty of Information Technology
   Author(s): David Hudak <xhudak03 AT fit.vutbr.cz>



*******************************************************************************/

#include <fitkitlib.h>
#include <keyboard/keyboard.h>
#include <lcd/display.h>


#include <string.h>



unsigned char clk_h, clk_m, clk_s;
unsigned short flag_timer;


char last_ch; //naposledy precteny znak
char char_cnt = 0;

void print_user_help(void) { }

unsigned char decode_user_cmd(char *cmd_ucase, char *cmd)
{
  return CMD_UNKNOWN;
}

void fpga_initialized()
{
  
	
}

enum states{ PAUSE, RUN, STOPPRINT, PRINT};
int run_bool = RUN; // Poèáteèní nastavení èinnosti stopek.
int stop_bool = PRINT;

void display_clock() {
  LCD_send_cmd(LCD_SET_DDRAM_ADDR, 0); // Pøevzato
  delay_ms(2);
  if(stop_bool == STOPPRINT) return;
  LCD_send_cmd((unsigned char)(clk_h / 10) + 48, 1); delay_ms(2);     // zobraz hh
  LCD_send_cmd((unsigned char)(clk_h % 10) + 48, 1); delay_ms(2); 
  LCD_send_cmd(0x3A, 1); delay_ms(2);                                 // :
  LCD_send_cmd((unsigned char)(clk_m / 10) + 48, 1); delay_ms(2);     // zobraz mm
  LCD_send_cmd((unsigned char)(clk_m % 10) + 48, 1); delay_ms(2);     
  LCD_send_cmd(0x3A, 1); delay_ms(2);                                 // :
  LCD_send_cmd((unsigned char)(clk_s / 10) + 48, 1); delay_ms(2);     // zobraz ss
  LCD_send_cmd((unsigned char)(clk_s % 10) + 48, 1); delay_ms(2);
}


unsigned char stack[9][3];
int stack_pointer = 0;
char fullness = 0;

void stack_init() // Inicializace zásobníku -- na poèátku jsou všechno nuly.
{ 
  unsigned char i, j;
  for(i = 0; i < 9; i++)
  {
    for(j = 0; j < 3; j++)
    {
      stack[i][j] = 0;
    }
  }
  stack_pointer = 0;
}

int keyboard_idle()
{
  char ch;
  ch = key_decode(read_word_keyboard_4x4());
  if (ch != last_ch) // osetreni drzeni klavesy
  {
    last_ch = ch;
    if (ch != 0) // pokud byla stisknuta klavesa
    {
      /*if (char_cnt > 31) {
         LCD_clear();
         char_cnt = 0;
      }*/
      if(ch == 'A') // Kontrola znakù -- pùvodnì bez prázných stavù, ale bez nich obèas kit házel neèekané výstupy.
      {
	
      }
      else if(ch == 'B') // Zmìò èinnost programu
      {
	if(run_bool == PAUSE)
	{
	  run_bool = RUN;
	}
	else
	{
	  run_bool = PAUSE;
	}	
      }
      else if(ch == 'C') // Vynulování èasu i zásobníku
      {
	clk_h = clk_m = clk_s = 0;
	stack_init();
      }
      else if(ch == 'D')
      {
	
      }
      else if(ch == '*')
      {
	stop_bool = STOPPRINT; // Teï se bude tisknout jenom zpráva -- zastav vypisování èasu
	LCD_clear();
	
	stack_pointer = (stack_pointer + 1) % 9; // Výpoèet nové hlavièky zásobníku -- hlavièka zahrnuje validní data.
	stack[stack_pointer][0] = clk_h;
	stack[stack_pointer][1] = clk_m;
	stack[stack_pointer][2] = clk_s;
	LCD_append_char('S'); // Tisk zprávy na výstup
	LCD_append_char('T');
	LCD_append_char('A');
	LCD_append_char('C');
	LCD_append_char('K');
	LCD_append_char('E');
	LCD_append_char('D');
	LCD_append_char(' ');
	LCD_append_char('T');
	LCD_append_char('I');
	LCD_append_char('M');
	LCD_append_char('E');
	delay_ms(700);
	LCD_clear();
	stop_bool = PRINT; // Nutno vrátit pùvodní stav
      }
      else if(ch >= '1' && ch <= '9' && run_bool == PAUSE) // Pokud dal uživatel pauzu, povol naètení ze zásobníku
      {
	int stack_tmp_pointer = (stack_pointer - (ch - '1'));
	if(stack_tmp_pointer < 0) // Zjisti vzdálenost od vrcholu -- pokud je záporná, pak je nutné èíslovat od konce pole.
	{
	  stack_tmp_pointer = 9 + stack_tmp_pointer; // 9 +- èíslo == 9 - èíslo
	}
	else
	{
	  stack_tmp_pointer = stack_tmp_pointer % 9; // Ochrana proti náhodnému pøeteèení
	}
	clk_h = stack[stack_tmp_pointer][0]; // Vytáhni hodnoty ze zásobníku
	clk_m = stack[stack_tmp_pointer][1];
	clk_s = stack[stack_tmp_pointer][2];
		
      }
      else if(ch == '0') // Vyèisti pouze zásobník
      {
	stack_init();
      }
      else if(ch == '#')
      {
	
      }
      display_clock(); // Pro jistotu zobraz výsledek
    }
  }
  
  return 0;
}

int main(void)
{
  run_bool = PAUSE;	
  initialize_hardware();
  char_cnt = 0;
  last_ch = 0;
  stack_init();
  keyboard_init();
  LCD_init();
  LCD_clear();
  LCD_send_cmd(LCD_DISPLAY_ON_OFF | LCD_DISPLAY_ON | LCD_CURSOR_OFF, 0); // vypni kurzor

  flag_timer = 0;
  WDTCTL = WDTPW + WDTHOLD;                // zastav watchdog

  P1DIR |= 0x01;                            // nastav P1.0 na vystup (budeme ovladat zelenou LED D5)

  CCTL0 = CCIE;                             // povol preruseni pro casovac (rezim vystupni komparace) 
  CCR0 = 0x0CCC;                            // nastav po kolika ticich (32768 = 0x8000, tj. za 1 s) ma dojit k preruseni
  TACTL = TASSEL_1 + MC_2;                  // ACLK (f_tiku = 32768 Hz = 0x8000 Hz), nepretrzity rezim

  P1OUT ^= 0x01;                            // inicializuj P1.0 na 1 (D5 zhasnuta)

  clk_h = 0;                                // inicializuj hodiny
  clk_m = 0;                                // inicializuj minuty
  clk_s = 0;                                // inicializuj sekundy

  while (1)
  {
    keyboard_idle();  // obsluha klavesnice
    terminal_idle();                       // obsluha terminalu
    if (flag_timer > 0) {
      display_clock();                    // zobraz aktualizovane hh:mm:ss
      flag_timer--;

    }
  } 
  
}

interrupt (TIMERA0_VECTOR) Timer_A (void)
{
  P1OUT ^= 0x01;      // invertuj bit P1.0 (kazdou 1/2 s)
  CCR0 += 0x8000;     // nastav po kolika ticich (32768 = 0x8000, tj. za 1 s) ma dojit k dalsimu preruseni

  if(run_bool == RUN) { clk_s++;}	
  if(clk_s>=60) { clk_m++; clk_s=0; }     // kazdou 60. sekundu inkrementuj minuty
  if(clk_m>=60) { clk_h++; clk_m=0; }     // kazdou 60. minutu inkrementuj hodiny
  if(clk_h>=24)                         // kazdych 24 hodin nuluj h,m,s
  {
    clk_h = clk_m = clk_s = 0;
  }
  flag_timer += 1;
}

