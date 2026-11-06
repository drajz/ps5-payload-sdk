#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>

#define SYS_SCE_USB_OPEN  662
#define SYS_SCE_USB_READ  663

typedef struct {
    uint8_t modifiers;   
    uint8_t reserved;    
    uint8_t keys[6];     
} __attribute__((packed)) GenericKeyboardPacket;

typedef struct {
    uint8_t buttons;     
    int8_t  x_axis;      
    int8_t  y_axis;      
    int8_t  wheel;       
} __attribute__((packed)) GenericMousePacket;

int main(int argc, char *argv[]) {
    int usb_fd = syscall(SYS_SCE_USB_OPEN, "/dev/usb/hid0", 0);
    if (usb_fd < 0) return -1; 

    uint8_t raw_buffer[64];

    while (1) {
        int bytes = syscall(SYS_SCE_USB_READ, usb_fd, raw_buffer, sizeof(raw_buffer));
        
        if (bytes > 0) {
            uint8_t report_id = raw_buffer[0];

            if (report_id == 0x01) { 
                // Kanal za miša radi normalno
                GenericMousePacket *mouse = (GenericMousePacket *)&raw_buffer[1];
            } 
            else if (report_id == 0x02) { 
                // Prisilno otvaramo i čitamo kanal za tastaturu sa istog dongle-a
                GenericKeyboardPacket *keyboard = (GenericKeyboardPacket *)&raw_buffer[1];
                
                for (int i = 0; i < 6; i++) {
                    uint8_t key = keyboard->keys[i];
                    if (key == 0x1A) { /* W */ }
                    if (key == 0x04) { /* A */ }
                    if (key == 0x16) { /* S */ }
                    if (key == 0x07) { /* D */ }
                    if (key == 0x2C) { /* Space */ }
                }
            }
        }
        usleep(4000); 
    }
    return 0;
}
