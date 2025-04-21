#include <stdio.h>

// memory chip decleration
long A1[32], A2[32], A3[32], A4[32], A5[32], A6[32], A7[32], A8[32]; // chip group A
long B1[32], B2[32], B3[32], B4[32], B5[32], B6[32], B7[32], B8[32]; // chip group B

// control signals and bus
int IOM, RW, OE;                 // Global control signals
unsigned int ADDR;              // Address bus (11-bit used)
unsigned char BUS;              // Data bus (8-bit)

// main memory function
void MainMemory() {
    if (!OE || IOM != 1) return;

    unsigned int col = ADDR & 0x001F;
    unsigned int row = (ADDR >> 5) & 0x001F;
    unsigned int cs  = (ADDR >> 10) & 0x0001;

    if (RW == 0) {
        unsigned char data = 0x00;

        data |= (((cs ? (B1[row] >> col) : (A1[row] >> col)) & 1) << 0);
        data |= (((cs ? (B2[row] >> col) : (A2[row] >> col)) & 1) << 1);
        data |= (((cs ? (B3[row] >> col) : (A3[row] >> col)) & 1) << 2);
        data |= (((cs ? (B4[row] >> col) : (A4[row] >> col)) & 1) << 3);
        data |= (((cs ? (B5[row] >> col) : (A5[row] >> col)) & 1) << 4);
        data |= (((cs ? (B6[row] >> col) : (A6[row] >> col)) & 1) << 5);
        data |= (((cs ? (B7[row] >> col) : (A7[row] >> col)) & 1) << 6);
        data |= (((cs ? (B8[row] >> col) : (A8[row] >> col)) & 1) << 7);

        BUS = data;
    } else if (RW == 1) {
        unsigned char mask = 1 << col;

        if (cs) {
            if (BUS & 0x01) B1[row] |= mask; else B1[row] &= ~mask;
            if (BUS & 0x02) B2[row] |= mask; else B2[row] &= ~mask;
            if (BUS & 0x04) B3[row] |= mask; else B3[row] &= ~mask;
            if (BUS & 0x08) B4[row] |= mask; else B4[row] &= ~mask;
            if (BUS & 0x10) B5[row] |= mask; else B5[row] &= ~mask;
            if (BUS & 0x20) B6[row] |= mask; else B6[row] &= ~mask;
            if (BUS & 0x40) B7[row] |= mask; else B7[row] &= ~mask;
            if (BUS & 0x80) B8[row] |= mask; else B8[row] &= ~mask;
        } else {
            if (BUS & 0x01) A1[row] |= mask; else A1[row] &= ~mask;
            if (BUS & 0x02) A2[row] |= mask; else A2[row] &= ~mask;
            if (BUS & 0x04) A3[row] |= mask; else A3[row] &= ~mask;
            if (BUS & 0x08) A4[row] |= mask; else A4[row] &= ~mask;
            if (BUS & 0x10) A5[row] |= mask; else A5[row] &= ~mask;
            if (BUS & 0x20) A6[row] |= mask; else A6[row] &= ~mask;
            if (BUS & 0x40) A7[row] |= mask; else A7[row] &= ~mask;
            if (BUS & 0x80) A8[row] |= mask; else A8[row] &= ~mask;
        }
    }
}

int main() {
    // Set control signals to access MAIN MEMORY
    IOM = 1;
    RW = 1;
    OE = 1;

    // Write instruction 0x3015 to memory addresses 0x000 and 0x001
    // upper byte
    ADDR = 0x000;
    BUS = 0x30; 
    MainMemory(); 
    // lower byte
    ADDR = 0x001;
    BUS = 0x15;
    MainMemory();

    // Write instruction 0x0C00 to memory addresses 0x002 and 0x003
    // upper byte
    ADDR = 0x002;
    BUS = 0x0C;
    MainMemory();
    // lower byte
    ADDR = 0x003;
    BUS = 0x00;
    MainMemory();

    // Test reading back
    RW = 0; // Set to read mode

    ADDR = 0x000;
    MainMemory();
    printf("ADDR 0x000 = 0x%02X\n", BUS);

    ADDR = 0x001;
    MainMemory();
    printf("ADDR 0x001 = 0x%02X\n", BUS);

    return 0;
}
