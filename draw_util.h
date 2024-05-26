#ifndef __DRAWUTIL_H__
#define __DRAWUTIL_H__

#define GFXFF 1

// #define FF18 &FreeSans12pt7b
#define FF18 &FreeSerif12pt7b

class drawMeterWidget {
  private:
  int _x, _y;
  int _radius;
  int _width;
  int _range;
  uint32_t _start_angle;
  uint32_t _end_angle;
  uint32_t _start_angle1;
  uint32_t _end_angle1;
  uint32_t _val_angle;
  float _minval, _maxval, _offset;
  float _step_size;
  int _color_step_size;
  uint16_t _fg1_color;
  uint16_t _fg2_color;
  uint16_t _font_color;
  uint16_t _bar_bg_color;
  uint16_t _bg_color;
  uint16_t _colorArry[10];
  TFT_eSprite *_ptrImg;
  bool _gradient;

  public:
  drawMeterWidget();
  ~drawMeterWidget();

  void Initialize(TFT_eSprite *ptrImg, int radius, int width, bool gradient,
                  float min_val, float max_val,
                  uint32_t _start_angle, uint32_t _end_angle,
                  uint32_t fg1_color, uint32_t fg2_color, uint32_t bar_bg_color, uint32_t bg_color);

  void draw( int x, int y, float value, uint32_t font_color, String string);

  uint32_t u16_to_rgb32(uint16_t value);
  uint16_t rgb_to_u16(uint8_t r,uint8_t g,uint8_t b);
  uint16_t rgb32_to_u16(uint32_t rgb);
};

#endif
