#include "dht11.h"
#include "delay.h"

// PA8设为输出模式（HAL库标准写法）
static void DHT11_IO_Output(void)
{
    GPIO_InitTypeDef gpio_cfg = {0};
    
    __HAL_RCC_GPIOA_CLK_ENABLE(); // 使能GPIOA时钟
    
    gpio_cfg.Pin = DHT11_GPIO_PIN;
    gpio_cfg.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽输出
    gpio_cfg.Pull = GPIO_NOPULL;          // 无上下拉
    gpio_cfg.Speed = GPIO_SPEED_FREQ_HIGH;// 高速模式
    HAL_GPIO_Init(DHT11_GPIO_PORT, &gpio_cfg);
}

// PA8设为输入模式（HAL库标准写法）
static void DHT11_IO_Input(void)
{
    GPIO_InitTypeDef gpio_cfg = {0};
    
    __HAL_RCC_GPIOA_CLK_ENABLE(); // 使能GPIOA时钟
    
    gpio_cfg.Pin = DHT11_GPIO_PIN;
    gpio_cfg.Mode = GPIO_MODE_INPUT;      // 输入模式
    gpio_cfg.Pull = GPIO_NOPULL;          // 无上下拉
    HAL_GPIO_Init(DHT11_GPIO_PORT, &gpio_cfg);
}

/************************* 复位DHT11 *************************/
void DHT11_Rst(void)
{
    DHT11_IO_Output();  // 切换为输出模式
    DHT11_DQ_OUT(0);    // 拉低总线（复位信号）
    delay_ms(20);       // 拉低≥18ms，满足DHT11协议
    DHT11_DQ_OUT(1);    // 释放总线
    delay_us(30);       // 主机拉高20~40us，等待传感器应答
}

/************************* 检测DHT11应答 *************************/
uint8_t DHT11_Check(void)
{
    uint8_t retry = 0;
    DHT11_IO_Input();   // 切换为输入模式
    
    // 等待传感器拉低总线（应答信号：40~80us）
    while (DHT11_DQ_IN() && retry < 200)  // 增大重试阈值，提高容错
    {
        retry++;
        delay_us(1);
    }
    if (retry >= 200) return 1; // 超时无应答
    
    retry = 0;
    // 等待传感器释放总线（应答后拉高40~80us）
    while (!DHT11_DQ_IN() && retry < 200)
    {
        retry++;
        delay_us(1);
    }
    if (retry >= 200) return 1; // 超时无应答
    
    return 0; // 检测到有效应答
}

/************************* 读取1个位 *************************/
uint8_t DHT11_Read_Bit(void)
{
    uint8_t retry = 0;
    
    // 等待总线拉低（位起始信号）
    while (DHT11_DQ_IN() && retry < 200)
    {
        retry++;
        delay_us(1);
    }
    retry = 0;
    
    // 等待总线拉高（数据位开始）
    while (!DHT11_DQ_IN() && retry < 200)
    {
        retry++;
        delay_us(1);
    }
    
    // 延时40us后检测电平（区分0/1：0=26~28us高，1=70us高）
    delay_us(40);
    return DHT11_DQ_IN() ? 1 : 0;
}

/************************* 读取1个字节 *************************/
uint8_t DHT11_Read_Byte(void)
{
    uint8_t i, data = 0;
    for (i = 0; i < 8; i++)
    {
        data <<= 1;                  // 左移1位，准备接收下一位
        data |= DHT11_Read_Bit();    // 拼接当前位
    }
    return data;
}

/************************* 初始化DHT11 *************************/
uint8_t DHT11_Init(void)
{
    DHT11_Rst();
    return DHT11_Check();
}

/************************* 读取温湿度数据 *************************/
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi)
{
    uint8_t buf[5];
    uint8_t i;
    
    DHT11_Rst();
    if (DHT11_Check() == 0)
    {
        // 读取5个字节（40位）
        for (i = 0; i < 5; i++)
        {
            buf[i] = DHT11_Read_Byte();
        }
        
        // 校验和验证（前4字节和 = 第5字节）
        if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humi = buf[0]; // 湿度整数部分（DHT11小数位为0）
            *temp = buf[2]; // 温度整数部分（DHT11小数位为0）
            return 0;       // 读取成功
        }
    }
    
    return 1; // 读取失败
}