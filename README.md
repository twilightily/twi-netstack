# twi-netstack : a netstack im creating simply for fun purposes 

right now this thing only operates on link layer i.e. it can parse an ethernet header, it has a tuntap alloc function to create a tap device and it can run ARP protocol to ping the ip that the tap device was given

just so i remember to add this in the final read me :

## how do you run this
run the makefile first to compile obvs 
`make -f Makefile.net`
you'll get an executable `./netstack`

## a few checks to make sure this thing will run when you run it

the linux kernel module that's responsible for virtual interfaces should be active so run `sudo modprobe tun`
now check whether the tap and tun paths we specified actually exist in your device with `ls -l /dev/net/tap /dev/net/tun`
if they dont exist simply run:
```sudo mkdir -p /dev/net```
```sudo mknod /dev/net/tap c 10 200```
```sudo chmod 666 /dev/net/tap```

this will create the dev/net directory and hence we'll have the path we need (ik there's more details than that i'll add here later)

yay ok so now you're all set, run `sudo ./netstack`

it should show something saying successfully created tap0 and waiting for packets basically this is the terminal thats listening
now you open up another terminal and put `sudo ip link set dev tap0 up`
with this you're configuring the new virtual link so that linux knows how to send data to it

now lets assign some ip address to this side of the link with `sudo ip addr add 192.168.32.1/25 dev tap0`
NOW here's the fun part, try pinging some random ahh IP address like say `ping 192.168.32.2`

when you switch back to th initial terminal you'll see "Received ARP Packet of so and so bytes" and it just keeps repeating itself because its a broadcast thats saying "who has 192.168.32.2? tell 192.168.32.1"

very very cool :D