#include "key.h"

struct keys key[5];                                                         // 声明结构体数组 -- 分别存放的是6个按键
/*按键按下为高电平*/

/*短按*/
void key_serv(void)
{
    key[1].key_sta = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4);                   // KEY0
    key[2].key_sta = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);                   // KEY_UP
    key[3].key_sta = HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_8);                   // KEY1
    key[4].key_sta = HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_3);                   // KEY3
    
    for (int i = 0; i < 5; i ++)                                            // for循环轮询按键状态
    {
        switch (key[i].judge_sta)
        {
            case 0:                                                         // 判断按键是否被按下
            {
                if (key[i].key_sta)                                         // 按键被按下(还未进行按键消抖)
                    key[i].judge_sta = 1;                                   // 进入按键消抖状态
            }break;
            case 1:                                                         // 按键消抖
            {
                if (key[i].key_sta)                                         // 确认按键被有效按下
                {
                    key[i].judge_sta = 2;                                   // 进入松手检测状态
                    key[i].short_flag = 1;                                  // 置位短按标志位
                }
                else
                    key[i].judge_sta = 0;                                   // 重新判断按键是否被按下
            }break;
            case 2:
            {
                if (!key[i].key_sta)                                        // 确认按键被松开
                    key[i].judge_sta = 0;                                   // 重新判断按键是否被按下
            }break;
        }
    }
}
