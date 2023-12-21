/*---------------------------------------------------------------------------------
Calculate Powere Spectral Density from "Numerical Recipes"

Two segments are processed for each FFT

Use half overlap.

Number points processed is (2*K+1)*M

Length of segment = 2*M = MM must be power of 2

Number of segments averaged = 2*K

---------------------------------------------------------------------------------*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<complex.h>
#include<string.h>
#include<fftw3.h>

static float complex sqrarg;
#define SQR(a) ( (sqrarg=(a)) == 0.0 ? 0.0 : sqrarg*conj(sqrarg) )

#define WINDOW(j,a,b) (1.0-fabs(((j)-(a))*(b)))       /* Bartlett */
// #define WINDOW(j,a,b) 1.0      /* Square */
// #define WINDOW(j,a,b) (0.53836 - 0.46164*cos(M_PI*j*b))       /* Hamming */



int main(int argc, char* argv[])
{
	int M = atoi(argv[1]);
	int K = atoi(argv[2]);
	char *name = argv[3];

	int MM = M+M;	// size of interval

	// power spectral density
	float *psd = (float*) malloc(M*sizeof(float));
	for (int j=0;j<M;j++) psd[j]=0.0;

	float facm=M;
	float facp=1.0/M;

	float sumw=0.0;
	for (int j=0;j<MM;j++) sumw += SQR(WINDOW(j,facm,facp));

	float *w1 = (float*) malloc(M*sizeof(float));
	float *w2 = (float*) malloc(M*sizeof(float));

	// open file for processing
	FILE *fp;
	fp = fopen(strcat(name,".bin"),"rb");

	fftw_complex *in, *out;
	fftw_plan plan;

	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*MM);			//pay attention
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*MM);		//pay attention
	plan = fftw_plan_dft_1d(MM, in, out, FFTW_MEASURE, FFTW_ESTIMATE); 	//Here we set which kind of transformation we want to perform

	fread(w1,sizeof(float),M,fp);
	float den = 0.0;
	for(int k = 0; k<K; k++){
		fread(w2,sizeof(float),M,fp);
		for(int j = 0; j < M; j++)
		{
			float w=WINDOW(j,facm,facp);
			in[j] = (w1[j] + I*w2[j])*w;
		}
		fread(w1,sizeof(float),M,fp);
		for(int j = 0; j < M; j++)
		{
			float w=WINDOW(M+j,facm,facp);
			in[M+j] = (w2[j] + I*w1[j])*w;
		}
		fftw_execute(plan); //Execution of FFT

		psd[0] += 0.5*SQR(out[0]); // average two const values from two segments
		for(int j = 1; j<M; j++)
		{
			psd[j] +=  SQR(out[j]);
			psd[j] +=  SQR(out[MM-j]);
		}
		den += sumw;
	}
	den *= MM;	// devide by N^2

	// printf("\nPower Spectral Density\n");
	for(int j = 0; j < M; j++)
	{
		psd[j] /= den;
		printf("%f \n", psd[j]); 
	}
	
	fftw_destroy_plan(plan);	 //Free memory
	fftw_free(in);			 //Free memory
	fftw_free(out);			 //Free memory
	free(w1);
	free(w2);

	return 0;
}
