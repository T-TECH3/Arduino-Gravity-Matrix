<div align="center">
  <h1>Arduino Gravity Matrix</h1>
</div>

![Gravity Matrix](Gravity%20Matrix%20Front%20Cover.png)

## Description
The Arduino Gravity Matrix is an embedded systems project that simulates real-time gravitational physics on a physical dot-matrix display. The system utilizes an Uno R3 microcontroller paired with an Inertial Measurement Unit (IMU) and an 8x8 LED matrix. The code accounts for IMU drift and is equipped with a reset button. Additionally, it features two distinct algorithms for dynamically calculating the state of the LEDs.
## Parts
- Elegoo UNO R3 (1x) ([Buy](https://www.amazon.com/ELEGOO-Board-ATmega328P-ATMEGA16U2-Compliant/dp/B01EWOE0UU?crid=374CAB1PJ7864&dib=eyJ2IjoiMSJ9.h6uflpb-pI_XRs8OGSeEmsAcnK4sqnHuU5TsAgWI6WjgcrqDoNKMksF7h2pg0o7w7EGw3FpMuGmElOWW9cPvVCtaRHA39ZIkLuOLcQaofjsy8BZsgLz5uxII_awQT7K3PNpD7hB3StZTDELfJVunpJ1JT9L7lQmyo-wYooDzrO4Xh_UQ2Xs10SxuZCJTAY-_ncVbnq83LCFc7f7AbsRU3WzoZo0kKQzXS9Muep5GIBA.tbHbkNNhlgxm0dk7VJyukU8L7ZGm3-oNMjpKN626Ifw&dib))
- MAX7219 Module (1x) ([Buy](https://www.amazon.com/HiLetgo-MAX7219-Matrix-Display-Control/dp/B07W6KZR5D?crid=3TAIP3WXOVLAJ&dib=eyJ2IjoiMSJ9.iGFft77a7DIUW6CUJemQ1Vij9O8umykJ3cWv2TTaxrTl1yuURWo29EROtvj7IT9oGzhDg2BGb9Qz7CNBYdGv_7_TrQktmCDq4f5JQRI_zXhqFFQ87nY2cQTwUe1eq2Y3k7jnFDqmUok3hJP3uJjdAPHhdHXp-RepdMdtGWP5OC9UFpfXZK5iJ498xlwrvGcbI9er6RZdkHE4SUqcfWQLx0aoGyYwaAHtnzGRvA49V04.y_3tEnvUwUKVVd3o6PG-_A7rfHtFDL5WXWM7OaIYzPk&dib))
- GY-521 Module (1x) ([Buy](https://www.amazon.com/AOICRIE-Accelerometer-Gyroscope-Pre-Soldered-Raspberry/dp/B0D2TJVMNY?crid=32XLF77415VEC&dib=eyJ2IjoiMSJ9.h3CDUQzBZlsZqyioORSHurs4FLE0ni9EFyHtbVRjgpl7zT76iLlePNi7cOuC77A-1RtY2ZmuMHNIOMGtD0Wwr04AxgpvGsJgZB8RUqLpQYpO6wtqfvQOswDcXuaoy80pnb3I1uJTkf6q2b5Pw9aMeAzFAsHEfcVGCZfO8bFjaD3Pus3oaX70ZVFPx_k4QrZgwo7-vhlg58VYvIYWoGD_6zxnyx8y4GWcKltT_E4SOGU.l0HB_qjV5h847wyEvnB2wssq6RxzMaB_SvVlTAFlBbQ&dib))
- Mini Breadboard (1x) ([Buy](https://www.amazon.com/PATIKIL-Breadboard-Solderless-Prototype-Electronics/dp/B0DBDP568C?crid=2NKRCPV22DA5L&dib=eyJ2IjoiMSJ9.dufGLUwPPHwtwQRK5n8GRdvGYxGAUGhCqiyNLDMX9AlM4ziSXt6xRmKzez0xv4pqd7BA9bDrod5NlSVzSE_BejTvPP2meKjSr8bDV5CyJqJ39dKkBZRnoGOsKd4QfASZeDcDqNZ4b7XPYamU7LwXSgMc6xIueJS1voHO2ZVRgzCFAY1V2wu-cmphSJrIGKgAECMDBgnWEh1vbyrflDseoD4NOk7F_TSsMA-cs_DjSQs.AyFRPLe_giW2rMZwM9wIMtnI8QqqBp5hp5Q5Fl-qyBY&dib))
- Momentary Button with Male Connection (1x) ([Buy](https://www.amazon.com/Momentary-pre-Wiring-Waterproof-Stainless-Normally/dp/B09BKXT1J1?crid=21J70PXVE1J5W&dib=eyJ2IjoiMSJ9.Jw97oCm4OBBl9lm-p-CJbyie2or7tQDNEXD4-UNJmkGUKYU7xFOoXhEn6KON6liQ8vsQtR8r3FD8SMgEij_f0MAMPVHsR23StLxB2NSA0Q8prZFSxydhK201U2cdATIubMNlRFYz5Vn1jF2AYFWFw1MlF4khAX5xASi7lJMXGFoljpAbzXB96mOQlfcDBx1-r6Vnb_n24raC2S1wfesbjc3bTDjc8d5ZCJkqn81u0co.eydqmbASvPkW8J9MmWaMVCO1iOyCZPL8G2wwx70KQF8&dib))
- Male to Male Jump Wire (3x) ([Buy](https://www.amazon.com/California-JOS-Breadboard-Optional-Multicolored/dp/B0BRTJQZRD?crid=1CE9IAJVC5F83&dib=eyJ2IjoiMSJ9.urRUqWt0ag6Vi-qn73oEAiTVhJ7Ox6lj4pHdXPhv6UOpdqkYyqJkNQoT_-RYRf636PiLxOTLbkKCJXtiU_Rp-AwAgSDVMq1C0-2dluCBmDQliNHXPqkFp6lRMd83697I68qoqpZrmvA3a_ajq3sr8E-wzo6iaf5dJkEqkA4aQq65HXMRfwvB5CAatTTB_DX_y-rRtoX652DcNEuI1fELNnEtHIz7yShKeNfW2QQZeWo.rnJUxR7MPI3vANhPBxKIi7CxpxivCuaaFlL5IvCe9nQ&dib))
- Male to Female Jump Wire (8x) ([Buy](https://www.amazon.com/California-JOS-Breadboard-Optional-Multicolored/dp/B0BRTHR2RL?crid=2PUM1PW8GLZCQ&dib=eyJ2IjoiMSJ9.OdANVAycega4z3tzwcWpwgM7KqtvuGeZB1Dxq0d-fVlbohuf4TzketN0Qhlrlgdg2Z98JaZwPMsLybEv7FAMpgiee8EpkNGLTFBtywKnseCIy6E4XzPWjBnIR8YFbLFMT4fMIgfUEGHI_V1pwsMVYXGKTw__8G5Xcboq1UqKO7RI_vqcT2Zfnc1Yd8gfmWt_nVJxa1kqd4muD7ecxJYWcYoaFW17Ow-VN7UX1Y5PjVE.eibJBjve-xyE2epJrpW7AengBWEGgupfbqTvJySlEYg&dib))
- Male to Male Pin Header (1x) ([Buy](https://www.amazon.com/Straight-Breakaway-Connector-Breadboard-Electronic/dp/B0FRZW75VS?crid=V3KCZOR1ODTD&dib=eyJ2IjoiMSJ9.eIyrgscRldYhIdB4VgLCMK9NJ0M0VW6jbcIUTBWbh72qV6q1UHurdPOTjBO4Uz4lgqNlPucNigJyXkLhaWlJQbigYJkPBGmT68ggCKrm9l5BIJvYjcHoO-BYW7dGElPsWnCgUgDIHO2NnGOq8srNMbczLuueRS7PzXOPgQSyVLgNzyxXqANRAg1vWY55VAy0Gn0ZVgocJVN2lrHeZ9jZ3uVhTWFnqZqFah9XjtRcR9I.ypBCWPRmKNMW2R3wsBF1hlCN_ThPem3z6t3DkGEfoSk&dib)))

## Wiring Schematics

## Simulation Algorithm
