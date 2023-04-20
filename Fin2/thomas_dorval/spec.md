# Word Filter (Redactor) Driver Spec

This character device will receive a message written to it and replace all the characters of any appearances of a word given via IOCTL with a given redacting character, also set via IOCTL.

The character device will show up in `/dev/` as a file called `redactor`. All users may interact with the file. The file state is shared among opened instances of the file descriptor.

When the device is loaded onto the kernel, it sets up the default values of each internal parameter used to redact, such as (but not limited to)

- Redacting Character
- Phrase to Redact
- Word to Redact from Phrase
- Redacted Phrase

Some example initial values can be:

- Asterisk `*` for Redacting Character
- `"John is at the door, while Jane is busy."` for Phrase to Redact
- `"John"` as a word to Redact from Phrase

When the device is unloaded from the kernel, it frees any and all memory that it has used, if applicable.

## Open

On open, the character driver redacts the initial Phrase to Redact and saves it.

## Read

Takes in a buffer, and the size of that buffer as input.

Place the redacted phrase on the given buffer. If the phrase to redact is empty, or the given size is less than the size of the redacted string, it returns a negative error. Otherwise, it returns the non-negative size of the bytes uses (the length of the redacted phrase).

## Write

Takes a buffer, and the size of that buffer as input.

Takes the information from a given buffer and saves it as the phrase to redact, then it redacts the phrase and saves it. If the write is successful, the function call returns the size of the redacted string as a non-negative number. Otherwise, it returns a negative error.

## Seek

Takes two arguments, offset and whence.

It updates the value of the file position with the value of `offset`, varying in behavior dictated by `whence`:

- `SEEK_SET` will set the value of the file position to the value of offset
- `SEEK_CUR` will add the offset to the file position
- `SEEK_END` will move the file position to the end

The file position will determine where to start reading the redacted phrase from when reading byte by byte.

If any of these operations results in placing the file out of bounds, then a negative error will be returned. Otherwise, the current, updated file position is returned.

## Ioctl

For some of the operations below, a message structure is needed. This is a `struct` that has a `size_t` size, and a `char *` buffer. An example of this can be found below:

```c
struct message {
    size_t size;
    char *buffer;
};
```

IO Control has several operations:

- `IOC_RDC_GET_LENGTH` takes a size argument and places the size of the redacted phrase in it. If there's no redacted phrase, it places 0. If there's no errors, it returns 0. Otherwise, it returns a negative error.

- `IOC_RDC_REVEAL` takes a message structure, and it places the original, non-redacted phrase on the given message's buffer, and then places the size of the original phrase on its size. If the buffer length is less than the size of the original phrase it returns a negative error. Otherwise, it returns 0.

- `IOC_RDC_TO_REDACT` takes a message structure, and it overwrites the previous word to redact with the contents of its buffer. If there's no errors, it returns 0. Otherwise, it returns a negative error.

- `IOC_RDC_REDACTING` takes a character, and it overwrites the previous redacting character with it. If there's no errors, it returns 0. Otherwise, it returns a negative error.

## Close

Takes no arguments.

When closing the file, it clears the phrase to redact along with the redacted phrase.
