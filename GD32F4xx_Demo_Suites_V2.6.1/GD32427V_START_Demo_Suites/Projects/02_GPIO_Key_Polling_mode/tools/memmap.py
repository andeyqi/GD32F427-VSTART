#python -m pip install -U prettytable
####      或者
#pip install -U prettytable

from prettytable import PrettyTable
x = PrettyTable()

x.title = 'Flash space'
x.field_names = ["Name", "Address Range","size(bytes)"]
x.add_row(["Sector 0", "0x0800 0000 - 0x08003FFF","16KB"])
x.add_row(["Sector 1", "0x0800 4000 - 0x08007FFF","16KB"])
x.add_row(["Sector 2", "0x0800 8000 - 0x0800BFFF","16KB"])
x.add_row(["Sector 3", "0x0800 C000 - 0x0800FFFF","16KB"])
x.add_row(["Sector 4", "0x0801 0000 - 0x0801FFFF","64KB"])
x.add_row(["Sector 5", "0x0802 0000 - 0x0803FFFF","128KB"])
x.add_row(["Sector 6", "0x0804 0000 - 0x0805FFFF","128KB"])
x.add_row(["Sector 7", "0x0806 0000 - 0x0807FFFF","128KB"])
x.add_row(["Sector 8", "0x0808 0000 - 0x0809FFFF","128KB"])
x.add_row(["Sector 9", "0x080A 0000 - 0x080BFFFF","128KB"])
x.add_row(["Sector 10", "0x080C 0000 - 0x080DFFFF","128KB"])
x.add_row(["Sector 11", "0x080E 0000 - 0x080FFFFF","128KB"])
x.add_row(["Sector 12", "0x0810 0000 - 0x08103FFF","16KB"])
x.add_row(["Sector 13", "0x0810 4000 - 0x08107FFF","16KB"])
print(x)

y = PrettyTable()
y.title = 'AHB perpherals'
y.field_names = ["Perpheral type", "Addr"]
y.add_row(["FAIM memory", "0x5001 0000~0x5001 4000"])
y.add_row(["MTB registers", "0x5000 C000~0x5001 0000"])
y.add_row(["DMA controller", "0x5000 8000~0x5000 C000"])
y.add_row(["SCTimer / PWM", "0x5000 4000~0x5000 8000"])
y.add_row(["CRC engine", "0x5000 0000~0x5000 4000"])
print(y)


z = PrettyTable()
z.title = 'APB perpherals'
z.field_names = ["Perpheral type", "Addr"]
z.add_row(["(reserved)", "0x4007 8000~0x4008 0000"])
z.add_row(["UART4", "0x4007 4000~0x4007 8000"])
z.add_row(["UART3", "0x4007 0000~0x4007 4000"])
z.add_row(["UART2", "0x4006 C000~0x4007 0000"])
z.add_row(["UART1", "0x4006 8000~0x4006 C000"])
z.add_row(["UART0", "0x4006 4000~0x4006 8000"])
z.add_row(["CapTouch", "0x4006 0000~0x4006 4000"])
z.add_row(["SPI1", "0x4005 C000~0x4006 0000"])
z.add_row(["SPI0", "0x4005 8000~0x4005 C000"])
z.add_row(["I2C1", "0x4005 4000~0x4005 8000"])
z.add_row(["I2C0", "0x4005 0000~0x4005 4000"])
z.add_row(["(reserved)", "0x4004 C000~0x4005 0000"])
z.add_row(["Syscon", "0x4004 8000~0x4004 C000"])
z.add_row(["IOCON", "0x4004 4000~0x4004 8000"])
z.add_row(["Flash controller", "0x4004 0000~0x4004 4000"])
z.add_row(["(reserved)", "0x4003 C000~0x4004 0000"])
z.add_row(["CTIMER 0", "0x4003 8000~0x4003 C000"])
z.add_row(["I2C3", "0x4003 4000~0x4003 8000"])
z.add_row(["I2C2", "0x4003 0000~0x4003 4000"])
z.add_row(["Input Multiplexing", "0x4002 C000~0x4003 0000"])
z.add_row(["(reserved)", "0x4002 8000~0x4002 C000"])
z.add_row(["Analog Comparator", "0x4002 4000~0x4002 8000"])
z.add_row(["PMU", "0x4002 0000~0x4002 4000"])
z.add_row(["ADC", "0x4001 C000~0x4002 0000"])
z.add_row(["DAC1", "0x4001 8000~0x4001 C000"])
z.add_row(["DAC0", "0x4001 4000~0x4001 8000"])
z.add_row(["FAIM controller", "0x4001 0000~0x4001 4000"])
z.add_row(["Switch Matrix", "0x4000 C000~0x4001 0000"])
z.add_row(["Wake-up Timer", "0x4000 8000~0x4000 C000"])
z.add_row(["Multi-Rate Timer", "0x4000 4000~0x4000 8000"])
z.add_row(["Watchdog timer", "0x4000 0000~0x4000 4000"])
print(z)