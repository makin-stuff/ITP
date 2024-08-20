This repo has all the design files, code, and documentation for [A Nervous System](https://www.priyankamakin.com/brain) by [Priyanka Makin](https://www.priyankamakin.com/)

# How to populate brain boards

For any version, each brain board consists of a string of 80 addressable LEDs, 40 on the front and 40 on the back. This is the only component used on the whole board which makes assembly pretty straight forward!

The addressable LED footprint used in this design was made to be used with [these LEDs I got from Adafruit.](https://www.adafruit.com/product/4957)

The orientation of the LED matters for the communication between LEDs to work, so always double-check before soldering!

If you are building up version 1.0 of these boards (with green soldermask) there is a white silkscreen triangle indicator for pin 1 on the LED footprint. Be sure to line this silkscreen up with the pin of the LED that also has a triangle cut out of it in the package.

<table class="table table-hover table-striped table-bordered">
  <tr align="center">
    <td><img src="https://github.com/makin-stuff/ITP/blob/main/Digital_Fabrication/Brain/footprint.JPG"></td>
    <td><img src="https://github.com/makin-stuff/ITP/blob/main/Digital_Fabrication/Brain/package.png"></td>
  </tr>
  <tr align="center">
    <td>Printed circuit board (v1.0) with silkscreen</td>
    <td>Adafruit NeoPixel</td>
  </tr>
</table>
