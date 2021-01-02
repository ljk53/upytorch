# Copied from: https://github.com/boochow/FBConsole

import framebuf
import uio

class FBConsole(uio.IOBase):
    def __init__(self, fb, bgcolor=0, fgcolor=-1, width=-1, height=-1, readobj=None):
        self.readobj = readobj
        self.fb = fb
        if width > 0:
            self.width=width
        else:
            try:
                self.width=fb.width
            except:
                raise ValueError
        if height > 0:
            self.height=height
        else:
            try:
                self.height=fb.height
            except:
                raise ValueError
        self.bgcolor = bgcolor
        self.fgcolor = fgcolor
        self.char_size(6, 8)
        self.cls()

    def cls(self):
        self.x = 0
        self.y = 0
        self.y_end = 0
        self.fb.fill(self.bgcolor)
        try:
            self.fb.show()
        except:
            pass

    def char_size(self, cw, ch):
        self.cw = cw
        self.ch = ch
        self.w = self.width // cw
        self.h = self.height // ch

    def _putc(self, c):
        c = chr(c)
        if c == '\n':
            self._newline()
        elif c == '\x08':
            self._backspace()
        elif c >= ' ':
            self.fb.fill_rect(self.x * self.cw, self.y * self.ch, self.cw, self.ch, self.bgcolor)
            self.fb.text(c, self.x * self.cw, self.y * self.ch, self.fgcolor)
            self.x += 1
            if self.x >= self.w:
                self._newline()

    def _esq_read_num(self, buf, pos):
        digit = 1
        n = 0
        while buf[pos] != 0x5b:
            n += digit * (buf[pos] - 0x30)
            pos -= 1
            digit *= 10
        return n

    def write(self, buf):
        self._draw_cursor(self.bgcolor)
        i = 0
        while i < len(buf):
            c = buf[i]
            if c == 0x1b:
                i += 1
                esc = i
                while chr(buf[i]) in '[;0123456789':
                    i += 1
                c = buf[i]
                if c == 0x4b and i == esc + 1:   # ESC [ K
                    self._clear_cursor_eol()
                elif c == 0x44:   # ESC [ n D
                    for _ in range(self._esq_read_num(buf, i - 1)):
                        self._backspace()
            else:
                self._putc(c)
            i += 1
        self._draw_cursor(self.fgcolor)
        try:
            self.fb.show()
        except:
            pass
        return len(buf)

    def readinto(self, buf, nbytes=0):
        if self.readobj != None:
            return self.readobj.readinto(buf, nbytes)
        else:
            return None

    def _newline(self):
        self.x = 0
        self.y += 1
        if self.y >= self.h:
            self.fb.scroll(0, -self.ch)
            self.fb.fill_rect(0, self.height - self.ch, self.width, self.ch, self.bgcolor)
            self.y = self.h - 1
        self.y_end = self.y

    def _backspace(self):
        if self.x == 0:
            if self.y > 0:
                self.y -= 1
                self.x = self.w - 1
        else:
            self.x -= 1

    def _clear_cursor_eol(self):
        self.fb.fill_rect(self.x * self.cw, self.y * self.ch, self.width, self.ch, self.bgcolor)
        for l in range(self.y + 1, self.y_end + 1):
            self.fb.fill_rect(0, l * self.ch, self.width, self.ch, self.bgcolor)
        self.y_end = self.y

    def _draw_cursor(self, color):
        self.fb.hline(self.x * self.cw, self.y * self.ch + self.ch - 1, self.cw, color)
