#include "Keys.h"

#define EVENT_CB(ev)   if(handle->cb[ev])handle->cb[ev]((void*)handle)
#define PRESS_REPEAT_MAX_NUM  15 /*!< The maximum value of the repeat counter */

 //Key handle list head.
static struct Key* head_handle = NULL;

// void Key_handler(struct Key* handle);

/**
  * @brief  Initializes the Key struct handle.
  * @param  handle: the Key handle struct.
  * @param  pin_level: read the HAL GPIO of the connected Key level.
  * @param  active_level: pressed GPIO level.
  * @param  Key_id: the Key id.
  * @retval None
  */
void Key_init(struct Key* handle, uint8_t(*pin_level)(uint8_t), uint8_t active_level, uint8_t Key_id)
{
    memset(handle, 0, sizeof(struct Key));
    handle->event = (uint8_t)NONE_PRESS;
    handle->hal_Key_Level = pin_level;
    handle->Key_level = !active_level;
    handle->active_level = active_level;
    handle->Key_id = Key_id;
}

/**
  * @brief  Attach the Key event callback function.
  * @param  handle: the Key handle struct.
  * @param  event: trigger event type.
  * @param  cb: callback function.
  * @retval None
  */
void Key_attach(struct Key* handle, PressEvent event, BtnCallback cb)
{
    handle->cb[event] = cb;
}

/**
  * @brief  Inquire the Key event happen.
  * @param  handle: the Key handle struct.
  * @retval Key event.
  */
PressEvent get_Key_event(struct Key* handle)
{
    return (PressEvent)(handle->event);
}

/**
  * @brief  Key driver core function, driver state machine.
  * @param  handle: the Key handle struct.
  * @retval None
  */
static void Key_handler(struct Key* handle)
{
    uint8_t read_gpio_level = handle->hal_Key_Level(handle->Key_id);
    if (read_gpio_level != handle->Key_level) handle->Key_level = read_gpio_level;

    //ticks counter working..
    if ((handle->state) > 0) handle->ticks++;

    /*------------Key debounce handle---------------*/
    // if (read_gpio_level != handle->Key_level)
    // {
    //     handle->Key_level = read_gpio_level;
        // if (++(handle->debounce_cnt) >= DEBOUNCE_TICKS)
        // {
        //     handle->Key_level = read_gpio_level;
        //     handle->debounce_cnt = 0;
        // }
    // }
    // else
    // { //level not change ,counter reset.
    //     handle->debounce_cnt = 0;
    // }

    /*-----------------State machine-------------------*/
    switch (handle->state)
    {
    case 0:
        if (handle->Key_level == handle->active_level)
        {	//start press down
            handle->event = (uint8_t)PRESS_DOWN;
            EVENT_CB(PRESS_DOWN);
            handle->ticks = 0;
            handle->repeat = 1;
            handle->state = 1;
        }
        else
        {
            handle->event = (uint8_t)NONE_PRESS;
        }
        break;

    case 1:
        if (handle->Key_level != handle->active_level)
        { //released press up
            handle->event = (uint8_t)PRESS_UP;
            EVENT_CB(PRESS_UP);
            handle->ticks = 0;
            handle->state = 2;
        }
        else if (handle->ticks > LONG_TICKS)
        {
            handle->event = (uint8_t)LONG_PRESS_START;
            EVENT_CB(LONG_PRESS_START);
            handle->state = 5;
        }
        break;

    case 2:
        if (handle->Key_level == handle->active_level)
        { //press down again
            handle->event = (uint8_t)PRESS_DOWN;
            EVENT_CB(PRESS_DOWN);
            if (handle->repeat != PRESS_REPEAT_MAX_NUM)
            {
                handle->repeat++;
            }
            EVENT_CB(PRESS_REPEAT); // repeat hit
            handle->ticks = 0;
            handle->state = 3;
        }
        else if (handle->ticks > SHORT_TICKS)
        { //released timeout
            if (handle->repeat == 1)
            {
                handle->event = (uint8_t)SINGLE_CLICK;
                EVENT_CB(SINGLE_CLICK);
            }
            else if (handle->repeat == 2)
            {
                handle->event = (uint8_t)DOUBLE_CLICK;
                EVENT_CB(DOUBLE_CLICK); // repeat hit
            }
            handle->state = 0;
        }
        break;

    case 3:
        if (handle->Key_level != handle->active_level)
        { //released press up
            handle->event = (uint8_t)PRESS_UP;
            EVENT_CB(PRESS_UP);
            if (handle->ticks < SHORT_TICKS)
            {
                handle->ticks = 0;
                handle->state = 2; //repeat press
            }
            else
            {
                handle->state = 0;
            }
        }
        else if (handle->ticks > SHORT_TICKS)
        { // SHORT_TICKS < press down hold time < LONG_TICKS
            handle->state = 1;
        }
        break;

    case 5:
        if (handle->Key_level == handle->active_level)
        {
            //continue hold trigger
            handle->event = (uint8_t)LONG_PRESS_HOLD;
            EVENT_CB(LONG_PRESS_HOLD);
        }
        else
        { //released
            handle->event = (uint8_t)PRESS_UP;
            EVENT_CB(PRESS_UP);
            handle->state = 0; //reset
        }
        break;
    default:
        handle->state = 0; //reset
        break;
    }
}

/**
  * @brief  Start the Key work, add the handle into work list.
  * @param  handle: target handle struct.
  * @retval 0: succeed. -1: already exist.
  */
int Key_start(struct Key* handle)
{
    struct Key* target = head_handle;
    while (target)
    {
        if (target == handle) return -1;
        target = target->next;
    }
    handle->next = head_handle;
    head_handle = handle;
    return 0;
}

/**
  * @brief  background ticks, timer repeat invoking interval 5ms.
  * @param  None.
  * @retval None
  */
void Key_ticks(void)
{
    struct Key* target;
    for (target = head_handle; target; target = target->next)
    {
        Key_handler(target);
    }
}