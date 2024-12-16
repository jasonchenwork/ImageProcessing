# Introduction

Write a c/c++ program to practice the automatic white balance process, which generates appropriate transferring functions to map the colors of the input image to the expected colors when captured under the Standard Illuminant D65.
Two algorithms were implemented
origin image
![enter image description here](https://github.com/jasonchenwork/ImageProcessing/blob/main/01whitebalance/InputImage01.bmp)after GWA
![enter image description here](https://github.com/jasonchenwork/ImageProcessing/blob/main/01whitebalance/InputImage01AfterGWA.bmp)after 2005 Lam Algorithm
![enter image description here](https://github.com/jasonchenwork/ImageProcessing/blob/main/01whitebalance/InputImage01After2005Lam.bmp)

# Gray World Assumption

This assumption refers to the idea that all captured images are rich in color. In other words, it is assumed that the probabilities of red, green, and blue appearing are equal. Based on this assumption, the average values of red, green, and blue will form a grayscale color.Under this theoretical assumption, if the illumination source is slightly biased toward red, the red stimulus in the image will increase, while green and blue will decrease, resulting in unequal average values for red, green, and blue. The algorithm aims to adjust the average values of red, green, and blue to be equal, as follows:
$$
R_{\text{new}} = R_{\text{org}} \times \frac{\text{GreenValue}}{R_{\text{avg}}}
$$

$$
G_{\text{new}} = G_{\text{org}} \times \frac{\text{GreenValue}}{G_{\text{avg}}}
$$

$$
B_{\text{new}} = B_{\text{org}} \times \frac{\text{GreenValue}}{B_{\text{avg}}}
$$

# 2005 Lam Algorithm

Lam (2005) proposed an algorithm that combines the above two methods, referred to as the **Lam algorithm** in this article [2]. Lam introduced a quadratic equation for automatic white balance processing, as shown in equations (7) and (8). Here, R^I\hat{R}_IR^I​, G^I\hat{G}_IG^I​, and B^I\hat{B}_IB^I​ represent the processed red, green, and blue pixel values, while RIR_IRI​, GIG_IGI​, and BIB_IBI​ are the red, green, and blue values before processing. The red channel coefficients are solved as shown in equations (9–12), and the blue channel can be calculated similarly based on equation (8).

Since the formula involves squaring the pixel values during processing, the resulting image has the characteristic of enhanced contrast. Similar to the Perfect Reflector Assumption (PRA) and the Gray World Assumption (GWA), the green pixel values remain unchanged, while only the red and blue values are adjusted.
$$
\hat{I}_R(x,y) = u_R \cdot I_R^2(x,y) + v_R I_R(x,y)
$$

$$
\hat{I}_B(x,y) = u_B \cdot I_B^2(x,y) + v_B I_B(x,y)
$$

$$
\sum_{x=1}^M \sum_{y=1}^N \hat{I}_R(x,y) = \sum_{x=1}^M \sum_{y=1}^N \hat{I}_B(x,y)
$$

$$
\text{Max}\{I_R(x,y)\} = \text{Max}\{I_G(x,y)\}
$$

$$
\mu \sum_{x=1}^M \sum_{y=1}^N I_r^2(x,y) + \nu \sum_{x=1}^M \sum_{y=1}^N I_r(x,y) = \sum_{x=1}^M \sum_{y=1}^N I_g(x,y)
$$

$$
\mu \max_{x,y} \{ I_r^2(x,y) \} + \nu \max_{x,y} \{ I_r(x,y) \} = \max_{x,y} \{ I_g(x,y) \}
$$
The Lam algorithm incorporates features of both GWA and PRA. When the image is rich in color, it produces results similar to GWA. When the image predominantly contains a single color, although it does not perform as well as the PRA-based method, it effectively mitigates the oversaturation issues caused by GWA. Therefore, by combining the characteristics of both GWA and PRA, the Lam algorithm is more versatile and suitable for automatic white balance processing under various shooting conditions.
