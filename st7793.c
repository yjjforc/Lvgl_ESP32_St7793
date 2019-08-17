/**
 * @file st7793.c
 *
 */


/*********************
 *      INCLUDES
 *********************/
#include "st7793.h"
#include "disp_spi.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "lvgl/lvgl.h"
/*********************
 *      DEFINES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/*The LCD needs a bunch of command/argument values to be initialized. They are stored in this struct. */
typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes; //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd7793_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void st7793_send_cmd(uint16_t cmd);
static void st7793_send_data(uint16_t * data, uint16_t length);
static void st7793_send_color(uint16_t * data, uint16_t length);


extern uint8_t spi_ready_flag;

#include "esp_log.h"

#define st7793_tag           "st7793_tag"

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS  0x0100,0x1038
 **********************/
static uint16_t Csenddata[]={0x0000,0x1038,0x0808,0x8000,0x6200,0x0001,0x0001,
0x01B0,0x0018,0x000F,0x002F,0x0014,0x0070,0x0000,0x00EF,0x0000,0x018F,
0x0300,0x3A1A,0x0D02,0x0612,0x0131,0x0300,0x8D19,0x0405,0x0611,0x0131,
0x000A,0x0000,0x0100,0x0000,0x0000
};
static uint8_t Csendatacount=0;
void st7793_init(void)
{
	//Reset the display
	gpio_set_direction(ST7793_BCKL, GPIO_MODE_OUTPUT);
	gpio_set_level(ST7793_BCKL, 0);

	gpio_set_direction(ST7793_RST, GPIO_MODE_OUTPUT);
	gpio_set_level(ST7793_RST, 1);
	vTaskDelay(10);
	gpio_set_level(ST7793_RST, 0);
	vTaskDelay(10);
	gpio_set_level(ST7793_RST, 1);
	vTaskDelay(10);

	st7793_send_cmd(0x0001);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);        

	st7793_send_cmd(0x0003);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);        

	st7793_send_cmd(0x0008);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);       

	st7793_send_cmd(0x0090);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);      

	st7793_send_cmd(0x0400);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);    

	st7793_send_cmd(0x0401);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);       
	//--------------------- Power Control Registers Initial ---------------------//
	st7793_send_cmd(0x00FF);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);        

	st7793_send_cmd(0x0102);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);        

	st7793_send_cmd(0x0710);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);         //GVDD=4.489V

	st7793_send_cmd(0x0712);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);         

	st7793_send_cmd(0x0752);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);     

	st7793_send_cmd(0x0724);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);        //Frame rate

	st7793_send_cmd(0x0759);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);  
	/******************************************************************/
	vTaskDelay(10);                   //ms
	//-----------------------Display Windows 240 X 400-------------------------//
	st7793_send_cmd(0x0210);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);       //Horizontal Address Start Position
	st7793_send_cmd(0x0211);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);      //Horizontal Address End Position
	st7793_send_cmd(0x0212);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);        //Vertical Address Start Position
	st7793_send_cmd(0x0213);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);       //Vertical Address End Position
	vTaskDelay(10 );                   //ms
	//-----------------------Gamma Cluster Setting----------------------------//
	st7793_send_cmd(0x0380);          //Gamma setting start
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);        

	st7793_send_cmd(0x0381);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);      

	st7793_send_cmd(0x0382);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);        

	st7793_send_cmd(0x0383);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);     

	st7793_send_cmd(0x0384);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);      

	st7793_send_cmd(0x0385);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);    

	st7793_send_cmd(0x0386);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);      

	st7793_send_cmd(0x0387);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);      

	st7793_send_cmd(0x0388);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);       

	st7793_send_cmd(0x0389);          //Gamma setting end
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);      
	//----------------------Vcom Setting-------------------------------//
	st7793_send_cmd(0x0702);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);       //VCOM

	st7793_send_cmd(0x00FF);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);       
	//----------------------End Vcom Setting-----------------------------//
	st7793_send_cmd(0x0007);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);    

	vTaskDelay(20);                   //ms

	st7793_send_cmd(0x0200);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);       
	st7793_send_cmd(0x0201);          
	st7793_send_data((uint16_t*)&Csenddata[Csendatacount++],1);  

	st7793_send_cmd(0x0202); 


	printf("lcm init finish.\n");







}
uint8_t stop_flag=0;

void st7793_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map)
{
	uint16_t data[4]={0};

	st7793_send_cmd(0x0210);
	data[0]=area->y1;
	st7793_send_data((uint16_t*)&data[0],1);	

	st7793_send_cmd(0x0211);
	data[1]=area->y2;
	st7793_send_data((uint16_t*)&data[1],1);

	st7793_send_cmd(0x0212);
	data[2]=area->x1;
	st7793_send_data((uint16_t*)&data[2],1);

	st7793_send_cmd(0x0213);
	data[3]=area->x2;
	st7793_send_data((uint16_t*)&data[3],1);


	st7793_send_cmd(0x00200);
	data[3]=area->y1;
	st7793_send_data((uint16_t*)&data[3],1);

	st7793_send_cmd(0x00201);
	data[3]=area->x1;
	st7793_send_data((uint16_t*)&data[3],1);


	st7793_send_cmd(0x0202);

	uint32_t size = lv_area_get_width(area) * lv_area_get_height(area);
	st7793_send_color((uint16_t*)color_map, size*2 );


	while(spi_ready_flag);
	lv_disp_t * disp = lv_refr_get_disp_refreshing();
    lv_disp_flush_ready(&disp->driver);


	ESP_LOGI(st7793_tag, "area->y1 is %d",area->y1);
	ESP_LOGI(st7793_tag, "area->y2 is %d",area->y2);
	ESP_LOGI(st7793_tag, "area->x1 is %d",area->x1);
	ESP_LOGI(st7793_tag, "area->x2 is %d",area->x2);
	// ESP_LOGI(st7793_tag, "color_map is %d",*(uint16_t*)color_map);
}
	






/**********************
 *   STATIC FUNCTIONS
 **********************/


static void st7793_send_cmd(uint16_t cmd)
{
	uint8_t scmd[3]={0};
	scmd[0]=0x70;
	scmd[1]=cmd>>8;
	scmd[2]=cmd;
	disp_spi_send_data(scmd,3);


}

static void st7793_send_data(uint16_t * data, uint16_t length)
{

	uint8_t sdata[3]={0};
	
	for (uint16_t i = 0; i < length; i++)
	{
		sdata[0]=0x72;
		sdata[1]=*((uint16_t*)data)>>8;
		sdata[2]=*((uint16_t*)data);
		disp_spi_send_data(sdata, 3);

		data++;
	}

}
//static uint8_t stdata[15967];
static void st7793_send_color(uint16_t * data, uint16_t length)
{
	// uint8_t count=0;
	// uint16_t surplus=0;
	// count=length/8192;
	// surplus=length%8192;
	// uint8_t  i_count = 0;
	// if(count>0)
	// {
	// 	for ( i_count = 0; i_count < count; i_count++)
	// 	{
	// 		for (uint16_t i = 0; i <4096*3; )
	// 		{
				
	// 			stdata[i++]=*((uint16_t*)data)>>8;
	// 			stdata[i++]=*((uint16_t*)data);
	// 			data++;
	// 		}
	// 		disp_spi_send_colors(stdata,4096*3);
	// 	}

	// 		for (uint16_t i = 0; i < surplus+surplus/2; )
	// 		{
	// 			stdata[i++]=0x72;
	// 			stdata[i++]=*((uint16_t*)data)>>8;
	// 			stdata[i++]=*((uint16_t*)data);
	// 			data++;
	// 		}
	// 		disp_spi_send_colors(stdata,surplus+surplus/2);	
	// }
	// else
	// {
	// 	for (uint16_t i = 0; i < surplus+surplus/2; )
	// 	{
	// 		stdata[i++]=0x72;
	// 		stdata[i++]=*((uint16_t*)data)>>8;
	// 		stdata[i++]=*((uint16_t*)data);
	// 		data++;
	// 	}
	// 	disp_spi_send_colors(stdata,surplus+surplus/2);			
	// }	
	// uint16_t i = 0;
	// stdata[i++]=0x72;
	// for ( i=1; i < length+1; )
	// {
	// 	stdata[i++]=*((uint16_t*)data)>>8;
	// 	stdata[i++]=*((uint16_t*)data);
	// }
	disp_spi_send_colors((uint8_t*)data,length);
}
