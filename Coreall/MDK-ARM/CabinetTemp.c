#include "CabinetTemp.h"
#include "stdio.h"

/**
 * @brief  180MHz主频下精准us延时（核心适配）
 * @param  us: 要延时的微秒数（范围：1~1000）
 * @retval 无
 * @note   空循环次数经180MHz主频校准，误差<1us
 */
static void DHT11_Delay_us(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 1000000 * us) / 9;
    while (delay--)
    {
        __NOP();  // 空操作，保证延时精度
    }
}

/**
 * @brief  DHT11引脚配置为输出模式（推挽）
 * @param  无
 * @retval 无
 */
static void DHT11_Set_Output_Mode(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;    // 推挽输出
    GPIO_InitStruct.Pull = GPIO_NOPULL;            // 无上下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 高速
    HAL_GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStruct);
}

/**
 * @brief  DHT11引脚配置为输入模式（上拉）
 * @param  无
 * @retval 无
 */
static void DHT11_Set_Input_Mode(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;        // 输入模式
    GPIO_InitStruct.Pull = GPIO_PULLUP;            // 上拉（防止总线浮空）
    HAL_GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStruct);
}

/**
 * @brief  发送DHT11起始信号（主机主动）
 * @param  无
 * @retval 无
 */
static void DHT11_Send_Start(void)
{
    DHT11_Set_Output_Mode();       // 切换为输出模式
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET);  // 拉低总线≥18ms
    HAL_Delay(20);                 // 延时20ms（满足起始信号要求）
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET);    // 释放总线，拉高20~40us
    DHT11_Delay_us(30);
}

/**
 * @brief  等待DHT11响应信号
 * @param  无
 * @retval 0:响应成功  1:响应失败（超时）
 */
static uint8_t DHT11_Wait_Response(void)
{
    uint32_t timeout = 0;
    DHT11_Set_Input_Mode();  // 切换为输入模式，等待传感器响应

    // 等待传感器拉低总线（响应信号：低电平80us）
    while (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET)
    {
        timeout++;
        DHT11_Delay_us(1);
        if (timeout > 100) return 1;  // 超时100us，响应失败
    }

    timeout = 0;
    // 等待传感器释放总线（响应信号：高电平80us）
    while (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_RESET)
    {
        timeout++;
        DHT11_Delay_us(1);
        if (timeout > 100) return 1;  // 超时100us，响应失败
    }

    return 0;  // 响应成功
}

/**
 * @brief  从DHT11读取1位数据
 * @param  无
 * @retval 读取的位值（0/1）
 */
static uint8_t DHT11_Read_Bit(void)
{
    uint32_t timeout = 0;
    // 等待总线拉低（每bit起始：低电平50us）
    while (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET)
    {
        timeout++;
        DHT11_Delay_us(1);
        if (timeout > 100) return 0;  // 超时默认返回0
    }

    // 跳过低电平，判断高电平时长（0:26~28us；1:70us）
    DHT11_Delay_us(27);
    uint8_t bit = 0;
    if (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET)
    {
        bit = 1;
    }

    // 等待当前bit传输结束（总线拉低）
    timeout = 0;
    while (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET)
    {
        timeout++;
        DHT11_Delay_us(1);
        if (timeout > 100) break;
    }

    return bit;
}

/**
 * @brief  从DHT11读取1字节数据（高位先行）
 * @param  无
 * @retval 读取的字节值（0~255）
 */
static uint8_t DHT11_Read_Byte(void)
{
    uint8_t byte = 0;
    // 逐位读取8位，高位先出
    for (uint8_t i = 0; i < 8; i++)
    {
        byte <<= 1;          // 左移1位，预留低位
        byte |= DHT11_Read_Bit();  // 读取当前位
    }
    return byte;
}

/**
 * @brief  DHT11初始化（对外接口，替代原DHT11_GPIO_Init）
 * @param  无
 * @retval 0:初始化成功
 */
uint8_t DHT11_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();  // 使能GPIOA时钟
    DHT11_Set_Output_Mode();       // 默认输出模式
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET);  // 引脚默认拉高
    return 0;
}

/**
 * @brief  获取湿度值（对外接口）
 * @param  无
 * @retval 湿度整数部分
 */
uint8_t Get_humidity(void)
{
    return DHT11_Hum_Int;
}

/**
 * @brief  获取温度值（对外接口）
 * @param  无
 * @retval 温度整数部分
 */
uint8_t Get_temperature(void)
{
    return DHT11_Temp_Int;
}

/**
 * @brief  读取DHT11温湿度数据（核心函数）
 * @param  无
 * @retval 0:读取成功  1:响应失败  2:校验错误
 */
uint8_t DHT11_Read_Data(void)
{
    uint8_t buf[5] = {0};  // 40位数据缓存：[0]湿度整 [1]湿度小 [2]温度整 [3]温度小 [4]校验和
    uint8_t i = 0;

    // 1. 发送起始信号
    DHT11_Send_Start();

    // 2. 等待传感器响应
    if (DHT11_Wait_Response() != 0)
    {
        return 1;  // 响应失败
    }

    // 3. 读取40位数据（5字节）
    for (i = 0; i < 5; i++)
    {
        buf[i] = DHT11_Read_Byte();
    }

    // 4. 校验数据（湿度整+湿度小+温度整+温度小 = 校验和）
    if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
    {
        DHT11_Hum_Int = buf[0];    // 湿度整数（DHT11小数位为0，无需处理）
        DHT11_Temp_Int = buf[2];   // 温度整数
        DHT11_CheckSum = buf[4];   // 校验和
        return 0;  // 读取成功
    }
    else
    {
        return 2;  // 校验错误
    }
}