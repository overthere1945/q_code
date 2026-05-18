/*
 * Copyright (c) 2022, 2023 QUALCOMM Technologies Incorporated. All rights reserved.
 */

#ifndef GPIO_DT_H
#define GPIO_DT_H

/* 
 * Macros used for Sleep settings internal to pinctrl.dtsi not recommended to use by client dtsi
 */
#define GPIO_PRG_YES             0x0100
#define GPIO_PRG_NO              0x0000
#define GPIO_OUT_LOW             0x0040
#define GPIO_OUT_HIGH            0x0080

/* 
 * Macros used for GPIO configurations for client dtsi
 */

/* GPIO Direction Input / Output. Default value is GPIO_INPUT */
#define GPIO_INPUT               0x0001
#define GPIO_OUTPUT              0x0002

/* GPIO Pull configuration. Default value is GPIO_PULL_DOWN */
#define GPIO_PULL_DOWN           0x0004
#define GPIO_PULL_UP             0x0008
#define GPIO_NO_PULL             0x0010
#define GPIO_KEEPER              0x0020

/* macro to enable GPIO Strong Pull. */
#define GPIO_STRONG_PULL         0x0200

/* 
   GPIO Drive strength, you can specify any value interms of 10uA.
   for example to configure 2mA drive strength - GPIO_DRIVE_STRENGTH(200).
   Existing TLMM HW support only 2mA, 4mA, 6mA, 8mA, 10mA, 12mA, 14mA and 16mA.
   if you specify 300 (3mA) then HW configure to next supported configuration such as 4mA.
   Default is 2mA drive strength 
  */
#define GPIO_DRIVE_STRENGTH(value) (value << 16)

/* GPIO Output state high/low, only for bit bang GPIOs not for pinctrl gpios */
#define GPIO_ACTIVE_HIGH         0x0080
#define GPIO_ACTIVE_LOW          0x0040


/* Not recommended to use by clients as per spec, https://devpi.qualcomm.com/qct/rel/coreplatform/latest/+d/settings/gpios.html#dt-gpios */
#define GPIO_CFG(dir, pulltype, drivestrength, strongpull) \
                 (dir | pulltype | drivestrength | strongpull)

/* 
  Note: For Example usage refer to tlmmtest.dtsi (core/systemdrivers/tlmm/test) for pinctrl settings ?        Or refer to https://devpi.qualcomm.com/qct/rel/coreplatform/latest/+d/settings/gpios.html#dt-gpios
  
*/

/* id for GPIO Devices */
#define GPIO_DEVICE_TLMM         0x0000	

#endif //GPIO_DT_H
