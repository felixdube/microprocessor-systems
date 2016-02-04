#ifndef STATISTICS_H
#define STATISTICS_H

void home_sub_f32 (float *inputA, float *inputB, float *output, int length);

void home_std_f32 (float *input, int length, float *output);

void home_mean_f32 (float *input, int length, float *output);

void home_correlate_f32 (float *inputA, float *inputB, int length, float *output);

void home_conv_f32 (float *inputA, float *inputB, int length, float *output);

#endif
