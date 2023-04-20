#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define DEVICE_FILE "/dev/caesar_cipher" //pretending this is the name of the dev file
#define IOC_CAESAR_SET_OFFSET _IO(0x11, 0)

int main()
{
	int fd, shift, read_bytes, write_bytes;
	char buffer[1024];

	fd = open(DEVICE_FILE, O_RDWR); //Opens the dev file
	if (fd < 0)
	{
		perror("Failed to open the dev file");
		exit(EXIT_FAILURE);
	}

	shift = 3;
	if (ioctl(fd, IOC_CAESAR_SET_OFFSET, shift) < 0) //Moves the value around 
	{
		perror("Failed to set the shift value");
		close(fd);
		exit(EXIT_FAILURE);
	}

	read_bytes = read(fd, buffer, sizeof(buffer)); //Reads the data and puts it into the buffer
	if (read_bytes < 0)
	{
		perror("Failed to read data from the file");
		close(fd);
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < read_bytes; i++) //Encrypts the message before going into the buffer
	{
		if (buffer[i] >= 'a' && buffer[i] <= 'z')
		{
			buffer[i] = 'a' + ((buffer[i] - 'a' + shift) % 26);
		}
		else if (buffer[i] >= 'A' && buffer[i] <= 'Z')
		{
			buffer[i] = 'A' + ((buffer[i] - 'A' + shift) % 26);
		}
	}

	if (lseek(fd, 0, SEEK_SET) < 0) //Sets the file position
	{
		perror("Failed to set the file position");
		close(fd);
		exit(EXIT_FAILURE);
	}

	write_bytes = write(fd, buffer, read_bytes); //Writes the encrypted data from the buffer back to the file
	if (write_bytes < 0)
	{
		perror("Failed to write data to the file");
		close(fd);
		exit(EXIT_FAILURE);
	}

	if (close(fd) < 0) //Closes the dev file
	{
		perror("Failed to close the dev file");
		exit(EXIT_FAILURE);
	}

	printf("Your message has been encrypted using the Caesars Cipher!\n"); //Letting you know if it had worked
	exit(EXIT_SUCCESS);
}
