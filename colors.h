struct COLORS {
  byte r;
  byte g;
  byte b;

   COLORS() : r(0), g(0), b(0) {}

  COLORS(byte red, byte green, byte blue) : r(red), g(green), b(blue) {}
};

const COLORS red[4] = {
  COLORS(167,0,0),
  COLORS(255,0,0),
  COLORS(255,82,82),
  COLORS(255,186,186)
};

const COLORS blue[3] = {
  COLORS(0, 0, 255),
  COLORS(0, 122, 255),
  COLORS(0, 204, 255),
};

const COLORS green[4] = {
  COLORS(0,156,26),
  COLORS(38,204,0),
  COLORS(123,227,130),
  COLORS(210,242,212)
};

const COLORS purple[4] = {
  COLORS(49,0,74),
  COLORS(76,0,164),
  COLORS(131,0,196),
  COLORS(255, 105, 180)
};

