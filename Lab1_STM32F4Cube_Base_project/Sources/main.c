/*************************************************************************************/
// File: main.c
// Description: Test assembly kalman filter and c kalman filter, and does substraction,
//              stadard deviation, average, correlation, convolution
// Date: February, 2016
// Author: August Lalande, Juan Morency Trudel, Felix Dube
/*************************************************************************************/

#include <stdio.h>
#include "KalmanFilter.h"
#include "statistics.h"
#include "arm_math.h"
#include "demo.h"

extern int kalmanFilter_asm(float* InputArray, float* OutputArray, int Length, struct kalman_state* kstate);

int main(void) {
  int i;
  float input[length];
  float output[length];

  // output for substraction
  float arm_sub[length];
  float home_sub[length];

  // outputs for standard deviation
  float arm_std;
  float home_std;

  // outputs for average
  float arm_mean;
  float home_mean;

  // outputs for correlation
  float arm_cor[2 * length - 1];
  float home_cor[2 * length - 1];

  // outputs for convolution
  float arm_conv[2 * length - 1];
  float home_conv[2 * length - 1];

  int error_code_C;
  int error_code_ASM;

  struct kalman_state s = {DEF_q, DEF_r, DEF_x, DEF_p, DEF_k};

  for(i = 0; i < length; i++) {
    input[i] = measurements[i];
  }

  error_code_ASM = kalmanFilter_asm(input, output, length, &s);
  printf("Error Code output of Kalman Filter ASM (21 if overflow or NaN, 16 otherwise) :  %d \n", error_code_ASM);
  printf("---- output for ASM ----\n");
  for (i = 0; i < length; i++) {
    printf("%f, ", output[i]);
  }
  error_code_C = Kalmanfilter_C(input, output, &s, length);
  printf("\n\nError Code output of Kalman Filter C (1 if overflow or NaN, 0 otherwise) :  %d \n", error_code_C);
  printf("---- output for C ----\n");
  for (i = 0; i < length; i++) {
    printf("%f, ", output[i]);
  }

  if (error_code_ASM == 16 || error_code_C == 1){
    /*** PART A ***/
    // substraction
    printf("\n\n---- Substration ----\n");
    arm_sub_f32(input, output, arm_sub, length);
    for (i = 0; i < length; i++) {
      printf("%f, ", arm_sub[i]);
    }
    printf("\n");

    home_sub_f32(input, output, home_sub, length);
    for (i = 0; i < length; i++) {
      printf("%f, ", home_sub[i]);
    }
    printf("\n");

    /*** PART B ***/
    // standard deviation
    printf("\n---- Sandard Deviation ----\n");
    arm_std_f32(arm_sub, length, &arm_std);
    printf("%f \n", arm_std);

    home_std_f32(home_sub, length, &home_std);
    printf("%f \n", home_std);

    // average
    printf("\n---- Average ----\n");
    arm_mean_f32(arm_sub, length, &arm_mean);
    printf("%f \n", arm_mean);

    home_mean_f32(home_sub, length, &home_mean);
    printf("%f \n", home_mean);

      /*** PART C ***/
    // correlation
    printf("\n---- Correlation ----\n");
    arm_correlate_f32(input, length, output, length, arm_cor);
    for (i = 0; i < (2 * length - 1) ; i++) {
      printf("%f, ", arm_cor[i]);
    }
    printf("\n");

    home_correlate_f32(input, output, length, home_cor);
    for (i = 0; i < (2 * length - 1) ; i++) {
      printf("%f, ", home_cor[i]);
    }
    printf("\n");

    /*** PART D ***/
    // convolution
    printf("\n---- Convolution ----\n");
    arm_conv_f32(input, length, output, length, arm_conv);
    for (i = 0; i < (2 * length - 1) ; i++) {
      printf("%f, ", arm_conv[i]);
    }
    printf("\n");

    home_conv_f32(input, output, length, home_conv);
    for (i = 0; i < (2 * length - 1) ; i++) {
      printf("%f, ", home_conv[i]);
    }
    printf("\n");

    return 0;
  }
}
