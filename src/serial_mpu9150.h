#ifndef __SERIAL_MPU9150_H__
#define __SERIAL_MPU9150_H__

#include <unistd.h>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <cmath>

#define MATH_PI 3.14159265 //using PI


/*Code Float to hex string used in Arduino FreeIMU library
void codeFloat(float f) {
  int8_t * b = (int8_t *) &f;
  for(int i=0; i<4; i++) {
    
    int8_t b1 = (b[i] >> 4) & 0x0f;
    int8_t b2 = (b[i] & 0x0f);
    
    char c1 = (b1 < 10) ? ('0' + b1) : 'A' + b1 - 10;
    char c2 = (b2 < 10) ? ('0' + b2) : 'A' + b2 - 10;
    std::cout<<c1;
    std::cout<<c2;
  }
}*/

//http://linux.die.net/man/3/usleep
void my_sleep(unsigned long milliseconds){
   usleep(milliseconds*1000); // 100 ms
}

//function Temperature to RGB
void TmpToRGB(float tmp,float color[3], float first, float end){
  float aux=(end-first)/5; //seven divisions, looking for the rainbow colors, 
                           //see -> http://simple.wikipedia.org/wiki/Rainbow
  if(tmp<=first) {
    //violet color
    color[0] = 143.0/255.0;  color[1] = 0.0/255.0; color[2] =255.0/255.0;
  }

  else if(tmp>first&&tmp<first+aux){
    //indigo color
    color[0] = 75.0/255.0;  color[1]= 0.0/255.0; color[2] =130.0/255.0;
  }

  else if(tmp>first+aux&&tmp<first+2*aux){
    //blue color
    color[0] = 0.0/255.0;  color[1]= 0.0/255.0; color[2] =255.0/255.0;
  }

  else if(tmp>first+2*aux&&tmp<first+3*aux){
    //green color
    color[0] = 0.0/255.0;  color[1]= 255.0/255.0; color[2] =0.0/255.0;
  }

  else if(tmp>first+3*aux&&tmp<first+4*aux){
    //yellow color
    color[0] = 255.0/255.0;  color[1]= 255.0/255.0; color[2] =0.0/255.0;
  }

  else if(tmp>first+4*aux&&tmp<first+5*aux){
    //orange color
    color[0] = 255.0/255.0;  color[1]= 127.0/255.0; color[2] =0.0/255.0;
  }

  else{
    //red color
    color[0] = 255.0/255.0; color[1] = 0.0/255.0; color[2] =0.0/255.0;
  }

}

//Decoder hex string to float number
float DecodeFloat(std::string inString){
  int8_t inData[4];
  char a[2],b[2],c[2],d[2];
  for(int j=0;j<2;j++){
    a[j] = inString[j];
    b[j] = inString[j+2];
    c[j] = inString[j+4];
    d[j] = inString[j+6];
  }

  inData[0] = (int8_t)strtol(a,NULL,16);
  inData[1] = (int8_t)strtol(b,NULL,16);
  inData[2] = (int8_t)strtol(c,NULL,16);
  inData[3] = (int8_t)strtol(d,NULL,16);

  //magic trick to transform int8 into int :)
  int bits = (inData[3] << 24) | ((inData[2] & 0xff) << 16) | 
  ((inData[1] & 0xff) << 8) | (inData[0] & 0xff);

  //transform to float, reference java intBitstoFloat
  int s = ((bits >> 31) == 0) ? 1 : -1;
  int e = ((bits >> 23) & 0xff);
  int m = (e == 0) ? (bits & 0x7fffff) << 1 : (bits & 0x7fffff) | 0x800000;
    return s*m*pow(2,e-150);
}

//If you need the others values, they are here.
//x,y,z,w,ax,ay,az,gx,gy,gz,mx,my,mz,tmp,pr
void GetData(std::string inString,float v[16]){
    std::string str;
    int cont=1,im1,aux;
      for(int i=0;i<inString.length();i++){
      if(inString[i]==','){
        switch(cont){
          case 1: im1=0; //x
                  str = inString.substr(im1,i);
                  im1 = i+1;
                  cont++;
                  v[0] = DecodeFloat(str);
                  break;
          case 2: aux = i-im1; //y
                  str = inString.substr(im1,aux);
                  im1 = i+1;
                  cont++;
                  v[1] = DecodeFloat(str);
                  break;
          case 3: aux = i-im1; //z
                  str = inString.substr(im1,aux);
                  im1 = i+1;
                  cont++;
				          v[2] = DecodeFloat(str);
                  break;
          case 4: aux = i-im1; //w
                  str = inString.substr(im1,aux);
                  im1 = i+1;
                  cont++;
                  v[3] = DecodeFloat(str);
                  break;
          case 5: aux = i-im1; //ax
                  str = inString.substr(im1,aux);
                  im1 = i+1;
                  cont++;
                  v[4] = DecodeFloat(str);
                  break;
          case 6: aux = i-im1; //ay
                  str = inString.substr(im1,aux);
                  im1 = i+1;  
                  cont++;
                  v[5] = DecodeFloat(str);
                  break;
          case 7: aux = i-im1; //az
                  str = inString.substr(im1,aux);
                  im1 = i+1;  
                  cont++;
                  v[6] = DecodeFloat(str);
                  break;
          case 8: aux = i-im1; //gx
                  str = inString.substr(im1,aux);
                  im1 = i+1;  
                  cont++;
                  v[7] = DecodeFloat(str);
                  break;
          case 9: aux = i-im1; //gy
                  str = inString.substr(im1,aux);
                  im1 = i+1;  
                  cont++;
                  v[8] = DecodeFloat(str);
                  break;
          case 10: aux = i-im1; //gz
                  str = inString.substr(im1,aux);
                  im1 = i+1;  
                  cont++;
                  v[9] = DecodeFloat(str);
                  break;
          case 11: aux = i-im1; //mx
                  str = inString.substr(im1,aux);
                  im1 = i+1;  
                  cont++;
                  v[10] = DecodeFloat(str);
                  break;
          case 12: aux = i-im1; //my
                  str = inString.substr(im1,aux);
                  im1 = i+1;  
                  cont++;
                  v[11] = DecodeFloat(str);
                  break;
          case 13: aux = i-im1; //mz
                  str = inString.substr(im1,aux);
                  im1 = i+1;  
                  cont++;
                  v[12] = DecodeFloat(str);
                  break;
          case 14: aux = i-im1; //temperature
                  str = inString.substr(im1,aux);
                  im1 = i+1;  
                  cont++;
                  v[13] = DecodeFloat(str);
                  break;
          case 15: aux = i-im1; //pressure
                  str = inString.substr(im1,aux);
                  im1 = i+1;  
                  cont++;
                  v[14] = DecodeFloat(str);
                  break;
          case 16: aux = i-im1; //heading
                  str = inString.substr(im1,aux);
                  im1 = i+1;  
                  cont++;
                  v[15] = DecodeFloat(str);
                  break;
        }
      }
    }
}

// void Rot(float x,float y, float z,float w,float *nx,float *ny, float *nz,float *nw, float ang){
//   float lx,ly,lz,lw;
//   ang = ang/360*(float)MATH_PI*2; //change to degrees to rad
//   //local rotations for the y axis
//   lw = cos(ang/2);
//   lx = x*sin(ang/2);
//   ly = y*sin(ang/2);
//   lz = z*sin(ang/2);
//   *nx = lx*((1-2*y*y-2*z*z)+ly*(2*x*y+2*w*z)+lz*(2*x*z-2*w*y));
//   std::cout<<"rotx="<<*nx<<",";
//   *ny = lx*((2*x*y-2*w*z)+ly*(1-2*x*x-2*z*z)+lz*(2*y*z-2*w*x));
//   std::cout<<"roty="<<*ny<<",";
//   *nz = lx*((2*x*z+2*w*y)+ly*(2*y*z+2*w*x)+lz*(1-2*x*x-2*y*y));
//   std::cout<<"rotz="<<*nz<<",";
//   *nw = lw;
//   std::cout<<"rotw="<<*nw<<"\n";
// }


// void quaternionToEuler(float q[4], float euler[3]) {
//   euler[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1); // psi
//   euler[1] = -asin(2 * q[1] * q[3] + 2 * q[0] * q[2]); // theta
//   euler[2] = atan2(2 * q[2] * q[3] - 2 * q[0] * q[1], 2 * q[0] * q[0] + 2 * q[3] * q[3] - 1); // phi
// }

// float *quatProd(float a[4], float b[4]) {
//   float q[4];
  
//   q[0] = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3];
//   q[1] = a[0] * b[1] + a[1] * b[0] + a[2] * b[3] - a[3] * b[2];
//   q[2] = a[0] * b[2] - a[1] * b[3] + a[2] * b[0] + a[3] * b[1];
//   q[3] = a[0] * b[3] + a[1] * b[2] - a[2] * b[1] + a[3] * b[0];
  
//   return q;
// }

// // returns a quaternion from an axis angle representation
// float *quatAxisAngle(float axis[3], float angle) {
//   float q[4];
  
//   float halfAngle = angle / 2.0;
//   float sinHalfAngle = sin(halfAngle);
//   q[0] = cos(halfAngle);
//   q[1] = -axis[0] * sinHalfAngle;
//   q[2] = -axis[1] * sinHalfAngle;
//   q[3] = -axis[2] * sinHalfAngle;
  
//   return q;
// }

// // return the quaternion conjugate of quat
// float *quatConjugate(float quat[4]) {
//   float conj[4];
  
//   conj[0] = quat[0];
//   conj[1] = -quat[1];
//   conj[2] = -quat[2];
//   conj[3] = -quat[3];
  
//   return conj;
// }

// /////////////////////////////////////////////////////////////////////////
// void getYawPitchRollRad(float *q,float *ypr) {
//   //float q[4]; // quaternion
//   float gx, gy, gz; // estimated gravity direction
  
//   gx = 2 * (q[1]*q[3] - q[0]*q[2]);
//   gy = 2 * (q[0]*q[1] + q[2]*q[3]);
//   gz = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];
  
//   ypr[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1);
//   ypr[1] = atan(gx / sqrt(gy*gy + gz*gz));
//   ypr[2] = atan(gy / sqrt(gx*gx + gz*gz));
// }

// //=============================================================
// // Get heading from magnetometer if LSM303 not available
// // code extracted from rob42/FreeIMU-20121106_1323 Github library
// // (https://github.com/rob42/FreeIMU-20121106_1323.git)
// // which is based on 
// //
// //=========================================================
// float calcMagHeading(){
//   float Head_X;
//   float Head_Y;
//   float cos_roll;
//   float sin_roll;
//   float cos_pitch;
//   float sin_pitch;
  
//   cos_roll = cos(-ypr[2]);
//   sin_roll = sin(-ypr[2]);
//   cos_pitch = cos(ypr[1]);
//   sin_pitch = sin(ypr[1]);
  
//   //Example calc
//   //Xh = bx * cos(theta) + by * sin(phi) * sin(theta) + bz * cos(phi) * sin(theta)
//   //Yh = by * cos(phi) - bz * sin(phi)
//   //return wrap((atan2(-Yh, Xh) + variation))
    
//   // Tilt compensated Magnetic field X component:
//   Head_X = magn[0]*cos_pitch+magn[1]*sin_roll*sin_pitch+magn[2]*cos_roll*sin_pitch;
//   // Tilt compensated Magnetic field Y component:
//   Head_Y = magn[1]*cos_roll-magn[2]*sin_roll;
//   // Magnetic Heading
//   return(atan2(-Head_Y,-Head_X)); 
// }

// //=============================================================
// // converted from Michael Shimniok Data Bus code
// // http://mbed.org/users/shimniok/code/AVC_20110423/

// float clamp360(float x) {
//     while ((x) >= 360.0) (x) -= 360.0; 
//     while ((x) < 0) (x) += 360.0; 
//     return x;
// }

// //==============================================================
// //
// float HeadingAvgCorr(float newx, float oldx) {
//     while ((newx + 180.0) < oldx) (newx) += 360.0;
//     while ((newx - 180.0) > oldx) (newx) -= 360.0;
//     while ((newx) == 360.0) (newx) = 0.0;
//     return newx;
// }


// //=======================================
// float iround(float number, float decimal) {
//   int ix;
//   ix = round(number*pow(10, decimal));
//   return float(ix)/pow(10, decimal);
// }

//=======================================

#endif // __SERIAL_MPU9150_H__