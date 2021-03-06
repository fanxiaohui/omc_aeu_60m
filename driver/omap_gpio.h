/*******************************************************************************
********************************************************************************
* 文件名称: omap_gpio.h
* 功能描述:  gpio的寄存器定义，和对gpio操作的宏定义
* 使用说明: 
* 文件作者:	H4
* 编写日期: （2012/06/15）
* 修改历史:
* 修改日期    修改人       修改内容
*-------------------------------------------------------------------------------

*******************************************************************************/
#ifndef _OMAP_GPIO_H
#define _OMAP_GPIO_H
#define GPIO_PHY_ADDR_BASE 0x01E26000



typedef struct {
	volatile unsigned int revid0;
	volatile unsigned int revid1;
	volatile unsigned int binten;
	volatile unsigned int rev0;
	volatile unsigned int         dir01;
	volatile unsigned int    out_data01;
	volatile unsigned int    set_data01;
	volatile unsigned int    clr_data01;
	volatile unsigned int     in_data01;
	volatile unsigned int set_ris_tri01;
	volatile unsigned int clr_ris_tri01;
	volatile unsigned int set_fal_tri01;
	volatile unsigned int clr_fal_tri01;
	volatile unsigned int     intstat01;
	volatile unsigned int         dir23;
	volatile unsigned int    out_data23;
	volatile unsigned int    set_data23;
	volatile unsigned int    clr_data23;
	volatile unsigned int     in_data23;
	volatile unsigned int set_ris_tri23;
	volatile unsigned int clr_ris_tri23;
	volatile unsigned int set_fal_tri23;
	volatile unsigned int clr_fal_tri23;
	volatile unsigned int     intstat23;
	volatile unsigned int         dir45;
	volatile unsigned int    out_data45;
	volatile unsigned int    set_data45;
	volatile unsigned int    clr_data45;
	volatile unsigned int     in_data45;
	volatile unsigned int set_ris_tri45;
	volatile unsigned int clr_ris_tri45;
	volatile unsigned int set_fal_tri45;
	volatile unsigned int clr_fal_tri45;
	volatile unsigned int     intstat45;
	volatile unsigned int         dir67;
	volatile unsigned int    out_data67;
	volatile unsigned int    set_data67;
	volatile unsigned int    clr_data67;
	volatile unsigned int     in_data67;
	volatile unsigned int set_ris_tri67;
	volatile unsigned int clr_ris_tri67;
	volatile unsigned int set_fal_tri67;
	volatile unsigned int clr_fal_tri67;
	volatile unsigned int     intstat67;
	volatile unsigned int         dir8;
	volatile unsigned int    out_data8;
	volatile unsigned int    set_data8;
	volatile unsigned int    clr_data8;
	volatile unsigned int     in_data8;
	volatile unsigned int set_ris_tri8;
	volatile unsigned int clr_ris_tri8;
	volatile unsigned int set_fal_tri8;
	volatile unsigned int clr_fal_tri8;
	volatile unsigned int     intstat8;
	
}gpio_reg;

#define GPIO_DIR_OUT(reg,pin) (gpio_t->reg)=((gpio_t->reg)&(~(pin)))
#define GPIO_DIR_IN(reg,pin) (gpio_t->reg)=((gpio_t->reg)|(pin))
#define GPIO_OUT(reg,pin) (gpio_t->reg)=(pin)
#define GPIO_IN(reg,pin)  (((gpio_t->reg)&(pin))?1:0)

#define GPIO_OUTPUT_DIR(n,m) (gpio_t->dir##n)=((gpio_t->dir##n)&(~(0x00000001<<m)))
#define GPIO_INPUT_DIR(n,m) (gpio_t->dir##n)=((gpio_t->dir##n)|(0x00000001<<m))
#define GPIO_SET(n,m) (gpio_t->set_data##n)=((0x00000001<<m))
#define GPIO_CLR(n,m) (gpio_t->clr_data##n)=((0x00000001<<m))
#define GPIO_IN(n,m,data) data=(((gpio_t->in_data##n)&(0x00000001<<m))?1:0)
#define GPIO_SET8_BYTE(m) (gpio_t->set_data8=m)
#define GPIO_CLR8_BYTE(m) (gpio_t->clr_data8=m)

void dru_iic_dir(unsigned int pin,unsigned char level);
unsigned int   dru_iic_gpio_in(unsigned int pin);

extern gpio_reg *gpio_t;

extern unsigned int iic_gpio_save;

#endif 

