/*=================================================================================
    Software Dispenser de Rem�dios
    TCC - T�cnico em Mecatr�nica
    
    Autor: Gustavo dos Santos Pereira
    
    Compilador: MikroC PRO for PIC
    PIC: 18F4550     48MHz
    
    Placa ustart PIC18F4550
    
    -----------------------------------------------------------------
    Blibiotecas do MikroC usadas:
    EEPROM;
    Lcd;
    -----------------------------------------------------------------
=================================================================================*/


//SA�DAS COMUNS
sbit LED  at PORTA.b3;
sbit LED2 at PORTA.b0;
sbit LED3 at PORTA.b1;
sbit LED4 at PORTA.b2;
sbit LED5 at PORTA.b4;
sbit SOM  at PORTB .b4;


//BITS SERVO MOTOR
sbit SM  at PORTB.b0;
sbit SM2 at PORTB.b1;
sbit SM3 at PORTB.b3;
sbit SM4 at PORTB.b2;

//BITS DO LCD
sbit LCD_RS at PORTD.b2;
sbit LCD_EN at PORTD.b3;
sbit LCD_D7 at PORTD.b7;
sbit LCD_D6 at PORTD.b6;
sbit LCD_D5 at PORTD.b5;
sbit LCD_D4 at PORTD.b4;

sbit LCD_LED at PORTC.b2;

//PINOS DO TRIS LCD
sbit LCD_RS_Direction at TRISD2_bit;
sbit LCD_EN_Direction at TRISD3_bit;
sbit LCD_D7_Direction at TRISD7_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D4_Direction at TRISD4_bit;


//ENTRADAS DOS BOTOES
sbit BOTAO1 at PORTC.b0;
sbit BOTAO2 at PORTC.b1;
sbit BOTAO3 at PORTA.b5;
sbit BOTAO4 at PORTC.b6;
sbit BOTAO5 at PORTC.b7;

//---------------------------
//----------Fun��es----------
//---------------------------
void ler_bot();                           //ler os bits dos bot�es
void disp();                              //controla o que � exibido no display
void num_un();                            //define as v�riaveis de exibi��o dos numeros para o display
void alarme();                            //alarme de aviso para quando acaba a contagem
void piscaLED();                          //controla os LEDs do circuito
void timebase();                          //contagem do dispenser n�1
void timebase2();                         //contagem do dispenser n�2
void timebase3();                         //contagem do dispenser n�3
void timebase4();                         //contagem do dispenser n�4
void toca_som();                          //controla o buzzer
void abre_mot();                          //define os bits de controle do motor n�1 para abertura
void abre_mot2();                         //define os bits de controle do motor n�2 para abertura
void abre_mot3();                         //define os bits de controle do motor n�3 para abertura
void abre_mot4();                         //define os bits de controle do motor n�4 para abertura
void fecha_mot();                         //define os bits de controle do motor n�1 para fechamento
void fecha_mot2();                        //define os bits de controle do motor n�2 para fechamento
void fecha_mot3();                        //define os bits de controle do motor n�3 para fechamento
void fecha_mot4();                        //define os bits de controle do motor n�4 para fechamento
void mot_aberto();                        //abre o motor n�1
void mot_aberto2();                       //abre o motor n�2
void mot_aberto3();                       //abre o motor n�3
void mot_aberto4();                       //abre o motor n�4
void mot_fechado();                       //fecha o motor n�1
void mot_fechado2();                      //fecha o motor n�2
void mot_fechado3();                      //fecha o motor n�3
void mot_fechado4();                      //fecha o motor n�4
void read_motbits();                      //ler os bits dos motores
void fast_incr();                         //incrementar num r�pidamente
void luz_lcd();                           //controlar a claridade do LCD

//--------------------------
//-------Vari�veis----------
//--------------------------
unsigned temp2        = 0x00,             //vari�vel de interrup��o do dispenser n�2
         temp         = 0x00,             //vari�vel de interrup��o do dispenser n�1
         temp3        = 0x00,             //vari�vel de interrup��o do dispenser n�3
         temp4        = 0x00,             //vari�vel de interrup��o do dispenser n�4
         temp_ligado  = 0x00,             //vari�vel de contagem ligada do dispenser n�1
         temp_ligado2 = 0x00,             //vari�vel de contagem ligada do dispenser n�2
         temp_ligado3 = 0x00,             //vari�vel de contagem ligada do dispenser n�3
         temp_ligado4 = 0x00,             //vari�vel de contagem ligada do dispenser n�4
         temp_led     = 0x00,             //vari�vel de temporiza��o do LED 1 (indica��o do dispenser n�1)
         temp_led2    = 0x00,             //vari�vel de temporiza��o do LED 2 (indica��o on/off do dispositivo)
         temp_led3    = 0x00,             //vari�vel de temporiza��o do LED 3 (indica��o do dispenser n�2)
         temp_led4    = 0x00,             //vari�vel de temporiza��o do LED 4 (indica��o do dispenser n�3)
         temp_led5    = 0x00,             //vari�vel de temporiza��o do LED 5 (indica��o do dispenser n�4)
         temp_som     = 0x00,             //vari�vel de temporiza��o do SOM
         temp_disp    = 0x00,             //vari�vel de temporiza��o do display
         x_mot        = 0x00,             //vari�vel de controle de vezes de repeti��o da fun��o do motor n�1
         x_mot2       = 0x00,             //vari�vel de controle de vezes de repeti��o da fun��o do motor n�2
         x_mot3       = 0x00,             //vari�vel de controle de vezes de repeti��o da fun��o do motor n�3
         x_mot4       = 0x00,             //vari�vel de controle de vezes de repeti��o da fun��o do motor n�4
         prog         = 0x00,             //vari�vel de controle do modo de programa��o
         num,      // = EEPROM_Read(0x01)   n�mero programado do dispenser n�1
         num2,     // = EEPROM_Read(0x02)   n�mero programado do dispenser n�2
         num3,     // = EEPROM_Read(0x05)   n�mero programado do dispenser n�3
         num4,     // = EEPROM_Read(0x07)   n�mero programado do dispenser n�4
         mult         = 0x00,             //vari�vel de compara��o com o temp_ligado (dispenser n�1)
         mult2        = 0x00,             //vari�vel de compara��o com o temp_ligado2 (dispenser n�2)
         mult3        = 0x00,             //vari�vel de compara��o com o temp_ligado3 (dispenser n�3)
         mult4        = 0x00,             //vari�vel de compara��o com o temp_ligado4 (dispenser n�4)
         vezes        = 0x00,             //vari�vel de vezes que o SOM toca
         option       = 0x00,             //vari�vel que define qual dispenser est� selecionado
         temp_inc     = 0x00,             //vari�vel de temporiza��o do bot�o para incremento r�pido
         temp_num     = 0x00,             //vari�vel de temporiza��o do numero para incremento r�pido
         temp_lcd     = 0x00,
         qtd_comp,   //= EEPROM_Read(0x09)   vari�vel para quantidade de comprimidos � despejar do dispenser n�1
         qtd_comp2,  //= EEPROM_read(0x0A)   vari�vel para quantidade de comprimidos � despejar do dispenser n�2
         qtd_comp3,  //= EEPROM_Read(0x0B)   vari�vel para quantidade de comprimidos � despejar do dispenser n�3
         qtd_comp4,  //= EEPROM_Read(0x0C)   vari�vel para quantidade de comprimidos � despejar do dispenser n�4
         comp         = 0x00,             //controle de vezes que o motor 1 despejar�
         comp2        = 0x00,             //controle de vezes que o motor 2 despejar�
         comp3        = 0x00,             //controle de vezes que o motor 3 despejar�
         comp4        = 0x00;             //controle de vezes que o motor 4 despejar�

bit      ligar,                           //bit de ligar/desligar a contagem
         un,       // = EEPROM_Read(0x03)   bit que define a unidade (dia/hora) do dispenser n�1
         un2,      // = EEPROM_Read(0x04)   bit que define a unidade (dia/hora) do dispenser n�2
         un3,      // = EEPROM_Read(0x06)   bit que define a unidade (dia/hora) do dispenser n�3
         un4,      // = EEPROM_Read(0x08)   bit que define a unidade (dia/hora) do dispenser n�4
         display,                         //bit high que configura o display
         display2,                        //bit low que configura o display
         toque,                           //bit que liga o alarme do dispenser n�1
         toque2,                          //bit que liga o alarme do dispenser n�2
         toque3,                          //bit que liga o alarme do dispenser n�3
         toque4,                          //bit que liga o alarme do dispenser n�4
         open_bit,                        //bit de controle de abertura do motor n�1
         open_bit2,                       //bit de controle de abertura do motor n�2
         open_bit3,                       //bit de controle de abertura do motor n�3
         open_bit4,                       //bit de controle de abertura do motor n�4
         close_bit,                       //bit de controle de fechamento do motor n�1
         close_bit2,                      //bit de controle de fechamento do motor n�2
         close_bit3,                      //bit de controle de fechamento do motor n�3
         close_bit4,                      //bit de controle de fechamento do motor n�4
         atv_mot,                         //bit de ativa��o do motor n�1
         atv_mot2,                        //bit de ativa��o do motor n�2
         atv_mot3,                        //bit de ativa��o do motor n�3
         atv_mot4,                        //bit de ativa��o do motor n�4
         b1_flag,                         //flag do bot�o 1
         b2_flag,                         //flag do bot�o 2
         b3_flag,                         //flag do bot�o 3
         b4_flag,                         //flag do bot�o 4
         b5_flag,                         //flag do bot�o 5
         lcdluz,                          //bit de luz do LCD
         block_cmd;                       //bit para bloqueio de fun��es
         fast_inc;                        //bit de ativa��o do incremento r�pido


char     dia[]        = "dia",            //vari�vel de exibi��o "dia"
         hora[]       = "h  ";            //vari�vel de exibi��o "h"
         
         
//================================================================================
//                            FUN��O DE INTERRUP��O
//================================================================================
void interrupt()                          //interrup��o
{

     if(TMR0IF_bit)                        //overflow em 100ms overflow
     {
      TMR0IF_bit =  0x00;                  //zera flag do timer0
      TMR0H      =  0xED;
      TMR0L      =  0xB1;                  //seta timer0 em 60849
      temp++;                              //incrementa temp
      temp2++;                             //incrementa temp2
      temp3++;                             //incrementa temp3
      temp4++;                             //incrementa temp4
      
      temp_led++;                         //incrementa temp_led
      temp_led2++;                        //incrementa temp_led2
      temp_led3++;                        //incrementa temp_led3
      temp_led4++;                        //incrementa temp_led4
      temp_led5++;                        //incrementa temp_led5
      temp_disp++;                        //incrementa temp_disp
      temp_som++;                         //incrementa temp_som
      temp_inc++;                         //incrementa temp_inc
      temp_lcd++;                         //incrementa temp_lcd
      temp_num++;                         //icrementa temp_num
      
     }                                     //end if TMR0IF



     piscaLED();                           //executa piscaLED
     timebase();                           //executa timebase
     timebase2();                          //executa timebase2
     timebase3();                          //executa timebase3
     timebase4();                          //executa timebase4


}                                          //end interrupt()

//======================================================================================
//                             FUN��O PRINCIPAL
//======================================================================================

void main (void)
{

// Registradores de Configura��o:

        ADCON1        =   0x0F;                   //Define os pinos como digital
        CMCON         =   0x07;                   //Desativa os comparadores

        TMR0H         =   0xED;
        TMR0L         =   0xB1;                   //Inicia timer0 em 60849


        INTCON.GIE    =   0x01;                   //Ativa interrup��o global
        INTCON.PEIE   =   0x01;                   //Ativa interrup��o por perif�ricos
        INTCON.TMR0IE =   0x01;                   //Ativa interrup��o por overflow

        TMR0IF_bit    =   0x00;                   //zera a flag do timer0


        INTCON2.RBPU  =   0x01;                   //Desliga os resistores de pull-ups do portB
        T0CON         =   0x87;                   //TMR0 16 bits, prescaler 1:256



        ligar         =   0x00;                   //     |
        block_cmd     =   0x00;                   //     |
        b1_flag       =   0x00;                   //     |
        b2_flag       =   0x00;                   //     |
        b3_flag       =   0x00;                   //     |
        b4_flag       =   0x00;                   //     |
        b5_flag       =   0x00;                   //     |
        display       =   0x00;                   //     |
        fast_inc      =   0x00;                   //     |
        toque         =   0x00;                   //     |
        toque2        =   0x00;                   //     |
        toque3        =   0x00;                   //     |
        toque4        =   0x00;                   //     |
        open_bit      =   0x00;                   //     |
        open_bit2     =   0x00;                   //     |
        open_bit3     =   0x00;                   //     |
        open_bit4     =   0x00;                   //     |
        close_bit     =   0x00;                   //     |
        close_bit2    =   0x00;                   //     |
        close_bit3    =   0x00;                   //     |
        close_bit4    =   0x00;                   //     |
        atv_mot       =   0x00;                   //     |
        atv_mot2      =   0x00;                   //     |
        atv_mot3      =   0x00;                   //     |
        atv_mot4      =   0x00;                   //    \ /
                                                  //     V
        SM            =   0x00;                   //zerando todos os bits
        SM2           =   0x00;
        SM3           =   0x00;
        SM4           =   0x00;
        
        LED2          =   0x00;
        LCD_LED       =   0x01;

        num           =   EEPROM_Read(0x01);      //l� os dados da EEPROM para vari�vel num
        un            =   EEPROM_Read(0x03);      //l� os dados da EEPROM para vari�vel un
        num2          =   EEPROM_Read(0x02);      //l� os dados da EEPROM para vari�vel num2
        un2           =   EEPROM_Read(0x04);      //l� os dados da EEPROM para vari�vel un2
        num3          =   EEPROM_Read(0x05);      //l� os dados da EEPROM para vari�vel num3
        un3           =   EEPROM_Read(0x06);      //l� os dados da EEPROM para vari�vel un3
        num4          =   EEPROM_Read(0x07);      //l� os dados da EEPROM para vari�vel num4
        un4           =   EEPROM_Read(0x08);      //l� os dados da EEPROM para vari�vel un4
        qtd_comp      =   EEPROM_Read(0x09);      //l� os dados da EEPROM para variavel qtd_comp
        qtd_comp2     =   EEPROM_Read(0x0A);      //l� os dados da EEPROM para variavel qtd_comp2
        qtd_comp3     =   EEPROM_Read(0x0B);      //l� os dados da EEPROM para variavel qtd_comp3
        qtd_comp4     =   EEPROM_Read(0x0C);      //l� os dados da EEPROM para variavel qtd_comp4
        
        if(qtd_comp==0xFF)qtd_comp=0x01;
        if(qtd_comp2==0xFF)qtd_comp2=0x01;
        if(qtd_comp3==0xFF)qtd_comp3=0x01;
        if(qtd_comp4==0xFF)qtd_comp4=0x01;

        TRISA = 0x20;                             //seta todos os bits do TRISA como sa�da
        TRISC = 0xC3;                             //seta os bits 0,1,2,6,7 como entrada
        TRISB = 0x00;                             //seta todos os bits do TRISB como sa�da

        LCD_Init();                               //inicia o LCD
        LCD_Cmd(_LCD_CLEAR);                      //limpa o LCD
        LCD_Cmd(_LCD_CURSOR_OFF);                 //desliga cursor do LCD
        

 while(1)                                         //loop infinito
 {
   ler_bot();                                     //executa ler_bot

   disp();                                        //executa disp
 }                                                //end while

}                                                 //end main

//===============================================================================
//                              FUN��O LER BOT�ES
//===============================================================================
void ler_bot()
{
  //____b1___
  if(!BOTAO1) b1_flag=0x01;                       //se bot�o1 for pressionado, flag do bot�o1 = 1

  if(BOTAO1 && b1_flag)                           //se bot�o1 estiver solto e flag do bot�o1 = 1
  {
   LCD_Cmd(_LCD_CLEAR);                           //limpa o LCD
   b1_flag = 0x00;                                //lmpa a flag do bot�o1
   LCD_LED = 0x01;                                //liga o LED do LCD
   if(!ligar)                                     //se bit ligar = 0
   {
    prog++;                                       //incrementa prog, muda a programa��o
    if(prog==4)                                   //se prog = 3
    {
     prog=0x00;                                   //zera a vari�vel prog
     EEPROM_Write(0x01,num);                      //grava num na EEPROM
     delay_ms(10);                                //espera 10 micro segundos
     EEPROM_Write(0x03,un);                       //grava un na EEPROM
     delay_ms(10);                                //espera 10 micro segundos
     EEPROM_Write(0x02,num2);                     //grava num2 na EEPROM
     delay_ms(10);                                //espera 10 micro segundos
     EEPROM_Write(0x04,un2);                      //grava un2 na EEPROM
     delay_ms(10);                                //espera 10 micro segundos
     EEPROM_Write(0x05,num3);                     //grava num3 na EEPROM
     delay_ms(10);                                //espera 10 micro segundos
     EEPROM_Write(0x06,un3);                      //grava un3 na EEPROM
     delay_ms(10);                                //espera 10 micro segundos
     EEPROM_Write(0x07,num4);                     //grava num4 na EEPROM
     delay_ms(10);                                //espera 10 micro segundos
     EEPROM_Write(0x08,un4);                      //grava un4 na EEPROM
     delay_ms(10);                                //espera 10 micro segundos
     EEPROM_Write(0x09,qtd_comp);                 //grava qtd_comp na EEPROM
     delay_ms(10);                                //espera 10 micro segundos
     EEPROM_Write(0x0A,qtd_comp2);                //grava qtd_comp2 na EEPROM
     delay_ms(10);                                //espera 10 micro segundos
     EEPROM_Write(0x0B,qtd_comp3);                //grava qtd_comp3 na EEPROM
     delay_ms(10);                                //espera 10 micro segundos
     EEPROM_Write(0x0C,qtd_comp4);                //grava qtd_comp4 na EEPROM
     delay_ms(10);                                //espera 10 micro segundos
     display=0x01;                                //bit display em 1
    
    }                                             //end if prog = 3
   
   }                                              //end if !ligar
  
  }                                               //end if BOTAO1 && b1_flag



  //___b2___
  if(!BOTAO2) 
  {
   b2_flag=0x01;                                  //se bot�o2 for pressionado, flag do bot�o2 = 1
   fast_incr();                                   //executa fast_incr
  }

  if(BOTAO2 && b2_flag)                           //se bot�o2 for solto e flag do bot�o2 for 1
  {
    LCD_Cmd(_LCD_CLEAR);                          //limpa LCD
    b2_flag    =  0x00;                           //limpa flag do bot�o
    fast_inc   =  0x00;                           //limpa bit de incremento r�pido
    LCD_LED = 0x01;                               //liga o LED do LCD
   if(option==0)                                  //dispenser n�1
   {
    if(prog==1)                                   //programa��o de numero
    {
     num++;                                       //incrementa num
    }
    if (prog==2)                                  //programa��o de unidade(hora/dia)
    {
      un = ~un;                                   //inverte un
    }                                             //end if prog==2
    
    if (prog==3)                                  //programa��o de quantidade de rem�dio
    {
     qtd_comp++;                                  //incrementa qtd_comp
    }

    if(qtd_comp>4) qtd_comp = 0x01;               //se quantidade de comprimidos passar de 4, retorna para 1
    if(num>24) num=0x00;                          //se o numero passar de 24, retorna para 0
    
   }                                              //end if option==0
   
   if(option==1)                                  //dispenser n�2
   {
    if(prog==1)                                   //programa��o do numero
    {
     num2++;                                      //incrementa num2
    }
    if (prog==2)                                  //programa��o de unidade(hora/dia)
    {
      un2 = ~un2;                                 //inverte un2
    }
    if (prog==3)                                  //programa��o de quantidade de rem�dio
    {
     qtd_comp2++;                                 //incrementa qtd_comp2
    }
    
    if(qtd_comp2>4) qtd_comp2 = 0x01;             //se quantidade de comprimidos passar de 4, retorna para 1
    if(num2>24) num2=0x00;                        //se o numero passar de 24, retorna para 0

   }                                              //end if option==1
   
   if(option==2)                                  //dispenser n�3
   {
    if(prog==1)                                   //programa��o do numero
    {
     num3++;                                      //incrementa num3
    }
    if (prog==2)                                  //programa��o de unidade(hora/dia)
    {
      un3 = ~un3;                                 //inverte un3
    }
    if (prog==3)                                  //programa��o de quantidade de rem�dio
    {
     qtd_comp3++;                                 //incrementa qtd_comp3
    }

    if(qtd_comp3>4) qtd_comp3 = 0x01;             //se quantidade de comprimidos passar de 4, retorna para 1
    if(num3>24) num3=0x00;                        //se o numero passar de 24, retorna para 0
    
   }                                              //end if option==2
   
   if(option==3)                                  //dispenser n�4
   {
    if(prog==1)                                   //programa��o do numero
    {
     num4++;                                      //incrementa num4
    }
    if (prog==2)                                  //programa��o de unidade(hora/dia)
    {
      un4 = ~un4;                                 //inverte un4
    }
    if (prog==3)                                  //programa��o de quantidade de rem�dio
    {
     qtd_comp4++;                                 //incrementa qtd_comp4
    }

    if(qtd_comp4>4) qtd_comp4 = 0x01;             //se quantidade de comprimidos passar de 4, retorna para 1
    if(num4>24) num4=0x00;                        //se o numero passar de 24, retorna para 0

   }                                              //end if option==3
   
   comp  = qtd_comp;
   comp2 = qtd_comp2;
   comp3 = qtd_comp3;
   comp4 = qtd_comp4;                             //igualando as variaveis para despejar o numero certo de comprimidos
   
  }                                               //end if BOTAO2 e b2_flag

  //___b3___
  if(!BOTAO3) b3_flag=0x01;                       //se bot�o3 for pressionado, flag do bot�o3 = 1

  if(BOTAO3 && b3_flag)                           //se bot�o3 for solto e flag do bot�o3 for 1
  {
   LCD_Cmd(_LCD_CLEAR);                           //limpa LCD
   b3_flag     =  0x00;                           //limpa flag do bot�o3
   LCD_LED = 0x01;                                //liga o LED do LCD
   if(option==0)                                  //dispenser n�1
   {
    if(prog==1)                                   //programa��o do numero
    {
     num--;                                       //decrementa num

    }
    if (prog==2)                                  //programa��o de unidade(hora/dia)
    {
     un = ~un;                                    //inverte un
    }
    if (prog==3)                                  //programa��o de quantidade de rem�dio
    {
     qtd_comp--;                                  //decrementa qtd_comp3
    }

    if(qtd_comp<1) qtd_comp = 0x04;               //se quantidade de comprimidos passar de 1, retorna para 4
    if(num>24) num=24;                            //se o numero for maior que 24, vai para 24
   }                                              //end if option==0
   
   if(option==1)                                  //dispenser n�2
   {
    if(prog==1)                                   //programa��o do n�mero
    {
     num2--;                                      //decrementa num2

    }
    if (prog==2)                                  //programa��o de unidade(hora/dia)
    {
     un2 = ~un2;                                  //inverte un2
    }
    if (prog==3)                                  //programa��o de quantidade de rem�dio
    {
     qtd_comp2--;                                 //decrementa qtd_comp2
    }

    if(qtd_comp2<1) qtd_comp2 = 0x04;             //se quantidade de comprimidos passar de 1, retorna para 4
    if(num2>24) num2=24;                          //se o numero for maior que 24, vai para 24
    
   }                                              //end if option==1
   
   if(option==2)                                  //dispenser n�3
   {
    if(prog==1)                                   //programa��o do n�mero
    {
     num3--;                                      //decrementa num3

    }
    if (prog==2)                                  //programa��o de unidade(hora/dia)
    {
     un3 = ~un3;                                  //inverte un3
    }
    if (prog==3)                                  //programa��o de quantidade de rem�dio
    {
     qtd_comp3--;                                 //decrementa qtd_comp3
    }

    if(qtd_comp3<1) qtd_comp3 = 0x04;             //se quantidade de comprimidos passar de 1, retorna para 4
    if(num3>24) num3=24;                          //se o numero for maior que 24, vai para 24
    
   }                                              //end if option==2
   
   if(option==3)                                  //dispenser n�4
   {
    if(prog==1)                                   //programa��o do n�mero
    {
     num4--;                                      //decrementa num4

    }
    if (prog==2)                                  //programa��o de unidade(hora/dia)
    {
     un4 = ~un4;                                  //inverte un4
    }
    if (prog==3)                                  //programa��o de quantidade de rem�dio
    {
     qtd_comp4--;                                 //decrementa qtd_comp4
    }

    if(qtd_comp4<1) qtd_comp4 = 0x04;             //se quantidade de comprimidos passar de 1, retorna para 4

    if(num4>24) num4=24;                          //se o numero for maior que 24, vai para 24

   }                                              //end if option==3
   
   comp  = qtd_comp;
   comp2 = qtd_comp2;
   comp3 = qtd_comp3;
   comp4 = qtd_comp4;                             //igualando as variaveis para despejar o numero certo de comprimidos
   
  }                                               //end if bot�o3 e b3_flag

  //___b4___
  if(!BOTAO4) b4_flag=0x01;                       //se bot�o4 for pressionado, flag do bot�o4 = 1

  if(BOTAO4 && b4_flag)                           //se bot�o4 for solto e flag do bot�o4 for 1
  {
    LCD_Cmd(_LCD_CLEAR);                          //limpa LCD
    b4_flag    =  0x00;                           //limpa flag do bot�o4
   if(num!=0 || num2!=0 || num3!=0 || num4!=0)    //se num, num2, num3 ou num4 for diferente 0
   {
    if(prog==0)                                   //se prog = 0
    {
     if(!toque && !toque2 && !toque3 && !toque4)  //se nenhum contador tiver finalizado...
     {
      if(!ligar)                                  //se bit de ligar = 0 (desligado)
      {
       temp =0x00;                                //zera temp
       temp2=0x00;                                //zera temp2
       temp3=0x00;                                //zera temp3
       temp4=0x00;                                //zera temp4
      
      }                                           //end if !ligar
     ligar = ~ligar;                              //inverte ligar
     if(!ligar) display2 = 0x01;                  //se ligar for 0, bit display2 = 1
     
     }                                            //end if !toque && !toque2 && !toque3 && !toque4
     
    }                                             //end if prog==0
    
   }                                              //end if num!=0 || num2!=0 ||num3!=0 || num4!=0
   
  }                                               //end if BOTAO4 && b4_flag

  if(!un)                                         //se un = 0
  {
   mult = num * 1;                                //mult � num * 3600 (hora)
  }
  if(un)                                          //se un = 1
  {
   mult = num * 5;                                //mult � num * 86400 (dia)
  }
  
  if(!un2)                                        //se un2 = 0
  {
   mult2 = num2 * 1;                              //mult2 � num2 * 3600 (hora)
  }
  if(un2)                                         //se un2 = 1
  {
   mult2 = num2 * 5;                              //mult2 � num2 * 86400 (dia)
  }
  
  if(!un3)                                        //se un3 = 0
  {
   mult3 = num3 * 1;                              //mult3 � num3 * 3600 (hora)
  }
  if(un3)                                         //se un3 = 1
  {
   mult3 = num3 * 5;                              //mult3 � num3 * 86400 (dia)
  }
  
  if(!un4)                                        //se un4 = 0
  {
   mult4 = num4 * 1;                              //mult4 � num4 * 3600 (hora)
  }
  if(un4)                                         //se un4 = 1
  {
   mult4 = num4 * 5;                              //mult4 � num4 * 86400 (dia)
  }

  //___b5___                                        botao para trocar de contador
  if(!BOTAO5) 
  {
   b5_flag = 0x01;                                //se bot�o5 for pressionado, flag do bot�o5 = 1
   luz_lcd();                                     //executa a fun��o de controle da luminosidade do display

  }                                               //end if !BOTAO5

  if(BOTAO5 && b5_flag)                           //se bot�o5 for solto e flag do bot�o5 for 1
  {
   LCD_Cmd(_LCD_CLEAR);                           //limpa LCD
   b5_flag = 0x00;                                //limpa flag do bot�o5
   if(!block_cmd)                                 //se bloqueio desativado
   {
    if(prog != 0)                                 //se prog for diferente de 0
    {
     option++;                                    //incrementa option
     if(option == 4) option=0x00;                 //se option for 4, option = 0
    }
   }                                              //end if !block_cmd
   
     if(toque || toque2 || toque3 || toque4)      //se algum comando do SOM estiver ativado...
     {                                            //(desligar despertador)
     if(toque)temp_ligado=0x00;                   //se for o toque 1, zera o contador 1
     if(toque2)temp_ligado2=0x00;                 //se for o toque 2, zera o contador 2
     if(toque3)temp_ligado3=0x00;                 //se for o toque 3, zera o contador 3
     if(toque4)temp_ligado4=0x00;                 //se for o toque 4, zera o contador 4
      toque =0x00;
      toque2=0x00;
      toque3=0x00;
      toque4=0x00;                                //zera todos os bits de SOM
      SOM=0x00;

     }                                            //end if toque || toque2 || toque3 || toque4
    

   block_cmd = 0x00;                              //desativa o bloqueio
   temp_lcd = 0x00;                               //zera o temp_lcd
  }                                               //end if BOTAO5 && b5_flag

}                                                 //end ler_bot()


//================================================================================
//               FUN��O PARA INCREMENTAR N�MEROS RAPIDAMENTE
//================================================================================
void fast_incr()
{
  if(temp_inc>7)temp_inc=0x00;                     //se temp_inc maior que 7, zera temp_inc
  if(temp_inc==7)                                  //conta 700ms...
  {
   temp_inc=0x00;                                  //zera temp_inc
   fast_inc=0x01;                                  //ativa fast_inc
  }                                                //end if temp_inc==7
 
 if(fast_inc)                                     //se fast_inc for ativado...
 {
  if(option==0 && prog==1)                        //se dispenser n�1 selecionado e program��o de n�mero
  {
   if(temp_num>5)temp_num=0x00;                   //se temp_num maior que 5, zera temp_num
   if(temp_num==5)                                //conta 500ms...
   {
    temp_num=0x00;                                //zera temp_num
    num += 2;                                     //acrescenta 2 em num
    
   }                                              //end if temp_num==5
   
  }                                               //end if option==0 && prog=1
  
  
  if(option==1 && prog==1)                        //se dispenser n�2 selecionado e program��o de n�mero
  {
   if(temp_num>5)temp_num=0x00;                   //se temp_num maior que 5, zera temp_num
   if(temp_num==5)                                //conta 500ms...
   {
    temp_num=0x00;                                //zera temp_num
    num2 += 2;                                    //acrescenta 2 em num2

   }                                              //end if temp_num==5

  }                                               //end if option==1 && prog=1
  
   if(option==2 && prog==1)                       //se dispenser n�3 selecionado e program��o de n�mero
  {
   if(temp_num>5)temp_num=0x00;                   //se temp_num maior que 5, zera temp_num
   if(temp_num==5)                                //conta 500ms...
   {
    temp_num=0x00;                                //zera temp_num
    num3 += 2;                                    //acrescenta 2 em num3

   }                                              //end if temp_num==5

  }                                               //end if option==2 && prog=1
  
   if(option==3 && prog==1)                       //se dispenser n�4 selecionado e program��o de n�mero
  {
   if(temp_num>5)temp_num=0x00;                   //se temp_num maior que 5, zera temp_num
   if(temp_num==5)                                //conta 500ms...
   {
    temp_num=0x00;                                //zera temp_num
    num4 += 2;                                    //acrescenta 2 em num4

   }                                              //end if temp_num==5

  }                                               //end if option==3 && prog=1
  
  if(num >24) num =0x00;                          //se o numero passar de 24, retorna para 0
  if(num2>24) num2=0x00;                          //se o numero passar de 24, retorna para 0
  if(num3>24) num3=0x00;                          //se o numero passar de 24, retorna para 0
  if(num4>24) num4=0x00;                          //se o numero passar de 24, retorna para 0
  
 }                                                //end if fast_inc
 
 

 
}                                                 //end void fast_incr()

//================================================================================
//                        FUN��O DE CONTROLE DA LUMINOSIDADE
//                                  DO DISPLAY LCD
//================================================================================
void luz_lcd()
{
 if(temp_lcd > 15)  temp_lcd = 0x00;
 if(temp_lcd == 15)                               //se o bot�o for segurado por ~= 1500ms
 {
  LCD_LED  = ~LCD_LED;                            //inverte a sa�da do LED do LCD
  temp_lcd = 0x00;                                //zera o temp_lcd
  block_cmd = 0x01;                               //bloqueia a a��o do bot�o
  
 }                                                //end if temp_lcd

}                                                 //end void luz_lcd

//================================================================================
//                      FUN��O DE EXIBI��O DO DISPLAY
//================================================================================
void disp()
{
  if(!ligar && prog==0)                           //se ligar for 0 e prog for 0
  {
   if(SM) SM   = 0x00;
   if(SM2)SM2  = 0x00;
   if(SM3)SM3  = 0x00;
   if(SM4)SM4  = 0x00;
   if(!display && !display2)                      //se os bits diplay e display2 forem 0
   {
     LCD_Out(1,1,"  DISPENSER DE  ");             //exibe "DISPENSER DE"
     LCD_Out(2,1,"    REMEDIOS    ");             //      "  REMEDIOS  "

    }
   else                                           //sen�o
   {
    if(display && !display2)                      //se bits display for 1 e display2 for 0
    {
     display=0x00;                                //limpa bit display
     LCD_Out(1,1,"ETEC LAURO GOMES");             //exibe "ETEC LAURO GOMES"
     LCD_Out(2,1,"  3A MECA 2021  ");             //      "  3A MECA 2021  "
     delay_ms(1000);                              //espera 1 segundo
    }
    if(!display && display2)                      //se os bits display for 0 e display2 for 1
    {
     display2=0x00;                               //limpa bit display2
     LCD_Out(1,1,"     PARANDO    ");             //exibe "     PARANDO    "
     LCD_Out(2,1,"     CONTAGEM   ");             //      "     CONTAGEM   "
     delay_ms(1000);                              //espera 1 segundo
    }
   }                                              //end else
  }                                               //end if !ligar && prog==0

  if(prog==1)                                     //programa��o de numeros
  {
   LCD_Out(1,1,"PROGRAME NUMERO:");               //exibe "PROGRAME NUMERO:
   num_un();                                      //executa num_un
   if(option==0) LCD_Chr(2,1,'1');               //se option for 0, exibe "1"
   if(option==1) LCD_Chr(2,1,'2');               //se option for 1, exibe "2"
   if(option==2) LCD_Chr(2,1,'3');               //se option for 2, exibe "3"
   if(option==3) LCD_Chr(2,1,'4');               //se option for 2, exibe "4"
  }

  if(prog==2)                                     //se prog for 2
  {
   LCD_Out(1,1,"PROGRAME Un.:");                  //exibe "PROGRAME Un.:"
   num_un();                                      //executa num_un
   if(option==0) LCD_Chr(2,1,'1');               //se option for 0, exibe "1"
   if(option==1) LCD_Chr(2,1,'2');               //se option for 1, exibe "2"
   if(option==2) LCD_Chr(2,1,'3');               //se option for 2, exibe "3"
   if(option==3) LCD_Chr(2,1,'4');               //se option for 2, exibe "4"
  }
  
  if(prog==3)                                     //programa��o de quantidade de comprimidos
  {
   LCD_Out(1,1,"Qtd. COMPRIMIDOS");               //exibe "Qtd. COMPRIMIDOS"
   num_un();                                      //executa num_un
   if(option==0) LCD_Chr(2,1,'1');               //se option for 0, exibe "1"
   if(option==1) LCD_Chr(2,1,'2');               //se option for 1, exibe "2"
   if(option==2) LCD_Chr(2,1,'3');               //se option for 2, exibe "3"
   if(option==3) LCD_Chr(2,1,'4');               //se option for 2, exibe "4"
  }

  if(ligar)                                       //se ligar for 1 (ligado)
  {
   LCD_Out(1,1,"DISP  TEMPO  Qtd");               //exibe "CONTANDO:   DISP";
   if(temp_disp>20) temp_disp=0x00;               //se temp_disp for maior que 20, zera temp_disp
   if(temp_disp == 20)                            //se temp_disp for 20 (2 segundos)
   {
    temp_disp=0x00;                               //zera temp_disp
    option++;                                     //incrementa option
    if(option == 4) option=0x00;                  //se option for 3, option = 0
    if(option==0) LCD_Chr(2,1,'1');              //se option for 0, exibe "1"
    if(option==1) LCD_Chr(2,1,'2');              //se option for 1, exibe "2"
    if(option==2) LCD_Chr(2,1,'3');              //se option for 2, exibe "3"
    if(option==3) LCD_Chr(2,1,'4');              //se option for 2, exibe "4"
   }
   num_un();                                      //executa num_un
  }

}                                                 //end disp()

//==================================================================================
//                           FUN��O N�MERO E UNIDADE
//                                 FORMATA��O
//==================================================================================
void num_un()
{
 char dig2,dig1,digcomp;                          //cria vari�veis d�gito 2 e d�gito 1

 if(option==0)                                    //se option for 0 (dispenser n� 1 selecionado)
 {
  dig2 = num/10;                                  //d�gito 2 � igual a num dividido por 10
  dig1 = num%10;                                  //digito 1 � igual a sobra da divis�o de num por 10

  LCD_Chr(2,7,dig2+0x30);                         //soma digito 2 com 30 hexa para exibi��o no display
  LCD_Chr_Cp (dig1+0x30);                         //soma digito 1 com 30 hexa para exibi��o no display

  if(!un) LCD_Out(2,10,hora);                      //se un for zero, exibe "hora"

  if(un) LCD_Out(2,10,dia);                        //se un for um, exibe "dia"
  
 }                                                //end if option==0
 

 if(option==1)                                    //se option for 1 (dispenser n�2 selecionado)
 {
  dig2 = num2/10;                                 //digito 2 � igual a num2 dividido por 10
  dig1 = num2%10;                                 //digito 1 � igual a sobra da divis�o de num2 por 10

  LCD_Chr(2,7,dig2+0x30);                         //soma digito 2 com 30 hexa para exibi��o no display
  LCD_Chr_Cp (dig1+0x30);                         //soma digito 1 com 30 hexa para exibi��o no display

  if(!un2) LCD_Out(2,10,hora);                     //se un2 for zero, exibe "hora"

  if(un2) LCD_Out(2,10,dia);                       //se un2 for um, exibe "dia"
  
 }                                                //end if option==1
 
 if(option==2)                                    //se option for 2 (dispenser n�3 selecionado)
 {
  dig2 = num3/10;                                 //digito 2 � igual a num3 dividido por 10
  dig1 = num3%10;                                 //digito 1 � igual a sobra da divis�o de num3 por 10

  LCD_Chr(2,7,dig2+0x30);                         //soma digito 2 com 30 hexa para exibi��o no display
  LCD_Chr_Cp (dig1+0x30);                         //soma digito 1 com 30 hexa para exibi��o no display

  if(!un3) LCD_Out(2,10,hora);                     //se un3 for zero, exibe "hora"

  if(un3) LCD_Out(2,10,dia);                       //se un3 for um, exibe "dia"

 }                                                //end if option==2
 
 if(option==3)                                    //se option for 3 (dispenser n�4 selecionado)
 {
  dig2 = num4/10;                                 //digito 2 � igual a num4 dividido por 10
  dig1 = num4%10;                                 //digito 1 � igual a sobra da divis�o de num4 por 10

  LCD_Chr(2,7,dig2+0x30);                         //soma digito 2 com 30 hexa para exibi��o no display
  LCD_Chr_Cp (dig1+0x30);                         //soma digito 1 com 30 hexa para exibi��o no display

  if(!un4) LCD_Out(2,10,hora);                     //se u4 for zero, exibe "hora"

  if(un4) LCD_Out(2,10,dia);                       //se un4 for um, exibe "dia"

 }                                                //end if option==3
 

  if(option==0)                                   //se dispenser n�1 selecionado
  {
   digcomp = qtd_comp;                            //digito � igual a quantidade de compimidos a ser despejado
   LCD_Chr(2,15,digcomp+0x30);                    //soma digito 1 com 30 hexa para exibi��o no display
  }
  
  if(option==1)                                   //se dispenser n�2 selecionado
  {
   digcomp = qtd_comp2;                           //digito � igual a quantidade de compimidos a ser despejado
   LCD_Chr(2,15,digcomp+0x30);                    //soma digito 1 com 30 hexa para exibi��o no display
  }
  
  if(option==2)                                   //se dispenser n�3 selecionado
  {
   digcomp = qtd_comp3;                           //digito � igual a quantidade de compimidos a ser despejado
   LCD_Chr(2,15,digcomp+0x30);                    //soma digito 1 com 30 hexa para exibi��o no display
  }
  
  if(option==3)                                   //se dispenser n�4 selecionado
  {
   digcomp = qtd_comp4;                           //digito � igual a quantidade de compimidos a ser despejado
   LCD_Chr(2,15,digcomp+0x30);                    //soma digito 1 com 30 hexa para exibi��o no display
  }

 
}                                                 //end num_un()


//=================================================================================
//                        FUN��O DE CONTROLE DOS LEDS
//=================================================================================
void piscaLED()
{

  if(ligar && !toque && num!=0)                   //se ligar for um(ligado), toque for zero e num diferente de zero...
  {                                               //ou seja, se o dispenser n�1 estiver ligado...
   if(temp_led>=10)                               //conta 1 segundo
   {
            temp_led = 0x00;                      //zera temp_led
            LED  = ~LED;                          //inverte LED de indica��o do dispenser n�1
   }                                              //end if temp_led>=10

  }                                               //end if ligar && !toque && num!=0

  else                                            //sen�o...
  {
   if(ligar && toque)                             //se ligar for 1 e toque for 1
   {                                              //ou seja, se o tempo do contador n� 1 acabou...
    if(temp_led>=2)                               //conta 200ms
    {
     temp_led=0x00;                               //zera temp_led
     LED = ~LED;                                  //inverte LED de indica��o do dispenser n�1
                                                  //(pisca o LED a cada 200 milisegundos
    }                                             //end if temp_led>=2

   }                                              //end if ligar && toque
   else                                           //sen�o...
   {
    if(prog>0 && option==0)                       //se estiver em programa��o e dispenser n�1 selecionado
    {
     LED = 0x01;                                  //LED do dispenser n�1 aceso
    }                                             //end if prog>0 && option==0

    else LED = 0x00;                              //sen�o, LED do dispenser n�1 desligado
    
   }                                              //end else
   
  }                                               //end else



 if(temp_led2 ==5)                                //conta 500ms
 {                                                //para o LED de indica��o geral
  temp_led2=0x00;                                 //zera temp_led2
  LED2 = ~LED2;                                   //inverte estado do LED de indica��o geral
  
 }                                                //end if temp_led==5
 
 if(ligar && !toque2 && num2!=0)                  //se ligar for 1, toque 2 for 0 e num2 diferente de 0...
 {                                                //ou seja, se o dispenser n�2 for ligado...
  if(temp_led3>=10)                               //conta 1 segundo
  {
   temp_led3 = 0x00;                              //zera temp_led3
   LED3 = ~LED3;                                  //inverte estado do LED de indica��o do dispenser n�2
   
  }                                               //end if temp_led3>=10
  
 }                                                //end if ligar && !toque2 && num2!=0
 
 else                                             //sen�o...
 {
  if(ligar && toque2)                             //se ligar e toque2 for 1
  {                                               //ou seja, o contador do dispenser n�2 acabou
   if(temp_led3>=2)                               //conta 200 milisegundos
   {
    temp_led3=0x00;                               //zera temp_led3
    LED3 = ~LED3;                                 //inverte estado do LED de indica��o do dispenser n�2
    
   }                                              //end if temp_led3>=2
   
  }                                               //end if ligar && toque2
  
  else
  {
   if(prog>0 && option==1)                        //se estiver em programa��o e dispenser n�2 selecionado
   {
    LED3 = 0x01;                                  //LED do dispenser n�2 aceso
   }                                              //end if prog>0 && option==1

  else LED3 = 0x00;                               //sen�o, LED do dispenser n�2 desligado
  
  }                                               //end else
  
 }                                                //end else
 
 if(ligar && !toque3 && num3!=0)                  //se ligar for 1, toque3 for 0 e num3 diferente de 0...
 {                                                //ou seja, se o dispenser n�3 for ligado...
  if(temp_led4>=10)                               //conta 1 segundo
  {
   temp_led4 = 0x00;                              //zera temp_led4
   LED4 = ~LED4;                                  //inverte estado do LED de indica��o do dispenser n�3

  }                                               //end if temp_led4>=10

 }                                                //end if ligar && !toque3 && num3!=0

 else                                             //sen�o...
 {
  if(ligar && toque3)                             //se ligar e toque3 for 1
  {                                               //ou seja, o contador do dispenser n�3 acabou
   if(temp_led4>=2)                               //conta 200 milisegundos
   {
    temp_led4=0x00;                               //zera temp_led4
    LED4 = ~LED4;                                 //inverte estado do LED de indica��o do dispenser n�3

   }                                              //end if temp_led4>=2

  }                                               //end if ligar && toque3

 else
  {
   if(prog>0 && option==2)                        //se estiver em programa��o e dispenser n�3 selecionado
   {
    LED4 = 0x01;                                  //LED do dispenser n�3 aceso
   }                                              //end if prog>0 && option==2

  else LED4 = 0x00;                               //sen�o, LED do dispenser n�3 desligado

  }                                               //end else

 }                                                //end else
 
 
 if(ligar && !toque4 && num4!=0)                  //se ligar for 1, toque4 for 0 e num4 diferente de 0...
 {                                                //ou seja, se o dispenser n�4 for ligado...
  if(temp_led5>=10)                               //conta 1 segundo
  {
   temp_led5 = 0x00;                              //zera temp_led5
   LED5 = ~LED5;                                  //inverte estado do LED de indica��o do dispenser n�4

  }                                               //end if temp_led5>=10

 }                                                //end if ligar && !toque4 && num4!=0

 else                                             //sen�o...
 {
  if(ligar && toque4)                             //se ligar e toque4 for 1
  {                                               //ou seja, o contador do dispenser n�4 acabou
   if(temp_led5>=2)                               //conta 200 milisegundos
   {
    temp_led5=0x00;                               //zera temp_led5
    LED5 = ~LED5;                                 //inverte estado do LED de indica��o do dispenser n�4

   }                                              //end if temp_led5>=2

  }                                               //end if ligar && toque4

 else
  {
   if(prog>0 && option==3)                        //se estiver em programa��o e dispenser n�4 selecionado
   {
    LED5 = 0x01;                                  //LED do dispenser n�4 aceso
   }                                              //end if prog>0 && option==3

  else LED5 = 0x00;                               //sen�o, LED do dispenser n�4 desligado

  }                                               //end else

 }                                                //end else
 
}                                                 //end piscaLED()


//================================================================================
//                      FUN��O DE EXECU��O DO CONTADOR
//                             -DISPENSER N�1-
//================================================================================
void timebase()
{
  if(!ligar)                                      //se ligar for 0
  {                                               //ou seja, se contador desligado...
   temp_ligado = 0x00;                            //zera temp_ligado
  }
  if(ligar && num!=0)                             //se ligar for 1 e num diferente de 0...
  {                                               //ativa o contador
   if(temp==10)                                   //conta 1 segundo
   {
    temp = 0x00;                                  //zera temp
    temp_ligado++;                                //incrementa temp_ligado a cada 1 segundo
    
   }                                              //end if temp==10
   
   if(temp_ligado==mult)                          //compara a igualdade de temp_ligado com mult
   {                                              //o contador atingiu o tempo programado...
    toque = 0x01;                                 //liga o bit toque
    atv_mot = 0x01;                               //liga o bit de ativa��o do motor
    comp = qtd_comp;
    abre_mot();                                   //define os bits de controle do motor para abri-lo
    
   }                                              //end if temp_ligado==mult
   
   if(atv_mot)                                    //se ativa��o do motor for ligada...
   {
    read_motbits();                               //executa a leitura dos bits de controle dos motores

   }                                              //end if atv_mot
   
   else                                           //sen�o...
   {
    alarme();                                     //executa o alarme
   }
  }                                               //end if ligar
  
}                                                 //end timebase()


//================================================================================
//                     FUN��O DE EXECU��O DO CONTADOR
//                             -DISPENSER N�2-
//================================================================================
void timebase2()
{
  if(!ligar)                                      //se ligar for 0
  {                                               //contadores desligados...
   temp_ligado2 = 0x00;                           //zera temp_ligado2
  }
  
  if(ligar && num2!=0)                            //se ligar for 1 e num2 diferente de 0
  {                                               //contador do dispenser n�2 ligado
   if(temp2==10)                                  //conta 1 segundo
   {
    temp2 = 0x00;                                 //zera temp2
    temp_ligado2++;                               //incrementa temp_ligado2 a cada 1 segundo
    
   }                                              //end if temp2==10
   
   if(temp_ligado2==mult2)                        //compara igualdade entre temp_ligado2 e mult2
   {                                              //acabou o tempo do contador do dispenser n�2...
    toque2 = 0x01;                                //liga o bit toque2
    atv_mot2 = 0x01;                              //liga o bit de aativa��o do motor 2
    comp2 = qtd_comp2;
    abre_mot2();                                  //define os bits de controle do motor 2 para abrir
    
   }                                              //end if temp_ligado2==mult2
   
   if(atv_mot2)                                   //se ativa��o do motor 2 ligada...
   {
    read_motbits();                               //executa a leitura dos bits de controle dos motores
    
   }                                              //end if atv_mot2

   else                                           //sen�o...
   {
    alarme();                                     //aciona o alarme
    
   }                                              //end else
   
  }                                               //end if ligar && num2!=0
  
}                                                 //end timebase2()


//================================================================================
//                     FUN��O DE EXECU��O DO CONTADOR
//                             -DISPENSER N�3-
//================================================================================
void timebase3()
{
  if(!ligar)                                      //se ligar for 0
  {                                               //contadores desligados...
   temp_ligado3 = 0x00;                           //zera temp_ligado3
  }

  if(ligar && num3!=0)                            //se ligar for 1 e num3 diferente de 0
  {                                               //contador do dispenser n�3 ligado
   if(temp3==10)                                  //conta 1 segundo
   {
    temp3 = 0x00;                                 //zera temp3
    temp_ligado3++;                               //incrementa temp_ligado3 a cada 1 segundo

   }                                              //end if temp3==10

   if(temp_ligado3==mult3)                        //compara igualdade entre temp_ligado3 e mult3
   {                                              //acabou o tempo do contador do dispenser n�3...
    toque3 = 0x01;                                //liga o bit toque3
    atv_mot3 = 0x01;                              //liga o bit de aativa��o do motor 3
    comp3 = qtd_comp3;
    abre_mot3();                                  //define os bits de controle do motor 3 para abrir

   }                                              //end if temp_ligado3==mult3

   if(atv_mot3)                                   //se ativa��o do motor 3 ligada...
   {
    read_motbits();                               //executa a leitura dos bits de controle dos motores

   }                                              //end if atv_mot3

   else                                           //sen�o...
   {
    alarme();                                     //aciona o alarme

   }                                              //end else

  }                                               //end if ligar && num3!=0

}                                                 //end timebase3()


//================================================================================
//                     FUN��O DE EXECU��O DO CONTADOR
//                             -DISPENSER N�4-
//================================================================================
void timebase4()
{
  if(!ligar)                                      //se ligar for 0
  {                                               //contadores desligados...
   temp_ligado4 = 0x00;                           //zera temp_ligado4
  }

  if(ligar && num4!=0)                            //se ligar for 1 e num4 diferente de 0
  {                                               //contador do dispenser n�4 ligado
   if(temp4==10)                                  //conta 1 segundo
   {
    temp4 = 0x00;                                 //zera temp4
    temp_ligado4++;                               //incrementa temp_ligado4 a cada 1 segundo

   }                                              //end if temp3==10

   if(temp_ligado4==mult4)                        //compara igualdade entre temp_ligado4 e mult4
   {                                              //acabou o tempo do contador do dispenser n�4...
    toque4 = 0x01;                                //liga o bit toque4
    atv_mot4 = 0x01;                              //liga o bit de ativa��o do motor 4
    comp4 = qtd_comp4;
    abre_mot4();                                  //define os bits de controle do motor 4 para abrir

   }                                              //end if temp_ligado4==mult4

   if(atv_mot4)                                   //se ativa��o do motor 4 ligada...
   {
    read_motbits();                               //executa a leitura dos bits de controle dos motores

   }                                              //end if atv_mot4

   else                                           //sen�o...
   {
    alarme();                                     //aciona o alarme

   }                                              //end else

  }                                               //end if ligar && num4!=0

}                                                 //end timebase4()


//================================================================================
//                      FUN��O DE EXECU��O DO ALARME
//================================================================================
void alarme()
{
 if(toque || toque2 || toque3 || toque4)          //se toque, toque2, toque3 ou toque4 for 1
 {                                                //algum dos contadores acabaram...
   if(vezes <200)                                 //se vezes for menor que 200 (toca por 1 minuto)
   {                                              //(vezes que o SOM tocar�)
    toca_som();                                   //executa a toca do SOM
   }                                              //end if vezes<10
   
   else                                           //sen�o...
   {
    if(toque)                                     //se toque for 1
    {
     toque=0x00;                                  //limpa o bit toque
     temp_ligado=0x00;                            //zera temp_ligado para recome�ar a contagem
     
    }                                             //end if toque
    
    if(toque2)                                    //se toque2 for 1
    {
     toque2=0x00;                                 //limpa o bit toque 2
     temp_ligado2=0x00;                           //zera temp_ligado2 para recome�ar a contagem
     
    }                                             //end if toque2
    
    if(toque3)                                    //se toque3 for 1
    {
     toque3=0x00;                                 //limpa o bit toque3
     temp_ligado3=0x00;                           //zera temp_ligado3 para recome�ar a contagem

    }                                             //end if toque3
    
    if(toque4)                                    //se toque4 for 1
    {
     toque4=0x00;                                 //limpa o bit toque4
     temp_ligado4=0x00;                           //zera temp_ligado4 para recome�ar a contagem

    }                                             //end if toque4
    
     vezes=0x00;                                  //zera vezes
     SOM = 0x00;                                  //desliga o som
     
   }                                              //end else
   
 }                                                //end if toque || toque2 || toque3 || toque4
 
}                                                 //end alarme()


//=================================================================================
//                      FUN��O DE ACIONAMENTO DO BUZZER
//=================================================================================
void toca_som()
{
 if(temp_som>3) temp_som=0x00;                    //se temp_som for maior que 3, zera temp_som
    if(temp_som==3)                               //conta 300 milisegundos...
    {
     temp_som = 0x00;                             //zera temp_som
     SOM = ~SOM;                                  //inverte o estado do buzzer
     vezes++;                                     //incrementa vezes
     
     if(!ligar)                                   //se ligar for 0 (desligado)
     {
      vezes = 0x00;                               //zera vezes
      SOM   = 0x00;                               //desliga o buzzer
      
     }                                            //end if !ligar
     
    }                                             //end if temp_som==3
    
}                                                 //end toca_som()


//================================================================================
//                          FUN��O DE CONFIGURA��O DOS
//                    BITS DE CONTROLE DO MOTOR 1 PARA ABRIR
//================================================================================
void abre_mot()
{
 open_bit  = 0x01;                                //seta bit de abertura
 close_bit = 0x00;                                //limpa bit de fechamento
 x_mot     = 0x00;                                //zera x_mot
 
}                                                 //end abre_mot()


//================================================================================
//                          FUN��O DE CONFIGURA��O DOS
//                    BITS DE CONTROLE DO MOTOR 2 PARA ABRIR
//================================================================================
void abre_mot2()
{
 open_bit2  = 0x01;                               //seta bit de abertura
 close_bit2 = 0x00;                               //limpa bit de fechamento
 x_mot2     = 0x00;                               //zera x_mot
 
}                                                 //end abre_mot2()


//================================================================================
//                          FUN��O DE CONFIGURA��O DOS
//                    BITS DE CONTROLE DO MOTOR 3 PARA ABRIR
//================================================================================
void abre_mot3()
{
 open_bit3  = 0x01;                               //seta bit de abertura
 close_bit3 = 0x00;                               //limpa bit de fechamento
 x_mot3     = 0x00;                               //zera x_mot

}                                                 //end abre_mot3()


//================================================================================
//                          FUN��O DE CONFIGURA��O DOS
//                    BITS DE CONTROLE DO MOTOR 4 PARA ABRIR
//================================================================================
void abre_mot4()
{
 open_bit4  = 0x01;                               //seta bit de abertura
 close_bit4 = 0x00;                               //limpa bit de fechamento
 x_mot4     = 0x00;                               //zera x_mot

}                                                 //end abre_mot4()


//================================================================================
//                       FUN��O DE ABERTURA DO MOTOR 1
//================================================================================
void mot_aberto()
{
 if(comp!=0)                                      //se a quantidade de comprimidos a serem despejados
 {                                                //for diferente de 0...
 
 if(x_mot<20)                                     //se x_mot for menor que 10...
  {
    SM=0;                                         //  ||
    delay_us(18000);                              //  ||
    SM=1;                                         //  ||
    delay_us(2000);                               //  \/
    SM=0;                                         // liga motor 1 no sentido hor�rio
    x_mot ++;                                     //incrementa x_mot
    
  }                                               //end if x_mot<10
  
  if(x_mot == 20)                                 //se x_mot for 10...
   {
    fecha_mot();                                  //executa a configura��o dos bits de controle do motor 1 para fechar
    
   }                                              //end if x_mot==10
 
 }                                                //end if comp!=0
 else                                             //sen�o...
 {
  open_bit = 0x00;                                //limpa bit de abertura
  close_bit= 0x00;                                //limpa bit de fechamento
 }

}                                                 //end mot_aberto()


//================================================================================
//                       FUN��O DE ABERTURA DO MOTOR 2
//================================================================================
void mot_aberto2()
{
 if(comp2!=0)                                      //se a quantidade de comprimidos a serem despejados
 {                                                //for diferente de 0...
 if(x_mot2<20)                                    //se x_mot2 for menor que 10...
  {
    SM2=0;                                        //  ||
    delay_us(18000);                              //  ||
    SM2=1;                                        //  ||
    delay_us(2000);                               //  \/
    SM2=0;                                        //liga motor 2 no sentido hor�rio
    x_mot2 ++;                                    //incrementa x_mot2
    
  }                                               //end if x_mot2<10
  
  if(x_mot2 == 20)                                //se x_mot2 for 10...
   {
    fecha_mot2();                                 //executa a configura��o dos bits de controle do motor 2 para fechar
    
   }                                              //end if x_mot2==10
 }                                                //end if comp!=0
 else                                             //sen�o...
 {
  open_bit2 = 0x00;                                //limpa bit de abertura
  close_bit2= 0x00;                                //limpa bit de fechamento
 }
   
}                                                 //end mot_aberto2()


//================================================================================
//                       FUN��O DE ABERTURA DO MOTOR 3
//================================================================================
void mot_aberto3()
{
 if(comp3!=0)                                     //se a quantidade de comprimidos a serem despejados
 {                                                //for diferente de 0...
 if(x_mot3<20)                                    //se x_mot3 for menor que 10...
  {
    SM3=0;                                        //  ||
    delay_us(18000);                              //  ||
    SM3=1;                                        //  ||
    delay_us(2000);                               //  \/
    SM3=0;                                        // liga motor 1 no sentido hor�rio
    x_mot3 ++;                                    //incrementa x_mot3

  }                                               //end if x_mot3<10

  if(x_mot3 == 20)                                //se x_mot3 for 10...
   {
    fecha_mot3();                                 //executa a configura��o dos bits de controle do motor 3 para fechar

   }                                              //end if x_mot3==10
 }                                                //end if comp!=0
 else                                             //sen�o...
 {
  open_bit3 = 0x00;                               //limpa bit de abertura
  close_bit3= 0x00;                               //limpa bit de fechamento
 }

}                                                 //end mot_aberto3()


//================================================================================
//                       FUN��O DE ABERTURA DO MOTOR 4
//================================================================================
void mot_aberto4()
{
 if(comp4!=0)                                      //se a quantidade de comprimidos a serem despejados
 {                                                //for diferente de 0...
 if(x_mot4<20)                                    //se x_mot4 for menor que 10...
  {
    SM4=0;                                        //  ||
    delay_us(18000);                              //  ||
    SM4=1;                                        //  ||
    delay_us(2000);                               //  \/
    SM4=0;                                        // liga motor 1 no sentido hor�rio
    x_mot4 ++;                                    //incrementa x_mot4

  }                                               //end if x_mot4<10

  if(x_mot4 == 20)                                //se x_mot4 for 10...
   {
    fecha_mot4();                                 //executa a configura��o dos bits de controle do motor 4 para fechar

   }                                              //end if x_mot4==10
 }                                                //end if comp!=0
 else                                             //sen�o...
 {
  open_bit4 = 0x00;                                //limpa bit de abertura
  close_bit4= 0x00;                                //limpa bit de fechamento
 }

}                                                 //end mot_aberto4()


//================================================================================
//                          FUN��O DE CONFIGURA��O DOS
//                    BITS DE CONTROLE DO MOTOR 1 PARA FECHAR
//================================================================================
void fecha_mot()
{
 open_bit  = 0x00;                                //limpa o bit de abertura
 close_bit = 0x01;                                //seta o bit de fechamento
 x_mot     = 0x00;                                //zera x_mot
 
}                                                 //end fecha_mot()


//================================================================================
//                          FUN��O DE CONFIGURA��O DOS
//                    BITS DE CONTROLE DO MOTOR 2 PARA FECHAR
//================================================================================
void fecha_mot2()
{
 open_bit2  = 0x00;                               //limpa o bit de abertura
 close_bit2 = 0x01;                               //seta o bit de fechamento
 x_mot2     = 0x00;                               //zera x_mot2
 
}                                                 //end fecha_mot2()


//================================================================================
//                          FUN��O DE CONFIGURA��O DOS
//                    BITS DE CONTROLE DO MOTOR 3 PARA FECHAR
//================================================================================
void fecha_mot3()
{
 open_bit3  = 0x00;                               //limpa o bit de abertura
 close_bit3 = 0x01;                               //seta o bit de fechamento
 x_mot3     = 0x00;                               //zera x_mot3

}                                                 //end fecha_mot3()


//================================================================================
//                          FUN��O DE CONFIGURA��O DOS
//                    BITS DE CONTROLE DO MOTOR 4 PARA FECHAR
//================================================================================
void fecha_mot4()
{
 open_bit4  = 0x00;                               //limpa o bit de abertura
 close_bit4 = 0x01;                               //seta o bit de fechamento
 x_mot4     = 0x00;                               //zera x_mot4

}                                                 //end fecha_mot4()


//================================================================================
//                       FUN��O DE FECHAMENTO DO MOTOR 1
//================================================================================
void mot_fechado()
{
 if(comp!=0)                                      //se a quantidade de comprimidos � despejar
 {                                                //for diferente de 0
 if(x_mot<20)                                     //se x_mot for menor que 10
  {
    SM=0;                                         //  ||
    delay_us(18500);                              //  ||
    SM=1;                                         //  ||
    delay_us(1500);                               //  \/
    SM=0;                                         //leva o motor para a posi��o inicial
    x_mot++;                                      //incrementa x_mot
    
  }                                               //end if x_mot<10
  
  if(x_mot==20)                                   //se x_mot for 10
    {
     abre_mot();                                  //define os bit de controle do motor para abrir
     comp--;                                      //decrementa comp
     
    }                                             //end if x_mot==10
 
 }                                                //end if comp!=0

}                                                 //end mot_fechado


//================================================================================
//                       FUN��O DE FECHAMENTO DO MOTOR 2
//================================================================================
void mot_fechado2()
{
 if(comp2!=0)                                      //se a quantidade de comprimidos � despejar
 {                                                //for diferente de 0
  if(x_mot2<20)                                   //se x_mot2 for menor que 10
   {
    SM2=0;                                        //  ||
    delay_us(18500);                              //  ||
    SM2=1;                                        //  ||
    delay_us(1500);                               //  \/
    SM2=0;                                        //leva o motor 2 para a posi��o inicial
    x_mot2++;                                     //incrementa x_mot2
    
   }                                               //end if x_mot2<10
  
  if(x_mot2==20)                                  //se x_mot2 for 10
    {
     abre_mot2();                                 //define os bit de controle do motor para abrir
     comp2--;                                     //decrementa comp2
     
    }                                             //end if x_mot2==10
    
  }                                               //end if comp!=0
    
}                                                 //end mot_fechado2()


//================================================================================
//                       FUN��O DE FECHAMENTO DO MOTOR 3
//================================================================================
void mot_fechado3()
{
 if(comp3!=0)                                      //se a quantidade de comprimidos � despejar
 {                                                //for diferente de 0
  if(x_mot3<20)                                   //se x_mot3 for menor que 10
  {
    SM3=0;                                        //  ||
    delay_us(18500);                              //  ||
    SM3=1;                                        //  ||
    delay_us(1500);                               //  \/
    SM3=0;                                        //leva o motor 3 para a posi��o inicial
    x_mot3++;                                     //incrementa x_mot3

  }                                               //end if x_mot2<10

  if(x_mot3==20)                                  //se x_mot3 for 10
    {
     abre_mot3();                                 //define os bit de controle do motor para abrir
     comp3--;                                     //decrementa comp3

    }                                             //end if x_mot3==10
   
 }                                                //end if comp3!=0

}                                                 //end mot_fechado3()


//================================================================================
//                       FUN��O DE FECHAMENTO DO MOTOR 4
//================================================================================
void mot_fechado4()
{
 if(comp4!=0)                                      //se a quantidade de comprimidos � despejar
 {                                                //for diferente de 0
  if(x_mot4<20)                                   //se x_mot4 for menor que 10
   {
    SM4=0;                                        //  ||
    delay_us(18500);                              //  ||
    SM4=1;                                        //  ||
    delay_us(1500);                               //  \/
    SM4=0;                                        //leva o motor 4 para a posi��o inicial
    x_mot4++;                                     //incrementa x_mot4

   }                                               //end if x_mot4<10

  if(x_mot4==20)                                  //se x_mot4 for 10
    {
     abre_mot4();                                 //define os bit de controle do motor para abrir
     comp4--;                                     //decrementa comp4

    }                                             //end if x_mot4==10
    
  }                                                //end if comp4!=0

}                                                 //end mot_fechado4()


//================================================================================
//                         FUN��O DE LEITURA DOS BITS
//                          DE CONTROLE DOS MOTORES
//================================================================================
void read_motbits()
{
   if(!open_bit && !close_bit) atv_mot = 0x00;    //se os bits de abertura e fechamento do motor 1 forem 0,
                                                  //desliga a ativa��o do motor 1

   if(open_bit && !close_bit)                     //se o bit de abertura for 1 e o de fechamento for 0...
   {
    mot_aberto();                                 //executa a abertura do motor 1
    
   }                                              //end if open_bit && !close_bit

   if(!open_bit && close_bit)                     //se o bit de abertura for 0 e o de fechamento for 1...
   {
    mot_fechado();                                //executa o fechamento do motor 1
    
   }                                              //end if !open_bit && close_bit
   
   
   if(!open_bit2 && !close_bit2) atv_mot2 = 0x00; //se os bits de abertura e fechamento do motor 2 forem 0,
                                                  //desliga a ativa��o do motor 2

   if(open_bit2 && !close_bit2)                   //se o bit de abertura for 1 e o de fechamento for 0...
   {
    mot_aberto2();                                //executa a abertura do motor 2
    
   }                                              //end if open_bit2 && !close_bit2

   if(!open_bit2 && close_bit2)                   //se o bit de abertura for 0 e o de fechamento for 1...
   {
    mot_fechado2();                               //executa o fechamento do motor 2
    
   }                                              //end if !open_bit2 && close_bit2
   
   
   if(!open_bit3 && !close_bit3) atv_mot3 = 0x00; //se os bits de abertura e fechamento do motor 3 forem 0,
                                                  //desliga a ativa��o do motor 3

   if(open_bit3 && !close_bit3)                   //se o bit de abertura for 1 e o de fechamento for 0...
   {
    mot_aberto3();                                //executa a abertura do motor 3

   }                                              //end if open_bit3 && !close_bit3

   if(!open_bit3 && close_bit3)                   //se o bit de abertura for 0 e o de fechamento for 1...
   {
    mot_fechado3();                               //executa o fechamento do motor 3

   }                                              //end if !open_bit3 && close_bit3
   
   
   if(!open_bit4 && !close_bit4) atv_mot4 = 0x00; //se os bits de abertura e fechamento do motor 4 forem 0,
                                                  //desliga a ativa��o do motor 4

   if(open_bit4 && !close_bit4)                   //se o bit de abertura for 1 e o de fechamento for 0...
   {
    mot_aberto4();                                //executa a abertura do motor 4

   }                                              //end if open_bit4 && !close_bit4

   if(!open_bit4 && close_bit4)                   //se o bit de abertura for 0 e o de fechamento for 1...
   {
    mot_fechado4();                               //executa o fechamento do motor 4

   }                                              //end if !open_bit4 && close_bit4
   
}                                                 //end read_motbits()