#include <net/if.h>


int tun_alloc(char *dev) //this function is used to create a TAP device in Linux. It takes a string argument dev, which specifies the name of the TAP device to be created. The function returns a file descriptor for the TAP device if successful, or a negative error code if there was an error.
{
    struct ifreq ifr; //ifr is a structure used to configure network interfaces in Linux. 
	//It is defined in the <net/if.h> header file and contains various fields that can be used to specify the properties of a network interface,
	// such as its name, flags, and address.
    int fd, err;

	//created fd can be used to read and write into the virtual device's ethernet buffer
    if( (fd = open("/dev/net/tap", O_RDWR)) < 0 ) { //checks whether file descriptor is valid or not
        print_error("Cannot open TUN/TAP dev");
        exit(1);
    }

    CLEAR(ifr); //this macro is used to clear the contents of the ifr structure, setting all its fields to zero.

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     */
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI; //this line sets the flags for the ifr structure to indicate that we want to create a TAP device (which operates at the Ethernet layer)
	//and that we do not want to provide packet information (PI) in the packets sent to or received from the device.
    if( *dev ) { // *dev is a string that specifies the name of the TAP device to be created. If it is not empty, the code copies its contents into the ifr_name field of the ifr structure using the strncpy function.
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

	//ioctl is a system call that performs device-specific input/output operations on file descriptors. In this case, it is used to configure the TAP device by passing the file descriptor fd, the TUNSETIFF command, and a pointer to the ifr structure. If the ioctl call fails (returns a negative value), an error message is printed, the file descriptor is closed, and the function returns the error code.
    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
        print_error("ERR: Could not ioctl tun: %s\n", strerror(errno));
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}