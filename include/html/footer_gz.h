#ifndef __FOOTER_GZ_H__
#define __FOOTER_GZ_H__

#include <Arduino.h>

const unsigned char footer_html_gz[] PROGMEM = {
  0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x03, 0x53, 0x80,
  0x02, 0x1b, 0xfd, 0x94, 0xcc, 0x32, 0x3b, 0x2e, 0x10, 0x0f, 0xca, 0x84,
  0xb0, 0x8b, 0x93, 0x8b, 0x32, 0x0b, 0x4a, 0x20, 0x12, 0x20, 0x49, 0x8d,
  0xb4, 0xd2, 0xbc, 0xe4, 0x92, 0xcc, 0xfc, 0x3c, 0x8d, 0xf2, 0xcc, 0xbc,
  0x94, 0xfc, 0x72, 0x1d, 0x85, 0x94, 0xfc, 0xe4, 0xd2, 0xdc, 0xd4, 0xbc,
  0x12, 0x4d, 0x85, 0x6a, 0xb0, 0x16, 0x90, 0x2a, 0x10, 0x86, 0x89, 0xeb,
  0xa5, 0xa7, 0x96, 0xb8, 0xe6, 0xa4, 0x82, 0x94, 0x38, 0x55, 0x7a, 0xa6,
  0x68, 0xa8, 0x97, 0xe4, 0xa7, 0xa7, 0xe7, 0xa4, 0xaa, 0x6b, 0xea, 0x25,
  0xa6, 0xa4, 0xb8, 0x96, 0x01, 0x75, 0xfa, 0x64, 0x16, 0x97, 0xa4, 0xe6,
  0xa5, 0x16, 0x69, 0xa8, 0x27, 0xe7, 0x64, 0x26, 0x67, 0xab, 0xeb, 0x28,
  0xc0, 0x2d, 0x49, 0x45, 0x37, 0x15, 0xbf, 0xc9, 0xa5, 0xc9, 0xd9, 0xa9,
  0x29, 0xbe, 0xa9, 0x79, 0xa5, 0x40, 0xd3, 0x93, 0x73, 0x12, 0x8b, 0x8b,
  0x41, 0x46, 0xeb, 0x41, 0x2c, 0x04, 0x9a, 0x5e, 0x5a, 0x5c, 0x92, 0x9f,
  0xab, 0x0b, 0x74, 0x49, 0xa9, 0x6e, 0x49, 0x29, 0x48, 0xa9, 0xba, 0xa6,
  0x35, 0x8a, 0x9b, 0xf1, 0x9b, 0x0e, 0x73, 0x37, 0xa6, 0xc9, 0x15, 0xe8,
  0x06, 0xd5, 0x22, 0x19, 0x5c, 0xab, 0xa9, 0x51, 0x92, 0x91, 0x59, 0xac,
  0xa3, 0x00, 0x22, 0xf5, 0x60, 0xc1, 0x02, 0x55, 0x60, 0xa3, 0x0f, 0x0b,
  0x61, 0xb0, 0x43, 0x6c, 0x20, 0x3c, 0x85, 0x92, 0xca, 0x82, 0x54, 0x5b,
  0xf5, 0x92, 0xd4, 0x8a, 0x12, 0xfd, 0xac, 0xc4, 0xb2, 0x44, 0x88, 0xa8,
  0x3a, 0x22, 0x1a, 0xca, 0x12, 0x8b, 0x14, 0x32, 0x75, 0x14, 0x12, 0x93,
  0x93, 0x15, 0x6c, 0xe1, 0x31, 0x80, 0x14, 0xd2, 0xc5, 0x4e, 0x95, 0xce,
  0x20, 0xff, 0xfb, 0x25, 0xe6, 0xa6, 0x6a, 0xa8, 0x03, 0x95, 0xe5, 0x17,
  0xa5, 0x00, 0x23, 0x0d, 0xd9, 0x99, 0x69, 0xf9, 0x45, 0x0a, 0x1a, 0x99,
  0x40, 0xed, 0x06, 0xd6, 0x0a, 0x99, 0x0a, 0x36, 0x20, 0xb3, 0xf4, 0x72,
  0x52, 0xf3, 0xd2, 0x4b, 0x32, 0x80, 0x7c, 0x6d, 0x6d, 0x4d, 0x90, 0x40,
  0x74, 0x66, 0x2c, 0x31, 0xb1, 0x84, 0x1e, 0x49, 0x60, 0x8f, 0x62, 0x86,
  0x52, 0x62, 0x72, 0x49, 0x66, 0x19, 0x30, 0xe2, 0x51, 0xc3, 0x1c, 0xe4,
  0x95, 0x54, 0xa0, 0x33, 0xc0, 0x9a, 0xf2, 0x80, 0x3e, 0x76, 0x85, 0x24,
  0x96, 0xe0, 0xcc, 0xa4, 0x9c, 0xcc, 0xbc, 0x74, 0x54, 0xc5, 0xea, 0x49,
  0x39, 0xf9, 0xc0, 0x14, 0xa2, 0x60, 0x6b, 0x6b, 0xab, 0x90, 0xaa, 0x57,
  0x5c, 0x52, 0x99, 0x93, 0xaa, 0x97, 0x92, 0x59, 0x5c, 0x90, 0x93, 0x58,
  0xa9, 0x60, 0x8f, 0x21, 0x62, 0xab, 0xa0, 0x9e, 0x97, 0x9f, 0x97, 0xaa,
  0xae, 0x60, 0x85, 0x4d, 0x0a, 0x62, 0x14, 0x38, 0xd8, 0x41, 0xf1, 0x0e,
  0x8b, 0x32, 0x44, 0x8c, 0xd8, 0xe8, 0x27, 0xe5, 0xa7, 0x54, 0xda, 0x71,
  0xd9, 0xe8, 0x67, 0x94, 0xe4, 0xe6, 0xd8, 0x01, 0x00, 0xde, 0x6e, 0x63,
  0x0a, 0x2b, 0x03, 0x00, 0x00
};
const unsigned int footer_html_gz_len = 425;

#endif
