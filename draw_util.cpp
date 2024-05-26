#include "Arduino.h"
#include <TFT_eSPI.h>

#include "DEV_Debug.h"
#include "draw_util.h"

drawMeterWidget::drawMeterWidget() {
  _x = 0;
  _y = 0;
  _radius = 20;
  _width = 8;
  _ptrImg = NULL;
  _gradient = false;
  _fg1_color = TFT_BLACK;
  _fg2_color = TFT_BLACK;
  _font_color = TFT_BLACK;
  _bg_color = TFT_BLACK;
}

drawMeterWidget::~drawMeterWidget() {
}

void drawMeterWidget::Initialize(TFT_eSprite *ptrImg, int radius, int width, bool gradient,
                                 float min_val, float max_val,
                                 uint32_t start_angle, uint32_t end_angle,
                                 uint32_t fg1_color, uint32_t fg2_color, uint32_t bar_bg_color, uint32_t bg_color) {

  _radius = radius;
  _width = width;
  _ptrImg = ptrImg;
  _minval = min_val;
  _maxval = max_val;
  _range = (max_val - min_val);
  _step_size = float(end_angle-start_angle)/_range;
  _color_step_size = ceil((end_angle-start_angle)/10);
  _start_angle = start_angle;
  _end_angle = end_angle;
  // adjustment for function of TFT_eSPI
  _start_angle1 = start_angle+180;
  // if( _start_angle1 >= 360)
  _start_angle1 = _start_angle1 % 360;

  _end_angle1 = end_angle+180;
  // if( _end_angle1 >= 360)
  _end_angle1 = _end_angle1 % 360;
  // adjustment for function of TFT_eSPI; end


  // _val_angle = ;
  _fg1_color = fg1_color;
  _fg2_color = fg2_color;
  _bar_bg_color = bar_bg_color;
  _bg_color = bg_color;
  _gradient = gradient;

  if( _minval < 0.0)
    _offset = _minval * -1;

  if( gradient) {
    uint32_t tmpColor1, tmpColor2;
    uint8_t r1, g1, b1;
    uint8_t r2, g2, b2;
    int dr, dg, db;

    tmpColor1 = u16_to_rgb32(fg1_color);
    r1 = (tmpColor1 >> 16) & 0xFF;
    g1 = (tmpColor1 >> 8) & 0xFF;
    b1 = (tmpColor1 >> 0) & 0xFF;
    tmpColor2 = u16_to_rgb32(fg2_color);
    r2 = (tmpColor2 >> 16) & 0xFF;
    g2 = (tmpColor2 >> 8) & 0xFF;
    b2 = (tmpColor2 >> 0) & 0xFF;
    dr = int(float(r2 - r1) *0.1);
    dg = int(float(g2 - g1) *0.1);
    db = int(float(b2 - b1) *0.1);

    // Debug("tmpColor1 %d, %d, %d\n", r1, g1, b1);
    // Debug("tmpColor2 %d, %d, %d\n", r2, g2, b2);
    // Debug("dColor %d, %d, %d\n", dr, dg, db);
    r2 = r1;
    g2 = g1;
    b2 = b1;

    for( int i = 0; i < 10; i++) {
      tmpColor2 = rgb_to_u16(r2, g2, b2);
      _colorArry[i] = tmpColor2 ;
      // Debug("_colorArry[%d] %d, %d, %d\n", i, r2, g2, b2);
      r2 += dr;
      g2 += dg;
      b2 += db;
    }
  }
  Debug("start_angle %d, _start_angle %d\n", start_angle, _start_angle);
  Debug("end_angle %d, _end_angle %d\n", end_angle, _end_angle);
  Debug("_minval %3.2f _maxval %3.2f _step_size %3.3f\n", _minval, _maxval,  _step_size);
  Debug("_offset %3.3f\n", _offset);
  Debug("_color_step_size %d\n", _color_step_size);
}

void drawMeterWidget::draw( int x, int y, float value, uint32_t font_color, String string) {
  int text_x, text_y, i, j, outofrange = 0;
  int segment_start, tmp_val_angle0, tmp_val_angle1;
  float newValue;
  uint16_t oldColor;
  uint16_t newColor;

  _x = x;
  _y = y;
  text_x = _x - (_radius >>1) , text_y = _y;
  newValue = value + _offset;

  if(( _width>>1) > _radius) {
    Error( "%s, %s: width > _radius\n", __FILE__, __LINE__);
    return;
  }

  if( value <= _minval || value > _maxval) {
    _val_angle = _start_angle + 180 + _width;
    outofrange = 1;
  }
  else
    _val_angle = int(newValue*_step_size)+(_start_angle+180);

  // if( _val_angle >= 360)
  _val_angle = _val_angle % 360;

  // _ptrImg->setFreeFont( FF18);
  _ptrImg->setFreeFont( NULL);
  _ptrImg->setTextDatum( CL_DATUM);
  _ptrImg->setTextColor( font_color, _bg_color, true);

  _ptrImg->drawSmoothArc( _x, _y, //int32_t x, int32_t y,
                     _radius+_width, _radius,  // int32_t r, int32_t ir,
                     _start_angle1, _end_angle1,  // uint32_t startAngle, uint32_t endAngle,
                     _bar_bg_color, _bg_color,  // uint32_t fg_color, uint32_t bg_color,
                     true); // bool roundEnds = false);

  i = int((newValue*10.0)/float(_range));
  if( i > 9)
    i = 9;

  // Debug( "i %d, newValue %3.1f, range %d, %d\n", i, newValue, _range, _gradient);

  if( _gradient && (outofrange == 0)) {
    segment_start = _start_angle1;
    newValue = (newValue == 0) ? newValue + 2 : newValue;
    oldColor = _bg_color;
    j = 0;
    for( int ii = 0; ii < 10; ii++) {
      tmp_val_angle0 = int(newValue*_step_size);
      tmp_val_angle1 = int(ii*_color_step_size);
      if( tmp_val_angle1 == 0)
        tmp_val_angle1 = _width;
      // Debug( "ii %d, tmp_val_angle0 %d, tmp_val_angle1 %d\n", ii, tmp_val_angle0, tmp_val_angle1);

      if( tmp_val_angle0 > tmp_val_angle1) {
        tmp_val_angle0 = tmp_val_angle1 + (_start_angle+180);
      } else {
        tmp_val_angle0 = tmp_val_angle0 + (_start_angle+180);
        j = 1;
        // Debug("\n");
      }

      newColor = _colorArry[ii];

      // if( tmp_val_angle0 >= 360)
      tmp_val_angle0 = tmp_val_angle0 % 360;

      // Debug( "ii %d, segment_start %d, tmp_val_angle0 %d\n", ii, segment_start, tmp_val_angle0);

      if( segment_start != tmp_val_angle0) {
        if( ii == 0) {
          _ptrImg->drawSmoothArc( _x, _y, //int32_t x, int32_t y,
                             _radius+_width, _radius,  // int32_t r, int32_t ir,
                             segment_start, tmp_val_angle0,  // uint32_t startAngle, uint32_t endAngle,
                             newColor, oldColor,  // uint32_t fg_color, uint32_t bg_color,
                             true); // bool roundEnds = false);
        } else {
          _ptrImg->drawSmoothArc( _x, _y, //int32_t x, int32_t y,
                             _radius+_width, _radius,  // int32_t r, int32_t ir,
                             segment_start, tmp_val_angle0,  // uint32_t startAngle, uint32_t endAngle,
                             newColor, oldColor,  // uint32_t fg_color, uint32_t bg_color,
                             false); // bool roundEnds = false);
        }
      }
      segment_start = tmp_val_angle0;
      oldColor = newColor;
      if( j) {
        // Debug("---\n\n");
        break;
      }
    }
    if( j == 0) {
      tmp_val_angle0 = int(newValue*_step_size)+(_start_angle+180);
      tmp_val_angle0 = tmp_val_angle0 % 360;
      if( segment_start != tmp_val_angle0) {
        _ptrImg->drawSmoothArc( _x, _y, //int32_t x, int32_t y,
                           _radius+_width, _radius,  // int32_t r, int32_t ir,
                           segment_start, tmp_val_angle0,  // uint32_t startAngle, uint32_t endAngle,
                           newColor, oldColor,  // uint32_t fg_color, uint32_t bg_color,
                           true); // bool roundEnds = false);
      }
    }
  } else {
    _ptrImg->drawSmoothArc( _x, _y, //int32_t x, int32_t y,
                       _radius+_width, _radius,  // int32_t r, int32_t ir,
                       _start_angle1, _val_angle,  // uint32_t startAngle, uint32_t endAngle,
                       _fg1_color, _bg_color,  // uint32_t fg_color, uint32_t bg_color,
                       true); // bool roundEnds = false);
  }

  // _ptrImg->drawString(string, text_x, text_y, GFXFF);
  _ptrImg->drawString(string, text_x, text_y, 4);

}

uint32_t drawMeterWidget::u16_to_rgb32(uint16_t value) {
  uint32_t out = 0;
  uint8_t red   = 0; // Red is the top 5 bits of a 16-bit colour value
  uint8_t green = 0; // Green is the middle 6 bits, but only top 5 bits used here
  uint8_t blue  = 0; // Blue is the bottom 5 bits

  red   = (value >> 11) & 0x1F;
  green = (value >> 5) & 0x3F;
  blue  = (value >> 0) & 0x1F;

  out = red << 16 | green << 8 | blue;
  return out;
}

uint16_t drawMeterWidget::rgb_to_u16(uint8_t r, uint8_t g, uint8_t b) {
  uint16_t rgb = 0;
  uint8_t red   = 0; // Red is the top 5 bits of a 16-bit colour value
  uint8_t green = 0; // Green is the middle 6 bits, but only top 5 bits used here
  uint8_t blue  = 0; // Blue is the bottom 5 bits

  red   = r & 0x1F;
  green = g & 0x3F;
  blue  = b & 0x1F;

  rgb = (red<<11) | (green<<5) | blue;
  return rgb;
}

uint16_t drawMeterWidget::rgb32_to_u16(uint32_t rgb) {
  uint16_t rgb16 = 0;
  uint8_t red   = 0; // Red is the top 5 bits of a 16-bit colour value
  uint8_t green = 0; // Green is the middle 6 bits, but only top 5 bits used here
  uint8_t blue  = 0; // Blue is the bottom 5 bits

  red   = (rgb >> 16) & 0x1F;
  green = (rgb >> 8 ) & 0x3F;
  blue  = (rgb >> 0 ) & 0x1F;

  rgb16 = (red<<11) | (green<<5) | blue;
  return rgb16;
}
