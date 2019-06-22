#include<math.h>

void RGB2Lab( float Rp, float Gp, float Bp, float * l_s, float * a_s, float * b_s )
{
  float R =  Rp / 255.0;
  float G =  Gp / 255.0;
  float B =  Bp / 255.0;
  float X =  0.412453 * R + 0.357580 * G + 0.180423 * B;
  float Y =  0.212671 * R + 0.715160 * G + 0.072169 * B;
  float Z =  0.019334 * R + 0.119193 * G + 0.950227 * B;
  float xr = X / 0.950456, yr = Y / 1.000, zr = Z / 1.088854;

  float fxr, fyr, fzr;
  const float param_13 = 1.0f / 3.0f;
  
  if (xr > 0.008856) {
    fxr = pow(xr, param_13);
  }
  else {
    fxr = (7.787 * xr) + (16.0 / 116.0);
  }

  if (yr > 0.008856) {
    fyr = pow(yr, param_13);
    *l_s = (116.0 * fyr) - 16.0;
  }
  else {
    fyr = (7.787 * yr) + (16.0 / 116.0);
    *l_s = 903.3 * yr;
  }

  if(zr > 0.008856) {
    fzr = pow(zr, param_13);
  }
  else {
    fzr = (7.787 * zr) + (16.0 / 116.0);
  }
 
  *a_s = 500.0 * (fxr - fyr);
  *b_s = 200.0 * (fyr - fzr);
}

void Lab2RGB( float l_s, float a_s, float b_s, float *R, float *G, float *B )
{
    float var_Y = ( l_s + 16. ) / 116.;
    float var_X = a_s / 500. + var_Y;
    float var_Z = var_Y - b_s / 200.;

    if ( pow(var_Y,3) > 0.008856 ) var_Y = pow(var_Y,3);
    else                      var_Y = ( var_Y - 16. / 116. ) / 7.787;
    if ( pow(var_X,3) > 0.008856 ) var_X = pow(var_X,3);
    else                      var_X = ( var_X - 16. / 116. ) / 7.787;
    if ( pow(var_Z,3) > 0.008856 ) var_Z = pow(var_Z,3);
    else                      var_Z = ( var_Z - 16. / 116. ) / 7.787;

    float X = 95.047 * var_X ;    
    float Y = 100.000 * var_Y  ;   
    float Z = 108.883 * var_Z ;    


    var_X = X / 100. ;      
    var_Y = Y / 100. ;       
    var_Z = Z / 100. ;      

    float var_R = var_X *  3.2406 + var_Y * -1.5372 + var_Z * -0.4986;
    float var_G = var_X * -0.9689 + var_Y *  1.8758 + var_Z *  0.0415;
    float var_B = var_X *  0.0557 + var_Y * -0.2040 + var_Z *  1.0570;

    if ( var_R > 0.0031308 ) var_R = 1.055 * pow(var_R , ( 1.0 / 2.4 ))  - 0.055;
    else                     var_R = 12.92 * var_R;
    if ( var_G > 0.0031308 ) var_G = 1.055 * pow(var_G , ( 1.0 / 2.4 ) )  - 0.055;
    else                     var_G = 12.92 * var_G;
    if ( var_B > 0.0031308 ) var_B = 1.055 * pow( var_B , ( 1.0 / 2.4 ) ) - 0.055;
    else                     var_B = 12.92 * var_B;

    *R = var_R * 255.;
    *G = var_G * 255.;
    *B = var_B * 255.;

}
