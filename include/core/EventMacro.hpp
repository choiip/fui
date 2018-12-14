#pragma once

#include "Signal.hpp"

#define FUI_DEFINE_EVENT(PROTOTYPE, EVENT)                                                                             \
public:                                                                                                                \
  typedef std::function<PROTOTYPE> EVENT##Handler;                                                                     \
  auto on##EVENT(const EVENT##Handler& handler)->decltype(this) {                                                      \
    _signal##EVENT += handler;                                                                                         \
    return this;                                                                                                       \
  }                                                                                                                    \
                                                                                                                       \
protected:                                                                                                             \
  Signal<PROTOTYPE> _signal##EVENT;

/* The unknown key */
#define FUI_KEY_UNKNOWN -1

/* Printable keys */
#define FUI_KEY_SPACE 32
#define FUI_KEY_APOSTROPHE 39 /* ' */
#define FUI_KEY_COMMA 44      /* , */
#define FUI_KEY_MINUS 45      /* - */
#define FUI_KEY_PERIOD 46     /* . */
#define FUI_KEY_SLASH 47      /* / */
#define FUI_KEY_0 48
#define FUI_KEY_1 49
#define FUI_KEY_2 50
#define FUI_KEY_3 51
#define FUI_KEY_4 52
#define FUI_KEY_5 53
#define FUI_KEY_6 54
#define FUI_KEY_7 55
#define FUI_KEY_8 56
#define FUI_KEY_9 57
#define FUI_KEY_SEMICOLON 59 /* ; */
#define FUI_KEY_EQUAL 61     /* = */
#define FUI_KEY_A 65
#define FUI_KEY_B 66
#define FUI_KEY_C 67
#define FUI_KEY_D 68
#define FUI_KEY_E 69
#define FUI_KEY_F 70
#define FUI_KEY_G 71
#define FUI_KEY_H 72
#define FUI_KEY_I 73
#define FUI_KEY_J 74
#define FUI_KEY_K 75
#define FUI_KEY_L 76
#define FUI_KEY_M 77
#define FUI_KEY_N 78
#define FUI_KEY_O 79
#define FUI_KEY_P 80
#define FUI_KEY_Q 81
#define FUI_KEY_R 82
#define FUI_KEY_S 83
#define FUI_KEY_T 84
#define FUI_KEY_U 85
#define FUI_KEY_V 86
#define FUI_KEY_W 87
#define FUI_KEY_X 88
#define FUI_KEY_Y 89
#define FUI_KEY_Z 90
#define FUI_KEY_LEFT_BRACKET 91  /* [ */
#define FUI_KEY_BACKSLASH 92     /* \ */
#define FUI_KEY_RIGHT_BRACKET 93 /* ] */
#define FUI_KEY_GRAVE_ACCENT 96  /* ` */
#define FUI_KEY_WORLD_1 161      /* non-US #1 */
#define FUI_KEY_WORLD_2 162      /* non-US #2 */

/* Function keys */
#define FUI_KEY_ESCAPE 256
#define FUI_KEY_ENTER 257
#define FUI_KEY_TAB 258
#define FUI_KEY_BACKSPACE 259
#define FUI_KEY_INSERT 260
#define FUI_KEY_DELETE 261
#define FUI_KEY_RIGHT 262
#define FUI_KEY_LEFT 263
#define FUI_KEY_DOWN 264
#define FUI_KEY_UP 265
#define FUI_KEY_PAGE_UP 266
#define FUI_KEY_PAGE_DOWN 267
#define FUI_KEY_HOME 268
#define FUI_KEY_END 269
#define FUI_KEY_CAPS_LOCK 280
#define FUI_KEY_SCROLL_LOCK 281
#define FUI_KEY_NUM_LOCK 282
#define FUI_KEY_PRINT_SCREEN 283
#define FUI_KEY_PAUSE 284
#define FUI_KEY_F1 290
#define FUI_KEY_F2 291
#define FUI_KEY_F3 292
#define FUI_KEY_F4 293
#define FUI_KEY_F5 294
#define FUI_KEY_F6 295
#define FUI_KEY_F7 296
#define FUI_KEY_F8 297
#define FUI_KEY_F9 298
#define FUI_KEY_F10 299
#define FUI_KEY_F11 300
#define FUI_KEY_F12 301
#define FUI_KEY_F13 302
#define FUI_KEY_F14 303
#define FUI_KEY_F15 304
#define FUI_KEY_F16 305
#define FUI_KEY_F17 306
#define FUI_KEY_F18 307
#define FUI_KEY_F19 308
#define FUI_KEY_F20 309
#define FUI_KEY_F21 310
#define FUI_KEY_F22 311
#define FUI_KEY_F23 312
#define FUI_KEY_F24 313
#define FUI_KEY_F25 314
#define FUI_KEY_KP_0 320
#define FUI_KEY_KP_1 321
#define FUI_KEY_KP_2 322
#define FUI_KEY_KP_3 323
#define FUI_KEY_KP_4 324
#define FUI_KEY_KP_5 325
#define FUI_KEY_KP_6 326
#define FUI_KEY_KP_7 327
#define FUI_KEY_KP_8 328
#define FUI_KEY_KP_9 329
#define FUI_KEY_KP_DECIMAL 330
#define FUI_KEY_KP_DIVIDE 331
#define FUI_KEY_KP_MULTIPLY 332
#define FUI_KEY_KP_SUBTRACT 333
#define FUI_KEY_KP_ADD 334
#define FUI_KEY_KP_ENTER 335
#define FUI_KEY_KP_EQUAL 336
#define FUI_KEY_LEFT_SHIFT 340
#define FUI_KEY_LEFT_CONTROL 341
#define FUI_KEY_LEFT_ALT 342
#define FUI_KEY_LEFT_SUPER 343
#define FUI_KEY_RIGHT_SHIFT 344
#define FUI_KEY_RIGHT_CONTROL 345
#define FUI_KEY_RIGHT_ALT 346
#define FUI_KEY_RIGHT_SUPER 347
#define FUI_KEY_MENU 348

#define FUI_KEY_LAST FUI_KEY_MENU
