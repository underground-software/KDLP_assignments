# Fin1 Spec: Basic calculator

Author: Ian Sodersjerna
Date: 4/17/2023

## Description:

* This character device will function as a basic calculator preforming addition, subtraction, multiplication, and division on two integers.

* The character device appears in '/dev/' as a character device named 'calc'.

* Any logged in user may interact with the driver.

* User interactions with the driver are shared between users

* the character device stores two integers

* the character device stores the operation to be performed

* when the kernel module is loaded the two values are zero and the default operation is addition

* division operations should only return an integer

* The character device supports the following operations:

    - open
    - close
    - read
        - Reading the message reads the value of the operation into the buffer.
        - If the operation is not possible the buffer will contain 'NAN', if the operation includes a division by zero it will return 'DIVZERO'.
        - it will return the length of the string of the resultant value of the operation.
    - write
        - Writing a message to the character device inputs the given value into the 'right' value and moves the original value of the 'right' variable to the 'left' variable 
        - Returns zero on success
        - maximum value is INT_MAX and min is INT_MIN
    - ioctl
        - 4 commands can be specified they return 0 if successful, otherwise '-EIVAL' is returned.
            - 'IOC_CALC_ADDITION'
                - add the two numbers
                - this command requires no parameters
            - 'IOC_CALC_SUBTRACT'
                - subtract the two numbers
                - this command requires no parameters
            - 'IOC_CALC_MULTIPLICATION'
                - multiply the two numbers
                - this command requires no parameters
            - 'IOC_CALC_DIVISION'
                - divide the two numbers
                - this command requires no parameters
    - seek
        - calling seek on the character device will reset the 'left' and 'right' values to zero
