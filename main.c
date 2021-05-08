/*
 * File:   main.c
 * Author: torii wataru
 *
 */


#include <xc.h>
#define _XTAL_FREQ 8000000 //8MHz
#define HIGH 1
#define LOW 0

#define LED         PORTCbits.RC2
#define PHASE_A     PORTAbits.RA4
#define ENABLE_A    PORTAbits.RA2
#define PHASE_B     PORTCbits.RC0  
#define ENABLE_B    PORTCbits.RC1
#define BPM         120
#define N           14

#pragma config FOSC     = INTOSC   // 内部ｸﾛｯｸ使用する(INTOSC)
#pragma config WDTE     = OFF      // ｳｵｯﾁﾄﾞｯｸﾞﾀｲﾏｰ無し(OFF)
#pragma config PWRTE    = ON       // 電源ONから64ms後にﾌﾟﾛｸﾞﾗﾑを開始する(ON)
#pragma config MCLRE    = ON      // 外部ﾘｾｯﾄ信号は使用せずにﾃﾞｼﾞﾀﾙ入力(RA3)ﾋﾟﾝとする(OFF)
#pragma config CP       = OFF      // ﾌﾟﾛｸﾞﾗﾑﾒﾓﾘｰを保護しない(OFF)
#pragma config BOREN    = OFF       // 電源電圧降下常時監視機能ON(ON)
#pragma config CLKOUTEN = OFF      // CLKOUTﾋﾟﾝをRA4ﾋﾟﾝで使用する(OFF)
#pragma config IESO     = OFF      // 外部・内部ｸﾛｯｸの切替えでの起動はなし(OFF)
#pragma config FCMEN    = OFF      // 外部ｸﾛｯｸ監視しない(OFF)
// コンフィギュレーション２の設定
#pragma config WRT      = OFF      // Flashﾒﾓﾘｰを保護しない(OFF)
#pragma config PPS1WAY  = ON      // ロック解除シーケンスを実行すれば何度でもPPSLOCKをセット/クリアできる
#pragma config ZCDDIS   = ON       // ゼロクロス検出回路は無効とする(ON)
#pragma config PLLEN    = OFF      //
#pragma config STVREN   = ON       // スタックがオーバフローやアンダーフローしたらリセットをする(ON)
#pragma config BORV     = LO       // 電源電圧降下常時監視電圧(2.5V)設定(HI)
#pragma config LPBOR    = OFF      // 低消費電力ブラウンアウトリセット(LPBOR_OFF)
#pragma config LVP      = ON      // 低電圧プログラミング機能使用(ON)

int song[N] = {0,0,4,4,5,5,4,3,3,2,2,1,1,0};
int  len[N] = {1,1,1,1,1,1,2,1,1,1,1,1,1,2};

void Blink(int n)
{
    do
    {
        LED = HIGH;
        __delay_ms(100);
        LED = LOW;
        __delay_ms(100);
    }while(--n > 0);
}

void Initialize()
{
    OSCCON = 0b01110010;        // 8MHz
    TRISA = 0x00;
    TRISC = 0x00;
    PORTA = 0x00;
    PORTC = 0x00;
    ANSELA = 0x00;
    ANSELC = 0x00;
    
    ENABLE_A = HIGH;
    ENABLE_B = HIGH;
    
    PEIE = 1;
    GIE = 1;
}

void RotateCCW(int n)
{
    const int w = 800;
    for(int i = 0; i < n; i++)
    {
        PHASE_B = HIGH;
        __delay_us(w);
        PHASE_A = HIGH;
        __delay_us(w);
        
        PHASE_B = LOW;
        __delay_us(w);
        PHASE_A = LOW;
        __delay_us(w);
    }
}

void RotateCW(int n)
{
    const int w = 800;
    for(int i = 0; i < n; i++)
    {
        PHASE_A = HIGH;
        __delay_us(w);
        PHASE_B = HIGH;
        __delay_us(w);
        
        PHASE_A = LOW;
        __delay_us(w);
        PHASE_B = LOW;
        __delay_us(w);
    }
}

/// dが正の時，音が高くなるのでCCW，負の時CWを呼ぶ
void MoveWhistle(int d)
{
    if (d > 0)
    {
        for(int i = 0; i < d; i++)
            RotateCCW(10*d);
    }
    else
    {
        for(int i = 0; i < -d; i++)
            RotateCW(10*-d);
    }
}

void Rest(int rest)
{
    for(int i = 0; i < 60000/BPM*rest; i++)
        __delay_ms(1);
}

void main(void) {
    Initialize();
    __delay_ms(1000);
    Blink(3);
    
    int repeatFlag = 1;
    
    int i = 0;
    int nowSound = song[i];
    int nowLen = len[i];
    int prevSound = 5;
    
    while(repeatFlag)
    {
        LED = LOW;
        for(; i < N; i++)
        {
            Blink(1);
            MoveWhistle(nowSound-prevSound);
            Rest(nowLen);
            prevSound = nowSound;
            if(i==N-1)
                break;
            nowSound = song[i+1];
            nowLen = len[i+1];
        }
        i = 0;
        LED = HIGH;
    }
    return;
}
