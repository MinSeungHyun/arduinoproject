


#define F_CPU 16000000UL
#define sbi(PORTX,bitX) PORTX|=(1<<bitX)
#define cbi(PORTX,bitX) PORTX&=~(1<<bitX)
#define tbi(PORTX,bitX) PORTX^=(1<<bitX)

#define FS_SEL 131
#define RAD2DEG 57.29578
#define DEG2RAD 0.01745

#include<avr/io.h> 
#include<avr/interrupt.h>
#include<util/delay.h>
#include<math.h>

 
void twi_write(unsigned char address,unsigned char data);
unsigned char twi_read(char addressr);
void USART_Transmit(unsigned char tx_data);
void USART_Transmit_init4(int data);
void get_raw_data();
void calibrate();
void MPU9250_init();


volatile double dt = 0.000;
volatile int temp;
volatile unsigned char a_x_l,a_x_h,a_y_l,a_y_h,a_z_l,a_z_h;
volatile unsigned char g_x_l,g_x_h,g_y_l,g_y_h,g_z_l,g_z_h;
volatile unsigned char m_x_l,m_x_h,m_y_l,m_y_h,m_z_l,m_z_h;
volatile double bas_a_x,bas_a_y,bas_a_z;
volatile double bas_g_x,bas_g_y,bas_g_z;
volatile double a_x,a_y,a_z;
volatile double g_x,g_y,g_z;
volatile double m_x,m_y,m_z;
volatile double mx,my,mz;
volatile double r_mx,r_my;
volatile double angle_ax,angle_ay,angle_az;
volatile double angle_gx,angle_gy,angle_gz;
volatile double las_angle_gx,las_angle_gy,las_angle_gz;
volatile double yaw_g,yaw_m;
volatile double roll,pitch,yaw;
volatile double alpha,beta;
volatile double ASAX,ASAY,ASAZ;



int main()
{  

	//UART
	UCSR0A = 0x00;
	UCSR0B = (1<<TXEN0); 
	UCSR0C = (3<<UCSZ00);  
	UBRR0H = 0;
	UBRR0L = 103;		//9600

	//TWI(I2C)
	TWCR = (1<<TWEN);
	TWBR = 12;	 		//400khz

	//TIMER0
	TCCR0 = (1<<CS02)|(1<<CS01);	//256 분주 
	TCNT0 = 256-125;				//125 번 => 0.002s
	TIMSK = (1<<TOIE0);


	MPU9250_init();
	calibrate();

	SREG = 0x80;

	while(1)
	{ 

		get_raw_data();

		las_angle_gx = roll;	//최근값 누적
		las_angle_gy = pitch;
		las_angle_gz = yaw;

		temp = (a_x_h<<8) | a_x_l;
		a_x = - temp;
		temp = (a_y_h<<8) | a_y_l;
		a_y = - temp;
		temp = (a_z_h<<8) | a_z_l;
		a_z = temp;
		temp = (g_x_h<<8) | g_x_l;
		g_x = temp;
		temp = (g_y_h<<8) | g_y_l;
		g_y = temp;
		temp = (g_z_h<<8) | g_z_l;
		g_z = temp;

		temp = (m_x_h<<8) | m_x_l;
		m_x = (double)temp*((ASAX+128.000)/256.000);
		temp = (m_y_h<<8) | m_y_l;
		m_y = (double)temp*((ASAY+128.000)/256.000);
		temp = (m_z_h<<8) | m_z_l;
		m_z = (double)temp*((ASAZ+128.000)/256.000);

		g_x = (g_x - bas_g_x)/FS_SEL;
		g_y = (g_y - bas_g_y)/FS_SEL;
		g_z = (g_z - bas_g_z)/FS_SEL;
		
		angle_ax = atan(-1.000*a_y/sqrt(pow(a_x,2) + pow(a_z,2)))*RAD2DEG;
		angle_ay = atan(a_x/sqrt(pow(a_y,2) + pow(a_z,2)))*RAD2DEG;

		angle_gx = g_x*dt + las_angle_gx;
		angle_gy = g_y*dt + las_angle_gy;
		angle_gz = g_z*dt + las_angle_gz;

		dt = 0.000;

		alpha = 0.96;
		roll = alpha*angle_gx + (1.000 - alpha)*angle_ax;
		pitch = alpha*angle_gy + (1.000 - alpha)*angle_ay;
		
		mx = (m_x-15)*0.9;
		my = m_y+65;
		mz = (m_z+125)*0.53;
		r_mx = (cos(70.000*DEG2RAD)*mx + sin(70.000*DEG2RAD)*mz) - 141;
		r_my = (cos(-70.000*DEG2RAD)*my - sin(-70.000*DEG2RAD)*mz) - 143;

		yaw_g = angle_gz;
		yaw_m = -atan2(r_mx,r_my)*RAD2DEG;		//+- 10도 안에서만 사용가능 
				
		beta = 0.92;
		if( (roll<10) && (roll>-10) && (pitch<10) && (pitch>-10) )
				yaw = beta*yaw_g + (1.000 - beta)*yaw_m;
		else
			yaw = yaw_g;
			
		/*
		USART_Transmit_init4(a_x);
		USART_Transmit('\t');
		USART_Transmit_init4(a_y);
		USART_Transmit('\t');
		USART_Transmit_init4(a_z);
		USART_Transmit('\t');
		*/
		/*
		USART_Transmit_init4(g_x);
		USART_Transmit('\t');
		USART_Transmit_init4(g_y);
		USART_Transmit('\t');
		USART_Transmit_init4(g_z);
		USART_Transmit('\t');
		*/
		
		USART_Transmit_init4(roll);
		USART_Transmit('\t');
		USART_Transmit_init4(pitch);
		USART_Transmit('\t');
		USART_Transmit_init4(yaw);
	
		/*		
		USART_Transmit_init4(yaw_g);
		USART_Transmit('\t');
		USART_Transmit_init4(temp2);
		USART_Transmit('\t');
		USART_Transmit_init4(yaw);
		*/
		
		USART_Transmit('\n');
		USART_Transmit('\r');
		
		_delay_ms(100);

	} 

} 


ISR(TIMER0_OVF_vect)	//0.002s
{
	dt += 0.002;

	TCNT0 = 256-125;
}




void calibrate()	//초기값 읽기 
{
	int cal = 10;

	for(int i=0; i<cal; i++)	//평균 
	{
		get_raw_data();
	
		temp = (a_x_h<<8) | a_x_l;
		a_x += - temp - 16383;
		temp = (a_y_h<<8) | a_y_l;
		a_y += - temp;
		temp = (a_z_h<<8) | a_z_l;
		a_z += temp;
		temp = (g_x_h<<8) | g_x_l;
		g_x += temp;
		temp = (g_y_h<<8) | g_y_l;
		g_y += temp;
		temp = (g_z_h<<8) | g_z_l;
		g_z += temp;

		_delay_ms(100);
	}	
	
	a_x /= cal;
	a_y /= cal;
	a_z /= cal;
	g_x /= cal;
	g_y /= cal;
	g_z /= cal;

	bas_a_x = a_x;	//초기 값으로 저장 
	bas_a_y = a_y;
	bas_a_z = a_z;
	bas_g_x = g_x;
	bas_g_y = g_y;
	bas_g_z = g_z;

}


void get_raw_data()
{
	a_x_h = twi_read(0x3B);		//x축 가속도
	a_x_l = twi_read(0x3C);
	a_y_h = twi_read(0x3D);		//y축 가속도 
	a_y_l = twi_read(0x3E);		
	a_z_h = twi_read(0x3F);		//z축 가속도 
	a_z_l = twi_read(0x40);		
	g_x_h = twi_read(0x43);		//x축 각속도 
	g_x_l = twi_read(0x44);		
	g_y_h = twi_read(0x45);		//y축 각속도 
	g_y_l = twi_read(0x46);		
	g_z_h = twi_read(0x47);		//z축 각속도 
	g_z_l = twi_read(0x48);		

	m_x_h = twi_read(0x49);		//x축 콤파스
	m_x_l = twi_read(0x4A);
	m_y_h = twi_read(0x4B);		//y축 콤파스
	m_y_l = twi_read(0x4C);
	m_z_h = twi_read(0x4D);		//z축 콤파스
	m_z_l = twi_read(0x4E);

}


void MPU9250_init()
{
	twi_write(0x6B, 0x03); //[PWR_MGMT_1] sleep 끔, Auto PLL
	twi_write(0x1A, 0x05); //[CONFIG] DLPF 10Hz

	twi_write(0x6A, 0x20); //[USER_CTRL] ,master enable
	twi_write(0x24, 0x0D); //[I2C_MST_CTRL] ,내부 i2c clock 400Hz
	twi_write(0x67, 0x0F); //[I2C_MST_DELAY_CTRL]

	twi_write(0x2E, 0x0C); //[I2C_SLV3_ADDR] AK8963 write
    twi_write(0x2F, 0x0A); //[I2C_SLV3_REG]
    twi_write(0x30, 0x81); //[I2C_SLV3_CTRL]
    twi_write(0x66, 0x11); //[I2C_SLV3_DO]

    twi_write(0x34, 0x13); //[I2C_SLV4_CTRL] 모든 slave 19+1번 샘플
	
	twi_write(0x25, 0x8C); //[I2C_SLV0_ADDR] AK8963 read.
    twi_write(0x26, 0x10); //[I2C_SLV0_REG]
    twi_write(0x27, 0x81); //[I2C_SLV0_CTRL]

	twi_write(0x28, 0x8C); //[I2C_SLV1_ADDR] AK8963 read.
    twi_write(0x29, 0x11); //[I2C_SLV1_REG] 
    twi_write(0x2A, 0x81); //[I2C_SLV1_CTRL]

	twi_write(0x2B, 0x8C); //[I2C_SLV2_ADDR] AK8963 read.
    twi_write(0x2C, 0x12); //[I2C_SLV2_REG]
    twi_write(0x2D, 0x81); //[I2C_SLV2_CTRL]

	_delay_ms(1000);

	ASAX = twi_read(0x49);
	ASAY = twi_read(0x4A);
	ASAZ = twi_read(0x4B);

	_delay_ms(1000);


	twi_write(0x25, 0x8C); //[I2C_SLV0_ADDR] AK8963 read.
    twi_write(0x26, 0x03); //[I2C_SLV0_REG]
    twi_write(0x27, 0xD2); //[I2C_SLV0_CTRL]

	twi_write(0x28, 0x8C); //[I2C_SLV1_ADDR] AK8963 read.
    twi_write(0x29, 0x05); //I2C_SLV1_REG]
    twi_write(0x2A, 0xD2); //[I2C_SLV1_CTRL]

	twi_write(0x2B, 0x8C); //[I2C_SLV2_ADDR] AK8963 read.
    twi_write(0x2C, 0x07); //[I2C_SLV2_REG]
    twi_write(0x2D, 0xD2); //[I2C_SLV2_CTRL]
}








void twi_write(unsigned char address,unsigned char data)
{ 
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	//START

	while(!(TWCR & (1<<TWINT))); //TWINT flag 기다림 
	while((TWSR&0xF8) != 0x08);  //START 상태(08) 기다림  

	TWDR = 0b11010000;			 //AD(1101000)+W(0) 
	TWCR = (1<<TWINT)|(1<<TWEN); //전송 

	while(!(TWCR & (1<<TWINT))); 
	while((TWSR&0xF8) != 0x18);  //SLA+W ACK 상태(18) 기다림

	TWDR = address; 			 //register address
	TWCR = (1<<TWINT)|(1<<TWEN); //전송

	while(!(TWCR & (1<<TWINT)));
	while((TWSR&0xF8) != 0x28);  //Data ACK 상태(28) 기다림 

	TWDR = data; 				 //data 
	TWCR = (1<<TWINT)|(1<<TWEN); //전송  

	while(!(TWCR & (1<<TWINT)));
	while((TWSR&0xF8) != 0x28);

	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN); //STOP
} 

 

unsigned char twi_read(char address)
{ 
	unsigned char data;

	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	//START

	while(!(TWCR & (1<<TWINT))); //TWINT flag 기다림 
	while((TWSR&0xF8) != 0x08);  //START 상태(08) 기다림  

	TWDR = 0b11010000;			 //AD(1101000)+W(0) 
	TWCR = (1<<TWINT)|(1<<TWEN); //전송 

	while(!(TWCR & (1<<TWINT))); 
	while((TWSR&0xF8) != 0x18);  //SLA+W ACK 상태(18) 기다림

	TWDR = address; 			 //register address
	TWCR = (1<<TWINT)|(1<<TWEN); //전송

	while(!(TWCR & (1<<TWINT)));
	while((TWSR&0xF8) != 0x28);  //Data ACK 상태(28) 기다림 

	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	//Repeat START

	while(!(TWCR & (1<<TWINT)));
	while((TWSR&0xF8) != 0x10);  //Repeat START 상태(08) 기다림

	TWDR = 0b11010001;			 //AD(1101000)+R(1) 
	TWCR = (1<<TWINT)|(1<<TWEN); //전송 

	while(!(TWCR & (1<<TWINT)));
	while((TWSR&0xF8) != 0x40);  //SLA+R ACK 상태(40) 기다림 

	TWCR = (1<<TWINT)|(1<<TWEN); //전송

	while(!(TWCR & (1<<TWINT)));
	while((TWSR&0xF8) != 0x58);  //ACK 상태(58) 기다림 

	data = TWDR; 

	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN); //STOP

	return data; 
}



void USART_Transmit(unsigned char tx_data)
{ 
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = tx_data; 
}


void USART_Transmit_init4(int data)
{
	if(data < 0)
	{
		data = -data;
		USART_Transmit('-');
	}
	else
		USART_Transmit(' ');

	int temp = 0;
	temp = data/10000;
	USART_Transmit(temp+48);
	temp = (data%10000)/1000;
	USART_Transmit(temp+48);
	temp = (data%1000)/100;
	USART_Transmit(temp+48);
	temp = (data%100)/10;	
	USART_Transmit(temp+48);
	temp = data%10; 
	USART_Transmit(temp+48);
}
