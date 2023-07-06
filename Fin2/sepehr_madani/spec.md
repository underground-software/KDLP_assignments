# ASCII Art Character Device
The asciiart character device accepts messages written to it, transforms them
into ASCII art, and allows them to be read back. A newly written message
replaces the old one. The device appears in `/dev/` as a special character file
named `asciiart`, which all users can interact with. The state of the file is
shared between all open file descriptors. All users can `open`, `read`, `write`,
`close`, `seek`, and execute `ioctl` operations on the file.

`asciiart` will use the *ascii* font of Dot Matrix [1], which is readily
available to use at [this Github Gist](https://gist.github.com/1ssepehr/0f38189b21eba70edad8522aa67433fe)
[2].

When the kernel module is loaded, the driver must initialize to a state as if
the last string written was the empty string.

## Supported Operations

### open:
* Opens the device for reading, writing, or both, depending on the specified
flags. The file position is initially set to zero.
If successful, returns a nonnegative file descriptor. Otherwise, returns a
negative error indicating the issue.

### read:
* Reads back the now-transformed variation of the last string written into the device, in ASCII art style.
* Copies the minimum of the requested data and the available data (between the
    current location and the end of the file) into the provided buffer, starting
    at the file position.
* If the file position is at or past the end of the file, returns 0, indicating
    end-of-file.
* If successful, returns the nonnegative number of bytes copied and advances the
    file position by that many bytes. Otherwise, returns a negative error
    indicating the issue.

### write:
* Receives a string as input and stores it in the device, replacing any existing
    string. The file position of this file descriptor is reset to zero.
* Transforms the specified byte count into ASCII art representation. If the
    resulting ASCII art exceeds the buffer size (LINE_LIMIT * CHAR_HEIGHT + 1),
    only the characters that fit into the buffer are transformed and stored.
    The constant for LINE_LIMIT and CHAR_HEIGHT along with other constants are
    defined in `dotmatrix.h` file, and are part of this specification.
* If successful, returns the nonnegative number of bytes read from the input
    buffer. Otherwise, returns a negative error indicating the issue.

Notes:
* The input string should only include ASCII characters in the range of 32
    (space) to 126 (~), inclusive. If a string includes extended ASCII or non-
    ASCII characters, the character is entirely skipped, and operation continues
    on the rest of the characters.
* Each character from the input string is transformed into an ASCII
    representation according to the predefined alphabet array mentioned above
    [1]. The transformed representation is 10 rows tall and 21 characters wide
    (for each character), including the spaces for padding. These blocks are to
    be placed horizontally together; meaning that the output of the program is
    ALWAYS 10 lines, all of which end with a newline. The output itself is also
    null-terminated.
* If successful, the operation returns the nonnegative number of bytes of the
    original input string (not the transformed ASCII art string). Otherwise, it
    returns a negative error indicating the issue.
    * As an example, the string "A" MUST EXACTLY match the following string:
    ```
    "       _       \n"
    "     _(_)_     \n"
    "   _(_) (_)_   \n"
    " _(_)     (_)_ \n"
    "(_) _  _  _ (_)\n"
    "(_)(_)(_)(_)(_)\n"
    "(_)         (_)\n"
    "(_)         (_)\n"
    "               \n"
    "               \n"
    ```
* Writing the empty string is not the same as resetting the device. An empty
    string write() call will fill the buffer with 10 blank lines that only hold
    a new line character.

### close:
* Takes no arguments, and closes the file descriptor. The device state persists across close operations.
* Returns 0 if successful, otherwise returns a negative error.

### lseek:
* Updates the file position based on `offset` and `whence`.
* If the resulting position would be outside the file bounds, returns `-EINVAL`.
    Otherwise, sets the file position to the target location and returns that
    offset.
    * `SEEK_SET` will set the value of the file position to the value of offset.
    * `SEEK_CUR` will add the offset to the file position.
    * `SEEK_END` will move the file position to the end.

### ioctl:
* Supports two commands, otherwise returns `-EINVAL`:
    * `IOC_ASCIIART_RESET` (macro with of `_IO(0x12, 0)` from `<sys/ioctl.h>`)
        * Resets the device back to the initial state where no string is stored
            inside, and the size is reset to 0.
        * Takes no arguments and returns 0 if successful, otherwise a negative
            error.
    * `IOC_ASCIIART_GET_LENGTH` (macro with `_IOR(0x12, 1, sizeof(size_t)` from
        `<sys/ioctl.h>`)
        * Provides the length of the currently stored ASCII art representation
            in the device.
        * Takes the address of a `size_t` variable as an argument and stores the
            length within it.
        * Returns zero if successful, otherwise a negative error to indicate the
            issue.

## Notes
* The device should handle concurrency, allowing multiple applications to use it
    simultaneously.
* Device state (i.e., the ASCII art it holds) persists across open-close
    operations and across different applications.

[1] The device uses the character set from
[FIGlet website](http://www.figlet.org/fonts/dotmatrix.flf), designed by
[Curtis Wanner](cwanner@acs.bu.edu) (revision 08/21/1995) for ASCII art
transformation.

[2] A header file with a singular definition of the character array making the
alphabet is available at the
[Dot Matrix Github Gist](https://gist.github.com/1ssepehr/0f38189b21eba70edad8522aa67433fe).
