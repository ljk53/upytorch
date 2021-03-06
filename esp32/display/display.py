from machine import Pin, SPI
import os

from . import m5stack
from . import glcdfont
from .fbconsole import FBConsole
from .ili934xnew import ILI9341, color565


class DisplayAdapter(object):
    def __init__(self, display, width, height):
        self.width = width
        self.height = height
        self.display = display
        self.display.erase()
        self.display.set_pos(0,0)
        self.scrolls = 0

    def fill(self, color):
        pass

    def fill_rect(self, x, y, w, h, color):
        self.display.fill_rectangle(x, self._y(y), w, h, color)

    def scroll(self, dx, dy):
        self.display.scroll(-dy)
        self.scrolls += -dy

    def hline(self, x, y, w, color):
        self.display.fill_rectangle(x, self._y(y), w, 1, color)

    def text(self, c, x, y, color):
        self.display.chars(c, x, self._y(y))

    def _y(self, y):
        return (y + self.scrolls) % self.height


def init():
    width = 320
    height = 240
    bgcolor = color565(255, 255, 255)
    fgcolor = color565(255, 0, 255)
    cursor_color = color565(0, 255, 255)

    power = Pin(m5stack.TFT_LED_PIN, Pin.OUT)
    power.value(1)
    spi = SPI(
        2,
        baudrate=40000000,
        miso=Pin(m5stack.TFT_MISO_PIN),
        mosi=Pin(m5stack.TFT_MOSI_PIN),
        sck=Pin(m5stack.TFT_CLK_PIN))

    display = ILI9341(
        spi,
        cs=Pin(m5stack.TFT_CS_PIN),
        dc=Pin(m5stack.TFT_DC_PIN),
        rst=Pin(m5stack.TFT_RST_PIN),
        w=height, # NB - the driver takes rotated size
        h=width,
        r=6)

    display.set_font(glcdfont)
    display.set_color(fg=fgcolor, bg=bgcolor)
    adapter = DisplayAdapter(display, width=width, height=height)
    scr = FBConsole(adapter, bgcolor=bgcolor, fgcolor=fgcolor, ccolor=cursor_color)
    os.dupterm(scr)
