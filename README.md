# _Penn_

_Description: Implementation of Micro, MATLAB, and python programming to create a pen that can track what you are writing and actually use machine learning to do character recognition on all the characters that you write, on real time._

## Project Setup

_How do I, as a developer, start working on the project?_ 

1. _The pen  folder has all the micro code to upload to the pen. It can stream the data either wirelessly, or via USB for trials._
2. _The base station folder has the c code for a second micro that acts as a receiver, connected to a computer, that receives the data and transmits it via USB to the computer._
3. _The MATLAB folder has all the attitude estimation calculations and Kalman Filter implementation so that we can see the attitude of the pen on the screen in real time and use it to cancel the gravity vector for all of our calculations._
4. _The ML folder has all the machine learning implementation_

## Testing

_If you stream data from an IMU and a gyro into the MATLAB code, you should be able to see the attitude of the penn_

## Contributors

- _de Villalobos, Francisco_
- _Lautman, Mike_
- _Yim, Justin_

## License

- _All license rights reserved by the authors_