#ifndef MORPHOLOGY_HPP
#define MORPHOLOGY_HPP
#include <bits/stdc++.h>

void Erode(uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
           uint16_t height, uint16_t filtersize);

void Dilation(uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
              uint16_t height, uint16_t filtersize);

void Opening(uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
             uint16_t height, uint16_t filtersize);
void Closing(uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
             uint16_t height, uint16_t filtersize);

void MorphologicalGradient(uint8_t *imageSource, uint8_t *imageTarget,
                           uint16_t width, uint16_t height,
                           uint16_t filtersize);
void TopHat(uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
            uint16_t height, uint16_t filtersize);
void BlackHat(uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
              uint16_t height, uint16_t filtersize);
#endif