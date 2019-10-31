#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

int main_menu(void);
int getch(void);

//------------------------------------------------------------------------
// 설 명 : 어플리케이션 처리 
// 매 계 : 명령행 매계변수
// 반 환 : 없음
// 주 의 : 
//------------------------------------------------------------------------
int main( int argc, char **argv )
{

    int     handle;
    struct  termios  oldtio,newtio;
    
    char    *TitleMessage = "Welcome Serial Port\r\n";
    char    Buff[256];
    int     RxCount;
    int     loop;
    int     ending;
    
    int key;
            
    // 화일을 연다.
    handle = open( "/dev/ttyTHS2", O_RDWR | O_NOCTTY );
    if( handle < 0 ) 
    {
        //화일 열기 실패
        printf( "Serial Open Fail [/dev/ttyTHS2]\r\n "  );
        exit(0);
    }
    
    tcgetattr( handle, &oldtio );  // 현재 설정을 oldtio에 저장

    memset( &newtio, 0, sizeof(newtio) );
    newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD ; 
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    //set input mode (non-canonical, no echo,.....)
    newtio.c_lflag = 0;
    
    newtio.c_cc[VTIME] = 128;    // time-out 값으로 사용된다. time-out 값은 TIME*0.1초 이다.
    newtio.c_cc[VMIN]  = 0;     // MIN은 read가 리턴되기 위한 최소한의 문자 개수
    
    tcflush( handle, TCIFLUSH );
    tcsetattr( handle, TCSANOW, &newtio );
    
    // 타이틀 메세지를 표출한다. 
    write( handle, TitleMessage, strlen( TitleMessage ) );
    
    // 1 문자씩 받아서 되돌린다. 
    ending = 0;
/*
    while(!ending)
    {
        // 한 문자를 수신한다.
        RxCount = read( handle, Buff, 1 );
        if( RxCount == 0 ) 
        {
                printf( "Receive Time Over\n" );
                continue;
        }        
        else if( RxCount < 0 ) 
        {
                printf( "Read Error\n" );
                break;
        }
//        else
//        {
//                printf( "[%c]", Buff[0] );
//        }
        for( loop = 0; loop < RxCount; loop++ )
        {
//             printf("[%c]", Buff[loop] );
//            printf( "Read Data [%c]\n", Buff[loop] );
            // ESC 키 문자가 들어오면 중지한다. 
            if( Buff[loop] == 0x1b ) ending = 1;
        }   
        
        // 수신된 문자를 되돌린다.
        write( handle, Buff, RxCount );
        //write( 1, Buff, RxCount );
        
        printf( "%c\n", Buff[0] );
        //printf( "-");
    }
*/


    while((key=main_menu()) != 0)
    {

        //serialPutchar(fd, key);
        //serialPuts(fd, "\n");

        switch(key)
        {

           case '1':
               printf("No.1\n");
               //gpioPWM(17, 24); // 6% 듀티비 인가 (24/255)=0.06
               Buff[0] = '1';
               write( handle, Buff, 1 );
               break;

           case '2':
               printf("No.2\n");
               //gpioPWM(17, 23);
               Buff[0] = '2';
               write( handle, Buff, 1 );
               break;

           case '3':
               printf("No.3\n");
               //gpioPWM(17, 25);
               Buff[0] = '3';
               write( handle, Buff, 1 );
               break;

           case '4':
               printf("No.4\n");
               //gpioPWM(18, 30);//중립값인가
               Buff[0] = '4';
               write( handle, Buff, 1 );
               break;  

           case '5':
               printf("No.5\n");
               //gpioPWM(18, 36);//오른쪽값인가
               Buff[0] = '5';
               write( handle, Buff, 1 );
               break;  

           case '6':
               printf("No.6\n");
               //gpioPWM(18, 24);//왼쪽값인가
               Buff[0] = '6';
               write( handle, Buff, 1 );
               break;  

           case '7':
               printf("No.7\n");
               Buff[0] = '7';
               write( handle, Buff, 1 );
               break;  

           case '8':
               printf("No.8\n");
               Buff[0] = '8';
               write( handle, Buff, 1 );
               break;

           case '9':
               printf("No.9\n");
               Buff[0] = '9';
               write( handle, Buff, 1 );
               break;

           case '0':
               printf("No.0\n");
               Buff[0] = '0';
               write( handle, Buff, 1 );
               break;

           case 'a':
               printf("No.a\n");
               Buff[0] = 'a';
               write( handle, Buff, 1 );
               break;

           case 'b':
               printf("No.b\n");
               Buff[0] = 'b';
               write( handle, Buff, 1 );
               break;

           case 'c':
               printf("No.c\n");
               Buff[0] = 'c';
               write( handle, Buff, 1 );
               break;

           case 'd':
               printf("No.d\n");
               Buff[0] = 'd';
               write( handle, Buff, 1 );
               break;

           case 'i':
               printf("No.i\n");
               Buff[0] = 'i';
               write( handle, Buff, 1 );
               break;

           case 'q':
               printf("exit\n");
               tcsetattr( handle, TCSANOW, &oldtio ); // 이전 상태로 되돌린다. 
               close( handle );   // 화일을 닫는다.
               //serialClose(fd);
               exit(0);
               break;

           default :
               printf("Wrong key ..... try again\n");
               break;
        }
    }



    
    tcsetattr( handle, TCSANOW, &oldtio ); // 이전 상태로 되돌린다. 
    
    close( handle );   // 화일을 닫는다.
    
    return 0;    
}

 
int main_menu(void)
{
    int key;

    printf("\n\n");
    printf("-------------------------------------------------\n");
    printf("                    MAIN MENU\n");
    printf("-------------------------------------------------\n");
    printf(" 1. RC Motor Stop                       \n");
    printf(" 2. RC Motor Forward                       \n");
    printf(" 3. RC Motor Backward                       \n");
    printf(" 4. Servo Motor Center                       \n");
    printf(" 5. Servo Motor Right                       \n");
    printf(" 6. Servo Motor Left                      \n");
    printf(" 7. Servo Motor Right one step                      \n");
    printf(" 8. Servo Motor Left  one step                    \n");

    printf("-------------------------------------------------\n");
    printf(" q. Motor Control application QUIT                 \n");
    printf("-------------------------------------------------\n");
    printf("\n\n");
 
    printf("SELECT THE COMMAND NUMBER : ");
/*
    key=getchar();
    if (key == '\n')
        ;
    else
        getchar();
*/
    key = getch();

    return key;
}

int getch(void)
{
  int ch;
  struct termios buf;
  struct termios save;

   tcgetattr(0, &save);
   buf = save;
   buf.c_lflag &= ~(ICANON|ECHO);
   buf.c_cc[VMIN] = 1;
   buf.c_cc[VTIME] = 0;
   tcsetattr(0, TCSAFLUSH, &buf);
   ch = getchar();
   tcsetattr(0, TCSAFLUSH, &save);
   return ch;
}
